/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: mutex_cond.c
@brief: ����������һ�����ڶ��̱߳���У���ֹ�����߳�ͬʱ��ͬһ������Դ������ȫ�ֱ�������
        �ж�д�Ļ��ơ�
        �����������������̼߳乲���ȫ�ֱ�������ͬ����һ�ֻ��ƣ���Ҫ��������������һ����
        �̵ȴ�"������������������"��������һ���߳�ʹ"��������"���������������źţ���Ϊ
        �˷�ֹ����������������ʹ�����Ǻ�һ�������������һ��
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

/**
* @brief ��û���������
* @param [in]  flag   �̻߳��������Ա�־ @see mutex_attr_flag
* @param [out] attr �߳�����
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
/*int_t
mutex_get_attr(const int_t flag, threadAttr_t  *attr)
{
    //int pthread_mutexattr_getpshared(pthread_mutexattr_t *mattr, int *pshared); ��ȡ�������ķ�Χ
    //int pthread_mutexattr_gettype(pthread_mutexattr_t  *attr , int  *type); ��ȡ����������������
    //int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol) ��ȡ���������Ե�Э��
    //int pthread_mutexattr_getprioceiling(const pthread_mutexatt_t *attr, int *prioceiling); ��ȡ���������Ե����ȼ�����
    //int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling);��ȡ�����������ȼ�����
    //int pthread_mutexattr_getrobust_np(const pthread_mutexattr_t *attr, int *robustness);�� ȡ��������ǿ������

}*/

/**
* @brief ����һ��������
* @param [in]  mutex  ������
* @param [in]  flag   �̻߳��������Ա�־ @see mutex_attr_flag
* @param [out] mutexattr  ����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_init(thread_mutex_t *mutex, const int_t flag, const thread_mutexattr_t *mutexattr)
{
    if(PARAMISNULL(mutex))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(flag!=0 && PARAMISNULL(mutexattr))  RETURN_ERR(ERR_PARA, THREAD_FAIL);

    pthread_mutexattr_t mattr;
    if(THREAD_SUCC != pthread_mutexattr_init(&mattr)) RETURN_ERR(ERR_PARA, THE_ERRNO);
    if(MUTEX_ATTR_SHARED&flag)          pthread_mutexattr_setpshared(&mattr, mutexattr->shared);                    //���������û�����������������
    if(MUTEX_ATTR_TYPE&flag)            pthread_mutexattr_settype(&mattr, mutexattr->type);                         //���û��������͵�����
    if(MUTEX_ATTR_PROTOCOL&flag)        pthread_mutexattr_setprotocol(&mattr, mutexattr->protocol);                 //���û��������Ե�Э��
    if(MUTEX_ATTR_ATTRPRIOCEILING&flag) pthread_mutexattr_setprioceiling(&mattr, mutexattr->attrPrioceiling);       //���û��������Ե����ȼ�����
    if(MUTEX_ATTR_ROBUSTNESS&flag)      pthread_mutexattr_setrobust_np(&mattr, mutexattr->robustness);              //���û�������ǿ������

    if(THREAD_SUCC != pthread_mutex_init(mutex, &mattr)){ //��ʼ����
        pthread_mutexattr_destroy(&mattr);
        RETURN_ERR(ERR_PARA, THE_ERRNO);
    }
    if(MUTEX_ATTR_PRIOCEILING&flag)     pthread_mutex_setprioceiling(mutex, mutexattr->prioceiling, NULL);         //���û����������ȼ�����
    pthread_mutexattr_destroy(&mattr);
    return THREAD_SUCC;
}

/**
* @brief ����һ��������
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_destroy(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_destroy(mutex); //����
}

/**
* @brief ������ ����
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_lock(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_lock(mutex); //����
}

/**
* @brief ������ ����
* @param [in]  mutex  ������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
mutex_unlock(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_unlock(mutex); //����
}



/**
* @brief ����һ����������
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
cond_init(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_init(cond, NULL);
}

/**
* @brief ����һ����������
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_destroy(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_destroy(cond);
}

/**
* @brief �ȴ������ź�
* @param [in]  cond  ��������
* @param [in]  mutex ������
* @param [in]  time  �ȴ�ʱ�� һֱ�ȴ�����NULL
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_wait(thread_cond_t *cond, thread_mutex_t *mutex, struct timespec *times)
{
    if(PARAMISNULL(cond)||PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(NULL == times)
        return pthread_cond_wait(cond, mutex);
    else
        return pthread_cond_timedwait(cond, mutex, times);
}


/**
* @brief ��ȴ������������ź�
* @note ���ݸ��ȴ��߳����ȼ��ĸߵ�ȷ���ĸ��߳̽��յ��źſ�ʼ����ִ�С�������߳����ȼ���ͬ��
        ����ݵȴ�ʱ��ĳ�����ȷ���ĸ��̻߳���źš����������һ ��cond_signal������෢��һ�Ρ�
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_signal(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_signal(cond);
}


/**
* @brief ��ȴ����������������ź�
* @param [in]  cond  ��������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
cond_broadcast(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_broadcast(cond);
}



