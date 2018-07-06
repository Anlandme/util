/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: timer.c
@brief: ��ʱ���������Ҫ�ǻ���POSIX��ʱ��������ƣ���Ҫ�Ƕ���һЩ�ṹ��İ�װ,ʹʹ���������ӱ��
        ��һ������ע����Ǳ���ʱ����Ҫ�� -lrt���ܳ��ɱ���
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#include ".././util/util_inc.h"
#include "timer.h"
/**
* @brief �򿪶�ʱ��
* @param [in]  timer   ����ʼ���Ķ�ʱ��
* @param [in]  event   ��ʱ���¼�
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
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
    //��ʼ��timer
    timer->stack = NULL;
    //����źŴ���Ϊ�����߳�
    if( event->timerNotify == TIMER_NOTIFY_CALLBACK )
    	{
        pthread_attr_init(&attr);
        timer->stack = timer_malloc(TIMER_STACK_SIZE);
        pthread_attr_setstack(&attr, timer->stack, TIMER_STACK_SIZE);     //�����߳��߳�ջ��ַ�ʹ�С
        sigEvent.sigev_notify_attributes = &attr;
        pthread_attr_destroy(&attr);
    }

    return  timer_create(event->clock, &sigEvent, &timer->timerid);
}


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
* @brief ֹͣ��ʱ��
* @param [in]  timer ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_stop(Timer_t timer)
{
    struct itimerspec value;
    value.it_value.tv_sec =0;
    value.it_value.tv_nsec =0;
    value.it_interval = value.it_value;  
    //ֹͣtimer ����:new_value->it_value�ĵ�ֵΪ0(������붼Ϊ0)
    return timer_settime(timer.timerid, 0, &value, NULL);
}


/**
* @brief ɾ����ʱ��
* @param [in]  timer ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
*/
int_t
timer_close(Timer_t *timer)
{
    if(PARAMISNULL(timer))  RETURN_ERR(ERR_PARA, TIMER_FAIL);
    if(timer->stack != NULL ) timer_free(timer->stack);
    return timer_delete(timer->timerid);
}


/**
* @brief ��ö�ʱ������ʱ���ʣ��ʱ��
* @param [in]  timer ��ʱ��
* @param [out]  period ����ʱ��
* @param [out]  remain ʣ��ʱ��
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
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
* @brief ��ö�ʱ����ʱ���� ����������еĴ������ڻ����DELAYTIMER_MAX����˵��û� ����DELAYTIMER_MAX��
* @param [in]  timer ��ʱ��
* @param [in]  overSum ��ʱ����
* @retval TIMER_SUCC  ��ʾ�ɹ�
* @retval TIMER_FAIL  ��ʾʧ��
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
