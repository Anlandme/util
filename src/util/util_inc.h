/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: utilinc.h
@brief: 头文件主要是一些公共的头文件以及标准模块头文件
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_UTIL_INC_H__
#define __INC_UTIL_INC_H__
/* 跨平台头文件*/
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <setjmp.h>
#include <syslog.h>
#include <time.h>
#include <execinfo.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <semaphore.h>
#include <mqueue.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/syscall.h>  


/* 自定义模块 公共头文件 */
#include "util_def.h"
#include ".././errno/err_pub.h"         //错误码公共头文件
#include ".././except/except_pub.h"     //异常处理公共头文件
#include ".././list/list_pub.h"         //链表公共头文件
#include ".././list/queue_pub.h"        //队列公共头文件
#include ".././list/stack_pub.h"        //栈公共头文件
#include ".././log/log_pub.h"           //pipe公共头文件
#include ".././thread/mutex_cond_pub.h" //互斥锁条件变量公共头文件
#include ".././thread/rwlock_pub.h"     //读写锁公共头文件
#include ".././thread/thread_pub.h"     //线程公共头文件
#include ".././timer/timer_pub.h"       //定时器公共头文件
#include ".././halloc/halloc_pub.h"     //高效内存申请公共头文件


#endif/*__INC_UTIL_INC_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
