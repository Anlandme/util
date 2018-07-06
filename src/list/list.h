/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: list.h
@brief:对于链表模块的结构体定义
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/29
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/29  | 0.0.0.1   | D.C.P    | 创建文件             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_LIST_H__
#define __INC_LIST_H__

/**  @brief 成功返回 */
#define LIST_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define LIST_FAIL FUNC_FAIL
/**  @brief 失败空指针 */
#define LIST_NULL FUNC_NULL

/**  @brief 打印宏 */
#define LIST_PRINTF printf

/**  @brief 节点为空 */
#define NODEISNULL(node) (NULL == node)
/**  @brief 链表当前节点为空 */
#define LISTISNULL(list) (0 == list->CurMember)
/**  @brief 节点已存在 */
#define NODEISEXIST(node) (NULL != node->Next || NULL != node->Prev)
/**  @brief 节点不存在 */
#define NODEISNOEXIST(node) (NULL == node->Next && NULL == node->Prev)

/**  @brief 链表节点结构体 */
typedef struct listnode ListNode;
struct listnode{
    struct listnode *Next; /**< 指向链表下一个节点*/
    struct listnode *Prev; /**< 指向链表上一个节点*/
};

/**  @brief 链表结构体 */
typedef struct{
    u32_t               CurMember;       /**< 指向链表当前成员数*/
    u32_t               SumMember;       /**< 指向链表包括删除所有成员数*/
    u32_t               DelMember;       /**< 指向链表删除的成员数*/
    struct listnode     *Head;           /**< 指向链表首部*/
    struct listnode     *Tail;           /**< 指向链表尾部*/
}ListHead;

/**  @brief 插入模式 */
enum insert_pattern{
    INSERT_TO_HEAD = 1, /**< 添加节点到尾部*/
    INSERT_TO_TAIL = 2, /**< 添加节点到首部*/
};

/******************************************************************
*   Name：私有接口宏
*   Desc：内部调用的一些宏，主要是获得异常函数调用堆栈
******************************************************************/
/**  
* @brief 节点指位移
* @param [in] head 头节点
* @param [in] tail 尾节点
*/
#define _list_node_offset(node, offset)\
do{\
    int_t i;\
    for(i=0; i<sum; i++) node = node->offset;\
}while(0)

/**  
* @brief 链表合并 
* @param [in] pattern 添加主链表位置
  - INSERT_TO_HEAD     添加主链表到首部
  - INSERT_TO_TAIL     添加主链表到尾部
* @param [in] mainHead     主链表首部
* @param [in] mainTail     主链表尾部
* @param [in] childrenHead 子链表首部
* @param [in] childrenTail 子链表尾部
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
* @brief 初始化节点 
* @param [in] node 被初始化的节点
*/
#define _list_init_node(node)\
do{\
    node->Prev = node;\
    node->Next = node;\
}while(0)
/**  
* @brief 删除节点 
* @param [in] prev 被删除节点的上一个节点
* @param [in] next 被删除节点的下一个节点
*/
#define _list_del(prev, next)\
do{\
    next->Prev = prev;\
    prev->Next = next;\
}while(0)
/**  
* @brief 添加节点
* @param [in] newNode   添加的节点
* @param [in] prev      添加的节点的上一个节点
* @param [in] next      添加的节点的下一个节点
*/
#define _list_add(newNode, prev, next)\
do{\
    next->Prev = newNode;\
    newNode->Next = next;\
    newNode->Prev = prev;\
    prev->Next = newNode;\
}while(0)



/**  @brief 定义队列链表初始化*/
#define queue_init list_init
/**  @brief 定义栈链表初始化*/
#define stack_init list_init

/** @brief 获得当前队列大小 */
#define queue_get_current_member list_get_current_member
/** @brief 获得当前栈大小 */
#define stack_get_current_member list_get_current_member

/** @brief 获得队列大小总数 */
#define queue_get_current_member list_get_current_member
/** @brief 获得所有的栈成员总数 */
#define stack_get_sum_member list_get_sum_member

/** @brief 获得当前被删除队列节点数 */
#define queue_get_delete_member list_get_delete_member
/** @brief 获得当前被删除栈数 */
#define stack_get_delete_member list_get_delete_member

/** @brief 队列打印 */
#define queue_printf list_printf
/** @brief 栈打印 */
#define stack_printf list_printf

/**@brief 初始化节点*/
#define _list_node_init(node)\
do{\
    (node)->Next = NULL;\
    (node)->Prev = NULL;\
}while(0)

/**
* @brief 取出包含节点的结构体
* @param [in] node   链表结点
* @param [in] type   包含节点的结构体类型
* @param [in] member 节点在结构体的内定义的结构体名
* @par 例子
* @code
*       struct example {
*         int value;
*         list_node_t example_node
*       }
*  使用例子：
*         struct example a;
*         list_node_t *node = &a.example_node;
*         struct example *tmp;
*         tmp = list_takeout_struct(node, struct example, example_node);//tmp获得a的地址
* @endcode
* @attention (U32)(&((type *)0)->member)算出member在type中的位置偏移，node减去偏移就向左位移到结构体的首地址
*/
#define _list_takeout_struct(node, type, member) ((type *)((char_t*)(node)-(size_t)(&((type *)0)->member)))

#endif/*__INC_LIST_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
