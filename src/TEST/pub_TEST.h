/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: pub_TEST.h
@brief: �ṩ���Ժ�Ĳ��Թ���ͷ�ļ�
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/04/6
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_PUB_TEST_H__
#define __INC_PUB_TEST_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "TEST.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/

/******************************************************************
*   Name�������ӿں�---�жϺ�
*   Desc��1. ASSERT_* ϵ�еĶ��ԣ�������ʧ��ʱ���˳���ǰ������ע�⣺�����˳���ǰ�������� 
*         2. eq��� ne�����  gt����  ltС�� ge���ڵ���  leС�ڵ���
******************************************************************/
/**  @brief ���Ժ������ؽ��Ϊ�� ����Ϊ֮����Դ��� condition �����ж� desc �������� */
#define TEST_FUNC_RETURN_SUCC(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_SUCC", "FUNC_FAIL/FUNC_NULL", ((condition) == FUNC_SUCC) , desc)
/**  @brief ���Ժ������ؽ��Ϊ�� ����Ϊ�١����Դ��� condition �����ж� desc �������� */
#define TEST_FUNC_RETURN_FAIL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_FAIL", "FUNC_SUCC", ((condition) == FUNC_FAIL), desc)
/**  @brief ���Ժ������ؽ��ΪNULL ����Ϊ�١����Դ��� condition �����ж� desc �������� */
#define TEST_FUNC_RETURN_NULL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_NULL", "FUNC_SUCC", ((condition) == FUNC_NULL), desc)
/**  @brief ���Ժ������ؽ��Ϊ��NULL ��ΪNULL�����Դ��� condition �����ж� desc �������� */
#define TEST_FUNC_RETURN_IS_NOT_NULL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_NULL", "FUNC_SUCC", ((condition) != FUNC_NULL), desc)
/**  @brief ���Ժ������ؽ��Ϊ��fail ��Ϊfail�����Դ��� condition �����ж� desc �������� */
#define TEST_FUNC_RETURN_IS_NOT_FAIL(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FUNC_FAIL", "FUNC_SUCC", ((condition) != FUNC_FAIL), desc)

/**  @brief ���Խ��Ϊ�� ����Ϊ֮����Դ��� condition �����ж� desc �������� */
#define TEST_IS_TRUE(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "TRUE", "FALSE", (condition), desc)
/**  @brief ���Խ��Ϊ�� ����Ϊ�١����Դ��� condition �����ж� desc �������� */
#define TEST_IS_FALSE(condition, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, "FALSE", "TRUE", !(condition), desc)

/**  @brief ���Խ��value1����value2 �����������*/
#define TEST_VALUE_IS_EQ(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 == value2), desc)
/**  @brief ���Խ��value1������value2 �����������*/
#define TEST_VALUE_IS_NE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 != value2), desc)
/**  @brief ���Խ��value1С��value2 �����������*/
#define TEST_VALUE_IS_LT(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 < value2), desc)
/**  @brief ���Խ��value1С�ڵ���value2 �����������*/
#define TEST_VALUE_IS_LE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 <= value2), desc)
/**  @brief ���Խ��value1����value2 �����������*/
#define TEST_VALUE_IS_GT(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 > value2), desc)
/**  @brief ���Խ��value1���ڵ���value2 �����������*/
#define TEST_VALUE_IS_GE(value1, value2, desc) RETURN_VAL(__FUNCTION__, __LINE__, __FILE__, value1, value2, (value1 >= value2), desc)

/**  @brief ���Խ���ַ���result����str�ַ��� ���Ȳ�������*/
#define TEST_STRING_IS_EQ(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) == 0), desc)
/**  @brief ���Խ���ַ���result������str�ַ��� �����������*/
#define TEST_STRING_IS_NE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) != 0), desc)
/**  @brief ���Խ���ַ���result����str�ַ��� �����������*/
#define TEST_STRING_IS_LT(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) > 0), desc)
/**  @brief ���Խ���ַ���result���ڵ���str�ַ��� �����������*/
#define TEST_STRING_IS_LE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) >= 0), desc)
/**  @brief ���Խ���ַ���resultС��str�ַ��� ���Ȳ�������*/
#define TEST_STRING_IS_GT(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) < 0), desc)
/**  @brief ���Խ���ַ���resultС�ڵ���str�ַ��� ���Ȳ�������*/
#define TEST_STRING_IS_GE(str, result, desc) RETURN_STR(__FUNCTION__, __LINE__, __FILE__, str, result, (strcmp(result, str) <= 0), desc)
/******************************************************************
*   Name�������ӿں�---������
*   Desc��1.�������Ե��׼�
*         2.����������ԵĴ����
*         2.�������Ե�ʵ���ʹ����ԵĴ����
******************************************************************/
/**
* @brief ��Ԫ���Գ�ʼ��
*/
#define Start_UnitTest _test_start_unit_test
/**
* @brief ������Ҫ���ԵĲ��Ժ�����ʼλ�� �������׼���ʱ��
* @param [in] name   �����׼���
*/
#define TEST_SUITE_START(name) _TEST_SUITE_START(name) 
/**
* @brief ������Ҫ���ԵĲ��Ժ�������λ��
*/
#define TEST_SUITE_END  _TEST_SUITE_END

/**
* @brief ������Ҫ���ԵĲ��Ժ�����ʼλ��
* @param [in] _testfunc   ���Ժ�����
*/
#define DEFINE_TEST_CODE_START(_testfunc) _DEFINE_TEST_CODE_START(_testfunc) 
/**
* @brief ������Ҫ���ԵĲ��Ժ�������λ��
*/
#define DEFINE_TEST_CODE_END _DEFINE_TEST_CODE_END

/**
* @brief ������Ҫ������Ĳ��Ժ�����ʼλ��
* @param [in] _testfunc   ���Ժ�����
*/
#define DEFINE_GROUP_TEST_CODE_START(_testfunc, stackName) _DEFINE_GROUP_TEST_CODE_START(_testfunc, stackName)

/**
* @brief ������Ҫ������Ĳ��Ժ�������λ��
*/
#define DEFINE_GROUP_TEST_CODE_END _DEFINE_GROUP_TEST_CODE_END 

/**
* @brief ����һ��ʵ��
* @param [in] _testfunc   �Ѿ�����Ĳ��Ժ�����
* @param [in] desc        ����ʵ����
* @param [in] desc        ����ʵ������
*/
#define TEST_CASE(_testfunc, name, desc) _TEST_CASE(_testfunc, name, desc)
/**
* @brief ��ʼ����һ�δ�������ʱ��
*/
#define TEST_START_TIMER() _TEST_START_TIMER
/**
* @brief ��������һ�δ�������ʱ��
*/
#define TEST_END_TIMER() _TEST_END_TIMER
/**
* @brief ���Դ�ӡ ÿ���ַ����������ӻ��з�
*/
#define test_printf(...) _TEST_PRINT(__VA_ARGS__)
/**
* @brief ���建��������װ�غ�����ÿ�ε���case�������װ�غ�ж��
* @param [in] _testsetup   װ�غ�����
*/
#define TEST_DEFINE_CASE_SETUP(_testsetup) _TEST_DEFINE_CASE_SETUP(_testsetup)
/**
* @brief ���建��������ж�غ��� ÿ�ε���case�������װ�غ�ж��
* @param [in] _testteardown  ж�غ�����
*/
#define TEST_DEFINE_CASE_TEARDOWN(_testteardown)  _TEST_DEFINE_CASE_TEARDOWN(_testteardown)
/**
* @brief ע��suite�µ�case������װ�غ�ж�غ���
*/
#define test_sutie_register_setup_teardown(setup, teardown) _TEST_SUITE_REGISTER_SETUP_TEARDOWN(setup, teardown)

/**
* @brief ����paramNum�����Բ����Ĳ���ջ����ʼ�� ���Ͳ�֧������ ������Ϊfloatʱ����ҪתΪdouble ���������ͳ���С��intʱ����ҪתΪint
* @param [in] stackName  ջ��
* @param [in] type1 ��1�����Բ�������
* @param [in] sum1  ��1�����Բ�������
*/
#define DEFINE_ONE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, ONE_PARAM, type1, sum1, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)

/**
* @brief ����paramNum�����Բ����Ĳ���ջ����ʼ�� ���Ͳ�֧������ ������Ϊfloatʱ����ҪתΪdouble ���������ͳ���С��intʱ����ҪתΪint
* @param [in] stackName  ջ��
* @param [in] type1 ��1�����Բ�������
* @param [in] sum1  ��1�����Բ�������
* @param [in] type2 ��2�����Բ�������
* @param [in] sum2  ��2�����Բ�������
*/
#define DEFINE_TWO_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, TWO_PARAM, type1, sum1, type2, sum2, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)


/**
* @brief ����paramNum�����Բ����Ĳ���ջ����ʼ�� ���Ͳ�֧������ ������Ϊfloatʱ����ҪתΪdouble ���������ͳ���С��intʱ����ҪתΪint
* @param [in] stackName  ջ��
* @param [in] type1 ��1�����Բ�������
* @param [in] sum1  ��1�����Բ�������
* @param [in] type2 ��2�����Բ�������
* @param [in] sum2  ��2�����Բ�������
* @param [in] type3 ��3�����Բ�������
* @param [in] sum3  ��3�����Բ�������
*/
#define DEFINE_THREE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, THREE_PARAM, type1, sum1, type2, sum2, type3, sum3, VOID_PARAM, NULL_SUM, VOID_PARAM, NULL_SUM)


/**
* @brief ����paramNum�����Բ����Ĳ���ջ����ʼ�� ���Ͳ�֧������ ������Ϊfloatʱ����ҪתΪdouble ���������ͳ���С��intʱ����ҪתΪint
* @param [in] stackName  ջ��
* @param [in] type1 ��1�����Բ�������
* @param [in] sum1  ��1�����Բ�������
* @param [in] type2 ��2�����Բ�������
* @param [in] sum2  ��2�����Բ�������
* @param [in] type3 ��3�����Բ�������
* @param [in] sum3  ��3�����Բ�������
* @param [in] type4 ��4�����Բ�������
* @param [in] sum4  ��4�����Բ�������
*/
#define DEFINE_FOUR_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3, type4, sum4) \
          _DEFINE_PARAM_STACK_AND_INIT_START(stackName, FOUR_PARAM, type1, sum1, type2, sum2, type3, sum3, type4, sum4, VOID_PARAM, NULL_SUM)


/**
* @brief ����paramNum�����Բ����Ĳ���ջ����ʼ�� ���Ͳ�֧������ ������Ϊfloatʱ����ҪתΪdouble ���������ͳ���С��intʱ����ҪתΪint
* @param [in] stackName  ջ��
* @param [in] type1 ��1�����Բ�������
* @param [in] sum1  ��1�����Բ�������
* @param [in] type2 ��2�����Բ�������
* @param [in] sum2  ��2�����Բ�������
* @param [in] type3 ��3�����Բ�������
* @param [in] sum3  ��3�����Բ�������
* @param [in] type4 ��4�����Բ�������
* @param [in] sum4  ��4�����Բ�������
* @param [in] type5 ��5�����Բ�������
* @param [in] sum5  ��5�����Բ�������
*/
#define DEFINE_FIVE_PARAM_STACK_AND_INIT_START(stackName,type1, sum1, type2, sum2, type3, sum3, type4, sum4, type5, sum5) \
              _DEFINE_PARAM_STACK_AND_INIT_START(stackName, FIVE_PARAM, type1, sum1, type2, sum2, type3, sum3, type4, sum4, type5, sum5)

/**
* @brief ���Բ���ջ��ʼ������
*/
#define DEFINE_PARAM_STACK_AND_INIT_END _DEFINE_PARAM_STACK_AND_INIT_END

/** 
* @brief һ������뵽�洢���������ջ��
* @param [in] stackName  ջ��
* @param [in] value1 ��һ��ջ�е�һ��ֵд��ջ 
* @param [in] value2 ��һ��ջ�еڶ���ֵд��ջ 
* @param [in] value3 ��һ��ջ�е�����ֵд��ջ 
* @param [in] value4 ��һ��ջ�е��ĸ�ֵд��ջ 
* @param [in] value5 ��һ��ջ�е����ֵд��ջ 
* @par ����
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
* @brief �Ӳ�����ջ��ÿ����ȡ����sequence������
* @param [in] stackName  ջ��
* @param [in] sequence  ����
 - FIRST  ȡ������ջ��Ŀ����һ������
 - SECOND ȡ������ջ��Ŀ���ڶ�������
 - THIRD  ȡ������ջ��Ŀ������������
 - FOURTH ȡ������ջ��Ŀ�����ĸ�����
 - FIFTH  ȡ������ջ��Ŀ�����������
*/
#define PUSH_SOME_PARAM_OUT(stackName, sequence) _PUSH_SOME_PARAM_OUT(stackName, sequence)


#endif /*__INC_PUB_TEST_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
