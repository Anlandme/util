/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer_pub.h
@brief:  timer����ͷ�ļ�
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
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_TIMER_PUB_H__
#define __INC_TIMER_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "timer.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ��ʱ�� */
typedef TIMER Timer_t;

/**  @brief ��ʱ�����õ�ʱ�� */
enum timer_clock {
    TIMER_REALTIME           = CLOCK_REALTIME,           /**< ϵͳ��ǰʱ�䣬��1970��1.1������ */
    TIMER_MONOTONIC          = CLOCK_MONOTONIC,          /**< ϵͳ������ʱ�䣬���ܱ����� */
    TIMER_PROCESS_CPUTIME_ID = CLOCK_PROCESS_CPUTIME_ID, /**< ��������ʱ�� */
    TIMER_THREAD_CPUTIME_ID  = CLOCK_THREAD_CPUTIME_ID,  /**< �߳�����ʱ�� */
    //TIMER_REALTIME_HR        = CLOCK_REALTIME_HR,        /**< CLOCK_REALTIME�ĸ߾��Ȱ汾  */
    //TIMER_MONOTONIC_HR       = CLOCK_MONOTONIC_HR,       /**< CLOCK_MONOTONIC�ĸ߾��Ȱ汾 */
};

/**  @brief ��ʱ��֪ͨ���� */
enum timer_notify {
    TIMER_NOTIFY_NONE     = SIGEV_NONE,   /**< ʲô��������ֻ�ṩͨ��timer_gettime��timer_getoverrun��ѯ��ʱ��Ϣ */
    TIMER_NOTIFY_SIGNAL   = SIGEV_SIGNAL, /**< ����ʱ�����ڣ��ں˻ὫtimerSignal��ָ�����źŴ��͸����̡�si_value�ᱻ�趨��sigev_value */
    TIMER_NOTIFY_CALLBACK = SIGEV_THREAD, /**< ����ʱ�����ڣ��ں˻ᴴ��һ���̣߳���������ִ��callbackFunc������signalValue��ΪΪһ�������� */
};

/**  @brief ��ʱ����ʼ������ */
typedef struct {
    clockid_t clock;                      /**< ��ʱ��ʹ��ʱ��Դ */
    int_t timerNotify;                    /**< �ź�֪ͨ���ź����� */
    int_t timerSignal;                    /**< ��ʱ����ʱ�����ź� */
    union sigval signalValue;             /**< �����źŴ�����ֵ */
    void (*callbackFunc)(union sigval);   /**< ��ʱ���ص����� */
}timerEvent_t;


/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/

/**
* @brief �򿪶�ʱ��
* @param [in]  timer   ����ʼ���Ķ�ʱ��
* @param [in]  event   ��ʱ���¼�
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_open(Timer_t* timer, timerEvent_t *event);

/**
* @brief ��ʼ��ʱ��
* @param [in]  timer  ��ʱ��
* @param [in]  mode ��ʱ��ģʽ
 - TIMER_MODE_NORMAL  ����ģʽ
 - TIMER_MODE_ABSTIME ģʽ����Ϊʱ��ֱ����һ�γ�ʱ���趨�ľ���ʱ�����timer_clock������ʱ��ֵ�Ĳ�ֵ
* @param [in]  period ��ʱ������
 - TIMER_ONCE һ���Զ�ʱ��
 - TIMER_LOOP     ѭ����ʱ��
* @param [in]  times  ��ʱ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_start(Timer_t timer, int_t mode, int_t period, struct timespec *times);

/**
* @brief ֹͣ��ʱ��
* @param [in]  timer ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_stop(Timer_t timer);

/**
* @brief ɾ����ʱ��
* @param [in]  timer ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_close(Timer_t *timer);

/**
* @brief ��ö�ʱ������ʱ���ʣ��ʱ��
* @param [in]  timer ��ʱ��
* @param [out]  period ����ʱ��
* @param [out]  remain ʣ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_get_time(Timer_t timer, struct timespec *period, struct timespec *remain);

/**
* @brief ��ö�ʱ����ʱ���� ����������еĴ������ڻ����DELAYTIMER_MAX����˵��û� ����DELAYTIMER_MAX��
* @param [in]  timer ��ʱ��
* @param [in]  overSum ��ʱ����
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_overrun(Timer_t timer, int_t *overSum);

/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/

#endif /*__INC_TIMER_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */