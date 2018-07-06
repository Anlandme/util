/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer_pub.h
@brief:  timer公共头文件
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
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_TIMER_PUB_H__
#define __INC_TIMER_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "timer.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 定时器 */
typedef TIMER Timer_t;

/**  @brief 定时器所用的时钟 */
enum timer_clock {
    TIMER_REALTIME           = CLOCK_REALTIME,           /**< 系统当前时间，从1970年1.1日算起 */
    TIMER_MONOTONIC          = CLOCK_MONOTONIC,          /**< 系统的启动时间，不能被设置 */
    TIMER_PROCESS_CPUTIME_ID = CLOCK_PROCESS_CPUTIME_ID, /**< 进程运行时间 */
    TIMER_THREAD_CPUTIME_ID  = CLOCK_THREAD_CPUTIME_ID,  /**< 线程运行时间 */
    //TIMER_REALTIME_HR        = CLOCK_REALTIME_HR,        /**< CLOCK_REALTIME的高精度版本  */
    //TIMER_MONOTONIC_HR       = CLOCK_MONOTONIC_HR,       /**< CLOCK_MONOTONIC的高精度版本 */
};

/**  @brief 定时器通知类型 */
enum timer_notify {
    TIMER_NOTIFY_NONE     = SIGEV_NONE,   /**< 什么都不做，只提供通过timer_gettime和timer_getoverrun查询超时信息 */
    TIMER_NOTIFY_SIGNAL   = SIGEV_SIGNAL, /**< 当定时器到期，内核会将timerSignal所指定的信号传送给进程。si_value会被设定会sigev_value */
    TIMER_NOTIFY_CALLBACK = SIGEV_THREAD, /**< 当定时器到期，内核会创建一个线程，并且让它执行callbackFunc，传入signalValue作为为一个参数。 */
};

/**  @brief 定时器初始化参数 */
typedef struct {
    clockid_t clock;                      /**< 定时器使用时钟源 */
    int_t timerNotify;                    /**< 信号通知的信号类型 */
    int_t timerSignal;                    /**< 定时器到时产生信号 */
    union sigval signalValue;             /**< 传入信号处理函数值 */
    void (*callbackFunc)(union sigval);   /**< 定时器回调函数 */
}timerEvent_t;


/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/

/**
* @brief 打开定时器
* @param [in]  timer   带初始化的定时器
* @param [in]  event   定时器事件
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_open(Timer_t* timer, timerEvent_t *event);

/**
* @brief 开始定时器
* @param [in]  timer  定时器
* @param [in]  mode 定时器模式
 - TIMER_MODE_NORMAL  正常模式
 - TIMER_MODE_ABSTIME 模式表现为时间直到下一次超时被设定的绝对时间和与timer_clock相联的时钟值的差值
* @param [in]  period 定时器周期
 - TIMER_ONCE 一次性定时器
 - TIMER_LOOP     循环定时器
* @param [in]  times  定时器时间
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_start(Timer_t timer, int_t mode, int_t period, struct timespec *times);

/**
* @brief 停止定时器
* @param [in]  timer 定时器
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_stop(Timer_t timer);

/**
* @brief 删除定时器
* @param [in]  timer 定时器
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_close(Timer_t *timer);

/**
* @brief 获得定时器周期时间和剩余时间
* @param [in]  timer 定时器
* @param [out]  period 周期时间
* @param [out]  remain 剩余时间
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_get_time(Timer_t timer, struct timespec *period, struct timespec *remain);

/**
* @brief 获得定时器超时次数 如果超限运行的次数等于或大于DELAYTIMER_MAX，则此调用会 返回DELAYTIMER_MAX。
* @param [in]  timer 定时器
* @param [in]  overSum 超时次数
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_overrun(Timer_t timer, int_t *overSum);

/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/

#endif /*__INC_TIMER_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */