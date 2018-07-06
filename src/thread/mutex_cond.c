/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: mutex_cond.c
@brief: 互斥锁：是一种用于多线程编程中，防止两条线程同时对同一公共资源（比如全局变量）进
        行读写的机制。
        条件变量：是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线
        程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。为
        了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。
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
* @brief 获得互斥锁属性
* @param [in]  flag   线程互斥锁属性标志 @see mutex_attr_flag
* @param [out] attr 线程属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
/*int_t
mutex_get_attr(const int_t flag, threadAttr_t  *attr)
{
    //int pthread_mutexattr_getpshared(pthread_mutexattr_t *mattr, int *pshared); 获取互斥锁的范围
    //int pthread_mutexattr_gettype(pthread_mutexattr_t  *attr , int  *type); 获取互斥锁的类型属性
    //int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol) 获取互斥锁属性的协议
    //int pthread_mutexattr_getprioceiling(const pthread_mutexatt_t *attr, int *prioceiling); 获取互斥锁属性的优先级上限
    //int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling);获取互斥锁的优先级上限
    //int pthread_mutexattr_getrobust_np(const pthread_mutexattr_t *attr, int *robustness);获 取互斥锁的强健属性

}*/

/**
* @brief 创建一个互斥锁
* @param [in]  mutex  互斥锁
* @param [in]  flag   线程互斥锁属性标志 @see mutex_attr_flag
* @param [out] mutexattr  互斥锁属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_init(thread_mutex_t *mutex, const int_t flag, const thread_mutexattr_t *mutexattr)
{
    if(PARAMISNULL(mutex))  RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(flag!=0 && PARAMISNULL(mutexattr))  RETURN_ERR(ERR_PARA, THREAD_FAIL);

    pthread_mutexattr_t mattr;
    if(THREAD_SUCC != pthread_mutexattr_init(&mattr)) RETURN_ERR(ERR_PARA, THE_ERRNO);
    if(MUTEX_ATTR_SHARED&flag)          pthread_mutexattr_setpshared(&mattr, mutexattr->shared);                    //可用来设置互斥锁变量的作用域
    if(MUTEX_ATTR_TYPE&flag)            pthread_mutexattr_settype(&mattr, mutexattr->type);                         //设置互斥锁类型的属性
    if(MUTEX_ATTR_PROTOCOL&flag)        pthread_mutexattr_setprotocol(&mattr, mutexattr->protocol);                 //设置互斥锁属性的协议
    if(MUTEX_ATTR_ATTRPRIOCEILING&flag) pthread_mutexattr_setprioceiling(&mattr, mutexattr->attrPrioceiling);       //设置互斥锁属性的优先级上限
    if(MUTEX_ATTR_ROBUSTNESS&flag)      pthread_mutexattr_setrobust_np(&mattr, mutexattr->robustness);              //设置互斥锁的强健属性

    if(THREAD_SUCC != pthread_mutex_init(mutex, &mattr)){ //初始化锁
        pthread_mutexattr_destroy(&mattr);
        RETURN_ERR(ERR_PARA, THE_ERRNO);
    }
    if(MUTEX_ATTR_PRIOCEILING&flag)     pthread_mutex_setprioceiling(mutex, mutexattr->prioceiling, NULL);         //设置互斥锁的优先级上限
    pthread_mutexattr_destroy(&mattr);
    return THREAD_SUCC;
}

/**
* @brief 销毁一个互斥锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_destroy(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_destroy(mutex); //毁锁
}

/**
* @brief 互斥锁 上锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_lock(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_lock(mutex); //加锁
}

/**
* @brief 互斥锁 解锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_unlock(thread_mutex_t *mutex)
{
    if(PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_mutex_unlock(mutex); //解锁
}



/**
* @brief 创建一个条件变量
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
cond_init(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_init(cond, NULL);
}

/**
* @brief 销毁一个条件变量
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_destroy(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_destroy(cond);
}

/**
* @brief 等待条件信号
* @param [in]  cond  条件变量
* @param [in]  mutex 互斥锁
* @param [in]  time  等待时长 一直等待则置NULL
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_wait(thread_cond_t *cond, thread_mutex_t *mutex, struct timespec *times)
{
    if(PARAMISNULL(cond)||PARAMISNULL(mutex)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    if(NULL == times)
        return pthread_cond_wait(cond, mutex);
    else
        return pthread_cond_timedwait(cond, mutex, times);
}


/**
* @brief 向等待的条件发送信号
* @note 根据各等待线程优先级的高低确定哪个线程接收到信号开始继续执行。如果各线程优先级相同，
        则根据等待时间的长短来确定哪个线程获得信号。但无论如何一 个cond_signal调用最多发信一次。
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_signal(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_signal(cond);
}


/**
* @brief 向等待的所有条件发送信号
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_broadcast(thread_cond_t *cond)
{
    if(PARAMISNULL(cond)) RETURN_ERR(ERR_PARA, THREAD_FAIL);
    return pthread_cond_broadcast(cond);
}



