/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file:  utildef.h
@brief: ͷ�ļ���Ҫ��һЩ�����ı�������
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_UTIL_DEF_H__
#define __INC_UTIL_DEF_H__

/* 
*#define PINT int *  ��
*typedef int * pint;  

*Const pint p; //p��ֵ���ܸı䣬pָ����ڴ����ֵ���Ըı䡣�൱��int *const p������Const���ε���p��pint��һ���������ͣ����ɲ��;����
*Const PINT p; //p��ֵ���Ըı䣬pָ����ڴ����ֵ���ܸı䣬�൱��const��int *p;��int const *p; defineֻ�ǵ������滻����

*pint a, b; //a��b����int���͵�ָ��
*PINT a, b; //�൱��int *a, b;ֻ��a��ָ��
*/

/**  @brief ������ */
typedef void             void_t; 
/**  @brief �ַ����� */
typedef int              int_t; 
/**  @brief �޷������� */
typedef uint             uint_t; 
/**  @brief �ַ����� */
typedef long             long_t; 
/**  @brief �ַ����� */
typedef unsigned long    ulong_t; 
/**  @brief �ַ����� */
typedef long int         longint_t; 
/**  @brief �ַ����� */
typedef unsigned long    ulongint_t; 
/**  @brief �ַ������� */
typedef char *           string_t; //���� #define string_t char*
/**  @brief �ַ����� */
typedef char              char_t; 
/**  @brief 8λ�з������� */
typedef int8_t          	s8_t;
/**  @brief 16λ�з������� */
typedef int16_t          	s16_t;
/**  @brief 32λ�з������� */
typedef int32_t		   		  s32_t;
/**  @brief 64λ�з������� */
typedef int64_t		   		  s64_t;


/**  @brief 8λ�޷������� */
typedef uint8_t   	      u8_t;
/**  @brief 16λ�޷������� */
typedef uint16_t  	      u16_t;
/**  @brief 32λ�޷������� */
typedef uint32_t   	      u32_t;
/**  @brief 64λ�޷������� */
typedef uint64_t		   		u64_t;

/**  @brief ���岼������ */
typedef u32_t 				    bool_t;

/**  @brief ���帡������ */
typedef float 				    float_t;
/**  @brief ���帡������ */
typedef double 				    double_t;


/**  @brief 8λ�з���������Сֵ */ 
# define S8_MIN           INT8_MIN//(-128)
/**  @brief 16λ�з���������Сֵ */ 
# define S16_MIN          INT16_MIN//(-32767-1)
/**  @brief 32λ�з���������Сֵ */   
# define S32_MIN          INT32_MIN//(-2147483647-1)
/**  @brief 64λ�з���������Сֵ */   
# define S64_MIN          INT64_MIN//(-__INT64_C(9223372036854775807)-1)  


/**  @brief 8λ�з����������ֵ */ 
# define S8_MAX           INT8_MAX//(127)
/**  @brief 16λ�з����������ֵ */   
# define S16_MAX          INT16_MAX//(32767)
/**  @brief 32λ�з����������ֵ */   
# define S32_MAX          INT32_MAX//(2147483647)
/**  @brief 64λ�з����������ֵ */   
# define S64_MAX          INT64_MAX//(__INT64_C(9223372036854775807))  
  

/**  @brief 8λ�޷����������ֵ */   
# define U8_MAX           UINT8_MAX//(255)
/**  @brief 16λ�޷����������ֵ */   
# define U16_MAX          UINT16_MAX//(65535)
/**  @brief 32λ�޷����������ֵ */   
# define U32_MAX          UINT32_MAX//(4294967295U)
/**  @brief 64λ�޷����������ֵ */   
# define U64_MAX          UINT64_MAX//(__UINT64_C(18446744073709551615))  


/**  @brief �������سɹ� */
#define FUNC_SUCC          (0)
/**  @brief ��������ʧ�� */
#define FUNC_FAIL          (-1)
/**  @brief �������ؿ�ָ�� */
#define FUNC_NULL         ((void*)0)

/**  @brief ���岼��ֵfalse */
#define FALSE             false
/**  @brief ���岼��ֵtrue */
#define TRUE              true

/**  @brief ʹ�� */
#define ENABLED 1
/**  @brief ʧ�� */
#define DISABLE 0

/**  @brief �����ǿյ� */
#define PARAMISNULL(param)                   (NULL == param)
/**  @brief �ַ����ǿյ� */
#define STRINGISNULL(string)                   (NULL == string)

/**  @brief ģ��� */
enum modules{
  MOD_SYSREM = 0,
  MOD_EXCEPT = 1,
  MOD_BUTT,
};

#endif/*__INC_UTIL_DEF_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
