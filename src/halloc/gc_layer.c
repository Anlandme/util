/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: gc_layer.c
@brief: �ڴ�й©����Ҫ�Ƕ����ڴ��й©�ļ�⣬����һ������㣬�����ڴ������һ����ѡ�ĸ����㣬
		������㷢�����ڴ�й©ʱ�򣬾ͻ�����leak�Ĺ���Ա���д����ڴ��⣬ʹ�������������Ҫ��
        1. ��ʹ���̣߳�����ʹ��halloc�ṩ���̴߳�������
        2.�ź�SIGLOST/SIGPWR�ѱ�ģ��ռ�ã����г�ͻ���޸ı�ģ���ź�
        ��������
        ��һ�����֣��ϱ���ʱblock����
        �ڶ������֣�gclayer�����Ա
        ���������֣�gclayer��ʼ������
        ���ĸ����֣�gclayer�ݻٺ���
        ��������֣��ڴ�й©ɨ�躯��
        ���������֣�SIGLOST/SIGPWR�źŴ�������stop the world��
        ϵͳ�ڴ��ͼ
            &_start
                :      �����
            &etext
                :
            &__data_start
                :      ��ʼ�����ݶ�
            &edata
                :      δ��ʼ�����ݶ�
            &end
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2018/01/30
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2018/01/30  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#include ".././util/util_inc.h"
#include "halloc.h"

/**  @brief �������� */
gc_t GC;
/******************************************************************
*   Name��gc�㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief �ȴ�����
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
* @brief ������ʱ��gc׼��
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

/**  @brief �ڴ���Ѵ��� */
#define EXIST_BLACKLIST(node) (NULL != node->Next || NULL != node->Prev)
/******************************************************************
*   Name���ڴ����㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief �ϱ���ʱblock
* @param [in]  block   �ϱ��ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_report_overtime_block(block_t *block)
{
    if(PARAMISNULL(block)) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
    list_node_t *blackNode = &block->blackNode;
    //����ʱ����գ��ϱ���ʱlog
    block->lease = 0;
	halloc_log("overtime block(module:%d file:%s line:%d func:%s) was fault\n", block->module, block->file, block->line, block->func);
    //���û��ʼ��GC�����߼���ڴ���Ƿ��Ѿ����������� ֱ�ӷ���
    if(GC.GCInit != ENABLED || EXIST_BLACKLIST(blackNode)) return HALLOC_SUCC;
    mutex_lock(&GC.listLock);
    //���������
    CHECK_FF(list_insert_node(INSERT_TO_HEAD,&GC.blackList, &block->blackNode));
    //������ʱ����׼��gc����
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
* @brief �ڴ�ɨ��
* @param [in]  start  ��ʼ��ַ
* @param [in]  end    ������ַ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_gclayer_memory_scan(void **start, void **end)
{
    checkinfo_t  *headcheck;
    block_t *block;

    for(; start <= end; start++) {
        //Ѱ��ָ���ڴ��ĵ�ַ
        if(GC.hallocHeadAddr<*start && *start<GC.hallocTailAddr){ 
            headcheck = (checkinfo_t *)((char *)*start - sizeof(checkinfo_t));
            if(MEMISOVERFLOW(headcheck)) continue;      
            //����ڴ��
            block = headcheck->block;
            //�ڴ�����Ƿ��Ѿ�������
            if(block->checkSign != 0 || block->color != BLOCK_RED) continue;
            block->checkSign++;
            //�ݹ�����ڴ����ڴ�
            CHECK_FL(_gclayer_memory_scan(block->headAddr, (void *)MEM_OFFSET(block->headAddr, block->blockSize)));
        }
    }
	return HALLOC_SUCC;
}


/**
* @brief �ڴ�ɨ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_halloc_memory_scan(list_head_t *pageList)
{
    #if 0
	page_t  *page  = NULL;//�ڴ�ҳ
    block_t *block = NULL;//�ڴ��
    list_node_t *blackNode = &block->blackNode;
    if(GC.GCStart != ENABLED) return HALLOC_SUCC;
    //�ȴ�ȫ���߳̾�λ
    //_gclayer_get_ready();

    //�����ڴ�ҳ
	traversal_list_do(page, pageList, pageNode){
        //���������ڴ��
        traversal_list_do(block, &page->blocksList, node){
            blackNode = &block->blackNode;
            if(block->checkSign != 0 || block->color != BLOCK_RED || EXIST_BLACKLIST(blackNode)) continue;
            block->checkSign++;
            //�ݹ�����ڴ����ڴ�
            CHECK_FL(_gclayer_memory_scan(block->headAddr, (void *)MEM_OFFSET(block->headAddr, block->blockSize)));
        }while_traversal_list(&page->blocksList);
	}while_traversal_list(pageList);

    //�ȴ��ָ��߳�
    _gclayer_wait_wakeup();
    #endif
	return HALLOC_SUCC;
}



/**
* @brief ��ʱ��������������Ա��
* @param [in]  value    ����ֵ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
void
gclayer_supervisor(union sigval value)
{
    halloc_log("start gclayer supervisor\n");
    block_t *block = NULL;//�ڴ��
    //�ȴ�ȫ���߳̾�λ
    GC.GCStart= ENABLED;
    //_gclayer_get_ready();
    //�����źŸ�ȫ���߳�
    CHECK_FG(thread_broadcast_signal(STOP_THREAD), signal_fail);
    //�����źŵ����߳�
    if(kill(GC.mainPid, 0) == HALLOC_SUCC) kill(GC.mainPid, STOP_THREAD);
    GC.totalThread = thread_get_number()+1+1;//mlayer_get_timersum()+1;
    //ɨ�農̬�ڴ�
    CHECK_FL(_gclayer_memory_scan(GC.startData, GC.endData));
    //�ȴ��ָ��߳�
    _gclayer_wait_wakeup();
    //�ָ�
    GC.totalThread = 0;
    GC.readySign = 0;
    //���������
    mutex_lock(&GC.listLock);
        //����������
        traversal_list_do(block, &GC.blackList, blackNode){
            if(block->checkSign != 0){//�ڴ�����ʱ����������о���
                CHECK_FL(list_delete_node(&GC.blackList, &block->blackNode));
                halloc_log("the block(module:%d file:%s line:%d func:%s) malloc time too long\n", \
			        block->module, block->file, block->line, block->func);
            }else if(block->checkSign == 0){//�ڴ��й©�����л���
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
* @brief stw �źŴ�����
* @param [in]  sig    �ź�
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
    //�ж�Ϊ������
    if(tid == GC.mainPid){
        stackbottom = GC.stackbottom;
    }else{
        CHECK_NG(thread = thread_self(), tid_fail);
        stackbottom = MEM_OFFSET(thread->attr.stackAddr, thread->attr.stackSize);
    }
    //ɨ���߳�ջ
    CHECK_FL(_gclayer_memory_scan(&stacktop,  stackbottom));
    //�ȴ��ָ��߳�
    _gclayer_wait_wakeup();

    return;
    tid_fail:
        halloc_log("the id:%d was illegal\n", tid);
    fail:
        return;
}

/**
* @brief ��ʼ��gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
 
    //������߳�ջ�׵�ַ
    CHECK_NF(statfp =fopen("/proc/self/stat","r"));
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &GC.stackbottom);
    fclose(statfp);
    GC.mainPid = getpid();
    //��þ�̬������ȫ�ֱ�����ַ
    GC.startData = &__data_start;
    GC.endData = &_end;
    //���halloc��ַ
    GC.hallocHeadAddr = slayer_get_headaddr();
    GC.hallocTailAddr = MEM_OFFSET(GC.hallocHeadAddr ,HALLOC_TOTAL_MEMORY);

    //��ʼ�������߳�stop the world�źŴ���
    memset(&action, 0, sizeof(action));     
    sigemptyset(&action.sa_mask);     
    action.sa_flags = 0;     
    action.sa_handler = gclayer_stop_the_world;   
    CHECK_FF(sigaction(STOP_THREAD, &action, NULL));     

    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
	event.callbackFunc= gclayer_supervisor;
	//��ʼ��
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_open(&GC.supervisor,&event)); //���Ա ����ڴ�Խ���
    GC.timerOpen = 0;
    GC.GCInit = ENABLED;
    GC.GCStart= DISABLE;
    GC.totalThread = 0;
    GC.currentThread = 0;
    GC.readySign = 0;
    GC.wakeupSign = 0;
    list_init(&GC.blackList);//������
    CHECK_FF(mutex_init(&GC.listLock, 0, NULL));
    CHECK_FF(mutex_init(&GC.lock, 0, NULL));
    CHECK_FF(cond_init(&GC.wakeup));
	return HALLOC_SUCC;
}


/**
* @brief �ݻ�gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_destroy(void)
{
	//�ݻٹ���Ա
	CHECK_FL(timer_stop(GC.supervisor));//�رն�ʱ��
	CHECK_FF(timer_close(&GC.supervisor)); //���Ա�ر�
    GC.GCInit = DISABLE;
    CHECK_FF(mutex_destroy(&GC.lock));
    CHECK_FF(cond_destroy(&GC.wakeup));
    return HALLOC_SUCC;
}