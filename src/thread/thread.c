/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thread.c
@brief: 
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "thread.h"

/**  @brief thread 管理结构体 */
ThreadManage_t manager;

/**
* @brief 修改线程属性
* @param [in]  flag   线程属性标志 @see thread_attr_flag
* @param [out] tattr   线程属性
* @param [out] attr    外部使用的线程属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
_thread_set_attr(const int_t flag,pthread_attr_t  *tattr, const threadAttr_t  *attr)
{
    if(ATTR_DETACH_STATE&flag)        CHECK_FF(pthread_attr_setdetachstate(tattr,  attr->detachState));      //设置线程分离状态
    if(ATTR_INHERIT_SCHEDULE&flag)    CHECK_FF(pthread_attr_setinheritsched(tattr, attr->inheritSchedule));  //设置线程继承性
    if(ATTR_SCHEDULE_POLICY&flag)     CHECK_FF(pthread_attr_setschedpolicy(tattr,  attr->schedulePolicy));   //设置线程调度策略
    if(ATTR_SCHEDULE_PARAM&flag)      CHECK_FF(pthread_attr_setschedparam(tattr,   &attr->schedParam));      //设置线程调度参数
    if(ATTR_SCOPE&flag)               CHECK_FF(pthread_attr_setscope(tattr,        attr->scope));            //设置线程作用域
    if(ATTR_GUARD_SIZE&flag)          CHECK_FF(pthread_attr_setguardsize(tattr,    attr->guardSize));        //设置线程栈末尾的警戒缓冲区大小
    if(ATTR_STACK_ADDR_AND_SIZE&flag) CHECK_FF(pthread_attr_setstack(tattr,        attr->stackAddr, attr->stackSize)); //设置线程线程栈地址　线程栈大小
    return THREAD_SUCC;
}

/**
* @brief 默认线程属性
* @param [out] tattr   线程属性
* @param [out] attr    外部使用的线程属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
_thread_default_set_attr(pthread_attr_t  *tattr, threadAttr_t  *attr)
{
    void *stack = thread_malloc(DEFAULT_STACK_SIZE);
    CHECK_FF(pthread_attr_setstack(tattr, stack, DEFAULT_STACK_SIZE));              //设置线程线程栈地址 线程栈大小
    attr->stackAddr = stack;
    attr->stackSize = DEFAULT_STACK_SIZE;
}

/**
* @brief 获得线程属性
* @param [in]  flag   线程属性标志 @see thread_attr_flag
* @param [out] attr 线程属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
/*int_t
thread_get_attr(const int_t flag, threadAttr_t  *attr)
{
    if(PARAMISNULL(attr))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(ATTR_DETACH_STATE|flag)     pthread_attr_getdetachstate(attr,  &(attr->detachState));     //获得线程分离状态
    if(ATTR_INHERIT_SCHEDULE|flag) pthread_attr_getinheritsched(attr, &(attr->inheritSchedule)); //获得线程继承性
    if(ATTR_SCHEDULE_POLICY|flag)  pthread_attr_getschedpolicy(attr,  &(attr->schedulePolicy));  //获得线程调度策略
    if(ATTR_SCHEDULE_PARAM|flag)   pthread_attr_getschedparam(attr,   &(attr->schedParam));      //获得线程调度参数
    if(ATTR_SCOPE|flag)            pthread_attr_getscope(attr,        &(attr->scope));           //获得线程作用域
    if(ATTR_STACK_ADDR|flag)       pthread_attr_getstackaddr(attr,    &(attr->stackAddr));       //获得线程栈地址
    if(ATTR_STACK_SIZE|flag)       pthread_attr_getstacksize(attr,    &(attr->stackSize));       //获得线程栈大小
    if(ATTR_GUARD_SIZE|flag)       pthread_attr_getguardsize(attr,    &(attr->guardSize));       //获得线程栈末尾的警戒缓冲区大小

}*/

