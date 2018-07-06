/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: list.c
@brief:  链表模块 一个循环双向链表，这个模块框架只是实现简单的删除和添加，由于查询和遍历由于key的
        不一样，为了链表的灵活性所以不加入查询和遍历的功能，如有需要可以使用list_get_head_node 
        list_get_next_node等函数实现查询和遍历的功能，
        功能：
        1.统计链表当前节点数，总结点数 删除节点数
        图解
            （首部）--next--> <--prev--(A)--next--> <--prev--(尾部)
                    （首部）--prev--> <--next--(尾部)
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "list.h"

/**
* @brief 初始化链表头
* @param [in] list   需要初始化的链表头
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
list_init( ListHead *list )
{
    if(NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    list->Head      = NULL;
    list->Tail      = NULL;
    list->CurMember = 0;
    list->SumMember = 0;
    list->DelMember = 0;
    return LIST_SUCC;
}

/**
* @brief 获得当前成员数
* @param [in] list   需要链表头
* @retval 当前成员数
* @retval LIST_FAIL  表示失败
*/
int_t 
list_get_current_member( ListHead *list )
{
    if(NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list->CurMember;
}

/**
* @brief 获得所有的成员数
* @param [in] list   需要链表头
* @retval 所有的成员数
* @retval LIST_FAIL  表示失败
*/
int_t 
list_get_sum_member( ListHead *list )
{
    if(NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list->SumMember;
}

/**
* @brief 获得当前被删除成员数
* @param [in] list   需要的链表头
* @retval 当前被删除成员数
* @retval LIST_FAIL  表示失败
*/
int_t 
list_get_delete_member( ListHead *list )
{
    if(NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list->DelMember;
}

/**
* @brief 链表删除节点
* @param [in] list   链表头
* @param [in] node   需要删除的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
* @attention 删除，如需释放内存需自行释放
*/
int_t 
list_delete_node( ListHead *list , ListNode *node )
{
    if( NODEISNULL(node) || NODEISNULL(list) || NODEISNOEXIST(node) || LISTISNULL(list) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);

    if( list->Head == node && list->Tail == node )//被删除节点为最后一个节点
    {
        list->Head = NULL;
        list->Tail = NULL;
    }
    if( list->Head == node ) list->Head = node->Next;//被删除节点为头部时候
    if( list->Tail == node ) list->Tail = node->Prev;//被删除节点为尾部时候

    _list_del(node->Prev, node->Next);
    node->Next = NULL;
    node->Prev = NULL;

    list->CurMember--;
    list->DelMember++;
    return LIST_SUCC;
}

/**
* @brief 链表添加节点到尾部
* @param [in] pattern 添加节点位置
  - INSERT_TO_HEAD        添加节点到首部
  - INSERT_TO_TAIL        添加节点到尾部
* @param [in] list   链表头
* @param [in] node   需要添加的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t
list_insert_node(u32_t pattern,  ListHead *list , ListNode *node )
{
    if( NODEISNULL(node) || NODEISNULL(list) || NODEISEXIST(node) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( INSERT_TO_TAIL != pattern && INSERT_TO_HEAD != pattern ) RETURN_ERR( ERR_PARA ,LIST_FAIL);

    _list_init_node(node);//初始化节点，使节点前后指针都指向自己
    if( NULL == list->Head && NULL == list->Tail)//第一次加入节点
    {
        list->Head = node;
        list->Tail = node;
        goto success;
    }

    _list_add(node, list->Tail, list->Head);
    switch(pattern)
    {
        case INSERT_TO_HEAD: list->Head = node; break;
        case INSERT_TO_TAIL: list->Tail = node; break;
    }

    success:
    list->CurMember++;
    list->SumMember++;
    return LIST_SUCC;
}


/**
* @brief 嵌入节点
* @param [in] pattern 嵌入节点位置
  - INSERT_TO_HEAD        嵌入节点的首部(前面)
  - INSERT_TO_TAIL        嵌入节点的尾部(后面)
* @param [in] list   链表头
* @param [in] node   节点
* @param [in] insert 需要嵌入的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t
list_embed_node(u32_t pattern, ListHead *list, ListNode *node, ListNode *embed)
{
    if( NODEISNULL(node)||NODEISNULL(list)||LISTISNULL(list)||NODEISNULL(embed)||NODEISEXIST(embed) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( INSERT_TO_TAIL != pattern && INSERT_TO_HEAD != pattern ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    list_node_t *next, *prev;
    _list_init_node(embed);//初始化节点，使节点前后指针都指向自己
    switch(pattern)
    {
        case INSERT_TO_HEAD:next = node;prev = node->Prev;if( node == list->Head) list->Head = embed;break;
        case INSERT_TO_TAIL:next = node->Next;prev = node;if( node == list->Tail) list->Tail = embed;break;
    }
    _list_add(embed, prev, next); 
    success:
    list->CurMember++;
    list->SumMember++;
    return LIST_SUCC;
}

/**
* @brief 链表追加
* @param [in] pattern 添加主链表位置
  - INSERT_TO_HEAD     添加主链表到首部
  - INSERT_TO_TAIL     添加主链表到尾部
* @param [in] main     主链表
* @param [in] children 子链表
* @param [in] sum 切割节点数量
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t
list_add_list(u32_t pattern, ListHead *main, ListHead *children, uint_t sum )
{
    if( NODEISNULL(main) || NODEISNULL(children) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( INSERT_TO_TAIL != pattern && INSERT_TO_HEAD != pattern ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( children->CurMember < sum || 0 == sum ) RETURN_ERR( ERR_PARA ,LIST_FAIL);

    //头<---子--->尾 头<---主--->尾  或者   头<---主--->尾 头<---子--->尾 
    if( children->CurMember == sum ){//切割刚好等于子链表全部长度
        _list_combine(main->Head, main->Tail, children->Head, children->Tail, pattern);
        children->Head = NULL;
        children->Tail = NULL;
    }else{//切割小于子链表全部长度
        ListNode *tmpHead = children->Head;
        ListNode *tmpTail = children->Tail;
        switch(pattern)
        {
            case INSERT_TO_HEAD: _list_node_offset(children->Head,Next); tmpTail=(children->Head)->Prev; break;
            case INSERT_TO_TAIL: _list_node_offset(children->Tail,Prev); tmpHead=(children->Tail)->Next; break;
        }
        (children->Head)->Prev = children->Tail;
        (children->Tail)->Next = children->Head;
        tmpHead->Prev = tmpTail;
        tmpTail->Next = tmpHead;
         _list_combine(main->Head, main->Tail, tmpHead, tmpTail, pattern);
    }

    main->CurMember = main->CurMember + sum;
    main->SumMember = main->SumMember + sum;
    children->CurMember = children->CurMember - sum;
    children->DelMember = children->DelMember + sum;
    return LIST_SUCC;
}

/**
* @brief 打印整个链表
* @param [in] list   链表头
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t
list_printf(ListHead *list)
{
    if( NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    list_node_t * tmp;
    LIST_PRINTF("head=%p\n", list->Head);
    tmp = list_get_head_node(list); 
    do{
        LIST_PRINTF("(next:%p<--node:%p-->prev:%p)\n", tmp->Next, tmp, tmp->Prev);
        tmp = list_get_next_node(tmp);
    }while(tmp != list_get_head_node(list));
    LIST_PRINTF("tail=%p\n", list->Tail);
    return LIST_SUCC;
}

/**
* @brief 队列添加节点到尾部
* @param [in] list   链表头
* @param [in] node   需要添加的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
queue_add_node( ListHead *list , ListNode *node )
{
    if( NODEISNULL(list)||NODEISNULL(node) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_insert_node(INSERT_TO_TAIL, list, node );
}

/**
* @brief 从队列取出节点
* @param [in] list   链表头
* @param [in] node   需要取出的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
ListNode *
queue_remove_node( ListHead *list )
{
    if( NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_NULL);
    if( LISTISNULL(list) ) return NULL;//队列为空

    ListNode *temp;
    temp = list->Head;
    list_delete_node(list, temp);
    return temp;
}

/**
* @brief 队列添加链表到尾部
* @param [in] list   链表头
* @param [in] add   需要入队的链表
* @param [in] sum    入队的链表节点数
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
queue_add_list(ListHead *list, ListHead *add, uint_t sum )
{
    if( NODEISNULL(list)||NODEISNULL(add) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( add->CurMember<sum ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_add_list(INSERT_TO_TAIL, list, add, sum );
}

/**
* @brief 从队列取出链表
* @param [in] list   链表头
* @param [in] move   需要出队的链表
* @param [in] sum    出队的链表节点数
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
queue_remove_list(ListHead *list, ListHead *move, uint_t sum )
{
    if( NODEISNULL(list)) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( LISTISNULL(list) || list->CurMember<sum ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_add_list(INSERT_TO_HEAD, move, list, sum );
}

/**
* @brief 节点入栈
* @param [in] list   链表头
* @param [in] node   需要入栈的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
stack_push( ListHead *list , ListNode *node )
{
    if( NODEISNULL(list)||NODEISNULL(node) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_insert_node(INSERT_TO_HEAD, list, node );
}

/**
* @brief 节点出栈
* @param [in] list   链表头
* @param [in] node   需要出栈的节点
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
ListNode *
stack_pop( ListHead *list )
{
    if( NODEISNULL(list) ) RETURN_ERR( ERR_PARA ,LIST_NULL);
    if( LISTISNULL(list) ) return NULL;//栈为空
    ListNode *temp;
    temp = list->Tail;
    list_delete_node(list, temp);
    return temp;
}

/**
* @brief 链表入栈
* @param [in] list   链表头
* @param [in] push   需要入栈的链表
* @param [in] sum    入栈的链表节点数
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t 
stack_push_list(ListHead *list, ListHead *push, uint_t sum )
{
    if( NODEISNULL(list)||NODEISNULL(push) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( push->CurMember<sum ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_add_list(INSERT_TO_HEAD, list, push, sum );
}

/**
* @brief 链表出栈
* @param [in] list   链表头
* @param [in] pop   需要出栈的链表
* @param [in] sum    出栈的链表节点数
* @retval LIST_SUCC  表示成功
* @retval LIST_FAIL  表示失败
*/
int_t
stack_pop_list( ListHead *list, ListHead *pop, uint_t sum )
{
    if( NODEISNULL(list)||NODEISNULL(pop) ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    if( LISTISNULL(list) || list->CurMember<sum ) RETURN_ERR( ERR_PARA ,LIST_FAIL);
    return list_add_list(INSERT_TO_HEAD, pop, list, sum );
}

