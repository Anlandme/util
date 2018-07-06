/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: queue_pub.h
@brief:对于队列模块的公共函数 公共宏定义
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

#ifndef __INC_QUEUEPUB_H__
#define __INC_QUEUEPUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "list.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 栈结构体 */
typedef ListHead stack_head_t;
/**  @brief 栈节点结构体 */
typedef ListNode stack_node_t;
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化栈
* @param [in] list   需要初始化的栈
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t 
stack_init( stack_head_t *list );
/**
* @brief 获得当前栈成员数
* @param [in] list   需要栈
* @retval 当前成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
satck_get_current_member( stack_head_t *list );

/**
* @brief 获得所有进入的成员数
* @param [in] list   需要栈
* @retval 所有的成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
stack_get_sum_member( stack_head_t *list );

/**
* @brief 获得当前被删除成员数
* @param [in] list   需要的栈
* @retval 当前被删除成员数
* @retval FUNC_FAIL  表示失败
*/
int_t 
stack_get_delete_member( stack_head_t *list );
/**
* @brief 节点出栈
* @param [in] list   链表头
* @param [in] node   需要出栈的节点
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
stack_node_t *
stack_pop( stack_head_t *list );
/**
* @brief 节点入栈
* @param [in] list   链表头
* @param [in] node   需要入栈的节点
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t 
stack_push( stack_head_t *list , stack_node_t *node );
/**
* @brief 链表入栈
* @param [in] list   链表头
* @param [in] push   需要入栈的链表
* @param [in] sum    入栈的链表节点数
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t 
stack_push_list( stack_head_t *list, stack_head_t *push, uint_t sum );
/**
* @brief 链表出栈
* @param [in] list   链表头
* @param [in] pop   需要出栈的链表
* @param [in] sum    出栈的链表节点数
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t
stack_pop_list( stack_head_t *list, stack_head_t *pop, uint_t sum );
/**
* @brief 打印整个链表
* @param [in] list   链表头
* @retval FUNC_SUCC  表示成功
* @retval FUNC_FAIL  表示失败
*/
int_t
stack_printf(list_head_t *list);
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
*         struct example *tmp;
*         tmp = list_takeout_struct(&a.example_node, struct example, example_node);//tmp获得a的地址
* @endcode
* @attention (U32)(&((type *)0)->member)算出member在type中的位置偏移，node减去偏移就向左位移到结构体的首地址
*/
#define stack_takeout_struct(node, type, member) _list_takeout_struct(node, type, member)
/**
* @brief 初始化节点
* @param [in] node   节点
*/
#define stack_node_init(node) _list_node_init(node)

#endif/*__INC_STACKPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */