/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: halloc_pub.h
@brief:  halloc����ͷ�ļ�
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

#ifndef __INC_HALLOC_PUB_H__
#define __INC_HALLOC_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "halloc.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief hallc ���ڴ��С Ĭ��150mb */
#define HALLOC_TOTAL_MEMORY 150*1024*1024
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ��gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gc_init(void);
/**
* @brief �ݻ�gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gc_destroy(void);
/**
* @brief ��ʼ��halloc
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
halloc_init(void);

/**
* @brief �ݻ�halloc
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
halloc_destroy(void);
/**
* @brief ��ȡ�ڴ�
* @param [out]  msize   �ڴ��С
* @param [out]  mtype   �ڴ�����
	- MEM_DYNAMIC ��̬�ڴ�
	- MEM_STATIC ��̬�ڴ�
	- MEM_THREAD_STACK �߳�ջ�ڴ�
* @param [out]  module  ģ������
* @param [out]  file    �ļ���
* @param [out]  func    ������
* @param [out]  line    �к�
* @retval �����ڴ�ָ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
void * 
hmalloc(uint_t msize, uint_t mtype, uint_t module, const char * file, const char * func, int_t line);
/**
* @brief ��ȡ�ڴ�
* @param [out]  msize   �ڴ��С
* @param [out]  mtype   �ڴ�����
	- MEM_DYNAMIC ��̬�ڴ�
	- MEM_STATIC ��̬�ڴ�
	- MEM_THREAD_STACK �߳�ջ�ڴ�
* @param [out]  module  ģ������
* @retval ����ָ��(void *)  ��ʾ�ɹ�
* @retval HALLOC_NULL       ��ʾʧ��
*/ 
#define halloc(msize, mtype, module) hmalloc(msize, mtype, module, __FILE__, __FUNCTION__,  __LINE__)
/**
* @brief �ͷ��ڴ�
* @param [in]  memory  �ͷ��ڴ�
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
hfree(void * memory);
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/




#endif/*__INC_HALLOC_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */