/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer.h
@brief:  timer 所有用到的宏
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

#ifndef __INC_TIMER_H__
#define __INC_TIMER_H__

/**  @brief 成功返回 */
#define TIMER_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define TIMER_FAIL FUNC_FAIL
/**  @brief 失败返回 */
#define TIMER_NULL FUNC_NULL

/**  @brief 内存申请函数 */
#define timer_malloc malloc
/**  @brief 内存释放函数 */
#define timer_free free
/**  @brief 默认栈大小 必须大于 PTHREAD_STACK_MIN 8192 */
#define TIMER_STACK_SIZE 10*1024 //10k

/**  @brief 定时器循环类型 */
enum timer_period {
    TIMER_ONCE = 0, /**< 一次性定时器 */
    TIMER_LOOP = 1, /**< 循环定时器 */
};

/**  @brief 定时器模式 */
enum timer_mode {
    TIMER_MODE_NORMAL  = 0, /**< 正常模式 */
    TIMER_MODE_ABSTIME = TIMER_ABSTIME, /**< 模式表现为时间直到下一次超时被设定的绝对时间和与timer_clock相联的时钟值的差值 */
};


/**  @brief 定时器初始化参数 */
typedef struct {
    timer_t timerid;  /**< 定时器 */
    void * stack;   /**< 线程栈 */
}TIMER;
/*
struct itimerval { 
    struct timeval it_interval; //下一次值
    struct timeval it_value;     //当前值
 }; 
 struct timeval { 
        long tv_sec;     //秒 
        long tv_usec;    //微妙
 };

 struct itimerval { 
    struct timespec it_interval; //下一次值
    struct timespec it_value;     //当前值
 }; 
struct timespec
{
    time_t tv_sec;       //秒
    long int tv_nsec;     //纳秒
};*/

#endif/*__INC_TIMER_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */