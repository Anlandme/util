/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_err.c
@brief: 主要是对单元测试模块的测试
@attention gcc TEST.c test_TEST.c -o test -lpthread
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/04/10
******************************************************************************/
#include ".././util/util_inc.h"
#include "pub_TEST.h"//测试模块的头文件不添加到util_inc.h当中，而是单独被调用


/**  @brief 测试当case成功的情况 */
DEFINE_TEST_CODE_START(test_success_case)
    TEST_IS_TRUE(1,);
DEFINE_TEST_CODE_END

/**  @brief 测试当case失败的情况 */
DEFINE_TEST_CODE_START(test_false_case)
    TEST_IS_TRUE(0,出现错误);
DEFINE_TEST_CODE_END

/**  @brief 测试当未知返回错误的失败的情况 */
DEFINE_TEST_CODE_START(test_unknown_false_case)
    return 3;
DEFINE_TEST_CODE_END

/**  @brief 测试当case定时器的情况 */
DEFINE_TEST_CODE_START(test_timer_case)
    long i = 10000000L;
    TEST_START_TIMER()
    while(i--);
    TEST_END_TIMER()
    ;
DEFINE_TEST_CODE_END



/**  @brief 测试条件判断正常的情况 */
DEFINE_TEST_CODE_START(test_condition_case)
    char *p="addr";
    TEST_VALUE_IS_EQ(1,1, 相等);
    TEST_VALUE_IS_NE(1,2, 不等);
    TEST_VALUE_IS_LT(2,3, 小于);
    TEST_VALUE_IS_LE(2,3, 小于等于);TEST_VALUE_IS_LE(3,3, 小于等于);
    TEST_VALUE_IS_GT(4,3, 大于);
    TEST_VALUE_IS_GE(4,3, 大于等于);TEST_VALUE_IS_GE(4,4, 大于等于);

    TEST_STRING_IS_EQ("1","1", 相等);
    TEST_STRING_IS_NE("1","2", 不等);
    TEST_STRING_IS_LT("2","3", 小于);
    TEST_STRING_IS_LE("2","3", 小于等于);TEST_STRING_IS_LE("3","3", 小于等于);
    TEST_STRING_IS_GT("4","3", 大于);
    TEST_STRING_IS_GE("4","3", 大于等于);TEST_STRING_IS_GE("4","4", 大于等于);

    TEST_FUNC_RETURN_SUCC(FUNC_SUCC, 函数执行返回为成功);
    TEST_FUNC_RETURN_FAIL(FUNC_FAIL, 函数执行返回为失败);
    TEST_FUNC_RETURN_NULL(FUNC_NULL, 函数执行返回指针为空);
    TEST_FUNC_RETURN_IS_NOT_NULL(p, 函数执行返回指针为非空);
    TEST_FUNC_RETURN_IS_NOT_FAIL(FUNC_SUCC, 函数执行返回指针为非失败);
DEFINE_TEST_CODE_END

/**  @brief 测试值条件判断错误的情况 */
DEFINE_TEST_CODE_START(test_value_false_case)
    TEST_VALUE_IS_EQ(2,1, 相等);
DEFINE_TEST_CODE_END

/**  @brief 测试字符串条件判断异常的情况 */
DEFINE_TEST_CODE_START(test_string_false_case)
    TEST_STRING_IS_EQ("2","1", 相等);
DEFINE_TEST_CODE_END

/**  @brief 测试打印的情况 */
DEFINE_TEST_CODE_START(test_print_case)
    string_t str="test string";
    test_printf("test printf\n");
    test_printf("printf %s\n", str);
DEFINE_TEST_CODE_END


/***************************测试装载卸载结构体********************************/
char_t *tmp;

TEST_DEFINE_CASE_SETUP( test_setup )
{
    int count;
    tmp = (char_t*)malloc(sizeof(char_t));
}
TEST_DEFINE_CASE_TEARDOWN( test_teardown )
{
    free(tmp);//如果不成功会出现内存泄漏
}

/**  @brief 测试装载卸载的函数的情况 */
DEFINE_TEST_CODE_START(test_setup_teardown_case)
DEFINE_TEST_CODE_END
/***************************测试组出错使用********************************/
int_t group_false_param(int_t tmp)
{
    if( tmp == 8)return TEST_FAIL;
    return tmp;
}

DEFINE_ONE_PARAM_STACK_AND_INIT_START(false_test, int_t, 10)
    int_t i;
    for( i=0; i<10; i++){
        PUSH_SOME_PARAM_IN(false_test, i);
    }
DEFINE_PARAM_STACK_AND_INIT_END

DEFINE_GROUP_TEST_CODE_START(test_group_false, false_test)
    TEST_FUNC_RETURN_IS_NOT_FAIL( group_false_param( PUSH_SOME_PARAM_OUT(false_test, FIRST) ), 测试错误时测试组的处理方式);//循环10次
DEFINE_GROUP_TEST_CODE_END

DEFINE_ONE_PARAM_STACK_AND_INIT_START(stack_false_test, int_t, 10)
    int_t i;
    for( i=0; i<10; i++){
        PUSH_SOME_PARAM_IN(stack_false_test, i);
        if( i == 8) return TEST_FAIL;
    }
DEFINE_PARAM_STACK_AND_INIT_END

DEFINE_GROUP_TEST_CODE_START(test_group_stack_false, stack_false_test)
DEFINE_GROUP_TEST_CODE_END
/****************************************************************************/
/***************************一个测试组使用********************************/
int_t group_one_param(int_t tmp)
{
    return tmp;
}

DEFINE_ONE_PARAM_STACK_AND_INIT_START(one_test, int_t, 10)
    int_t i;
    for( i=0; i<10; i++){
        PUSH_SOME_PARAM_IN(one_test, i);
    }
DEFINE_PARAM_STACK_AND_INIT_END

DEFINE_GROUP_TEST_CODE_START(test_group_one, one_test)
    test_printf("group test value=%d\n", group_one_param( PUSH_SOME_PARAM_OUT(one_test, FIRST) ));//循环10次打印
DEFINE_GROUP_TEST_CODE_END
/****************************************************************************/
/***************************多个测试组使用********************************/
DEFINE_FIVE_PARAM_STACK_AND_INIT_START(five_test, int_t, 2,  int_t * , 2, string_t, 2, int_t, 2, double_t, 2)
    int_t i =10;
    PUSH_SOME_PARAM_IN(five_test, 1, &i, "test first", 'a', 2.2);
    PUSH_SOME_PARAM_IN(five_test, 2, &i, "test second", 'b', 2.3);
DEFINE_PARAM_STACK_AND_INIT_END

DEFINE_GROUP_TEST_CODE_START(test_group_five, five_test)
    test_printf( "first value:%d\n", PUSH_SOME_PARAM_OUT(five_test, FIRST) );//循环2次打印
    test_printf( "second value:%d\n", *PUSH_SOME_PARAM_OUT(five_test, SECOND) );//循环2次打印
    test_printf( "third value:%s\n", PUSH_SOME_PARAM_OUT(five_test, THIRD) );//循环2次打印
    test_printf( "fourth value:%c\n", PUSH_SOME_PARAM_OUT(five_test, FOURTH) );//循环2次打印
    //test_printf( "fifth value:%lf\n", PUSH_SOME_PARAM_OUT(five_test, FIFTH) );//循环2次打印
DEFINE_GROUP_TEST_CODE_END
/****************************************************************************/
int main( void)
{
    Start_UnitTest();//开始单元测试

    TEST_SUITE_START(测试suite接口正常使用)
        long i = 10000000L;
        while(i--);
    TEST_SUITE_END

    TEST_SUITE_START(测试case接口正常使用)
        TEST_CASE(test_success_case,         case成功测试,              测试当case成功的情况);
        TEST_CASE(test_false_case,           case失败测试,              测试当case失败的情况);
        TEST_CASE(test_timer_case,           case定时器测试,            测试当case定时器的情况);
        TEST_CASE(test_unknown_false_case,   case未知失败测试,           测试当case未知失败的情况);
        TEST_CASE(test_condition_case,       case条件判断正常测试,       测试当case条件判断正常的情况);
        TEST_CASE(test_value_false_case,     case值条件判断错误测试,     测试当case值条件判断错误的情况);
        TEST_CASE(test_string_false_case,    case字符串条件判断错误测试,  测试当case字符串条件判断错误的情况);
        TEST_CASE(test_print_case,           case打印测试,               测试当case打印的情况);
    TEST_SUITE_END

    TEST_SUITE_START(测试suite接口检查内存泄漏)
        int *p1=(int*)malloc(sizeof(int)*100);
        int *p2=(int*)malloc(sizeof(int));
        free(p1);
    TEST_SUITE_END

    TEST_SUITE_START(测试装载卸载函数使用)
        test_sutie_register_setup_teardown(test_setup, test_teardown);//注册装载和卸载函数
        TEST_CASE(test_setup_teardown_case,           case装载卸载测试,               测试当case装载卸载情况);
    TEST_SUITE_END

    TEST_SUITE_START(测试组使用)
        TEST_CASE(test_group_stack_false,   case测试组参数栈出错,                 测试case测试组参数栈出错情况);
        TEST_CASE(test_group_false,         case测试组出错,                      测试case测试组出错情况);
        TEST_CASE(test_group_one,           case一个参数测试组使用,               测试case一个参数测试组使用情况);
        TEST_CASE(test_group_five,          case多个参数测试组使用,               测试case多个参数测试组使用情况);
    TEST_SUITE_END
}

