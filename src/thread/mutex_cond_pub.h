/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: MUTEX_cond_pub.h
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

#ifndef __INC_MUTEX_COND_PUB_H__
#define __INC_MUTEX_COND_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "thread.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ������ */
typedef pthread_mutex_t thread_mutex_t;
/**  @brief �������� */
typedef pthread_cond_t thread_cond_t;

/**  @brief ���������� */
typedef struct{
    int_t shared;          /**< ������������������ */
    int_t type;            /**< ���������������� */
    int_t protocol;        /**< ���������Ե�Э�� */
    int_t attrPrioceiling; /**< ���������Ե����ȼ����� */
    int_t prioceiling;     /**< �����������ȼ����� */
    int_t robustness;      /**< ��������ǿ������ */
}thread_mutexattr_t;

/**  @brief ������������������� */
enum mutexattr_shared {
    MUTEXATTR_PROCESS_SHARED  = PTHREAD_PROCESS_SHARED,  /**< ��������е��߳�֮�乲������*/
    MUTEXATTR_PROCESS_PRIVATE = PTHREAD_PROCESS_PRIVATE, /**< ͬһ�����̴������̲߳��ܹ�����û�����*/
};

/**  @brief �������������Ͳ��� */
enum mutexattr_type {
    MUTEXATTR_MUTEX_NORMAL     = PTHREAD_MUTEX_NORMAL,     /**< �����͵Ļ���������������������߳��ڲ����Ƚ���������������
                                                                �������������û����������������������Խ���������߳������Ļ�
                                                                �����������ȷ������Ϊ��������Խ�������Ļ�����δ����������
                                                                ����ȷ������Ϊ��*/
    
    MUTEXATTR_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK, /**< �����͵Ļ��������ṩ�����顣����߳��ڲ����Ƚ��������������
                                                                ����³������������û���������᷵�ش�������̳߳��Խ������
                                                                �Ļ������Ѿ��������߳���������᷵�ش�������̳߳��Խ������
                                                                �Ļ�����δ��������᷵�ش���*/
    
    MUTEXATTR_MUTEX_RECURSIVE  = PTHREAD_MUTEX_RECURSIVE,  /**< ����߳��ڲ����Ƚ������������������³������������û���������
                                                                �ɳɹ������û������� �� PTHREAD_MUTEX_NORMAL ���͵Ļ�������ͬ
                                                                ���Դ����ͻ�����������������ʱ�������������������������������
                                                                Ҫ������ͬ�����Ľ�������ſ����ͷŸ�����Ȼ�������̲߳��ܻ�ȡ�û�
                                                                ����������̳߳��Խ�������Ļ������Ѿ��������߳���������᷵�ش�
                                                                �� ����̳߳��Խ�������Ļ�����δ��������᷵�ش���*/

    MUTEXATTR_MUTEX_DEFAULT    = PTHREAD_MUTEX_DEFAULT,    /**< ��������Եݹ鷽ʽ���������͵Ļ���������������ȷ������Ϊ������
                                                                �����ɵ����߳������Ĵ����ͻ�������������Զ������������������
                                                                ��ȷ������Ϊ��������δ�����Ĵ����ͻ�������������Զ������������
                                                                Ҳ�������ȷ������Ϊ��������ʵ���н��û�����ӳ�䵽��������������
                                                                ֮һ��*/
};

/**  @brief ����������Э����� */
//enum mutexattr_protocol {
//    MUTEXATTR_PRIO_NONE    = PTHREAD_PRIO_NONE,     /**< �̵߳����ȼ��͵��Ȳ����ܵ�������ӵ��Ȩ��Ӱ��*/
//    MUTEXATTR_PRIO_PROTECT = PTHREAD_PRIO_PROTECT,  /**< �߳�ӵ��һ������ʹ��������Գ�ʼ���Ļ�����ʱ����Э��ֵ��Ӱ�������̵߳����ȼ��͵��ȡ�*/
//    MUTEXATTR_PRIO_INHERIT = PTHREAD_PRIO_INHERIT,  /**< ��Э��ֵ���� thrd1����Ӱ���̵߳����ȼ��͵��ȡ�����������ȼ����߳��� thrd1 ��ӵ�е�һ
//                                                         ��������������������������Щ���������� PTHREAD_PRIO_INHERIT ��ʼ���ģ��� thrd1 ��
//                                                         �Ը����������ȼ������������ڵȴ���Щ����������Щ�������� thrd1 ָ��ӵ�еĻ�����������
//                                                         �̵�������ȼ����С���� thrd1 ����һ���߳� (thrd3) ӵ�еĻ�������������������ͬ����
//                                                         �ȼ��̳�ЧӦ���Եݹ鷽ʽ������ thrd3��*/
//};

/**  @brief ��������ǿ������ */
enum mutexattr_robustness {
    MUTEXATTR_MUTEX_ROBUST_NP  = PTHREAD_MUTEX_ROBUST_NP,  /**< Ĭ�� ���������������ʧ�ܣ����Ժ�� mutex_lock() �����е��ý��Բ�ȷ���ķ�ʽ������*/
    MUTEXATTR_MUTEX_STALLED_NP = PTHREAD_MUTEX_STALLED_NP, /**< ������������ʧ��ʱ�������������û�����������������һ����������ȡ�û����� */
};

/**  @brief ���������Ա�־ */
enum mutex_attr_flag{
  MUTEX_ATTR_SHARED          = 1<<0, /**< ������������������    */
  MUTEX_ATTR_TYPE            = 1<<1, /**< ����������������     */
  MUTEX_ATTR_PROTOCOL        = 1<<2, /**< ���������Ե�Э��     */
  MUTEX_ATTR_ATTRPRIOCEILING = 1<<3, /**< ���������Ե����ȼ�����  */
  MUTEX_ATTR_PRIOCEILING     = 1<<4, /**< �����������ȼ�����    */
  MUTEX_ATTR_ROBUSTNESS      = 1<<5, /**< ��������ǿ������ */
};
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/

/**
* @brief ����һ���߳�
* @param [in]  flag   �߳����Ա�־ @see thread_attr_flag
* @param [out] attr �߳�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
//int_t
//mutex_get_attr(const int_t flag, threadAttr_t  *attr);

/**
* @brief ����һ��������
* @param [in]  mutex  ������
* @param [in]  flag   �̻߳��������Ա�־ @see mutex_attr_flag
* @param [out] mutexattr  ����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_init(thread_mutex_t *mutex, const int_t flag, const thread_mutexattr_t *mutexattr);

/**
* @brief ����һ��������
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_destroy(thread_mutex_t *mutex);

/**
* @brief ������ ����
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_lock(thread_mutex_t *mutex);

/**
* @brief ������ ����
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_unlock(thread_mutex_t *mutex);

/**
* @brief ����һ����������
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
cond_init(thread_cond_t *cond);

/**
* @brief ����һ����������
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_destroy(thread_cond_t *cond);

/**
* @brief �ȴ������ź�
* @param [in]  cond  ��������
* @param [in]  mutex ������
* @param [in]  time  �ȴ�ʱ�� һֱ�ȴ�����NULL
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_wait(thread_cond_t *cond, thread_mutex_t *mutex, struct timespec *time);


/**
* @brief ��ȴ������������ź�
* @note ���ݸ��ȴ��߳����ȼ��ĸߵ�ȷ���ĸ��߳̽��յ��źſ�ʼ����ִ�С�������߳����ȼ���ͬ��
        ����ݵȴ�ʱ��ĳ�����ȷ���ĸ��̻߳���źš����������һ ��cond_signal������෢��һ�Ρ�
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_signal(thread_cond_t *cond);


/**
* @brief ��ȴ����������������ź�
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_broadcast(thread_cond_t *cond);
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/




#endif/*__INC_MUTEX_COND_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */