/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thrad.h
@brief:  thread 所有用到的宏
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

#ifndef __INC_THREAD_H__
#define __INC_THREAD_H__

/**  @brief 成功返回 */
#define THREAD_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define THREAD_FAIL FUNC_FAIL
/**  @brief 失败返回 */
#define THREAD_NULL FUNC_NULL

/**  @brief 管理结构体已初始化宏 */
#define MNANGE_IS_INITIALIZED 1

/**  @brief 内存申请函数 */
#define thread_malloc malloc
/**  @brief 内存释放函数 */
#define thread_free free

/**  @brief 默认栈大小 必须大于 PTHREAD_STACK_MIN 8192 */
#define DEFAULT_STACK_SIZE 100*1024 //100k

/**  @brief 获得内核线程参数 */
#define gettid() syscall(__NR_gettid) 

/**  @brief thread 管理结构体 */
typedef struct {
    u8_t InitFlag;          /**< 初始化标志位 */
    list_head_t threadList; /**< thread链表*/
    pthread_mutex_t mutex; /**< 互斥锁*/
} ThreadManage_t;

/**  @brief 线程的调度参数 */
/*struct sched_param { 
    int32_t  sched_priority; 
    int32_t  sched_curpriority; 
    union { 
        int32_t  reserved[8]; 
        struct {    
            int32_t  __ss_low_priority;  
            int32_t  __ss_max_repl;  
            struct timespec     __ss_repl_period;   
            struct timespec     __ss_init_budget;   
        }           __ss;   
    }           __ss_un;    
}*/

/**  @brief 线程属性 */
typedef struct  {
  int_t detachState;               /**< 线程的分离状态 */
  int_t schedulePolicy;            /**< 线程调度策略  */
  struct sched_param schedParam;   /**< 线程的调度参数 schedulePolicy设置为THREAD_FIFO才有效*/
  int_t inheritSchedule;           /**< 线程的继承性  */
  int_t scope;                     /**< 线程的作用域  */
  size_t guardSize;                /**< 线程栈末尾的警戒缓冲区大小 默认值是PAGESIZE大小*/
  void *stackAddr;                 /**< 线程栈的位置  */
  size_t stackSize;                /**< 线程栈的大小  */
  int_t autoInit;                  /**< 线程属性自动初始化 可不填写*/
  int_t flag;                      /**< 线程属性初始化标志 */
  //int_t stackaddrSet;            /**< 堆栈的地址集  */
}threadAttr_t;

/**  @brief thread 管理 */
typedef struct{
    u32_t module;      /**< moudle 类型*/
    pthread_t id;      /**< 线程标识符*/
    int tid;           /**< 内核线程标识符*/
    threadAttr_t attr; /**< thread 属性 */
    list_node_t Node;  /**< thread 节点 */
}Thread;

enum block_operation{
    OPERATION_BLOCK     = 0, /**< 阻塞操作 */
    OPERATION_NON_BLOCK = 1, /**< 非阻塞操作*/
};

#endif/*__INC_THREAD_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */