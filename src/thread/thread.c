/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thread.c
@brief: 
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#include ".././util/util_inc.h"
#include "thread.h"

/**  @brief thread ����ṹ�� */
ThreadManage_t manager;

/**
* @brief �޸��߳�����
* @param [in]  flag   �߳����Ա�־ @see thread_attr_flag
* @param [out] tattr   �߳�����
* @param [out] attr    �ⲿʹ�õ��߳�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
_thread_set_attr(const int_t flag,pthread_attr_t  *tattr, const threadAttr_t  *attr)
{
    if(ATTR_DETACH_STATE&flag)        CHECK_FF(pthread_attr_setdetachstate(tattr,  attr->detachState));      //�����̷߳���״̬
    if(ATTR_INHERIT_SCHEDULE&flag)    CHECK_FF(pthread_attr_setinheritsched(tattr, attr->inheritSchedule));  //�����̼̳߳���
    if(ATTR_SCHEDULE_POLICY&flag)     CHECK_FF(pthread_attr_setschedpolicy(tattr,  attr->schedulePolicy));   //�����̵߳��Ȳ���
    if(ATTR_SCHEDULE_PARAM&flag)      CHECK_FF(pthread_attr_setschedparam(tattr,   &attr->schedParam));      //�����̵߳��Ȳ���
    if(ATTR_SCOPE&flag)               CHECK_FF(pthread_attr_setscope(tattr,        attr->scope));            //�����߳�������
    if(ATTR_GUARD_SIZE&flag)          CHECK_FF(pthread_attr_setguardsize(tattr,    attr->guardSize));        //�����߳�ջĩβ�ľ��仺������С
    if(ATTR_STACK_ADDR_AND_SIZE&flag) CHECK_FF(pthread_attr_setstack(tattr,        attr->stackAddr, attr->stackSize)); //�����߳��߳�ջ��ַ���߳�ջ��С
    return THREAD_SUCC;
}

/**
* @brief Ĭ���߳�����
* @param [out] tattr   �߳�����
* @param [out] attr    �ⲿʹ�õ��߳�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
_thread_default_set_attr(pthread_attr_t  *tattr, threadAttr_t  *attr)
{
    void *stack = thread_malloc(DEFAULT_STACK_SIZE);
    CHECK_FF(pthread_attr_setstack(tattr, stack, DEFAULT_STACK_SIZE));              //�����߳��߳�ջ��ַ �߳�ջ��С
    attr->stackAddr = stack;
    attr->stackSize = DEFAULT_STACK_SIZE;
}

/**
* @brief ����߳�����
* @param [in]  flag   �߳����Ա�־ @see thread_attr_flag
* @param [out] attr �߳�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
/*int_t
thread_get_attr(const int_t flag, threadAttr_t  *attr)
{
    if(PARAMISNULL(attr))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(ATTR_DETACH_STATE|flag)     pthread_attr_getdetachstate(attr,  &(attr->detachState));     //����̷߳���״̬
    if(ATTR_INHERIT_SCHEDULE|flag) pthread_attr_getinheritsched(attr, &(attr->inheritSchedule)); //����̼̳߳���
    if(ATTR_SCHEDULE_POLICY|flag)  pthread_attr_getschedpolicy(attr,  &(attr->schedulePolicy));  //����̵߳��Ȳ���
    if(ATTR_SCHEDULE_PARAM|flag)   pthread_attr_getschedparam(attr,   &(attr->schedParam));      //����̵߳��Ȳ���
    if(ATTR_SCOPE|flag)            pthread_attr_getscope(attr,        &(attr->scope));           //����߳�������
    if(ATTR_STACK_ADDR|flag)       pthread_attr_getstackaddr(attr,    &(attr->stackAddr));       //����߳�ջ��ַ
    if(ATTR_STACK_SIZE|flag)       pthread_attr_getstacksize(attr,    &(attr->stackSize));       //����߳�ջ��С
    if(ATTR_GUARD_SIZE|flag)       pthread_attr_getguardsize(attr,    &(attr->guardSize));       //����߳�ջĩβ�ľ��仺������С

}*/

/**
* @brief �̼߳�������
* @param [out] thread  �߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
_thread_add_node(Thread *thread)
{
    //����
	//��ʼ���ڵ�
	list_node_init(&thread->Node);
	//��socket�ڵ���ӵ�����
    pthread_mutex_lock(&manager.mutex);  
	CHECK_FF(list_insert_node(INSERT_TO_HEAD, &manager.threadList, &thread->Node));
    pthread_mutex_unlock(&manager.mutex); 
    return THREAD_SUCC;
}

/**
* @brief �߳��Ƴ�������
* @param [out] thread  �߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
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
* @brief �߳��������в���
* @param [out] id  �߳�id
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
Thread *
_thread_find_node(pthread_t id)
{
    list_node_t * tmp;
    Thread *node=NULL;
    //˳ʱ������ڵ�
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
* @brief ����һ���߳�
* @param [out] thread          �̺߳�
* @param [out] module          ģ������
* @param [in]  attr            �߳�����
* @param [in]  start_routine   �̺߳���
* @param [in]  arg             �̺߳�������
* @retval �����߳�  ��ʾ�ɹ�
* @retval THREAD_NULL ��ʾʧ��
*/
Thread *
thread_create(u32_t module,
              threadAttr_t  *attr, 
              void *(*startRoutine)(void *), 
              void  * arg)
{
    if(PARAMISNULL(startRoutine)) RETURN_ERR(ERR_PARA, THREAD_NULL);
    if(module >= MOD_BUTT) RETURN_ERR(ERR_PARA, THREAD_NULL);
    //�̹߳����ʼ��
	if (MNANGE_IS_INITIALIZED != manager.InitFlag){
		manager.InitFlag = MNANGE_IS_INITIALIZED;
		list_init(&manager.threadList);
        pthread_mutex_init(&manager.mutex, NULL);
	}
    Thread *thread;
    pthread_attr_t  tattr;
    if(THREAD_SUCC != pthread_attr_init(&tattr)) RETURN_ERR(ERR_PARA, THREAD_NULL); //��ʼ��
    thread = thread_malloc(sizeof(Thread));//��ʼ���߳̽ṹ��
    //�����߳�����
    if(attr != NULL){//��ʼ������
        if( THREAD_SUCC != _thread_set_attr(attr->flag, &tattr, attr)) goto create_fault;
        thread->attr = *attr;//copy ����
        thread->attr.autoInit =0;
    }else{
        _thread_default_set_attr(&tattr, &thread->attr);
        thread->attr.autoInit =1;
        thread->attr.detachState =THREAD_JOINABLE;
    }
    //�����߳�
    if( THREAD_SUCC != pthread_create(&thread->id, &tattr, startRoutine, arg)){
        goto create_fault;
    }

    //���̼߳����̶߳���
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
* @brief ��ñ����߳�
* @retval ���ر����߳���Ϣ  ��ʾ�ɹ�
* @retval THREAD_NULL     ��ʾʧ��
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
* @brief �����߳����
* @param [in] one  һ���߳�
* @param [in] another  ��һ���߳�
* @retval THREAD_SUCC  ��ʾ���
* @retval THREAD_FAIL  ��ʾ����
*/
int_t
thread_equal(Thread* const one, Thread* const another)
{
    if(PARAMISNULL(one)||PARAMISNULL(another)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(pthread_equal(one->id,another->id) ) return THREAD_SUCC;
    return THREAD_FAIL;
}

/**
* @brief �ڶ��̱߳�̻�����,�������û�����ڶ���߳��У�initRoutine()������ִ��һ��
* @param [in] state ���ñ��̶߳�Cancel�źŵķ�Ӧ ����������Ϊ-1
 - PTHREAD_CANCEL_ENABLE   �յ��źź���ΪCANCLED״̬(Ĭ��)
 - PTHREAD_CANCEL_DISABLE ����CANCEL�źż�������
* @param [in] type ���ñ��߳�ȡ��������ִ��ʱ�� ����������Ϊ-1
 - PTHREAD_CANCEL_DEFERRED    ����Cancel״̬ΪPTHREAD_CANCEL_ENABLEʱ��Ч,�յ��źź������������һ��ȡ�������˳� 
 - PTHREAD_CANCEL_ASYNCHRONOUS ����Cancel״̬ΪPTHREAD_CANCEL_ENABLEʱ��Ч,����ִ��ȡ���������˳���
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
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
* @brief �����߳�ȡ����
* @param [out] exited  ���˳����߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t 
thread_cancel_point(void)
{
    pthread_testcancel();
    return THREAD_SUCC;
}

/**
* @brief ����CANCEL�ź���Ϣ�˳�ĳ���߳�
* @attention�� �߳̽��յ�CANCEL�źŵ�ȱʡ�����Ǽ���������ȡ���㣬
               pthreads��׼ָ���˼���ȡ���㣬���а�����
                   (1)ͨ��thread_cancel_point�����Ա�̷�ʽ�����߳�ȡ���㡣 
                   (2)�̵߳ȴ�pthread_cond_wait��pthread_cond_timewait()�е��ض������� 
                   (3)��sigwait(2)�����ĺ��� 
                   (4)һЩ��׼�Ŀ���á�ͨ������Щ���ð����߳̿ɻ��������ĺ����� 
* @param [out] exited  ���˳����߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
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
* @brief �����߳��˳�
* @attention�� �߳��˳��󣬲������ͷ��߳���Դ
* @param [out] retval  �߳��˳�����ֵ thread_join�������ֵ
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
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
* @brief �����߳�
* @attention�� ���κ�һ��ʱ����ϣ��߳��ǿɽ�ϵģ�joinable�������Ƿ���ģ�detached����
               һ���ɽ�ϵ��߳��ܹ��������߳��ջ�����Դ��ɱ�����ڱ������̻߳���֮ǰ������
               �洢����Դ������ջ���ǲ��ͷŵġ��෴��һ��������߳��ǲ��ܱ������̻߳��ջ�
               ɱ���ģ����Ĵ洢����Դ������ֹʱ��ϵͳ�Զ��ͷš�
* @param [in] thread  �̺߳�
* @param [out] retval  �߳��˳�����ֵ thread_exit_self�������ֵ
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
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
* @brief �����߳�
* @attention�� ���κ�һ��ʱ����ϣ��߳��ǿɽ�ϵģ�joinable�������Ƿ���ģ�detached����
               һ���ɽ�ϵ��߳��ܹ��������߳��ջ�����Դ��ɱ�����ڱ������̻߳���֮ǰ������
               �洢����Դ������ջ���ǲ��ͷŵġ��෴��һ��������߳��ǲ��ܱ������̻߳��ջ�
               ɱ���ģ����Ĵ洢����Դ������ֹʱ��ϵͳ�Զ��ͷš�
               ����󣬲���Ҫthread_join�ͷ���Դ��ϵͳ���Զ��ͷ���Դ
* @param [in] thread  �̺߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_detach(Thread *thread)
{
    if(PARAMISNULL(thread)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    thread->attr.detachState = THREAD_DETACHED;
    return pthread_detach(thread->id);
}

/**
* @brief �ڶ��̱߳�̻�����,�������û�����ڶ���߳��У�initRoutine()������ִ��һ��
* @param [in] onceControl ���Ʊ��� ����ʹ��PTHREAD_ONCE_INIT�꾲̬�س�ʼ����
* @param [in] initRoutine ��ʼ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t 
thread_once(pthread_once_t *onceControl, void (*initRoutine) (void))
{
    if(PARAMISNULL(onceControl)||PARAMISNULL(initRoutine)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_once(onceControl, initRoutine);
}

/**
* @brief �ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] clean ���ڽ����Ծֲ�����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_localdata_create(pthread_key_t *key, void (*clean)(void*))
{
    if(PARAMISNULL(key)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_key_create(key, clean);
}

/**
* @brief �����ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] value �ֲ�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_set_localdata(pthread_key_t key, const void *value)
{
    return pthread_setspecific(key, value);

}

/**
* @brief ����ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] value �ֲ����ݵ�ַ
* @retval ����ֵ��ַ   ��ʾ�ɹ�
* @retval THREAD_FAIL ��ʾʧ��
*/
void *
thread_get_localdata(pthread_key_t key)
{
    return pthread_getspecific(key);//��û��setpecificʱ��ֱ�ӷ��ؿ�ָ��
}


/**
* @brief ɾ���ֲ߳̾�����
* @param [in] key �ֲ�����key
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_delete_localdata(pthread_key_t key)
{
    return pthread_key_delete(key);
}


/**
* @brief �����źŵ�ĳ���߳� SIGSTOP SIGKILL �������ý���
* @param [in] thread  �߳�
* @param [in] sig  �ź�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_signal(Thread *thread, int sig)
{
    if(PARAMISNULL(thread)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_kill(thread->id, sig);
}

/**
* @brief �㲥�ź�
* @param [in] sig  �ź�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_broadcast_signal(int sig)
{
    list_node_t * tmp;
    Thread *node=NULL;
    uint threadSum = 0;
    //˳ʱ������ڵ�
    pthread_mutex_lock(&manager.mutex);
    threadSum = list_get_current_member(&manager.threadList);
    tmp = list_get_head_node(&manager.threadList); 
    for(;threadSum>0;threadSum--){
        node = list_takeout_struct(tmp, Thread, Node);
        tmp = list_get_next_node(tmp);
        if(pthread_kill(node->id, 0) == THREAD_SUCC ){
            CHECK_FL(pthread_kill(node->id, sig));
        }else{//û���ɾ���߳�
            CHECK_FL(list_delete_node(&manager.threadList, &node->Node));
        }
    }
    pthread_mutex_unlock(&manager.mutex); 
    return THREAD_SUCC;
}

/**
* @brief �õ���ǰ�߳���
* @retval �����߳���  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_get_number(void)
{
    return list_get_current_member(&manager.threadList);
}
