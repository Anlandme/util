/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_timer.c
@brief: 主要是简单的定时器功能使用，但是在线程模式下使用定时器时候，会出泄漏0X110大小的内存泄漏
        跟踪查找位置在线程库，怀疑是由于线程仍未回收导致。但是之后的同样方式调用并未出现泄漏内存
        所以我估计是内核申请的线程资源，在退出进程才会释放内存
@attention gcc .././TEST/TEST.c .././errno/err.c list.c test_list.c -o test
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/14
******************************************************************************/
#include ".././util/util_inc.h"
#include ".././TEST/pub_TEST.h"

int timerValue =0;
void test_signal_timer(int sigval)
{
    timerValue++;
    printf("signal come in!\n");
}
void test_timer(union sigval v)
{
    timerValue++;
    printf("signal come in!\n");
}

/**********************************************************************************************
*   Name：测试h函数错误函数
**********************************************************************************************/
/**  @brief 测试当错误加入节点的情况 */
DEFINE_TEST_CODE_START(test_timer_false)
    Timer_t timer;
    timerEvent_t event;
    struct timespec times;
    TEST_FUNC_RETURN_FAIL(timer_open(NULL,NULL), 测试timer_open错误情况);
    TEST_FUNC_RETURN_FAIL(timer_open(NULL,&event), 测试timer_open错误情况);
    TEST_FUNC_RETURN_FAIL(timer_open(&timer,NULL), 测试timer_open错误情况);
    TEST_FUNC_RETURN_FAIL(timer_start(timer,TIMER_MODE_NORMAL, TIMER_ONCE, NULL), 测试timer_start错误情况);
    TEST_FUNC_RETURN_FAIL(timer_start(timer,TIMER_MODE_NORMAL, TIMER_LOOP+1, &times), 测试timer_start错误情况);
    TEST_FUNC_RETURN_FAIL(timer_start(timer,TIMER_MODE_ABSTIME+1, TIMER_ONCE, &times), 测试timer_start错误情况);
    TEST_FUNC_RETURN_FAIL(timer_get_time(timer,NULL, &times), 测试timer_get_time错误情况);
    TEST_FUNC_RETURN_FAIL(timer_get_time(timer,&times, NULL), 测试timer_get_time错误情况);
    TEST_FUNC_RETURN_FAIL(timer_overrun(timer, NULL), 测试timer_overrun错误情况);
DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试定时器正常使用接口
**********************************************************************************************/