/**
* @brief 线程加入链表
* @param [out] thread  线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
_thread_add_node(Thread *thread)
{
    //加锁
	//初始化节点
	list_node_init(&thread->Node);
	//将socket节点添加到链表
    pthread_mutex_lock(&manager.mutex);  
	CHECK_FF(list_insert_node(INSERT_TO_HEAD, &manager.threadList, &thread->Node));
    pthread_mutex_unlock(&manager.mutex); 
    return THREAD_SUCC;
}

/**
* @brief 线程移除出链表
* @param [out] thread  线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
_thread_delete_node(Thread *thread)
{
    pthread_mutex_lock(&manager.mutex);  
    CHECK_FF(list_delete_node(&manager.threadList, &thread->Node));
    pthread_mutex_unlock(&manager.mutex); 
    if(thread->attr.autoInit == 1) thread_free(thread->attr.stackAddr);
    thread_free(thread);
    return THREAD_SUCC;
}

/**
* @brief 线程在链表中查找
* @param [out] id  线程id
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
Thread *
_thread_find_node(pthread_t id)
{
    list_node_t * tmp;
    Thread *node=NULL;
    //顺时针遍历节点
    tmp = list_get_head_node(&manager.threadList); 
    if(tmp == NULL) return THREAD_NULL; 
    do{
        node = list_takeout_struct(tmp, Thread, Node);
        if(pthread_equal(id,node->id)) return node;
        tmp = list_get_next_node(tmp);
    }while(tmp != list_get_head_node(&manager.threadList));
    return THREAD_NULL;
}


/**
* @brief 创建一个线程
* @param [out] thread          线程号
* @param [out] module          模块类型
* @param [in]  attr            线程属性
* @param [in]  start_routine   线程函数
* @param [in]  arg             线程函数参数
* @retval 返回线程  表示成功
* @retval THREAD_NULL 表示失败
*/
Thread *
thread_create(u32_t module,
              threadAttr_t  *attr, 
              void *(*startRoutine)(void *), 
              void  * arg)
{
    if(PARAMISNULL(startRoutine)) RETURN_ERR(ERR_PARA, THREAD_NULL);
    if(module >= MOD_BUTT) RETURN_ERR(ERR_PARA, THREAD_NULL);
    //线程管理初始化
	if (MNANGE_IS_INITIALIZED != manager.InitFlag){
		manager.InitFlag = MNANGE_IS_INITIALIZED;
		list_init(&manager.threadList);
        pthread_mutex_init(&manager.mutex, NULL);
	}
    Thread *thread;
    pthread_attr_t  tattr;
    if(THREAD_SUCC != pthread_attr_init(&tattr)) RETURN_ERR(ERR_PARA, THREAD_NULL); //初始化
    thread = thread_malloc(sizeof(Thread));//初始化线程结构体
    //配置线程属性
    if(attr != NULL){//初始化属性
        if( THREAD_SUCC != _thread_set_attr(attr->flag, &tattr, attr)) goto create_fault;
        thread->attr = *attr;//copy 属性
        thread->attr.autoInit =0;
    }else{
        _thread_default_set_attr(&tattr, &thread->attr);
        thread->attr.autoInit =1;
        thread->attr.detachState =THREAD_JOINABLE;
    }
    //创建线程
    if( THREAD_SUCC != pthread_create(&thread->id, &tattr, startRoutine, arg)){
        goto create_fault;
    }

    //将线程加入线程队列
    thread->tid = gettid();
    thread->module = module;
    _thread_add_node(thread);
    pthread_attr_destroy(&tattr);
    return thread;

    create_fault:
        pthread_attr_destroy(&tattr);
        thread_free(thread);
        thread = NULL;
        return THREAD_NULL;
}


/**
* @brief 获得本身线程
* @retval 返回本身线程信息  表示成功
* @retval THREAD_NULL     表示失败
*/
Thread *
thread_self(void)
{
    pthread_t id = pthread_self();
    pthread_mutex_lock(&manager.mutex);  
    Thread * node = _thread_find_node(id);
    pthread_mutex_unlock(&manager.mutex); 
     return node;
}

/**
* @brief 两个线程相等
* @param [in] one  一个线程
* @param [in] another  另一个线程
* @retval THREAD_SUCC  表示相等
* @retval THREAD_FAIL  表示不等
*/
int_t
thread_equal(Thread* const one, Thread* const another)
{
    if(PARAMISNULL(one)||PARAMISNULL(another)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(pthread_equal(one->id,another->id) ) return THREAD_SUCC;
    return THREAD_FAIL;
}

/**
* @brief 在多线程编程环境下,函数调用会出现在多个线程中，initRoutine()函数仅执行一次
* @param [in] state 设置本线程对Cancel信号的反应 不配置设置为-1
 - PTHREAD_CANCEL_ENABLE   收到信号后设为CANCLED状态(默认)
 - PTHREAD_CANCEL_DISABLE 忽略CANCEL信号继续运行
* @param [in] type 设置本线程取消动作的执行时机 不配置设置为-1
 - PTHREAD_CANCEL_DEFERRED    仅当Cancel状态为PTHREAD_CANCEL_ENABLE时有效,收到信号后继续运行至下一个取消点再退出 
 - PTHREAD_CANCEL_ASYNCHRONOUS 仅当Cancel状态为PTHREAD_CANCEL_ENABLE时有效,立即执行取消动作（退出）
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_cancel_set(int_t state, int_t type)
{
    if(PTHREAD_CANCEL_ENABLE != state || PTHREAD_CANCEL_DISABLE != state) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(PTHREAD_CANCEL_DEFERRED != type || PTHREAD_CANCEL_ASYNCHRONOUS != type) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    pthread_setcanceltype(type, NULL);
    pthread_setcancelstate(state, NULL);
    return THREAD_SUCC;
} 

/**
* @brief 设置线程取消点
* @param [out] exited  被退出的线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_cancel_point(void)
{
    pthread_testcancel();
    return THREAD_SUCC;
}

/**
* @brief 发送CANCEL信号消息退出某个线程
* @attention： 线程接收到CANCEL信号的缺省处理是继续运行至取消点，
               pthreads标准指定了几个取消点，其中包括：
                   (1)通过thread_cancel_point调用以编程方式建立线程取消点。 
                   (2)线程等待pthread_cond_wait或pthread_cond_timewait()中的特定条件。 
                   (3)被sigwait(2)阻塞的函数 
                   (4)一些标准的库调用。通常，这些调用包括线程可基于阻塞的函数。 
* @param [out] exited  被退出的线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_cancel_some(Thread *exited)
{
    if(PARAMISNULL(exited)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(THREAD_SUCC == pthread_cancel(exited->id)){
       if( exited->attr.detachState == THREAD_DETACHED )  _thread_delete_node(exited);
       return THREAD_SUCC;
    }
    return THREAD_FAIL;
}



/**
* @brief 本身线程退出
* @attention： 线程退出后，并不会释放线程资源
* @param [out] retval  线程退出返回值 thread_join接收这个值
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_exit_self( void * retval)
{
    pthread_t id = pthread_self();
    pthread_mutex_lock(&manager.mutex);  
    Thread * node = _thread_find_node(id);
    pthread_mutex_unlock(&manager.mutex); 
    if( node != NULL && node->attr.detachState == THREAD_DETACHED ) _thread_delete_node(node);
    pthread_exit(retval);
    return THREAD_SUCC;
}

/**
* @brief 连接线程
* @attention： 在任何一个时间点上，线程是可结合的（joinable）或者是分离的（detached）。
               一个可结合的线程能够被其他线程收回其资源和杀死。在被其他线程回收之前，它的
               存储器资源（例如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或
               杀死的，它的存储器资源在它终止时由系统自动释放。
* @param [in] thread  线程号
* @param [out] retval  线程退出返回值 thread_exit_self发送这个值
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_join(Thread *thread, void **retval)
{
    if(PARAMISNULL(thread)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    CHECK_FF(pthread_join(thread->id, retval));
    _thread_delete_node(thread);
    return THREAD_SUCC;
}


/**
* @brief 分离线程
* @attention： 在任何一个时间点上，线程是可结合的（joinable）或者是分离的（detached）。
               一个可结合的线程能够被其他线程收回其资源和杀死。在被其他线程回收之前，它的
               存储器资源（例如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或
               杀死的，它的存储器资源在它终止时由系统自动释放。
               分离后，不需要thread_join释放资源，系统会自动释放资源
* @param [in] thread  线程号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_detach(Thread *thread)
{
    if(PARAMISNULL(thread)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    thread->attr.detachState = THREAD_DETACHED;
    return pthread_detach(thread->id);
}

/**
* @brief 在多线程编程环境下,函数调用会出现在多个线程中，initRoutine()函数仅执行一次
* @param [in] onceControl 控制变量 必须使用PTHREAD_ONCE_INIT宏静态地初始化。
* @param [in] initRoutine 初始化函数
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_once(pthread_once_t *onceControl, void (*initRoutine) (void))
{
    if(PARAMISNULL(onceControl)||PARAMISNULL(initRoutine)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_once(onceControl, initRoutine);
}

/**
* @brief 线程局部数据
* @param [in] key 局部数据key
* @param [in] clean 现在结束对局部数据清理函数
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_localdata_create(pthread_key_t *key, void (*clean)(void*))
{
    if(PARAMISNULL(key)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_key_create(key, clean);
}

/**
* @brief 设置线程局部数据
* @param [in] key 局部数据key
* @param [in] value 局部数据
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_set_localdata(pthread_key_t key, const void *value)
{
    return pthread_setspecific(key, value);

}

/**
* @brief 获得线程局部数据
* @param [in] key 局部数据key
* @param [in] value 局部数据地址
* @retval 返回值地址   表示成功
* @retval THREAD_FAIL 表示失败
*/
void *
thread_get_localdata(pthread_key_t key)
{
    return pthread_getspecific(key);//当没有setpecific时候，直接返回空指针
}


/**
* @brief 删除线程局部数据
* @param [in] key 局部数据key
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_delete_localdata(pthread_key_t key)
{
    return pthread_key_delete(key);
}


/**
* @brief 发送信号到某个线程 SIGSTOP SIGKILL 都会作用进程
* @param [in] thread  线程
* @param [in] sig  信号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_signal(Thread *thread, int sig)
{
    if(PARAMISNULL(thread)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_kill(thread->id, sig);
}

/**
* @brief 广播信号
* @param [in] sig  信号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_broadcast_signal(int sig)
{
    list_node_t * tmp;
    Thread *node=NULL;
    uint threadSum = 0;
    //顺时针遍历节点
    pthread_mutex_lock(&manager.mutex);
    threadSum = list_get_current_member(&manager.threadList);
    tmp = list_get_head_node(&manager.threadList); 
    for(;threadSum>0;threadSum--){
        node = list_takeout_struct(tmp, Thread, Node);
        tmp = list_get_next_node(tmp);
        if(pthread_kill(node->id, 0) == THREAD_SUCC ){
            CHECK_FL(pthread_kill(node->id, sig));
        }else{//没存活删除线程
            CHECK_FL(list_delete_node(&manager.threadList, &node->Node));
        }
    }
    pthread_mutex_unlock(&manager.mutex); 
    return THREAD_SUCC;
}

/**
* @brief 得到当前线程数
* @retval 返回线程数  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_get_number(void)
{
    return list_get_current_member(&manager.threadList);
}
