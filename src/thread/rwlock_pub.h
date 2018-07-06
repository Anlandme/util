/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: rwlock_pub.h
@brief:  rwlock公共头文件
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

#ifndef __INC_RWLOCK_PUB_H__
#define __INC_RWLOCK_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 读写锁 */
typedef pthread_rwlock_t thread_rwlock_t;
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化一个读写锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
rwlock_init(thread_rwlock_t *rwlock);

/**
* @brief 销毁一个读写锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_destroy(thread_rwlock_t *rwlock);

/**
* @brief 解锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_unlock(thread_rwlock_t *rwlock);

/**
* @brief 申请读锁
* @param [in]  rwlock  读写锁
* @param [in]  operate  读锁操作
  - OPERATION_BLOCK  阻塞操作
  - OPERATION_NON_BLOCK 非阻塞操作
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_rdlock(thread_rwlock_t *rwlock, int_t operate);

/**
* @brief 申请写锁
* @param [in]  rwlock  读写锁
* @param [in]  operate  读锁操作
  - OPERATION_BLOCK  阻塞操作
  - OPERATION_NON_BLOCK 非阻塞操作
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_wrlock(thread_rwlock_t *rwlock, int_t operate);
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/




#endif/*__INC_RWLOCK_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */