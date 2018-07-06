/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: err.h
@brief:���ڴ�����Ķ��� �ӿں����Ķ���
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/02  | 0.0.0.1   | D.C.P    | �����ļ�             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_ERR_H__
#define __INC_ERR_H__


/**  @brief �ɹ����� */
#define ERR_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define ERR_FAIL FUNC_FAIL

/**  @brief ������ṹ�� */
typedef struct {
    u32_t       ErrCode;    /**< Operation not permitted ������*/
    string_t    ErrString;  /**< Operation not permitted ��������*/
}Err;
/******************************************************************
*   Name��˽�нӿں� 
*   Desc���ڲ����õ�һЩ�꣬��Ҫ�ǻ���쳣�������ö�ջ
******************************************************************/
/**  
* @brief �����ӡ
*/
#define _ERR_LOG(result) printf("errno:%s, err-file:%s, err-line:%d, err-function:%s\n",err_strerr(errno), __FILE__, __LINE__, __FUNCTION__)

/**  
* @brief ���ش���� 
* @param [in] Errno ������
* @param [in] Return ����ERR_FAIL����ERR_NULL
* @attention glibc�Ⲣ����ʼ��errno��Ĭ��ΪEWOULDBLOCK or EAGAIN
*/
#define _RETURN_ERR( Errno, Return ) \
do{\
    errno = Errno;\
    return Return;\
}while(0)

/**  
* @brief ���ش���� 
* @param [in] Errno ������
* @param [in] Return ����ERR_FAIL����ERR_NULL
* @param [in] Desc  ֱ�Ӵ�ӡ�Ĵ����Զ�������
*/
#define _RETURN_ERRDESC( Errno, Return, Desc...) \
do{\
    printf("func:%s err:%d \n", __FUNCTION__ ,Errno);\
    printf(Desc);\
    errno = Errno;\
    return Return;\
}while(0)

/**  
* @brief �����Ƿ񷵻ش���
* @param [in] result ��������
* @param [in] Return ����ֵ
*/
#define _CHECK_RETURN( result, Return )\
do{\
    if( result ) {_ERR_LOG(result);return Return;}\
}while (0)

/**  
* @brief �����Ƿ񷵻�goto��ĳ��
* @param [in] result ����ֵ
* @param [in] Goto  goto���Ŀ�ĵ�
*/
#define _CHECK_GOTO( result, Goto )\
do{\
    if( result ) {_ERR_LOG(result);goto Goto;}\
}while (0)

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL ���򷵻�FUNC_FAIL/FUNC_NULL
* @param [in] result ����ֵ
*/
#define _CHECK_FF( result )  _CHECK_RETURN( (result) != FUNC_SUCC, FUNC_FAIL )
#define _CHECK_NF( result )  _CHECK_RETURN( (result) == FUNC_NULL, FUNC_FAIL )
#define _CHECK_FN( result )  _CHECK_RETURN( (result) != FUNC_SUCC, FUNC_NULL )
#define _CHECK_NN( result )  _CHECK_RETURN( (result) == FUNC_NULL, FUNC_NULL )

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL ����goto ��ĳ��
* @param [in] result ����ֵ
* @param [in] Goto  goto���Ŀ�ĵ�
*/
#define _CHECK_FG( result, Goto )  _CHECK_GOTO( (result) != FUNC_SUCC, Goto )
#define _CHECK_NG( result, Goto )  _CHECK_GOTO( (result) == FUNC_NULL, Goto )

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL ����goto ��ĳ��
* @param [in] result ����ֵ
*/
#define _CHECK_FL( result)  if( (result) != FUNC_SUCC ) _ERR_LOG(result)
#define _CHECK_NL( result)  if( (result) == FUNC_NULL ) _ERR_LOG(result)

#endif/*__INC_ERR_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
