/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_timer.c
@brief: 
@attention gcc .././TEST/TEST.c .././errno/err.c list.c test_list.c -o test
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/14
******************************************************************************/
#include ".././util/util_inc.h"
#include ".././TEST/pub_TEST.h"

int testValue =0;

void *test_thread(void *arg)
{
    testValue++;
    printf("start the thread!\n");
}

/**********************************************************************************************
*   Name：测试函数错误函数
**********************************************************************************************/
/**  @brief 测试当错误加入节点的情况 */
DEFINE_TEST_CODE_START(test_thread_false)
    thread_t test;
    threadAttr_t attr;
    TEST_FUNC_RETURN_NULL(thread_create(MOD_BUTT,NULL,NULL,NULL), 测试thread_create错误情况);
    TEST_FUNC_RETURN_NULL(thread_create(MOD_SYSREM,&attr,NULL,NULL), 测试thread_create错误情况);
    TEST_FUNC_RETURN_FAIL(thread_equal(test,NULL), 测试thread_equal错误情况);
    TEST_FUNC_RETURN_FAIL(thread_equal(NULL,test), 测试thread_equal错误情况);
    TEST_FUNC_RETURN_FAIL(thread_cancel_set(10,PTHREAD_CANCEL_ASYNCHRONOUS), 测试thread_cancel_set错误情况);
    TEST_FUNC_RETURN_FAIL(thread_cancel_set(PTHREAD_CANCEL_ENABLE,10), 测试thread_cancel_set错误情况);
    TEST_FUNC_RETURN_FAIL(thread_cancel_some(NULL), 测试thread_cancel_some错误情况);
    TEST_FUNC_RETURN_FAIL(thread_join(NULL,NULL), 测试thread_join错误情况);
    TEST_FUNC_RETURN_FAIL(thread_detach(NULL), 测试thread_detach错误情况);
    TEST_FUNC_RETURN_FAIL(thread_once(NULL,NULL), 测试thread_once错误情况);
    TEST_FUNC_RETURN_FAIL(thread_localdata_create(NULL,NULL), 测试thread_localdata_create错误情况);
    TEST_FUNC_RETURN_FAIL(thread_signal(NULL,0), 测试thread_signal错误情况);
DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试线程正常使用接口
**********************************************************************************************/

DEFINE_TEST_CODE_START(test_thread_non_init_attr)
    thread_t id;
    testValue =0;
    id = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id, 创建线程);
    TEST_FUNC_RETURN_SUCC(thread_join(id,NULL), 等待线程退出);
   TEST_VALUE_IS_EQ(testValue, 1, 线程修改后值);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_thread_init_attr)
    thread_t id;
    threadAttr_t attr;
    testValue =0;
    attr.flag = ATTR_DETACH_STATE | ATTR_SCHEDULE_POLICY | ATTR_SCHEDULE_PARAM |
                ATTR_INHERIT_SCHEDULE | ATTR_SCOPE | ATTR_GUARD_SIZE |ATTR_STACK_ADDR_AND_SIZE;

    attr.detachState = THREAD_JOINABLE;
    attr.schedulePolicy = THREAD_FIFO;//这个权限必须是root
    attr.schedParam.sched_priority = 10;
    attr.inheritSchedule = THREAD_EXPLICIT_SCHED;
    attr.scope = THREAD_SCOPE_SYSTEM;
    attr.guardSize = 4*1024;
    attr.stackSize = DEFAULT_STACK_SIZE;
    attr.stackAddr = malloc(DEFAULT_STACK_SIZE);
    id = thread_create(MOD_SYSREM, &attr, test_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id, 创建线程);
    TEST_FUNC_RETURN_SUCC(thread_join(id,NULL), 等待线程退出);
    free(attr.stackAddr);
    TEST_VALUE_IS_EQ(testValue, 1, 线程修改后值);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_threads)
    thread_t id1, id2, id3;
    testValue =0;
    id1 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
    id2 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
    id3 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id1, 创建线程1);
    TEST_FUNC_RETURN_IS_NOT_NULL(id2, 创建线程2);
    TEST_FUNC_RETURN_IS_NOT_NULL(id3, 创建线程3);
    TEST_FUNC_RETURN_SUCC(thread_join(id1,NULL), 等待线程1退出);
    TEST_FUNC_RETURN_SUCC(thread_join(id2,NULL), 等待线程2退出);
    TEST_FUNC_RETURN_SUCC(thread_join(id3,NULL), 等待线程3退出);
    TEST_VALUE_IS_EQ(testValue, 3, 线程修改后值);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试局部数据功能接口
**********************************************************************************************/
pthread_key_t key;
int value1;
int value2;
int value3;
void *test1_thread(void *arg)
{
    int value =10;
    thread_set_localdata(key, &value);
    value1 = *(int *)thread_get_localdata(key);
}
void *test2_thread(void *arg)
{
    int value =20;
    thread_set_localdata(key, &value);
    value2 = *(int *)thread_get_localdata(key);
}
void *test3_thread(void *arg)
{
    int value =30;
    thread_set_localdata(key, &value);
    value3 = *(int *)thread_get_localdata(key);
}
DEFINE_TEST_CODE_START(test_thread_localdata)
    TEST_FUNC_RETURN_SUCC(thread_localdata_create(&key,NULL), 创建局部变量key);
    thread_t id1, id2, id3;
    id1 = thread_create(MOD_SYSREM, NULL, test1_thread, NULL);
    id2 = thread_create(MOD_SYSREM, NULL, test2_thread, NULL);
    id3 = thread_create(MOD_SYSREM, NULL, test3_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id1, 创建线程1);
    TEST_FUNC_RETURN_IS_NOT_NULL(id2, 创建线程2);
    TEST_FUNC_RETURN_IS_NOT_NULL(id3, 创建线程3);
    TEST_FUNC_RETURN_SUCC(thread_join(id1,NULL), 等待线程1退出);
    TEST_FUNC_RETURN_SUCC(thread_join(id2,NULL), 等待线程2退出);
    TEST_FUNC_RETURN_SUCC(thread_join(id3,NULL), 等待线程3退出);
    TEST_VALUE_IS_EQ(value1, 10, 线程修改后值);
    TEST_VALUE_IS_EQ(value2, 20, 线程修改后值);
    TEST_VALUE_IS_EQ(value3, 30, 线程修改后值);
    TEST_FUNC_RETURN_SUCC(thread_delete_localdata(key), 创建局部变量key);
DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试线程其他功能接口
**********************************************************************************************/
void *cancel_thread(void *arg)   
{   
    thread_cancel_set(PTHREAD_CANCEL_ENABLE, PTHREAD_CANCEL_DEFERRED); 
    thread_cancel_point(); 
    testValue++;
}   

DEFINE_TEST_CODE_START(test_thread_cancel)
    thread_t id;
    testValue =0;
    id = thread_create(MOD_SYSREM, NULL, cancel_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id, 创建线程);
    TEST_FUNC_RETURN_SUCC(thread_cancel_some(id), 向线程发送取消);
    TEST_FUNC_RETURN_SUCC(thread_join(id,NULL), 等待线程退出);
    TEST_VALUE_IS_EQ(testValue, 0, 线程修改后值);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试线程其他功能接口
**********************************************************************************************/
int value =17;
void *exit_thread(void *arg)
{
    //thread_detach(thread_self());
    thread_exit_self((void*)&value);
}
DEFINE_TEST_CODE_START(test_thread_exit)
    thread_t id1, id2;
    void *testValue1, *testValue2;
    id1 = thread_create(MOD_SYSREM, NULL, exit_thread, NULL);
    id2 = thread_create(MOD_SYSREM, NULL, exit_thread, NULL);
    TEST_FUNC_RETURN_IS_NOT_NULL(id1, 创建线程);
    TEST_FUNC_RETURN_IS_NOT_NULL(id2, 创建线程);
    //while(THREAD_SUCC == thread_signal(id,0));
    TEST_FUNC_RETURN_SUCC(thread_join(id1,&testValue1), 等待线程退出);
    TEST_FUNC_RETURN_SUCC(thread_join(id2,&testValue2), 等待线程退出);
    TEST_VALUE_IS_EQ(*(int *)testValue1, 17, 线程修改后值);
    TEST_VALUE_IS_EQ(*(int *)testValue2, 17, 线程修改后值);
DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试主函数
**********************************************************************************************/
int main (void)
{

    TEST_SUITE_START(测试函数错误函数)
        TEST_CASE(test_thread_false,          函数运行失败测试,       测试失败的操作);
    TEST_SUITE_END

   TEST_SUITE_START(测试线程正常使用接口)
        TEST_CASE(test_thread_non_init_attr,   非初始化线程属性功能测试,  非初始化线程属性操作);
        TEST_CASE(test_thread_init_attr,       初始化线程属性功能测试,    初始化线程属性操作);
        TEST_CASE(test_threads,                多线程功能测试,           多线程操作);
    TEST_SUITE_END
 
    TEST_SUITE_START(测试线程局部数据功能接口)
        TEST_CASE(test_thread_localdata,       局部数据功能测试,  局部数据操作);
    TEST_SUITE_END

    TEST_SUITE_START(测试线程其他功能接口)
        //这个泄漏个人推测，线程资源没有释放，等待下一次申请线程再次使用，因为申请多个线程但是泄漏没有增加
        TEST_CASE(test_thread_cancel,     线程取消功能测试,  官方取消操作 引起一定量泄漏);
        TEST_CASE(test_thread_exit,       线程退出功能测试,  官方pthread_exit退出操作 引起一定量泄漏);
    TEST_SUITE_END

    return 0;
}
