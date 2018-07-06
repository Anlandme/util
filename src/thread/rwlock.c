/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: rwlock.c
@brief: ��д������Ҫ������Դ��дʱ��ʹ�ã���Ҫ���ڱ����ʱ����Ҫ�ȴ��������
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
* @brief ��ʼ��һ����д��
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
rwlock_init(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_init(rwlock, NULL); 
}

/**
* @brief ����һ����д��
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_destroy(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_destroy(rwlock);
}

/**
* @brief ����
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_unlock(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_unlock (rwlock); 
}


/**
* @brief �������
* @param [in]  rwlock  ��д��
* @param [in]  operate  ��������
  - OPERATION_BLOCK  ��������
  - OPERATION_NON_BLOCK ����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_rdlock(thread_rwlock_t *rwlock, int_t operate)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK != operate && OPERATION_NON_BLOCK != operate)  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK == operate) 
        return pthread_rwlock_rdlock(rwlock); 
    else
        return pthread_rwlock_tryrdlock(rwlock); 
}


/**
* @brief ����д��
* @param [in]  rwlock  ��д��
* @param [in]  operate  ��������
  - OPERATION_BLOCK  ��������
  - OPERATION_NON_BLOCK ����������
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_wrlock(thread_rwlock_t *rwlock, int_t operate)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK != operate && OPERATION_NON_BLOCK != operate)  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK == operate) 
        return pthread_rwlock_wrlock(rwlock); 
    else
        return pthread_rwlock_trywrlock(rwlock); 
}
