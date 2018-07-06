/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: stack_pub.h
@brief:����ջģ��Ĺ������� �����궨��
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

#ifndef __INC_STACKPUB_H__
#define __INC_STACKPUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "list.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ���нṹ�� */
typedef ListHead queue_head_t;
/**  @brief ���нڵ�ṹ�� */
typedef ListNode queue_node_t;
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ������ͷ
* @param [in] list   ��Ҫ��ʼ���Ķ���ͷ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_init( queue_head_t *list );
/**
* @brief ��õ�ǰ���г�Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ��ǰ��Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_get_current_member( queue_head_t *list );

/**
* @brief ������н���Ķ��г�Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ���еĳ�Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_get_sum_member( queue_head_t *list );

/**
* @brief ��õ�ǰ��ɾ�����г�Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ��ǰ��ɾ����Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_get_delete_member( queue_head_t *list );
/**
* @brief ������ӽڵ㵽β��
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫ��ӵĽڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_add_node( queue_head_t *list , queue_node_t *node );

/**
* @brief �Ӷ���ȡ���ڵ�
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫȡ���Ľڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
queue_node_t *
queue_remove_node( queue_head_t *list );
/**
* @brief �����������β��
* @param [in] list   ����ͷ
* @param [in] add   ��Ҫ��ӵ�����
* @param [in] sum    ��ӵ�����ڵ���
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_add_list(queue_head_t *list, queue_head_t *add, uint_t sum );

/**
* @brief �Ӷ���ȡ������
* @param [in] list   ����ͷ
* @param [in] move   ��Ҫ���ӵ�����
* @param [in] sum    ���ӵ�����ڵ���
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
queue_remove_list(queue_head_t *list, queue_head_t *move, uint_t sum );

/**
* @brief ��ӡ��������
* @param [in] list   ����ͷ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t
queue_printf(list_head_t *list);
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
#define queue_takeout_struct(node, type, member) _list_takeout_struct(node, type, member)
/**
* @brief ��ʼ���ڵ�
* @param [in] node   �ڵ�
*/
#define queue_node_init(node) _list_node_init(node)

#endif/*__INC_STACKPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */