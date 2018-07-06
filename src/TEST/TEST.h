/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: TEST.h
@brief:对于链表模块的结构体定义
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/29
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/29  | 0.0.0.1   | D.C.P    | 创建文件             
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_TEST_H__
#define __INC_TEST_H__

#if !defined( OS_MAC)
        #include <mcheck.h>//内存泄漏检查头文件
#endif
/**  @brief 成功返回 */
#define TEST_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define TEST_FAIL FUNC_FAIL
/**  @brief 失败空指指针 */
#define TEST_NULL FUNC_NULL

/**  @brief 内存申请函数 */
#define TEST_malloc malloc
/**  @brief 内存释放函数 */
#define TEST_free free

/**  @brief 空参数 */
#define VOID_PARAM void *
/**  @brief 空的数 */
#define NULL_SUM 1

/**  @brief 打开测试组功能 */
#define OPEN_TESTGROUP -1
/**  @brief 关闭测试组功能 */
#define CLOSE_TESTGROUP -2

/**  @brief 监控内存文件 */
#define MONITOR_MALLOC_FILE "./stdout"
/**  @brief 装载和卸载函数指针*/
typedef void(*data_handler_t)(void);

/**  @brief 套件结构体 */
struct suite {
        string_t SuiteName;           /**< 套件名字*/
        u8_t SuiteSum;                /**< 测试实例数*/
        u8_t SuiteErrSum;             /**< 测试实例出错误*/
        struct timespec SuiteTime;    /**< 测试用时 精度纳秒*/
        data_handler_t SuiteSetup;    /**< case装载数据函数*/
        data_handler_t SuiteTeardown; /**< case卸载数据函数*/
		data_handler_t TestGroupFree; /**< 测试组结构体释放函数 */
};

/**  @brief 实例结构结构体 */
struct Case {
		struct suite *MySuite;	  /**< 我的套件 */	
        string_t CaseName;        /**< 实例名字*/
        u32_t Line;               /**< 出错位置所在的行数 */
        string_t File;            /**< 出错位置所在的文件 */
        string_t Function;        /**< 出错位置所在的函数 */
        struct timespec CaseTime; /**< 测试用时 精度纳秒*/
        u32_t PrintLen;           /**< 需要打印的内容的长度*/
        char ErrText[512];        /**< 错误内容*/
        char PrintText[1024];     /**< 需要打印的内容*/
		int_t TESTGroupSP;		  /**< 测试组的参数栈指针*/
};

/**  @brief 参数数量 */
enum parameter_number{
    ONE_PARAM    = 1,  /**< 参数的栈的目标层有一个参数*/
    TWO_PARAM    = 2,  /**< 参数的栈的目标层有二个参参数*/
    THREE_PARAM  = 3,  /**< 参数的栈的目标层有三个参参数*/
    FOUR_PARAM   = 4,  /**< 参数的栈的目标层有四个参参数*/
    FIVE_PARAM   = 5,  /**< 参数的栈的目标层有五个参参数*/
};
/******************************************************************
*   Name：私有函数接口
*   Desc：内部调用的一些函数
******************************************************************/
/**
* @brief 计算时间函数
* @param [in] start    开始时间
* @param [in] end      结束时间
*/
struct timespec 
_test_gettime(struct timespec start, struct timespec end);
/**
* @brief 初始化测试套件
* @param [in] testSuite   测试套件
*/
int_t
_test_init_suite(struct suite *testSuite);
/**
* @brief 开始一个套件测试
* @param [in] testSuite 套件结构体
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t 
_test_suite_start(struct suite *testSuite);
/**
* @brief 结束一个套件测试
* @param [in] testSuite 套件结构体
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t 
_test_suite_end(struct suite *testSuite);
/**
* @brief 初始化套件的测试实例
* @param [in] testCase   实例
*/
int_t 
_test_init_case(struct Case *testCase);
/**
* @brief 处理测试的实例结果
* @param [in] testCase  实例结构体
* @param [in] desc      实例描述
* @param [in] result    实例测试结果
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t 
_test_show_case_result(struct suite *testSuite, struct Case *testCase, string_t desc, u32_t result);
/**
* @brief 单元测试初始化
*/
int_t
_test_start_unit_test(void);
/******************************************************************
*   Name：私有接口宏 
*   Desc：内部调用的一些宏，主要是获得异常函数调用堆栈
******************************************************************/
/**
* @brief 符号条件判断，失败时候，退出实例测试
* @param [in] func   出错函数名
* @param [in] line   出错行数
* @param [in] file   出错文件
* @param [in] value1   参与条件判断的值1
* @param [in] value2   参与条件判断的值2
* @param [in] condition   判断解决
* @param [in] desc   错误描述
*/
#define RETURN_VAL(func, line, file, value1, value2, condition, desc)                                 										\
do{                                                                                                   										\
	if (!(condition)){       																												\
		u32_t textLen = 0;                                                                     												\
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]预期值(expected value)：%d\n", value1);           		 \
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]结果值(result value) %d\n", value2);               		  \
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]预期结果(expected result)：%s\n", #desc);          		\
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]错误码(err number)：%s\n", err_strerr(THE_ERRNO)); 		 \
	    if(testCase->TESTGroupSP != CLOSE_TESTGROUP) 												 										 \
			textLen += snprintf(testCase->ErrText, 512-textLen, "[ ERR    ]出错测试组所在栈位置(expected value)：%d\n", testCase->TESTGroupSP);\
	    testCase->Line = (u32_t)line;                                                                 										\
	    testCase->File = (char *)file;                                                                										\
	    testCase->Function = (char *)func;                                                            										\
	    return TEST_FAIL;                                                                             										\
	}                                                                                                 										\
}while (0)
/**
* @brief 字符串条件判断，失败时候，退出实例测试
* @param [in] func   出错函数名
* @param [in] line   出错行数
* @param [in] file   出错文件
* @param [in] str   期望结果
* @param [in] result   返回字符串结果
* @param [in] condition   判断解决
* @param [in] desc   错误描述
*/
#define RETURN_STR(func, line, file, str, result, condition, desc) 																			\
do{                                               																							\
    if (!(condition)){                                         																				\
		u32_t textLen = 0;                                                                     												\
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]预期字符串(expected string)：%s\n", str);            	   \
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]结果字符串(result string)： %s\n", result);               \
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]预期结果(expected result)： %s\n", #desc);          		\
	    	textLen += snprintf(testCase->ErrText+textLen, 512-textLen, "[ ERR    ]错误码(err number)：%s\n", err_strerr(THE_ERRNO)); 		 \
	    if(testCase->TESTGroupSP != CLOSE_TESTGROUP) 												 										\
			textLen += snprintf(testCase->ErrText, 512-textLen, "[ ERR    ]出错测试组所在栈位置(expected value)：%d\n", testCase->TESTGroupSP);\
        testCase->Line = (u32_t )line;        																								\
        testCase->File = (char *)file;        																								\
        testCase->Function = (char *)func;    																								\
        return TEST_FAIL;                     																								\
    }                                         																								\
}while(0)
/**
* @brief 定义需要测试的测试函数开始位置 并启动套件计时器
* @param [in] name   测试套件名
* @param [in] setup  装载case初始化函数
* @param [in] name   测试套件名
*/
#define _TEST_SUITE_START(name)    \
{                                  \
    struct suite testSuite;        \
    _test_init_suite(&testSuite);  \
    testSuite.SuiteName = #name;   \
    _test_suite_start(&testSuite); \
    struct timespec start, end;    \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

/**
* @brief 定义需要测试的测试函数结束位置
*/
#define _TEST_SUITE_END                             \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);  \
    testSuite.SuiteTime = _test_gettime(start, end);\
    _test_suite_end(&testSuite);                    \
}
/**
* @brief 注册suite下的case的数据装载和卸载函数
*/
#define _TEST_SUITE_REGISTER_SETUP_TEARDOWN(setup, teardown) \
do{                                                          \
        testSuite.SuiteSetup = setup;                        \
                testSuite.SuiteTeardown = teardown;          \
} while (0)


/**
* @brief 定义需要测试的测试函数开始位置
* @param [in] _testfunc   测试函数名
*/
#define _DEFINE_TEST_CODE_START(_testfunc)    \
static int_t _testfunc(struct Case *testCase) \
{                              

/**
* @brief 定义需要测试的测试函数结束位置
*/
#define _DEFINE_TEST_CODE_END \
    return TEST_SUCC;         \
}


/**
* @brief 定义需要测试组的测试函数开始位置
* @param [in] _testfunc   测试函数名
*/
#define _DEFINE_GROUP_TEST_CODE_START(_testfunc, stackName) 			 \
static int_t _testfunc(struct Case *testCase) 						     \
{   																     \
	testCase->MySuite->TestGroupFree = stackName##_stack_free;			 \
    if( TEST_FAIL == stackName##_stack_init())							 \
		RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "TEST_SUCC", "TEST_FAIL", TEST_FAIL , "参数栈初始化出错");\
	testCase->TESTGroupSP = OPEN_TESTGROUP;								 \
	while( testCase->TESTGroupSP < stackName##Manager->stackPointer){	 \
	testCase->TESTGroupSP++;
	


/**
* @brief 定义需要测试组的测试函数结束位置
*/
#define _DEFINE_GROUP_TEST_CODE_END \
	}								\
    return TEST_SUCC;         		\
}
/**
* @brief 测试一个实例
* @param [in] _testfunc   已经定义的测试函数名
* @param [in] name        测试实例名
* @param [in] desc        测试实例描述
*/
#define _TEST_CASE(_testfunc, name, desc)                             \
do{                                                                   \
        struct Case testCase;                                         \
        _test_init_case(&testCase);                                   \
		testCase.MySuite = &testSuite;								  \
        testCase.CaseName = #name;                                    \
        if (NULL != testSuite.SuiteSetup)                             \
                testSuite.SuiteSetup();                               \
        u32_t result = _testfunc(&testCase);                          \
        if (NULL != testSuite.SuiteTeardown)                          \
                testSuite.SuiteTeardown();                            \
        if (NULL != testSuite.TestGroupFree)						  \
				testSuite.TestGroupFree();							  \
        _test_show_case_result(&testSuite, &testCase, #desc, result); \
}while (0)

/**
* @brief 开始测试一段代码运行时计时
*/
#define _TEST_START_TIMER          \
        struct timespec caseStart, caseEnd;\
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &caseStart);
/**
* @brief 结束测试一段代码运行时计时
*/
#define _TEST_END_TIMER                                   \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &caseEnd);\
        testCase->CaseTime = _test_gettime(caseStart, caseEnd);
/**
* @brief 测试打印
*/
#define _TEST_PRINT(...) testCase->PrintLen += snprintf(testCase->PrintText+testCase->PrintLen ,1024-testCase->PrintLen,__VA_ARGS__);
/**
* @brief 定义缓冲区数据装载函数每次调用case都会进行装载和卸载
* @param [in] _testsetup   装载函数名
*/
#define _TEST_DEFINE_CASE_SETUP(_testsetup)  static void _testsetup(void) 
/**
* @brief 定义缓冲区数据装载函数每次调用case都会进行装载和卸载
* @param [in] _testteardown  卸载函数函数名
*/
#define _TEST_DEFINE_CASE_TEARDOWN(_testteardown)  static void _testteardown(void)


/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] paramNum   栈内测试参数数量
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数值
* @param [in] type2 第2个测试参数类型
* @param [in] sum2  第2个测试参数数值
* @param [in] type3 第3个测试参数类型
* @param [in] sum3  第3个测试参数数值
* @param [in] type4 第4个测试参数类型
* @param [in] sum4  第4个测试参数数值
* @param [in] type5 第5个测试参数类型
* @param [in] sum5  第5个测试参数数值
 - VOID_PARAM 当不需要使用这个参数，置为无效的，和NULL_SUM同时使用
 - NULL_SUM   当不需要使用这个参数，置为无效的，和VOID_PARAM同时使用
*/
#define _DEFINE_PARAM_STACK_AND_INIT_START(stackName, paramNum, type1, sum1, type2, sum2, type3, sum3, type4, sum4, type5, sum5) \
struct stackName##_stack                                                                                                         \
    {                                                                                                                            \
	type1 first;                                                                                                                 \
	type2 second;                                                                                                                \
	type3 third;                                                                                                                 \
	type4 fourth;                                                                                                                \
	type5 fifth;                                                                                                                 \
};                                                                                                                               \
struct stackName##Manage{                                                                                                        \
	int_t stackPointer;                                                                                                          \
	int_t stackSize;                                                                                                             \
	u8_t paramNumber;                                                                                                            \
	struct stackName##_stack paramStack[sum1*sum2*sum3*sum4*sum5];                                                               \
};                                                                                                                  			 \
struct stackName##Manage *stackName##Manager;																				     \
static int_t stackName##_push_in_stack( struct stackName##Manage *stack, ...)													 \
{																																 \
	va_list value; 																												 \
	int_t count=0; 																												 \
	stackName##Manager->stackPointer++;																							 \
    va_start(value,stack);																										 \
    if(paramNum >= ONE_PARAM)   {stack->paramStack[stack->stackPointer].first  = va_arg(value, type1); count++;}	 		     \
	if(paramNum >= TWO_PARAM)   {stack->paramStack[stack->stackPointer].second = va_arg(value, type2); count++;}	 		     \
	if(paramNum >= THREE_PARAM) {stack->paramStack[stack->stackPointer].third  = va_arg(value, type3); count++;}	 		     \
	if(paramNum >= FOUR_PARAM)  {stack->paramStack[stack->stackPointer].fourth = va_arg(value, type4); count++;}		 	     \
	if(paramNum >= FIVE_PARAM)  {stack->paramStack[stack->stackPointer].fifth  = va_arg(value, type5); count++;}		 	     \
    va_end(value);																												 \
	if( paramNum > count ) RETURN_ERRDESC( ERR_USER, TEST_FAIL, "parameter less than " #paramNum "\n"); 						 \
}																																 \
static void stackName##_stack_free(void)												 										 \
{																																 \
	TEST_free(stackName##Manager);																								 \
}																																 \
static int_t stackName##_stack_init(void)                                                                                        \
{     																															 \
	stackName##Manager = (struct stackName##Manage *)TEST_malloc(sizeof(struct stackName##Manage));								 \
	if(NULL == stackName##Manager) RETURN_ERRDESC( ERR_USER, TEST_FAIL,"stack malloc fault\n" );     			                 \
	if( paramNum > FIVE_PARAM  || paramNum < ONE_PARAM ) RETURN_ERRDESC( ERR_USER, TEST_FAIL, "param number was wrong\n"); 	     \
	stackName##Manager->paramNumber = paramNum;	                      										                     \
	stackName##Manager->stackPointer = -1;                                                                                       \
	stackName##Manager->stackSize = sum1 * sum2 * sum3 * sum4 * sum5;

/**
* @brief 测试参数栈初始化结束
*/
#define _DEFINE_PARAM_STACK_AND_INIT_END \
    return TEST_SUCC;         \
}


/** 
* @brief 一个层放入到存储五个参数的栈中
* @param [in] stackName  栈名
* @param [in] value1 将一层栈中第一个值写入栈 
* @param [in] value2 将一层栈中第二个值写入栈 
* @param [in] value3 将一层栈中第三个值写入栈 
* @param [in] value4 将一层栈中第四个值写入栈 
* @param [in] value5 将一层栈中第五个值写入栈 
* @par 例子
* @code
*    PUSH_SOME_PARAM_IN(stackName, value1);
*    PUSH_SOME_PARAM_IN(stackName, value1, value2);   
*    PUSH_SOME_PARAM_IN(stackName, value1, value2, value3);   
*    PUSH_SOME_PARAM_IN(stackName, value1, value2, value3, value4);      
*    PUSH_SOME_PARAM_IN(stackName, value1, value2, value3, value4, value5);         
* @endcode
*/
#define _PUSH_SOME_PARAM_IN(stackName, ...) stackName##_push_in_stack( stackName##Manager, __VA_ARGS__ )
    //if( TEST_FAIL == stackName##_push_in_stack( stackName##Manager, __VA_ARGS__ ))					     \
		//RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "TEST_SUCC", "TEST_FAIL", TEST_FAIL , "参数入栈出错");\



/**  @brief 参数序列 */
#define FIRST  first   /**< 取参数的栈的目标层第一个参数*/
#define SECOND second  /**< 取参数的栈的目标层第二个参数*/
#define THIRD  third   /**< 取参数的栈的目标层第三个参数*/
#define FOURTH fourth  /**< 取参数的栈的目标层第四个参数*/
#define FIFTH  fifth   /**< 取参数的栈的目标层第五个参数*/

/** 
* @brief 从参数的栈的每层中取出第sequence个参数
* @param [in] stackName  栈名
* @param [in] sequence  序列
 - FIRST  取参数的栈的目标层第一个参数
 - SECOND 取参数的栈的目标层第二个参数
 - THIRD  取参数的栈的目标层第三个参数
 - FOURTH 取参数的栈的目标层第四个参数
 - FIFTH  取参数的栈的目标层第五个参数
*/
#define _PUSH_SOME_PARAM_OUT(stackName, sequence) stackName##Manager->paramStack[testCase->TESTGroupSP].sequence

#endif /*__INC_TEST_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
