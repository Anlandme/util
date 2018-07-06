/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thread_pub.h
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

#ifndef __INC_THREAD_PUB_H__
#define __INC_THREAD_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "thread.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 线程值 */
typedef Thread* thread_t;

/**  @brief 线程属性标志 */
enum thread_attr_flag{
  ATTR_DETACH_STATE        = 1<<0, /**< 线程的分离状态 */
  ATTR_SCHEDULE_POLICY     = 1<<1, /**< 线程调度策略  */
  ATTR_SCHEDULE_PARAM      = 1<<2, /**< 线程的调度参数 */
  ATTR_INHERIT_SCHEDULE    = 1<<3, /**< 线程的继承性  */
  ATTR_SCOPE               = 1<<4, /**< 线程的作用域  */
  ATTR_GUARD_SIZE          = 1<<5, /**< 线程栈末尾的警戒缓冲区大小 */
  ATTR_STACK_ADDR_AND_SIZE = 1<<6, /**< 线程栈的位置 */
//ATTR_STACK_ADDR_SET   = 1<<7, /**< 堆栈的地址集 */
};

/**  @brief 线程属性 线程的分离状态 */
enum thread_attr_detach{
	THREAD_DETACHED = PTHREAD_CREATE_DETACHED,  /**< 分离线程没有被其他的线程所等待，自己运行结束 马上释放系统资源*/
	THREAD_JOINABLE = PTHREAD_CREATE_JOINABLE,  /**< 非分离状态,默认原有的线程等待新的线程结束。只当thread_join函数
													 返回时，才能释放自己占用的系统资源*/
};

/**  @brief 线程属性 线程调度策略 THREAD_FIFO和THREAD_RR只有在root权限下才有效*/
enum thread_attr_policy{
	THREAD_FIFO  = SCHED_FIFO,   /**<循环策略 如果这些线程未被优先级更高的线程抢占，则会继续处理该线程，直到该线程放弃或阻塞为止。*/
	THREAD_RR    = SCHED_RR,     /**<先入先出策略 每个线程都有一个执行时间配额 在系统确定的时间段内将一直执行这些线程*/
	THREAD_OTHER = SCHED_OTHER,  /**<自定义策略 线程默认使用 SCHED_OTHER 调度策略。线程一旦开始运行，直到被抢占或者直到线程阻塞或停止为止。*/
};

/**  @brief 线程属性 线程的继承性 */
enum thread_attr_schedule{
	THREAD_EXPLICIT_SCHED = PTHREAD_EXPLICIT_SCHED, /**<默认 使用结构pthread_attr_t指定的调度算法*/
	THREAD_INHERIT_SCHED  = PTHREAD_INHERIT_SCHED,  /**<继承父线程使用的调度算法*/
};

/**  @brief 线程属性 线程的作用域 */
enum thread_attr_scope{
	THREAD_SCOPE_SYSTEM  = PTHREAD_SCOPE_SYSTEM,  /**<默认 在整个系统内竞争CPU资源*/
	THREAD_SCOPE_PROCESS = PTHREAD_SCOPE_PROCESS, /**<在同一进程内竞争CPU资源*/
};
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 创建一个线程
* @param [out] thread          线程号
* @param [out] module          模块类型
* @param [in]  attr            线程属性
* @param [in]  start_routine   线程函数
* @param [in]  arg             线程函数参数
* @retval 返回线程  表示成功
* @retval THREAD_NULL 表示失败
*/
thread_t
thread_create(u32_t module,
              threadAttr_t  *attr, 
              void *(*startRoutine)(void *), 
              void  * arg);



/**
* @brief 获得本身线程
* @retval 返回本身线程信息  表示成功
* @retval THREAD_NULL     表示失败
*/
thread_t
thread_self(void);

/**
* @brief 两个线程相等
* @param [in] one  一个线程
* @param [in] another  另一个线程
* @retval THREAD_SUCC  表示相等
* @retval THREAD_FAIL  表示不等
*/
int_t
thread_equal(thread_t const one, thread_t const another);

/**
* @brief 在多线程编程环境下,函数调用会出现在多个线程中，initRoutine()函数仅执行一次
* @param [in] state 设置本线程对Cancel信号的反应 不配置设置为-1
 - PTHREAD_CANCEL_ENABLE   收到信号后设为CANCLED状态(默认)
 - PTHREAD_CANCEL_DISABLE 忽略CANCEL信号继续运行
* @param [in] type 设置本线程取消动作的执行时机 不配置设置为-1
 - PTHREAD_CANCEL_DEFFERED    仅当Cancel状态为PTHREAD_CANCEL_ENABLE时有效,收到信号后继续运行至下一个取消点再退出
 - PTHREAD_CANCEL_ASYCHRONOUS 仅当Cancel状态为PTHREAD_CANCEL_ENABLE时有效,立即执行取消动作（退出）
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_cancel_set(int_t state, int_t type);

/**
* @brief 设置线程取消点
* @param [out] exited  被退出的线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_cancel_point(void);

/**
* @brief 发送CANCEL信号消息退出某个线程
* @attention： 线程接收到CANCEL信号的缺省处理是继续运行至取消点，
               pthreads标准指定了几个取消点，其中包括：
                   (1)通过thread_cancel_point调用以编程方式建立线程取消点。 
                   (2)线程等待pthread_cond_wait或pthread_cond_timewait()中的特定条件。 
                   (3)被sigwait(2)阻塞的函数 
                   (4)一些标准的库调用。通常，这些调用包括线程可基于阻塞的函数。 
* @param [out] exited  被退出的线程
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_cancel_some(thread_t exited);

/**
* @brief 本身线程退出
* @attention： 线程退出后，并不会释放线程资源
* @param [out] retval  线程退出返回值 thread_join接收这个值
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_exit_self( void * retval);

/**
* @brief 连接线程
* @attention： 在任何一个时间点上，线程是可结合的（joinable）或者是分离的（detached）。
               一个可结合的线程能够被其他线程收回其资源和杀死。在被其他线程回收之前，它的
               存储器资源（例如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或
               杀死的，它的存储器资源在它终止时由系统自动释放。
* @param [in] thread  线程号
* @param [out] retval  线程退出返回值 thread_exit_self发送这个值
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_join(Thread *thread, void **retval);

/**
* @brief 分离线程
* @attention： 在任何一个时间点上，线程是可结合的（joinable）或者是分离的（detached）。
               一个可结合的线程能够被其他线程收回其资源和杀死。在被其他线程回收之前，它的
               存储器资源（例如栈）是不释放的。相反，一个分离的线程是不能被其他线程回收或
               杀死的，它的存储器资源在它终止时由系统自动释放。
               分离后，不需要thread_join释放资源，系统会自动释放资源
* @param [in] thread  线程号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_detach(thread_t thread);

/**
* @brief 在多线程编程环境下,函数调用会出现在多个线程中，initRoutine()函数仅执行一次
* @param [in] onceControl 控制变量 必须使用PTHREAD_ONCE_INIT宏静态地初始化。
* @param [in] initRoutine 初始化函数
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t 
thread_once(pthread_once_t *onceControl, void (*initRoutine) (void));

/**
* @brief 线程局部数据
* @param [in] key 局部数据key
* @param [in] clean 现在结束对局部数据清理函数
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_localdata_create(pthread_key_t *key, void (*clean)(void*));

/**
* @brief 设置线程局部数据
* @param [in] key 局部数据key
* @param [in] value 局部数据
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_set_localdata(pthread_key_t key, const void *value);

/**
* @brief 获得线程局部数据
* @param [in] key 局部数据key
* @param [in] value 局部数据地址
* @retval 返回值地址   表示成功
* @retval THREAD_FAIL 表示失败
*/
void *
thread_get_localdata(pthread_key_t key);

/**
* @brief 删除线程局部数据
* @param [in] key 局部数据key
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_delete_localdata(pthread_key_t key);
/**
* @brief 发送信号到某个线程 SIGSTOP SIGKILL 都会作用进程
* @param [in] thread  线程
* @param [in] sig  信号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_signal(Thread *thread, int sig);
/**
* @brief 广播信号
* @param [in] sig  信号
* @retval THREAD_SUCC  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_broadcast_signal(int sig);
/**
* @brief 得到当前线程数
* @retval 返回线程数  表示成功
* @retval THREAD_FAIL  表示失败
*/
int_t
thread_get_number(void);
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/




#endif/*__INC_THREAD_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */