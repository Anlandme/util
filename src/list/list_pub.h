/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: list_pub.h
@brief:��������ģ��Ĺ������� �����궨��
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

#ifndef __INC_LISTPUB_H__
#define __INC_LISTPUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "list.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ����ṹ�� */
typedef ListHead list_head_t;
/**  @brief ����ڵ�ṹ�� */
typedef ListNode list_node_t;

/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ������ͷ
* @param [in] list   ��Ҫ��ʼ��������ͷ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
list_init( list_head_t *list );

/**
* @brief ��õ�ǰ��Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ��ǰ��Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
list_get_current_member( list_head_t *list );

/**
* @brief ������н���ĳ�Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ���еĳ�Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
list_get_sum_member( list_head_t *list );

/**
* @brief ��õ�ǰ��ɾ����Ա��
* @param [in] list   ��Ҫ����ͷ
* @retval ��ǰ��ɾ����Ա��
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
list_get_delete_member( list_head_t *list );
/**
* @brief ����ɾ���ڵ�
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫɾ���Ľڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
* @attention ɾ���������ͷ��ڴ��������ͷ�
*/
int_t 
list_delete_node( list_head_t *list , list_node_t *node );
/**
* @brief ������ӽڵ㵽β��
* @param [in] pattern ��ӽڵ�λ��
  - INSERT_TO_HEAD        ��ӽڵ㵽�ײ�
  - INSERT_TO_TAIL        ��ӽڵ㵽β��
* @param [in] list   ����ͷ
* @param [in] node   ��Ҫ��ӵĽڵ�
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t 
list_insert_node(u32_t pattern,  list_head_t *list , list_node_t *node );
/**
* @brief Ƕ��ڵ�
* @param [in] pattern Ƕ��ڵ�λ��
  - INSERT_TO_HEAD        Ƕ��ڵ���ײ�(ǰ��)
  - INSERT_TO_TAIL        Ƕ��ڵ��β��(����)
* @param [in] list   ����ͷ
* @param [in] node   �ڵ�
* @param [in] insert ��ҪǶ��Ľڵ�
* @retval LIST_SUCC  ��ʾ�ɹ�
* @retval LIST_FAIL  ��ʾʧ��
*/
int_t
list_embed_node(u32_t pattern, ListHead *list , ListNode *node, ListNode *embed);
/**
* @brief ����׷��
* @param [in] pattern ���������λ��
  - INSERT_TO_HEAD     ����������ײ�
  - INSERT_TO_TAIL     ���������β��
* @param [in] main     ������
* @param [in] children ������
* @param [in] sum �и�ڵ�����
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t
list_add_list(u32_t pattern, list_head_t *main, list_head_t *children, uint_t sum );
/**
* @brief ��ӡ��������
* @param [in] list   ����ͷ
* @retval FUNC_SUCC  ��ʾ�ɹ�
* @retval FUNC_FAIL  ��ʾʧ��
*/
int_t
list_printf(list_head_t *list);
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
*         list_node_t *node = &a.example_node;
*         struct example *tmp;
*         tmp = list_takeout_struct(node, struct example, example_node);//tmp���a�ĵ�ַ
* @endcode
* @attention (U32)(&((type *)0)->member)���member��type�е�λ��ƫ�ƣ�node��ȥƫ�ƾ�����λ�Ƶ��ṹ����׵�ַ
*/
#define list_takeout_struct(node, type, member) _list_takeout_struct(node, type, member)
/**
* @brief �������ͷ���ڵ�
* @param [in] list   ��Ҫ����ͷ
*/
#define list_get_head_node(list) ((list)->Head)
/**
* @brief �������β���ڵ�
* @param [in] list   ��Ҫ����ͷ
*/
#define list_get_tail_node(list) ((list)->Tail)
/**
* @brief ��ýڵ����һ���ڵ�
* @param [in] node  ��Ҫ�ڵ�
*/
#define list_get_next_node(node) ((node)->Next)
/**
* @brief ��ýڵ����һ���ڵ�
* @param [in] node  ��Ҫ�ڵ�
*/
#define list_get_prev_node(node) ((node)->Prev)
/**
* @brief ��ʼ���ڵ�
* @param [in] node   �ڵ�
*/
#define list_node_init(node) _list_node_init(node)

#endif/*__INC_LISTPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
