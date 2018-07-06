/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file:  utildef.h
@brief: 头文件主要是一些公共的变量定义
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_UTIL_DEF_H__
#define __INC_UTIL_DEF_H__

/* 
*#define PINT int *  　
*typedef int * pint;  

*Const pint p; //p的值不能改变，p指向的内存里的值可以改变。相当于int *const p，这里Const修饰的是p，pint是一个变量类型，不可拆分;　　
*Const PINT p; //p的值可以改变，p指向的内存里的值不能改变，相当于const　int *p;或int const *p; define只是单纯的替换！！

*pint a, b; //a和b都是int类型的指针
*PINT a, b; //相当于int *a, b;只有a是指针
*/

/**  @brief 空类型 */
typedef void             void_t; 
/**  @brief 字符类型 */
typedef int              int_t; 
/**  @brief 无符号类型 */
typedef uint             uint_t; 
/**  @brief 字符类型 */
typedef long             long_t; 
/**  @brief 字符类型 */
typedef unsigned long    ulong_t; 
/**  @brief 字符类型 */
typedef long int         longint_t; 
/**  @brief 字符类型 */
typedef unsigned long    ulongint_t; 
/**  @brief 字符串类型 */
typedef char *           string_t; //区别 #define string_t char*
/**  @brief 字符类型 */
typedef char              char_t; 
/**  @brief 8位有符号类型 */
typedef int8_t          	s8_t;
/**  @brief 16位有符号类型 */
typedef int16_t          	s16_t;
/**  @brief 32位有符号类型 */
typedef int32_t		   		  s32_t;
/**  @brief 64位有符号类型 */
typedef int64_t		   		  s64_t;


/**  @brief 8位无符号类型 */
typedef uint8_t   	      u8_t;
/**  @brief 16位无符号类型 */
typedef uint16_t  	      u16_t;
/**  @brief 32位无符号类型 */
typedef uint32_t   	      u32_t;
/**  @brief 64位无符号类型 */
typedef uint64_t		   		u64_t;

/**  @brief 定义布尔类型 */
typedef u32_t 				    bool_t;

/**  @brief 定义浮点类型 */
typedef float 				    float_t;
/**  @brief 定义浮点类型 */
typedef double 				    double_t;


/**  @brief 8位有符号类型最小值 */ 
# define S8_MIN           INT8_MIN//(-128)
/**  @brief 16位有符号类型最小值 */ 
# define S16_MIN          INT16_MIN//(-32767-1)
/**  @brief 32位有符号类型最小值 */   
# define S32_MIN          INT32_MIN//(-2147483647-1)
/**  @brief 64位有符号类型最小值 */   
# define S64_MIN          INT64_MIN//(-__INT64_C(9223372036854775807)-1)  


/**  @brief 8位有符号类型最大值 */ 
# define S8_MAX           INT8_MAX//(127)
/**  @brief 16位有符号类型最大值 */   
# define S16_MAX          INT16_MAX//(32767)
/**  @brief 32位有符号类型最大值 */   
# define S32_MAX          INT32_MAX//(2147483647)
/**  @brief 64位有符号类型最大值 */   
# define S64_MAX          INT64_MAX//(__INT64_C(9223372036854775807))  
  

/**  @brief 8位无符号类型最大值 */   
# define U8_MAX           UINT8_MAX//(255)
/**  @brief 16位无符号类型最大值 */   
# define U16_MAX          UINT16_MAX//(65535)
/**  @brief 32位无符号类型最大值 */   
# define U32_MAX          UINT32_MAX//(4294967295U)
/**  @brief 64位无符号类型最大值 */   
# define U64_MAX          UINT64_MAX//(__UINT64_C(18446744073709551615))  


/**  @brief 函数返回成功 */
#define FUNC_SUCC          (0)
/**  @brief 函数返回失败 */
#define FUNC_FAIL          (-1)
/**  @brief 函数返回空指针 */
#define FUNC_NULL         ((void*)0)

/**  @brief 定义布尔值false */
#define FALSE             false
/**  @brief 定义布尔值true */
#define TRUE              true

/**  @brief 使能 */
#define ENABLED 1
/**  @brief 失能 */
#define DISABLE 0

/**  @brief 参数是空的 */
#define PARAMISNULL(param)                   (NULL == param)
/**  @brief 字符串是空的 */
#define STRINGISNULL(string)                   (NULL == string)

/**  @brief 模块表 */
enum modules{
  MOD_SYSREM = 0,
  MOD_EXCEPT = 1,
  MOD_BUTT,
};

#endif/*__INC_UTIL_DEF_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
