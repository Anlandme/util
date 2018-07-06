/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: halloc_pub.h
@brief:  halloc公共头文件
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
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_HALLOC_PUB_H__
#define __INC_HALLOC_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "halloc.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief hallc 总内存大小 默认150mb */
#define HALLOC_TOTAL_MEMORY 150*1024*1024
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gc_init(void);
/**
* @brief 摧毁gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gc_destroy(void);
/**
* @brief 初始化halloc
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
halloc_init(void);

/**
* @brief 摧毁halloc
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
halloc_destroy(void);
/**
* @brief 获取内存
* @param [out]  msize   内存大小
* @param [out]  mtype   内存类型
	- MEM_DYNAMIC 动态内存
	- MEM_STATIC 静态内存
	- MEM_THREAD_STACK 线程栈内存
* @param [out]  module  模块类型
* @param [out]  file    文件名
* @param [out]  func    函数名
* @param [out]  line    行号
* @retval 返回内存指针  表示成功
* @retval HALLOC_NULL  表示失败
*/
void * 
hmalloc(uint_t msize, uint_t mtype, uint_t module, const char * file, const char * func, int_t line);
/**
* @brief 获取内存
* @param [out]  msize   内存大小
* @param [out]  mtype   内存类型
	- MEM_DYNAMIC 动态内存
	- MEM_STATIC 静态内存
	- MEM_THREAD_STACK 线程栈内存
* @param [out]  module  模块类型
* @retval 返回指针(void *)  表示成功
* @retval HALLOC_NULL       表示失败
*/ 
#define halloc(msize, mtype, module) hmalloc(msize, mtype, module, __FILE__, __FUNCTION__,  __LINE__)
/**
* @brief 释放内存
* @param [in]  memory  释放内存
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
hfree(void * memory);
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/




#endif/*__INC_HALLOC_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */