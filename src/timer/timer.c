/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer.c
@brief: 定时器的设计主要是基于POSIX定时器开发设计，主要是对于一些结构体的包装,使使用起来更加便捷
        有一个必须注意的是编译时候需要加 -lrt才能长成编译
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
#include "timer.h"
/**
* @brief 打开定时器
* @param [in]  timer   带初始化的定时器
* @param [in]  event   定时器事件
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t timer_open(Timer_t *timer, timerEvent_t *event)
{
    if(PARAMISNULL(timer)||PARAMISNULL(event))  RETURN_ERR(ERR_PARA, TIMER_FAIL);
    pthread_attr_t attr;
    struct sigevent sigEvent;
    sigEvent.sigev_notify = event->timerNotify;
    sigEvent.sigev_signo = event->timerSignal;
    memcpy(&sigEvent.sigev_value, &event->signalValue, sizeof(sigEvent.sigev_value));
    sigEvent.sigev_notify_function = event->callbackFunc;
    //初始化timer
    timer->stack = NULL;
    //如果信号处理为创建线程
    if( event->timerNotify == TIMER_NOTIFY_CALLBACK )
    	{
        pthread_attr_init(&attr);
        timer->stack = timer_malloc(TIMER_STACK_SIZE);
        pthread_attr_setstack(&attr, timer->stack, TIMER_STACK_SIZE);     //设置线程线程栈地址和大小
        sigEvent.sigev_notify_attributes = &attr;
        pthread_attr_destroy(&attr);
    }

    return  timer_create(event->clock, &sigEvent, &timer->timerid);
}


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
timer_start(Timer_t timer, int_t mode, int_t period, struct timespec *times)
{
    if(PARAMISNULL(times)) RETURN_ERR(ERR_PARA, TIMER_FAIL);
    if(period != TIMER_ONCE && TIMER_LOOP != period) RETURN_ERR(ERR_PARA, TIMER_FAIL);
    if(mode != TIMER_MODE_NORMAL && TIMER_MODE_ABSTIME != mode) RETURN_ERR(ERR_PARA, TIMER_FAIL);
    struct itimerspec value;
    if(TIMER_ONCE == period){
        value.it_interval.tv_sec = 0;
        value.it_interval.tv_nsec = 0;
    }else if(TIMER_LOOP == period){
        value.it_interval = *times;
    }

    value.it_value = *times;
    return timer_settime(timer.timerid, mode, &value, NULL);
}

/**
* @brief 停止定时器
* @param [in]  timer 定时器
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_stop(Timer_t timer)
{
    struct itimerspec value;
    value.it_value.tv_sec =0;
    value.it_value.tv_nsec =0;
    value.it_interval = value.it_value;  
    //停止timer 设置:new_value->it_value的的值为0(秒和纳秒都为0)
    return timer_settime(timer.timerid, 0, &value, NULL);
}


/**
* @brief 删除定时器
* @param [in]  timer 定时器
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_close(Timer_t *timer)
{
    if(PARAMISNULL(timer))  RETURN_ERR(ERR_PARA, TIMER_FAIL);
    if(timer->stack != NULL ) timer_free(timer->stack);
    return timer_delete(timer->timerid);
}


/**
* @brief 获得定时器周期时间和剩余时间
* @param [in]  timer 定时器
* @param [out]  period 周期时间
* @param [out]  remain 剩余时间
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_get_time(Timer_t timer, struct timespec *period, struct timespec *remain)
{
    if(PARAMISNULL(period)||PARAMISNULL(remain)) RETURN_ERR(ERR_PARA, TIMER_FAIL);
    struct itimerspec value;
    if(TIMER_SUCC != timer_gettime(timer.timerid, &value)){
        return TIMER_FAIL;
    }
    period->tv_sec  = value.it_interval.tv_sec;
    period->tv_nsec = value.it_interval.tv_nsec;
    remain->tv_sec  = value.it_value.tv_sec;
    remain->tv_nsec = value.it_value.tv_nsec;
    return TIMER_SUCC;
}



/**
* @brief 获得定时器超时次数 如果超限运行的次数等于或大于DELAYTIMER_MAX，则此调用会 返回DELAYTIMER_MAX。
* @param [in]  timer 定时器
* @param [in]  overSum 超时次数
* @retval TIMER_SUCC  表示成功
* @retval TIMER_FAIL  表示失败
*/
int_t
timer_overrun(Timer_t timer, int_t *overSum)
{
    if(PARAMISNULL(overSum)) RETURN_ERR(ERR_PARA, TIMER_FAIL);
    if(TIMER_FAIL == (*overSum = timer_getoverrun(timer.timerid))){
        return TIMER_FAIL;
    }
    return TIMER_SUCC;
}
