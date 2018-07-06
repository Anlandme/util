/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer.h
@brief:  timer �����õ��ĺ�
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

#ifndef __INC_TIMER_H__
#define __INC_TIMER_H__

/**  @brief �ɹ����� */
#define TIMER_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define TIMER_FAIL FUNC_FAIL
/**  @brief ʧ�ܷ��� */
#define TIMER_NULL FUNC_NULL

/**  @brief �ڴ����뺯�� */
#define timer_malloc malloc
/**  @brief �ڴ��ͷź��� */
#define timer_free free
/**  @brief Ĭ��ջ��С ������� PTHREAD_STACK_MIN 8192 */
#define TIMER_STACK_SIZE 10*1024 //10k

/**  @brief ��ʱ��ѭ������ */
enum timer_period {
    TIMER_ONCE = 0, /**< һ���Զ�ʱ�� */
    TIMER_LOOP = 1, /**< ѭ����ʱ�� */
};

/**  @brief ��ʱ��ģʽ */
enum timer_mode {
    TIMER_MODE_NORMAL  = 0, /**< ����ģʽ */
    TIMER_MODE_ABSTIME = TIMER_ABSTIME, /**< ģʽ����Ϊʱ��ֱ����һ�γ�ʱ���趨�ľ���ʱ�����timer_clock������ʱ��ֵ�Ĳ�ֵ */
};


/**  @brief ��ʱ����ʼ������ */
typedef struct {
    timer_t timerid;  /**< ��ʱ�� */
    void * stack;   /**< �߳�ջ */
}TIMER;
/*
struct itimerval { 
    struct timeval it_interval; //��һ��ֵ
    struct timeval it_value;     //��ǰֵ
 }; 
 struct timeval { 
        long tv_sec;     //�� 
        long tv_usec;    //΢��
 };

 struct itimerval { 
    struct timespec it_interval; //��һ��ֵ
    struct timespec it_value;     //��ǰֵ
 }; 
struct timespec
{
    time_t tv_sec;       //��
    long int tv_nsec;     //����
};*/

#endif/*__INC_TIMER_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */