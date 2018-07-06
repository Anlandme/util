/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: err.h
@brief:对于错误码的定义 接口函数的定义
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/02  | 0.0.0.1   | D.C.P    | 创建文件             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_ERR_H__
#define __INC_ERR_H__


/**  @brief 成功返回 */
#define ERR_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define ERR_FAIL FUNC_FAIL

/**  @brief 错误码结构体 */
typedef struct {
    u32_t       ErrCode;    /**< Operation not permitted 错误码*/
    string_t    ErrString;  /**< Operation not permitted 错误描述*/
}Err;
/******************************************************************
*   Name：私有接口宏 
*   Desc：内部调用的一些宏，主要是获得异常函数调用堆栈
******************************************************************/
/**  
* @brief 错误打印
*/
#define _ERR_LOG(result) printf("errno:%s, err-file:%s, err-line:%d, err-function:%s\n",err_strerr(errno), __FILE__, __LINE__, __FUNCTION__)

/**  
* @brief 返回错误宏 
* @param [in] Errno 错误码
* @param [in] Return 返回ERR_FAIL或者ERR_NULL
* @attention glibc库并不初始化errno，默认为EWOULDBLOCK or EAGAIN
*/
#define _RETURN_ERR( Errno, Return ) \
do{\
    errno = Errno;\
    return Return;\
}while(0)

/**  
* @brief 返回错误宏 
* @param [in] Errno 错误码
* @param [in] Return 返回ERR_FAIL或者ERR_NULL
* @param [in] Desc  直接打印的错误自定义描述
*/
#define _RETURN_ERRDESC( Errno, Return, Desc...) \
do{\
    printf("func:%s err:%d \n", __FUNCTION__ ,Errno);\
    printf(Desc);\
    errno = Errno;\
    return Return;\
}while(0)

/**  
* @brief 返回是否返回错误
* @param [in] result 返回条件
* @param [in] Return 返回值
*/
#define _CHECK_RETURN( result, Return )\
do{\
    if( result ) {_ERR_LOG(result);return Return;}\
}while (0)

/**  
* @brief 返回是否返回goto到某处
* @param [in] result 返回值
* @param [in] Goto  goto语句目的地
*/
#define _CHECK_GOTO( result, Goto )\
do{\
    if( result ) {_ERR_LOG(result);goto Goto;}\
}while (0)

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是则返回FUNC_FAIL/FUNC_NULL
* @param [in] result 返回值
*/
#define _CHECK_FF( result )  _CHECK_RETURN( (result) != FUNC_SUCC, FUNC_FAIL )
#define _CHECK_NF( result )  _CHECK_RETURN( (result) == FUNC_NULL, FUNC_FAIL )
#define _CHECK_FN( result )  _CHECK_RETURN( (result) != FUNC_SUCC, FUNC_NULL )
#define _CHECK_NN( result )  _CHECK_RETURN( (result) == FUNC_NULL, FUNC_NULL )

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是则goto 到某处
* @param [in] result 返回值
* @param [in] Goto  goto语句目的地
*/
#define _CHECK_FG( result, Goto )  _CHECK_GOTO( (result) != FUNC_SUCC, Goto )
#define _CHECK_NG( result, Goto )  _CHECK_GOTO( (result) == FUNC_NULL, Goto )

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是则goto 到某处
* @param [in] result 返回值
*/
#define _CHECK_FL( result)  if( (result) != FUNC_SUCC ) _ERR_LOG(result)
#define _CHECK_NL( result)  if( (result) == FUNC_NULL ) _ERR_LOG(result)

#endif/*__INC_ERR_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