DEFINE_TEST_CODE_START(test_timer_non_thread)
    Timer_t timer;
    timerEvent_t event;
    struct sigaction act;  
    struct timespec times;
    timerValue =0;
    memset(&act, 0, sizeof(act));  
    act.sa_handler = test_signal_timer;  
    act.sa_flags = 0;   
    TEST_FUNC_RETURN_SUCC(sigaction(SIGUSR1, &act, NULL), 初始化信号处理);
 

    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_SIGNAL;
    event.timerSignal = SIGUSR1;
    TEST_FUNC_RETURN_SUCC(timer_open(&timer,&event), 测试timer_open正常情况);
    times.tv_sec =1;
    times.tv_nsec = 0;
    TEST_FUNC_RETURN_SUCC(timer_start(timer,TIMER_MODE_NORMAL, TIMER_LOOP, &times), 开启定时器循环两次);
    test_printf("start->timerValue=%d\n", timerValue);
    sleep(1);//每次睡眠都会被打断，需要重新进入睡眠
    sleep(1);
    test_printf("end->timerValue=%d\n", timerValue);
    TEST_FUNC_RETURN_SUCC(timer_close(&timer), 关闭定时器);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_timer_thread)
    Timer_t timer;
    timerEvent_t event;  
    struct timespec times;
    timerValue =0;
 
    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
    event.callbackFunc = test_timer;
    TEST_FUNC_RETURN_SUCC(timer_open(&timer,&event), 测试timer_open正常情况);
    times.tv_sec =1;
    times.tv_nsec = 0;
    TEST_FUNC_RETURN_SUCC(timer_start(timer,TIMER_MODE_NORMAL, TIMER_LOOP, &times), 开启定时器循环两次);
    test_printf("start->timerValue=%d\n", timerValue);
    sleep(1);//每次睡眠都会被打断，需要重新进入睡睡眠
    sleep(1);
    sleep(1);
    test_printf("end->timerValue=%d\n", timerValue);
    TEST_FUNC_RETURN_SUCC(timer_close(&timer), 关闭定时器);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试定时器其他功能接口 暂停 获取当前定时器
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_timer_stop)
    Timer_t timer;
    timerEvent_t event;
    struct sigaction act;  
    struct timespec times;
    timerValue =0;
 
    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
    event.timerSignal = SIGUSR2;
    event.callbackFunc = test_timer;
    TEST_FUNC_RETURN_SUCC(timer_open(&timer,&event), 测试timer_open正常情况);
    times.tv_sec =1;
    times.tv_nsec = 0;
    TEST_FUNC_RETURN_SUCC(timer_start(timer,TIMER_MODE_NORMAL, TIMER_LOOP, &times), 开启定时器循环);
    test_printf("start->timerValue=%d\n", timerValue);
    sleep(2);
    TEST_FUNC_RETURN_SUCC(timer_stop(timer), 暂停定时器);
    test_printf("end->timerValue=%d\n", timerValue);
    sleep(1);//查看timerValue是否还有增加
    sleep(1);//查看timerValue是否还有增加
    sleep(1);//查看timerValue是否还有增加
    test_printf("stop timer after, timerValue=%d\n", timerValue);//定时器会把最后一次定时走完才会完全停止定时器所以会出现2
    TEST_FUNC_RETURN_SUCC(timer_close(&timer), 关闭定时器);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_get_timer_value)
    Timer_t timer;
    timerEvent_t event;
    struct sigaction act;  
    struct timespec times;
    struct timespec period;
    struct timespec remain;
    int value;

    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_NONE;
    event.timerSignal = SIGUSR1;
    TEST_FUNC_RETURN_SUCC(timer_open(&timer,&event), 测试timer_open正常情况);
    times.tv_sec =1;
    times.tv_nsec = 0;
    TEST_FUNC_RETURN_SUCC(timer_start(timer,TIMER_MODE_NORMAL, TIMER_LOOP, &times), 开启定时器循环两次);
    sleep(2);//每次睡眠都会被打断，需要重新进入睡眠
    TEST_FUNC_RETURN_SUCC(timer_overrun(timer, &value), 获取定时器超时次数);
    test_printf("over number=%d\n", value);
    TEST_FUNC_RETURN_SUCC(timer_get_time(timer, &period, &remain), 获取定时器超时次数);
    test_printf("period.sec=%d period.nsec=%d\n", period.tv_sec, period.tv_nsec);
    test_printf("remain.sec=%d remain.nsec=%d\n", remain.tv_sec, remain.tv_nsec);
    TEST_FUNC_RETURN_SUCC(timer_close(&timer), 关闭定时器);
DEFINE_TEST_CODE_END




/**********************************************************************************************
*   Name：测试主函数
**********************************************************************************************/
int main (void)
{
  
    TEST_SUITE_START(测试函数错误函数)
        TEST_CASE(test_timer_false,          函数运行失败测试,       测试失败的操作);
    TEST_SUITE_END

   TEST_SUITE_START(测试定时器正常使用接口)
        TEST_CASE(test_timer_non_thread,   非线程定时器功能测试,  测试非线程定时器操作);
        TEST_CASE(test_timer_thread,       线程定时器功能测操作,    测试线程定时器操作);
    TEST_SUITE_END
 
    TEST_SUITE_START(测试定时器其他功能接口)
        TEST_CASE(test_timer_stop,         定时器暂停功能测试,  测试定时器暂停操作);
        TEST_CASE(test_get_timer_value,    定时器获取值测试,    测试获取定时器值操作);
    TEST_SUITE_END
 
    return 0;
}
