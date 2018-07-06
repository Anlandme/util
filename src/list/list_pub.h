/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: list_pub.h
@brief:对于链表模块的公共函数 公共宏定义
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

#ifndef __INC_LISTPUB_H__
#define __INC_LISTPUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "list.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 链表结构体 */
typedef ListHead list_head_t;
/**  @brief 链表节点结构体 */
typedef ListNode list_node_t;

/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化链表头
* @param [in] list   需要初始化的链表头
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t 
list_init( list_head_t *list );

/**
* @brief 获得当前成员数
* @param [in] list   需要链表头
* @retval 当前成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
list_get_current_member( list_head_t *list );

/**
* @brief 获得所有进入的成员数
* @param [in] list   需要链表头
* @retval 所有的成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
list_get_sum_member( list_head_t *list );

/**
* @brief 获得当前被删除成员数
* @param [in] list   需要链表头
* @retval 当前被删除成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
list_get_delete_member( list_head_t *list );
/**
* @brief 链表删除节点
* @param [in] list   链表头
* @param [in] node   需要删除的节点
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
* @attention 删除，如需释放内存需自行释放
*/
int_t 
list_delete_node( list_head_t *list , list_node_t *node );
/**
* @brief 链表添加节点到尾部
* @param [in] pattern 添加节点位置
  - INSERT_TO_HEAD        添加节点到首部
  - INSERT_TO_TAIL        添加节点到尾部
* @param [in] list   链表头
* @param [in] node   需要添加的节点
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t 
list_insert_node(u32_t pattern,  list_head_t *list , list_node_t *node );
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
list_embed_node(u32_t pattern, ListHead *list , ListNode *node, ListNode *embed);
/**
* @brief 链表追加
* @param [in] pattern 添加主链表位置
  - INSERT_TO_HEAD     添加主链表到首部
  - INSERT_TO_TAIL     添加主链表到尾部
* @param [in] main     主链表
* @param [in] children 子链表
* @param [in] sum 切割节点数量
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t
list_add_list(u32_t pattern, list_head_t *main, list_head_t *children, uint_t sum );
/**
* @brief 打印整个链表
* @param [in] list   链表头
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t
list_printf(list_head_t *list);
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/
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
#define list_takeout_struct(node, type, member) _list_takeout_struct(node, type, member)
/**
* @brief 获得链表头部节点
* @param [in] list   需要链表头
*/
#define list_get_head_node(list) ((list)->Head)
/**
* @brief 获得链表尾部节点
* @param [in] list   需要链表头
*/
#define list_get_tail_node(list) ((list)->Tail)
/**
* @brief 获得节点的下一个节点
* @param [in] node  需要节点
*/
#define list_get_next_node(node) ((node)->Next)
/**
* @brief 获得节点的上一个节点
* @param [in] node  需要节点
*/
#define list_get_prev_node(node) ((node)->Prev)
/**
* @brief 初始化节点
* @param [in] node   节点
*/
#define list_node_init(node) _list_node_init(node)

#endif/*__INC_LISTPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
