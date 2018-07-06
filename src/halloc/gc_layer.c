/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: gc_layer.c
@brief: 内存泄漏层主要是对于内存存泄漏的检测，不是一个必须层，算是内存管理层的一个可选的附属层，
		当管理层发现有内存泄漏时候，就会启用leak的管理员进行处理内存检测，使用这个层有两个要求
        1. 若使用线程，必须使用halloc提供的线程创建函数
        2.信号SIGLOST/SIGPWR已被模块占用，如有冲突则修改本模块信号
        函数部分
        第一个部分：上报超时block函数
        第二个部分：gclayer层管理员
        第三个部分：gclayer初始化函数
        第四个部分：gclayer摧毁函数
        第五个部分：内存泄漏扫描函数
        第六个部分：SIGLOST/SIGPWR信号处理函数（stop the world）
        系统内存简图
            &_start
                :      代码段
            &etext
                :
            &__data_start
                :      初始化数据段
            &edata
                :      未初始化数据段
            &end
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2018/01/30
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2018/01/30  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "halloc.h"

/**  @brief 垃圾回收 */
gc_t GC;
/******************************************************************
*   Name：gc层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 等待醒来
*/
#define _gclayer_wait_wakeup()\
do{\
    mutex_lock(&GC.lock);\
    GC.currentThread++;\
    if(GC.wakeupSign == 0){\
        if(GC.totalThread == GC.currentThread){\
            cond_broadcast(&GC.wakeup);\
            GC.readySign++;\
        }else{\
            while(GC.currentThread != GC.totalThread) cond_wait(&GC.wakeup,&GC.lock,NULL);\
        }\
    }\
    mutex_unlock(&GC.lock);\
}while(0)

/**
* @brief 各个定时器gc准备
*/
#define _gclayer_get_ready()\
do{\
    mutex_lock(&GC.lock);\
    GC.currentThread++;\
    if(GC.readySign == 0){\
        if(mlayer_get_timersum()+1 <= GC.currentThread){\
            cond_broadcast(&GC.wakeup);\
            GC.readySign++;\
            GC.wakeupSign=0;\
            GC.currentThread = 0;\
        }else{\
            while(GC.currentThread != GC.totalThread) cond_wait(&GC.wakeup,&GC.lock,NULL);\
        }\
    }\
    mutex_unlock(&GC.lock);\
}while(0)

