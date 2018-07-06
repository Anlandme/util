/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: pub_TEST.h
@brief: 提供测试宏的测试公共头文件
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/04/6
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_PUB_TEST_H__
#define __INC_PUB_TEST_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "TEST.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/

/******************************************************************
*   Name：公共接口宏---判断宏
*   Desc：1. ASSERT_* 系列的断言，当检查点失败时，退出当前函数（注意：并非退出当前案例）。 
*         2. eq相等 ne不相等  gt大于  lt小于 ge大于等于  le小于等于
******************************************************************/
/**  @brief 测试函数返回结果为真 当不为之真测试错误 condition 条件判断 desc 条件描述 */
#define TEST_FUNC_RETURN_SUCC(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_SUCC", "FUNC_FAIL/FUNC_NULL", ((condition) == FUNC_SUCC) , desc)
/**  @brief 测试函数返回结果为假 当不为假。测试错误 condition 条件判断 desc 条件描述 */
#define TEST_FUNC_RETURN_FAIL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_FAIL", "FUNC_SUCC", ((condition) == FUNC_FAIL), desc)
/**  @brief 测试函数返回结果为NULL 当不为假。测试错误 condition 条件判断 desc 条件描述 */
#define TEST_FUNC_RETURN_NULL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_NULL", "FUNC_SUCC", ((condition) == FUNC_NULL), desc)
/**  @brief 测试函数返回结果为非NULL 当为NULL。测试错误 condition 条件判断 desc 条件描述 */
#define TEST_FUNC_RETURN_IS_NOT_NULL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_NULL", "FUNC_SUCC", ((condition) != FUNC_NULL), desc)
/**  @brief 测试函数返回结果为非fail 当为fail。测试错误 condition 条件判断 desc 条件描述 */
#define TEST_FUNC_RETURN_IS_NOT_FAIL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_FAIL", "FUNC_SUCC", ((condition) != FUNC_FAIL), desc)

/**  @brief 测试结果为真 当不为之真测试错误 condition 条件判断 desc 条件描述 */
#define TEST_IS_TRUE(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "TRUE", "FALSE", (condition), desc)
/**  @brief 测试结果为假 当不为假。测试错误 condition 条件判断 desc 条件描述 */
#define TEST_IS_FALSE(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FALSE", "TRUE", !(condition), desc)

/**  @brief 测试结果value1等于value2 否则产生错误*/
#define TEST_VALUE_IS_EQ(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 == value2), desc)
/**  @brief 测试结果value1不等于value2 否则产生错误*/
#define TEST_VALUE_IS_NE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 != value2), desc)
/**  @brief 测试结果value1小于value2 否则产生错误*/
#define TEST_VALUE_IS_LT(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 < value2), desc)
/**  @brief 测试结果value1小于等于value2 否则产生错误*/
#define TEST_VALUE_IS_LE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 <= value2), desc)
/**  @brief 测试结果value1大于value2 否则产生错误*/
#define TEST_VALUE_IS_GT(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 > value2), desc)
/**  @brief 测试结果value1大于等于value2 否则产生错误*/
#define TEST_VALUE_IS_GE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 >= value2), desc)

/**  @brief 测试结果字符串result等于str字符串 不等产生错误*/
#define TEST_STRING_IS_EQ(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) == 0), desc)
/**  @brief 测试结果字符串result不等于str字符串 否则产生错误*/
#define TEST_STRING_IS_NE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) != 0), desc)
/**  @brief 测试结果字符串result大于str字符串 否则产生错误*/
#define TEST_STRING_IS_LT(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) > 0), desc)
/**  @brief 测试结果字符串result大于等于str字符串 否则产生错误*/
#define TEST_STRING_IS_LE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) >= 0), desc)
/**  @brief 测试结果字符串result小于str字符串 不等产生错误*/
#define TEST_STRING_IS_GT(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) < 0), desc)
/**  @brief 测试结果字符串result小于等于str字符串 不等产生错误*/
#define TEST_STRING_IS_GE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) <= 0), desc)
/******************************************************************
*   Name：公共接口宏---基础宏
*   Desc：1.建立测试的套件
*         2.定义所需测试的代码段
*         2.建立测试的实例和待测试的代码段
******************************************************************/
/**
* @brief 单元测试初始化
*/
#define Start_UnitTest _test_start_unit_test
/**
* @brief 定义需要测试的测试函数开始位置 并启动套件计时器
* @param [in] name   测试套件名
*/
#define TEST_SUITE_START(name) _TEST_SUITE_START(name) 
/**
* @brief 定义需要测试的测试函数结束位置
*/
#define TEST_SUITE_END  _TEST_SUITE_END

