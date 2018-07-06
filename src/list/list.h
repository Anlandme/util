/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: list.h
@brief:��������ģ��Ľṹ�嶨��
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

#ifndef __INC_LIST_H__
#define __INC_LIST_H__

/**  @brief �ɹ����� */
#define LIST_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define LIST_FAIL FUNC_FAIL
/**  @brief ʧ�ܿ�ָ�� */
#define LIST_NULL FUNC_NULL

/**  @brief ��ӡ�� */
#define LIST_PRINTF printf

/**  @brief �ڵ�Ϊ�� */
#define NODEISNULL(node) (NULL == node)
/**  @brief ����ǰ�ڵ�Ϊ�� */
#define LISTISNULL(list) (0 == list->CurMember)
/**  @brief �ڵ��Ѵ��� */
#define NODEISEXIST(node) (NULL != node->Next || NULL != node->Prev)
/**  @brief �ڵ㲻���� */
#define NODEISNOEXIST(node) (NULL == node->Next && NULL == node->Prev)

/**  @brief ����ڵ�ṹ�� */
typedef struct listnode ListNode;
struct listnode{
    struct listnode *Next; /**< ָ��������һ���ڵ�*/
    struct listnode *Prev; /**< ָ��������һ���ڵ�*/
};

/**  @brief ����ṹ�� */
typedef struct{
    u32_t               CurMember;       /**< ָ������ǰ��Ա��*/
    u32_t               SumMember;       /**< ָ���������ɾ�����г�Ա��*/
    u32_t               DelMember;       /**< ָ������ɾ���ĳ�Ա��*/
    struct listnode     *Head;           /**< ָ�������ײ�*/
    struct listnode     *Tail;           /**< ָ������β��*/
}ListHead;

/**  @brief ����ģʽ */
enum insert_pattern{
    INSERT_TO_HEAD = 1, /**< ��ӽڵ㵽β��*/
    INSERT_TO_TAIL = 2, /**< ��ӽڵ㵽�ײ�*/
};

/******************************************************************
*   Name��˽�нӿں�
*   Desc���ڲ����õ�һЩ�꣬��Ҫ�ǻ���쳣�������ö�ջ
******************************************************************/
/**  
* @brief �ڵ�ָλ��
* @param [in] head ͷ�ڵ�
* @param [in] tail β�ڵ�
*/
#define _list_node_offset(node, offset)\
do{\
    int_t i;\
    for(i=0; i<sum; i++) node = node->offset;\
}while(0)

/**  
* @brief ����ϲ� 
* @param [in] pattern ���������λ��
  - INSERT_TO_HEAD     ����������ײ�
  - INSERT_TO_TAIL     ���������β��
* @param [in] mainHead     �������ײ�
* @param [in] mainTail     ������β��
* @param [in] childrenHead �������ײ�
* @param [in] childrenTail ������β��
*/
#define _list_combine(mainHead, mainTail, childrenHead, childrenTail, pattern)\
do{\
    if( mainHead == NULL && mainTail == NULL ){\
        mainHead = childrenHead;\
        mainTail = childrenTail;\
    }else{\
        mainHead->Prev = childrenTail;\
        childrenTail->Next = mainHead;\
        mainTail->Next = childrenHead;\
        childrenHead->Prev = mainTail;\
        switch(pattern)\
        {\
            case INSERT_TO_HEAD: mainHead = childrenHead; break;\
            case INSERT_TO_TAIL: mainTail = childrenTail; break;\
        }\
    }\
}while(0)
/**  
* @brief ��ʼ���ڵ� 
* @param [in] node ����ʼ���Ľڵ�
*/
#define _list_init_node(node)\
do{\
    node->Prev = node;\
    node->Next = node;\
}while(0)
/**  
* @brief ɾ���ڵ� 
* @param [in] prev ��ɾ���ڵ����һ���ڵ�
* @param [in] next ��ɾ���ڵ����һ���ڵ�
*/
#define _list_del(prev, next)\
do{\
    next->Prev = prev;\
    prev->Next = next;\
}while(0)
/**  
* @brief ��ӽڵ�
* @param [in] newNode   ��ӵĽڵ�
* @param [in] prev      ��ӵĽڵ����һ���ڵ�
* @param [in] next      ��ӵĽڵ����һ���ڵ�
*/
#define _list_add(newNode, prev, next)\
do{\
    next->Prev = newNode;\
    newNode->Next = next;\
    newNode->Prev = prev;\
    prev->Next = newNode;\
}while(0)



/**  @brief ������������ʼ��*/
#define queue_init list_init
/**  @brief ����ջ�����ʼ��*/
#define stack_init list_init

/** @brief ��õ�ǰ���д�С */
#define queue_get_current_member list_get_current_member
/** @brief ��õ�ǰջ��С */
#define stack_get_current_member list_get_current_member

/** @brief ��ö��д�С���� */
#define queue_get_current_member list_get_current_member
/** @brief ������е�ջ��Ա���� */
#define stack_get_sum_member list_get_sum_member

/** @brief ��õ�ǰ��ɾ�����нڵ��� */
#define queue_get_delete_member list_get_delete_member
/** @brief ��õ�ǰ��ɾ��ջ�� */
#define stack_get_delete_member list_get_delete_member

/** @brief ���д�ӡ */
#define queue_printf list_printf
/** @brief ջ��ӡ */
#define stack_printf list_printf

/**@brief ��ʼ���ڵ�*/
#define _list_node_init(node)\
do{\
    (node)->Next = NULL;\
    (node)->Prev = NULL;\
}while(0)

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
#define _list_takeout_struct(node, type, member) ((type *)((char_t*)(node)-(size_t)(&((type *)0)->member)))

#endif/*__INC_LIST_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
