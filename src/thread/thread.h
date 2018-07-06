/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thrad.h
@brief:  thread �����õ��ĺ�
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_THREAD_H__
#define __INC_THREAD_H__

/**  @brief �ɹ����� */
#define THREAD_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define THREAD_FAIL FUNC_FAIL
/**  @brief ʧ�ܷ��� */
#define THREAD_NULL FUNC_NULL

/**  @brief ����ṹ���ѳ�ʼ���� */
#define MNANGE_IS_INITIALIZED 1

/**  @brief �ڴ����뺯�� */
#define thread_malloc malloc
/**  @brief �ڴ��ͷź��� */
#define thread_free free

/**  @brief Ĭ��ջ��С ������� PTHREAD_STACK_MIN 8192 */
#define DEFAULT_STACK_SIZE 100*1024 //100k

/**  @brief ����ں��̲߳��� */
#define gettid() syscall(__NR_gettid) 

/**  @brief thread ����ṹ�� */
typedef struct {
    u8_t InitFlag;          /**< ��ʼ����־λ */
    list_head_t threadList; /**< thread����*/
    pthread_mutex_t mutex; /**< ������*/
} ThreadManage_t;

/**  @brief �̵߳ĵ��Ȳ��� */
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

/**  @brief �߳����� */
typedef struct  {
  int_t detachState;               /**< �̵߳ķ���״̬ */
  int_t schedulePolicy;            /**< �̵߳��Ȳ���  */
  struct sched_param schedParam;   /**< �̵߳ĵ��Ȳ��� schedulePolicy����ΪTHREAD_FIFO����Ч*/
  int_t inheritSchedule;           /**< �̵߳ļ̳���  */
  int_t scope;                     /**< �̵߳�������  */
  size_t guardSize;                /**< �߳�ջĩβ�ľ��仺������С Ĭ��ֵ��PAGESIZE��С*/
  void *stackAddr;                 /**< �߳�ջ��λ��  */
  size_t stackSize;                /**< �߳�ջ�Ĵ�С  */
  int_t autoInit;                  /**< �߳������Զ���ʼ�� �ɲ���д*/
  int_t flag;                      /**< �߳����Գ�ʼ����־ */
  //int_t stackaddrSet;            /**< ��ջ�ĵ�ַ��  */
}threadAttr_t;

/**  @brief thread ���� */
typedef struct{
    u32_t module;      /**< moudle ����*/
    pthread_t id;      /**< �̱߳�ʶ��*/
    int tid;           /**< �ں��̱߳�ʶ��*/
    threadAttr_t attr; /**< thread ���� */
    list_node_t Node;  /**< thread �ڵ� */
}Thread;

enum block_operation{
    OPERATION_BLOCK     = 0, /**< �������� */
    OPERATION_NON_BLOCK = 1, /**< ����������*/
};

#endif/*__INC_THREAD_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */