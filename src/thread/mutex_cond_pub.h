/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: MUTEX_cond_pub.h
@brief:  thread公共头文件
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

#ifndef __INC_MUTEX_COND_PUB_H__
#define __INC_MUTEX_COND_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "thread.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 互斥锁 */
typedef pthread_mutex_t thread_mutex_t;
/**  @brief 条件变量 */
typedef pthread_cond_t thread_cond_t;

/**  @brief 互斥锁属性 */
typedef struct{
    int_t shared;          /**< 互斥锁变量的作用域 */
    int_t type;            /**< 互斥锁的类型属性 */
    int_t protocol;        /**< 互斥锁属性的协议 */
    int_t attrPrioceiling; /**< 互斥锁属性的优先级上限 */
    int_t prioceiling;     /**< 互斥锁的优先级上限 */
    int_t robustness;      /**< 互斥锁的强健属性 */
}thread_mutexattr_t;

/**  @brief 互斥锁属性作用域参数 */
enum mutexattr_shared {
    MUTEXATTR_PROCESS_SHARED  = PTHREAD_PROCESS_SHARED,  /**< 多个进程中的线程之间共享互斥锁*/
    MUTEXATTR_PROCESS_PRIVATE = PTHREAD_PROCESS_PRIVATE, /**< 同一个进程创建的线程才能够处理该互斥锁*/
};

/**  @brief 互斥锁属性类型参数 */
enum mutexattr_type {
    MUTEXATTR_MUTEX_NORMAL     = PTHREAD_MUTEX_NORMAL,     /**< 此类型的互斥锁不会检测死锁。如果线程在不首先解除互斥锁的情况下
                                                                尝试重新锁定该互斥锁，则会产生死锁。尝试解除由其他线程锁定的互
                                                                斥锁会产生不确定的行为。如果尝试解除锁定的互斥锁未锁定，则会产
                                                                生不确定的行为。*/
    
    MUTEXATTR_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK, /**< 此类型的互斥锁可提供错误检查。如果线程在不首先解除锁定互斥锁的
                                                                情况下尝试重新锁定该互斥锁，则会返回错误。如果线程尝试解除锁定
                                                                的互斥锁已经由其他线程锁定，则会返回错误。如果线程尝试解除锁定
                                                                的互斥锁未锁定，则会返回错误。*/
    
    MUTEXATTR_MUTEX_RECURSIVE  = PTHREAD_MUTEX_RECURSIVE,  /**< 如果线程在不首先解除锁定互斥锁的情况下尝试重新锁定该互斥锁，则
                                                                可成功锁定该互斥锁。 与 PTHREAD_MUTEX_NORMAL 类型的互斥锁不同
                                                                ，对此类型互斥锁进行重新锁定时不会产生死锁情况。多次锁定互斥锁需
                                                                要进行相同次数的解除锁定才可以释放该锁，然后其他线程才能获取该互
                                                                斥锁。如果线程尝试解除锁定的互斥锁已经由其他线程锁定，则会返回错
                                                                误。 如果线程尝试解除锁定的互斥锁未锁定，则会返回错误。*/

    MUTEXATTR_MUTEX_DEFAULT    = PTHREAD_MUTEX_DEFAULT,    /**< 如果尝试以递归方式锁定此类型的互斥锁，则会产生不确定的行为。对于
                                                                不是由调用线程锁定的此类型互斥锁，如果尝试对它解除锁定，则会产生
                                                                不确定的行为。对于尚未锁定的此类型互斥锁，如果尝试对它解除锁定，
                                                                也会产生不确定的行为。允许在实现中将该互斥锁映射到其他互斥锁类型
                                                                之一。*/
};

/**  @brief 互斥锁属性协议参数 */
//enum mutexattr_protocol {
//    MUTEXATTR_PRIO_NONE    = PTHREAD_PRIO_NONE,     /**< 线程的优先级和调度不会受到互斥锁拥有权的影响*/
//    MUTEXATTR_PRIO_PROTECT = PTHREAD_PRIO_PROTECT,  /**< 线程拥有一个或多个使用这个属性初始化的互斥锁时，此协议值会影响其他线程的优先级和调度。*/
//    MUTEXATTR_PRIO_INHERIT = PTHREAD_PRIO_INHERIT,  /**< 此协议值（如 thrd1）会影响线程的优先级和调度。如果更高优先级的线程因 thrd1 所拥有的一
//                                                         个或多个互斥锁而被阻塞，而这些互斥锁是用 PTHREAD_PRIO_INHERIT 初始化的，则 thrd1 将
//                                                         以高于它的优先级或者所有正在等待这些互斥锁（这些互斥锁是 thrd1 指所拥有的互斥锁）的线
//                                                         程的最高优先级运行。如果 thrd1 因另一个线程 (thrd3) 拥有的互斥锁而被阻塞，则相同的优
//                                                         先级继承效应会以递归方式传播给 thrd3。*/
//};

/**  @brief 互斥锁的强健属性 */
enum mutexattr_robustness {
    MUTEXATTR_MUTEX_ROBUST_NP  = PTHREAD_MUTEX_ROBUST_NP,  /**< 默认 如果互斥锁的属主失败，则以后对 mutex_lock() 的所有调用将以不确定的方式被阻塞*/
    MUTEXATTR_MUTEX_STALLED_NP = PTHREAD_MUTEX_STALLED_NP, /**< 互斥锁的属主失败时，将会解除锁定该互斥锁。互斥锁的下一个属主将获取该互斥锁 */
};

/**  @brief 互斥锁属性标志 */
enum mutex_attr_flag{
  MUTEX_ATTR_SHARED          = 1<<0, /**< 互斥锁变量的作用域    */
  MUTEX_ATTR_TYPE            = 1<<1, /**< 互斥锁的类型属性     */
  MUTEX_ATTR_PROTOCOL        = 1<<2, /**< 互斥锁属性的协议     */
  MUTEX_ATTR_ATTRPRIOCEILING = 1<<3, /**< 互斥锁属性的优先级上限  */
  MUTEX_ATTR_PRIOCEILING     = 1<<4, /**< 互斥锁的优先级上限    */
  MUTEX_ATTR_ROBUSTNESS      = 1<<5, /**< 互斥锁的强健属性 */
};
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/

/**
* @brief 创建一个线程
* @param [in]  flag   线程属性标志 @see thread_attr_flag
* @param [out] attr 线程属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
//int_t
//mutex_get_attr(const int_t flag, threadAttr_t  *attr);

/**
* @brief 创建一个互斥锁
* @param [in]  mutex  互斥锁
* @param [in]  flag   线程互斥锁属性标志 @see mutex_attr_flag
* @param [out] mutexattr  互斥锁属性
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_init(thread_mutex_t *mutex, const int_t flag, const thread_mutexattr_t *mutexattr);

/**
* @brief 销毁一个互斥锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_destroy(thread_mutex_t *mutex);

/**
* @brief 互斥锁 上锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_lock(thread_mutex_t *mutex);

/**
* @brief 互斥锁 解锁
* @param [in]  mutex  互斥锁
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
mutex_unlock(thread_mutex_t *mutex);

/**
* @brief 创建一个条件变量
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
cond_init(thread_cond_t *cond);

/**
* @brief 销毁一个条件变量
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_destroy(thread_cond_t *cond);

/**
* @brief 等待条件信号
* @param [in]  cond  条件变量
* @param [in]  mutex 互斥锁
* @param [in]  time  等待时长 一直等待则置NULL
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_wait(thread_cond_t *cond, thread_mutex_t *mutex, struct timespec *time);


/**
* @brief 向等待的条件发送信号
* @note 根据各等待线程优先级的高低确定哪个线程接收到信号开始继续执行。如果各线程优先级相同，
        则根据等待时间的长短来确定哪个线程获得信号。但无论如何一 个cond_signal调用最多发信一次。
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_signal(thread_cond_t *cond);


/**
* @brief 向等待的所有条件发送信号
* @param [in]  cond  条件变量
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int 
cond_broadcast(thread_cond_t *cond);
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/




#endif/*__INC_MUTEX_COND_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */