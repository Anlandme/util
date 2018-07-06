/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: rwlock.c
@brief: 读写锁，主要是在资源读写时候使用，主要是在避免读时候需要等待锁的情况
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "thread.h"

/**
* @brief 初始化一个读写锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
rwlock_init(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_init(rwlock, NULL); 
}

/**
* @brief 销毁一个读写锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_destroy(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_destroy(rwlock);
}

/**
* @brief 解锁
* @param [in]  rwlock  读写锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
rwlock_unlock(thread_rwlock_t *rwlock)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_rwlock_unlock (rwlock); 
}


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
rwlock_rdlock(thread_rwlock_t *rwlock, int_t operate)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK != operate && OPERATION_NON_BLOCK != operate)  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK == operate) 
        return pthread_rwlock_rdlock(rwlock); 
    else
        return pthread_rwlock_tryrdlock(rwlock); 
}


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
rwlock_wrlock(thread_rwlock_t *rwlock, int_t operate)
{
    if(PARAMISNULL(rwlock))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK != operate && OPERATION_NON_BLOCK != operate)  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(OPERATION_BLOCK == operate) 
        return pthread_rwlock_wrlock(rwlock); 
    else
        return pthread_rwlock_trywrlock(rwlock); 
}
