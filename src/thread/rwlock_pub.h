/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: rwlock_pub.h
@brief:  rwlock����ͷ�ļ�
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

#ifndef __INC_RWLOCK_PUB_H__
#define __INC_RWLOCK_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ��д�� */
typedef pthread_rwlock_t thread_rwlock_t;
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ��һ����д��
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int_t
rwlock_init(thread_rwlock_t *rwlock);

/**
* @brief ����һ����д��
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_destroy(thread_rwlock_t *rwlock);

/**
* @brief ����
* @param [in]  rwlock  ��д��
* @retval THREAD_SUCC  ��ʾ�ɹ�
* @retval THREAD_FAIL  ��ʾʧ��
*/
int 
rwlock_unlock(thread_rwlock_t *rwlock);

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
rwlock_rdlock(thread_rwlock_t *rwlock, int_t operate);

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
rwlock_wrlock(thread_rwlock_t *rwlock, int_t operate);
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/




#endif/*__INC_RWLOCK_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */