/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thread_pub.h
@brief:  thread����ͷ�ļ�
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_THREAD_PUB_H__
#define __INC_THREAD_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "thread.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief �߳�ֵ */
typedef Thread* thread_t;

/**  @brief �߳����Ա�־ */
enum thread_attr_flag{
  ATTR_DETACH_STATE        = 1<<0, /**< �̵߳ķ���״̬ */
  ATTR_SCHEDULE_POLICY     = 1<<1, /**< �̵߳��Ȳ���  */
  ATTR_SCHEDULE_PARAM      = 1<<2, /**< �̵߳ĵ��Ȳ��� */
  ATTR_INHERIT_SCHEDULE    = 1<<3, /**< �̵߳ļ̳���  */
  ATTR_SCOPE               = 1<<4, /**< �̵߳�������  */
  ATTR_GUARD_SIZE          = 1<<5, /**< �߳�ջĩβ�ľ��仺������С */
  ATTR_STACK_ADDR_AND_SIZE = 1<<6, /**< �߳�ջ��λ�� */
//ATTR_STACK_ADDR_SET   = 1<<7, /**< ��ջ�ĵ�ַ�� */
};

/**  @brief �߳����� �̵߳ķ���״̬ */
enum thread_attr_detach{
	THREAD_DETACHED = PTHREAD_CREATE_DETACHED,  /**< �����߳�û�б��������߳����ȴ����Լ����н��� �����ͷ�ϵͳ��Դ*/
	THREAD_JOINABLE = PTHREAD_CREATE_JOINABLE,  /**< �Ƿ���״̬,Ĭ��ԭ�е��̵߳ȴ��µ��߳̽�����ֻ��thread_join����
													 ����ʱ�������ͷ��Լ�ռ�õ�ϵͳ��Դ*/
};

/**  @brief �߳����� �̵߳��Ȳ��� THREAD_FIFO��THREAD_RRֻ����rootȨ���²���Ч*/
enum thread_attr_policy{
	THREAD_FIFO  = SCHED_FIFO,   /**<ѭ������ �����Щ�߳�δ�����ȼ����ߵ��߳���ռ��������������̣߳�ֱ�����̷߳���������Ϊֹ��*/
	THREAD_RR    = SCHED_RR,     /**<�����ȳ����� ÿ���̶߳���һ��ִ��ʱ����� ��ϵͳȷ����ʱ����ڽ�һֱִ����Щ�߳�*/
	THREAD_OTHER = SCHED_OTHER,  /**<�Զ������ �߳�Ĭ��ʹ�� SCHED_OTHER ���Ȳ��ԡ��߳�һ����ʼ���У�ֱ������ռ����ֱ���߳�������ֹͣΪֹ��*/
};

/**  @brief �߳����� �̵߳ļ̳��� */
enum thread_attr_schedule{
	THREAD_EXPLICIT_SCHED = PTHREAD_EXPLICIT_SCHED, /**<Ĭ�� ʹ�ýṹpthread_attr_tָ���ĵ����㷨*/
	THREAD_INHERIT_SCHED  = PTHREAD_INHERIT_SCHED,  /**<�̳и��߳�ʹ�õĵ����㷨*/
};

/**  @brief �߳����� �̵߳������� */
enum thread_attr_scope{
	THREAD_SCOPE_SYSTEM  = PTHREAD_SCOPE_SYSTEM,  /**<Ĭ�� ������ϵͳ�ھ���CPU��Դ*/
	THREAD_SCOPE_PROCESS = PTHREAD_SCOPE_PROCESS, /**<��ͬһ�����ھ���CPU��Դ*/
};
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
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
thread_t
thread_create(u32_t module,
              threadAttr_t  *attr, 
              void *(*startRoutine)(void *), 
              void  * arg);



/**
* @brief ��ñ����߳�
* @retval ���ر����߳���Ϣ  ��ʾ�ɹ�
* @retval THREAD_NULL     ��ʾʧ��
*/
thread_t
thread_self(void);

/**
* @brief �����߳����
* @param [in] one  һ���߳�
* @param [in] another  ��һ���߳�
* @retval THREAD_SUCC  ��ʾ���
* @retval THREAD_FAIL  ��ʾ����
*/
int_t
thread_equal(thread_t const one, thread_t const another);

/**
* @brief �ڶ��̱߳�̻�����,�������û�����ڶ���߳��У�initRoutine()������ִ��һ��
* @param [in] state ���ñ��̶߳�Cancel�źŵķ�Ӧ ����������Ϊ-1
 - PTHREAD_CANCEL_ENABLE   �յ��źź���ΪCANCLED״̬(Ĭ��)
 - PTHREAD_CANCEL_DISABLE ����CANCEL�źż�������
* @param [in] type ���ñ��߳�ȡ��������ִ��ʱ�� ����������Ϊ-1
 - PTHREAD_CANCEL_DEFFERED    ����Cancel״̬ΪPTHREAD_CANCEL_ENABLEʱ��Ч,�յ��źź������������һ��ȡ�������˳�
 - PTHREAD_CANCEL_ASYCHRONOUS ����Cancel״̬ΪPTHREAD_CANCEL_ENABLEʱ��Ч,����ִ��ȡ���������˳���
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t 
thread_cancel_set(int_t state, int_t type);

/**
* @brief �����߳�ȡ����
* @param [out] exited  ���˳����߳�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t 
thread_cancel_point(void);

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
thread_cancel_some(thread_t exited);

/**
* @brief �����߳��˳�
* @attention�� �߳��˳��󣬲������ͷ��߳���Դ
* @param [out] retval  �߳��˳�����ֵ thread_join�������ֵ
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_exit_self( void * retval);

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
thread_join(Thread *thread, void **retval);

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
thread_detach(thread_t thread);

/**
* @brief �ڶ��̱߳�̻�����,�������û�����ڶ���߳��У�initRoutine()������ִ��һ��
* @param [in] onceControl ���Ʊ��� ����ʹ��PTHREAD_ONCE_INIT�꾲̬�س�ʼ����
* @param [in] initRoutine ��ʼ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t 
thread_once(pthread_once_t *onceControl, void (*initRoutine) (void));

/**
* @brief �ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] clean ���ڽ����Ծֲ�����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_localdata_create(pthread_key_t *key, void (*clean)(void*));

/**
* @brief �����ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] value �ֲ�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_set_localdata(pthread_key_t key, const void *value);

/**
* @brief ����ֲ߳̾�����
* @param [in] key �ֲ�����key
* @param [in] value �ֲ����ݵ�ַ
* @retval ����ֵ��ַ   ��ʾ�ɹ�
* @retval THREAD_FAIL ��ʾʧ��
*/
void *
thread_get_localdata(pthread_key_t key);

/**
* @brief ɾ���ֲ߳̾�����
* @param [in] key �ֲ�����key
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_delete_localdata(pthread_key_t key);
/**
* @brief �����źŵ�ĳ���߳� SIGSTOP SIGKILL �������ý���
* @param [in] thread  �߳�
* @param [in] sig  �ź�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_signal(Thread *thread, int sig);
/**
* @brief �㲥�ź�
* @param [in] sig  �ź�
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_broadcast_signal(int sig);
/**
* @brief �õ���ǰ�߳���
* @retval �����߳���  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
thread_get_number(void);
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/




#endif/*__INC_THREAD_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */