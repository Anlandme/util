/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: utilinc.h
@brief: ͷ�ļ���Ҫ��һЩ������ͷ�ļ��Լ���׼ģ��ͷ�ļ�
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_UTIL_INC_H__
#define __INC_UTIL_INC_H__
/* ��ƽ̨ͷ�ļ�*/
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


/* �Զ���ģ�� ����ͷ�ļ� */
#include "util_def.h"
#include ".././errno/err_pub.h"         //�����빫��ͷ�ļ�
#include ".././except/except_pub.h"     //�쳣������ͷ�ļ�
#include ".././list/list_pub.h"         //������ͷ�ļ�
#include ".././list/queue_pub.h"        //���й���ͷ�ļ�
#include ".././list/stack_pub.h"        //ջ����ͷ�ļ�
#include ".././log/log_pub.h"           //pipe����ͷ�ļ�
#include ".././thread/mutex_cond_pub.h" //������������������ͷ�ļ�
#include ".././thread/rwlock_pub.h"     //��д������ͷ�ļ�
#include ".././thread/thread_pub.h"     //�̹߳���ͷ�ļ�
#include ".././timer/timer_pub.h"       //��ʱ������ͷ�ļ�
#include ".././halloc/halloc_pub.h"     //��Ч�ڴ����빫��ͷ�ļ�


#endif/*__INC_UTIL_INC_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