/**
* @brief 定义需要测试的测试函数开始位置
* @param [in] _testfunc   测试函数名
*/
#define DEFINE_TEST_CODE_START(_testfunc) _DEFINE_TEST_CODE_START(_testfunc) 
/**
* @brief 定义需要测试的测试函数结束位置
*/
#define DEFINE_TEST_CODE_END _DEFINE_TEST_CODE_END

/**
* @brief 定义需要测试组的测试函数开始位置
* @param [in] _testfunc   测试函数名
*/
#define DEFINE_GROUP_TEST_CODE_START(_testfunc, stackName) _DEFINE_GROUP_TEST_CODE_START(_testfunc, stackName)

/**
* @brief 定义需要测试组的测试函数结束位置
*/
#define DEFINE_GROUP_TEST_CODE_END _DEFINE_GROUP_TEST_CODE_END 

/**
* @brief 测试一个实例
* @param [in] _testfunc   已经定义的测试函数名
* @param [in] desc        测试实例名
* @param [in] desc        测试实例描述
*/
#define TEST_CASE(_testfunc, name, desc) _TEST_CASE(_testfunc, name, desc)
/**
* @brief 开始测试一段代码运行时间
*/
#define TEST_START_TIMER() _TEST_START_TIMER
/**
* @brief 结束测试一段代码运行时间
*/
#define TEST_END_TIMER() _TEST_END_TIMER
/**
* @brief 测试打印 每串字符串后面必须加换行符
*/
#define test_printf(...) _TEST_PRINT(__VA_ARGS__)
/**
* @brief 定义缓冲区数据装载函数，每次调用case都会进行装载和卸载
* @param [in] _testsetup   装载函数名
*/
#define TEST_DEFINE_CASE_SETUP(_testsetup) _TEST_DEFINE_CASE_SETUP(_testsetup)
/**
* @brief 定义缓冲区数据卸载函数 每次调用case都会进行装载和卸载
* @param [in] _testteardown  卸载函数名
*/
#define TEST_DEFINE_CASE_TEARDOWN(_testteardown)  _TEST_DEFINE_CASE_TEARDOWN(_testteardown)
/**
* @brief 注册suite下的case的数据装载和卸载函数
*/
#define test_sutie_register_setup_teardown(setup, teardown) _TEST_SUITE_REGISTER_SETUP_TEARDOWN(setup, teardown)

/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数量
*/
#define DEFINE_ONE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, ONE_PARAM, type1, sum1, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)

/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数量
* @param [in] type2 第2个测试参数类型
* @param [in] sum2  第2个测试参数数量
*/
#define DEFINE_TWO_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, TWO_PARAM, type1, sum1, type2, sum2, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)


/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数量
* @param [in] type2 第2个测试参数类型
* @param [in] sum2  第2个测试参数数量
* @param [in] type3 第3个测试参数类型
* @param [in] sum3  第3个测试参数数量
*/
#define DEFINE_THREE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, THREE_PARAM, type1, sum1, type2, sum2, type3, sum3, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)


/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数量
* @param [in] type2 第2个测试参数类型
* @param [in] sum2  第2个测试参数数量
* @param [in] type3 第3个测试参数类型
* @param [in] sum3  第3个测试参数数量
* @param [in] type4 第4个测试参数类型
* @param [in] sum4  第4个测试参数数量
*/
#define DEFINE_FOUR_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3, type4, sum4) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, FOUR_PARAM, type1, sum1, type2, sum2, type3, sum3, type4, sum4, VOID_PARAM, NULL_SUM)


/**
* @brief 定义paramNum个测试参数的参数栈并初始化 类型不支持数组 当类型为float时候需要转为double 当整数类型长度小于int时候需要转为int
* @param [in] stackName  栈名
* @param [in] type1 第1个测试参数类型
* @param [in] sum1  第1个测试参数数量
* @param [in] type2 第2个测试参数类型
* @param [in] sum2  第2个测试参数数量
* @param [in] type3 第3个测试参数类型
* @param [in] sum3  第3个测试参数数量
* @param [in] type4 第4个测试参数类型
* @param [in] sum4  第4个测试参数数量
* @param [in] type5 第5个测试参数类型
* @param [in] sum5  第5个测试参数数量
*/
#define DEFINE_FIVE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3, type4, sum4, type5, sum5) \
              _DEFINE_PARAM_STACK_AND_INIT_START(stackName, FIVE_PARAM, type1, sum1, type2, sum2, type3, sum3, type4, sum4, type5, sum5)

/**
* @brief 测试参数栈初始化结束
*/
#define DEFINE_PARAM_STACK_AND_INIT_END _DEFINE_PARAM_STACK_AND_INIT_END

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
#define PUSH_SOME_PARAM_IN(stackName, ...) _PUSH_SOME_PARAM_IN(stackName, __VA_ARGS__) 

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
#define PUSH_SOME_PARAM_OUT(stackName, sequence) _PUSH_SOME_PARAM_OUT(stackName, sequence)


#endif /*__INC_PUB_TEST_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