/**  @brief 内存块已存在 */
#define EXIST_BLACKLIST(node) (NULL != node->Next || NULL != node->Prev)
/******************************************************************
*   Name：内存管理层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 上报超时block
* @param [in]  block   上报内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_report_overtime_block(block_t *block)
{
    if(PARAMISNULL(block)) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
    list_node_t *blackNode = &block->blackNode;
    //租赁时间清空，上报超时log
    block->lease = 0;
	halloc_log("overtime block(module:%d file:%s line:%d func:%s) was fault\n", block->module, block->file, block->line, block->func);
    //如果没初始化GC，或者检查内存块是否已经加入了链表 直接返回
    if(GC.GCInit != ENABLED || EXIST_BLACKLIST(blackNode)) return HALLOC_SUCC;
    mutex_lock(&GC.listLock);
    //加入黑名单
    CHECK_FF(list_insert_node(INSERT_TO_HEAD,&GC.blackList, &block->blackNode));
    //开启定时器，准备gc回收
    if(GC.timerOpen == 0){
        struct timespec times;
        times.tv_sec = SUPERVISOR_TIME;
        times.tv_nsec = 0;
        CHECK_FG(timer_start(GC.supervisor,TIMER_MODE_NORMAL, TIMER_ONCE, &times), timer_fail);
        GC.timerOpen = 1;
    }
    mutex_unlock(&GC.listLock);
	return HALLOC_SUCC;

    timer_fail:
        mutex_unlock(&GC.listLock);
        return HALLOC_FAIL;

}


/**
* @brief 内存扫描
* @param [in]  start  开始地址
* @param [in]  end    结束地址
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_gclayer_memory_scan(void **start, void **end)
{
    checkinfo_t  *headcheck;
    block_t *block;

    for(; start <= end; start++) {
        //寻找指向内存块的地址
        if(GC.hallocHeadAddr<*start && *start<GC.hallocTailAddr){ 
            headcheck = (checkinfo_t *)((char *)*start - sizeof(checkinfo_t));
            if(MEMISOVERFLOW(headcheck)) continue;      
            //获得内存块
            block = headcheck->block;
            //内存块检查是否已经被遍历
            if(block->checkSign != 0 || block->color != BLOCK_RED) continue;
            block->checkSign++;
            //递归查找内存块的内存
            CHECK_FL(_gclayer_memory_scan(block->headAddr, (void *)MEM_OFFSET(block->headAddr, block->blockSize)));
        }
    }
	return HALLOC_SUCC;
}


/**
* @brief 内存扫描
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_halloc_memory_scan(list_head_t *pageList)
{
    #if 0
	page_t  *page  = NULL;//内存页
    block_t *block = NULL;//内存块
    list_node_t *blackNode = &block->blackNode;
    if(GC.GCStart != ENABLED) return HALLOC_SUCC;
    //等待全部线程就位
    //_gclayer_get_ready();

    //遍历内存页
	traversal_list_do(page, pageList, pageNode){
        //遍历查找内存块
        traversal_list_do(block, &page->blocksList, node){
            blackNode = &block->blackNode;
            if(block->checkSign != 0 || block->color != BLOCK_RED || EXIST_BLACKLIST(blackNode)) continue;
            block->checkSign++;
            //递归查找内存块的内存
            CHECK_FL(_gclayer_memory_scan(block->headAddr, (void *)MEM_OFFSET(block->headAddr, block->blockSize)));
        }while_traversal_list(&page->blocksList);
	}while_traversal_list(pageList);

    //等待恢复线程
    _gclayer_wait_wakeup();
    #endif
	return HALLOC_SUCC;
}



/**
* @brief 定时器处理函数（管理员）
* @param [in]  value    传入值
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
void
gclayer_supervisor(union sigval value)
{
    halloc_log("start gclayer supervisor\n");
    block_t *block = NULL;//内存块
    //等待全部线程就位
    GC.GCStart= ENABLED;
    //_gclayer_get_ready();
    //发送信号给全部线程
    CHECK_FG(thread_broadcast_signal(STOP_THREAD), signal_fail);
    //发送信号到主线程
    if(kill(GC.mainPid, 0) == HALLOC_SUCC) kill(GC.mainPid, STOP_THREAD);
    GC.totalThread = thread_get_number()+1+1;//mlayer_get_timersum()+1;
    //扫描静态内存
    CHECK_FL(_gclayer_memory_scan(GC.startData, GC.endData));
    //等待恢复线程
    _gclayer_wait_wakeup();
    //恢复
    GC.totalThread = 0;
    GC.readySign = 0;
    //处理黑名单
    mutex_lock(&GC.listLock);
        //遍历黑名单
        traversal_list_do(block, &GC.blackList, blackNode){
            if(block->checkSign != 0){//内存块持有时间过长，进行警告
                CHECK_FL(list_delete_node(&GC.blackList, &block->blackNode));
                halloc_log("the block(module:%d file:%s line:%d func:%s) malloc time too long\n", \
			        block->module, block->file, block->line, block->func);
            }else if(block->checkSign == 0){//内存块泄漏，进行回收
                CHECK_FL(list_delete_node(&GC.blackList, &block->blackNode));
                CHECK_FL(alayer_free(get_malloc_addr(block->headAddr)));
                halloc_log("the block(module:%d file:%s line:%d func:%s) was leak\n", \
			        block->module, block->file, block->line, block->func);
            }
        }while_traversal_list(&GC.blackList);
    mutex_unlock(&GC.listLock);

    signal_fail:
        return;
}

/**
* @brief stw 信号处理函数
* @param [in]  sig    信号
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
void 
gclayer_stop_the_world(int sig)
{
    int tid = gettid();
    thread_t thread;
    void *stackbottom;
    void *stacktop;

    if(GC.GCStart != ENABLED) {
        halloc_log("the id:%d recv signal, but GC wasn`t start\n", tid);
        goto fail;
    }
    halloc_log("the thread:%d stop, start to gc\n", tid);
    //中断为主进程
    if(tid == GC.mainPid){
        stackbottom = GC.stackbottom;
    }else{
        CHECK_NG(thread = thread_self(), tid_fail);
        stackbottom = MEM_OFFSET(thread->attr.stackAddr, thread->attr.stackSize);
    }
    //扫描线程栈
    CHECK_FL(_gclayer_memory_scan(&stacktop,  stackbottom));
    //等待恢复线程
    _gclayer_wait_wakeup();

    return;
    tid_fail:
        halloc_log("the id:%d was illegal\n", tid);
    fail:
        return;
}

/**
* @brief 初始化gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_init(void)
{
    timerEvent_t event;
    struct timespec times;
    struct sigaction action;
    extern int _end[];
    extern int __data_start[];
    FILE *statfp;
 
    //获得主线程栈底地址
    CHECK_NF(statfp =fopen("/proc/self/stat","r"));
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &GC.stackbottom);
    fclose(statfp);
    GC.mainPid = getpid();
    //获得静态变量和全局变量地址
    GC.startData = &__data_start;
    GC.endData = &_end;
    //获得halloc地址
    GC.hallocHeadAddr = slayer_get_headaddr();
    GC.hallocTailAddr = MEM_OFFSET(GC.hallocHeadAddr ,HALLOC_TOTAL_MEMORY);

    //初始化各个线程stop the world信号处理
    memset(&action, 0, sizeof(action));     
    sigemptyset(&action.sa_mask);     
    action.sa_flags = 0;     
    action.sa_handler = gclayer_stop_the_world;   
    CHECK_FF(sigaction(STOP_THREAD, &action, NULL));     

    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
	event.callbackFunc= gclayer_supervisor;
	//初始化
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_open(&GC.supervisor,&event)); //监管员 监管内存越界等
    GC.timerOpen = 0;
    GC.GCInit = ENABLED;
    GC.GCStart= DISABLE;
    GC.totalThread = 0;
    GC.currentThread = 0;
    GC.readySign = 0;
    GC.wakeupSign = 0;
    list_init(&GC.blackList);//黑名单
    CHECK_FF(mutex_init(&GC.listLock, 0, NULL));
    CHECK_FF(mutex_init(&GC.lock, 0, NULL));
    CHECK_FF(cond_init(&GC.wakeup));
	return HALLOC_SUCC;
}


/**
* @brief 摧毁gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_destroy(void)
{
	//摧毁管理员
	CHECK_FL(timer_stop(GC.supervisor));//关闭定时器
	CHECK_FF(timer_close(&GC.supervisor)); //监管员关闭
    GC.GCInit = DISABLE;
    CHECK_FF(mutex_destroy(&GC.lock));
    CHECK_FF(cond_destroy(&GC.wakeup));
    return HALLOC_SUCC;
}