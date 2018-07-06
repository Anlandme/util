/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: queue_pub.h
@brief:���ڶ���ģ��Ĺ������� �����궨��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/29
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/29  | 0.0.0.1   | D.C.P    | �����ļ�             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_QUEUEPUB_H__
#define __INC_QUEUEPUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "list.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ջ�ṹ�� */
typedef ListHead stack_head_t;
/**  @brief ջ�ڵ�ṹ�� */
typedef ListNode stack_node_t;
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ��ջ
* @param [in] list   ��Ҫ��ʼ����ջ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
stack_init( stack_head_t *list );
/**
* @brief ��õ�ǰջ��Ա��
* @param [in] list   ��Ҫջ
* @retval ��ǰ��Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
satck_get_current_member( stack_head_t *list );

/**
* @brief ������н���ĳ�Ա��
* @param [in] list   ��Ҫջ
* @retval ���еĳ�Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
stack_get_sum_member( stack_head_t *list );

/**
* @brief ��õ�ǰ��ɾ����Ա��
* @param [in] list   ��Ҫ��ջ
* @retval ��ǰ��ɾ����Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
stack_get_delete_member( stack_head_t *list );
/**
* @brief �ڵ��ջ
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫ��ջ�Ľڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
stack_node_t *
stack_pop( stack_head_t *list );
/**
* @brief �ڵ���ջ
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫ��ջ�Ľڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
stack_push( stack_head_t *list , stack_node_t *node );
/**
* @brief ������ջ
* @param [in] list   ����ͷ
* @param [in] push   ��Ҫ��ջ������
* @param [in] sum    ��ջ������ڵ���
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
stack_push_list( stack_head_t *list, stack_head_t *push, uint_t sum );
/**
* @brief �����ջ
* @param [in] list   ����ͷ
* @param [in] pop   ��Ҫ��ջ������
* @param [in] sum    ��ջ������ڵ���
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t
stack_pop_list( stack_head_t *list, stack_head_t *pop, uint_t sum );
/**
* @brief ��ӡ��������
* @param [in] list   ����ͷ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t
stack_printf(list_head_t *list);
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/
/**
* @brief ȡ�������ڵ�Ľṹ��
* @param [in] node   ������
* @param [in] type   �����ڵ�Ľṹ������
* @param [in] member �ڵ��ڽṹ����ڶ���Ľṹ����
* @par ����
* @code
*       struct example {
*         int value;
*         list_node_t example_node
*       }
*  ʹ�����ӣ�
*         struct example a;
*         struct example *tmp;
*         tmp = list_takeout_struct(&a.example_node, struct example, example_node);//tmp���a�ĵ�ַ
* @endcode
* @attention (U32)(&((type *)0)->member)���member��type�е�λ��ƫ�ƣ�node��ȥƫ�ƾ�����λ�Ƶ��ṹ����׵�ַ
*/
#define stack_takeout_struct(node, type, member) _list_takeout_struct(node, type, member)
/**
* @brief ��ʼ���ڵ�
* @param [in] node   �ڵ�
*/
#define stack_node_init(node) _list_node_init(node)

#endif/*__INC_STACKPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */