/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log.h
@brief:对于log模块的的结构体 宏定义
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
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_LOG_H__
#define __INC_LOG_H__

/**  @brief 成功返回 */
#define LOG_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define LOG_FAIL FUNC_FAIL
/**  @brief 失败返回 */
#define LOG_NULL FUNC_NULL
/**  @brief LOG 等级为无效的 */
#define LOG_LEVEL_IS_INVALID(level) ( (level > LOG_DEBUG) || ( level < LOG_EMERG) )
/**  @brief 管理结构体已初始化宏 */
#define LOG_IS_INITIALIZED 1
/**  @brief log消息等级为空 */
#define LOG_NONE LOG_EMERG-1

/**  @brief log 管理结构体 */
typedef struct {
  u8_t bitLevel;      /**< 过滤的log等级*/
  u8_t print;         /**< 打印   ENABLED使能 DISABLE关闭使能*/
  u32_t logNumber[7]; /**< 不同消息等级发送log计数*/
} mod_t;

/**  @brief log 管理结构体 */
typedef struct {
  u8_t     InitFlag;               /**< 初始化标志位   */
  char_t   logName[255];           /**< log名字    */
  u8_t     logOption;              /**< log操作    */
  u8_t     logType;                /**< log类型    */
  char_t   logPath[255];           /**< log写入路径*/
  mod_t    module[MOD_BUTT];       /**< module   log数量统计 */
} log;


/******************************************************************
*   Name：私有接口宏 
*   Desc：内部调用的一些宏，主要是获得异常函数调用堆栈
******************************************************************/
/**
* @brief 解析日志轮转参数
* @param [in] paraitem 参数数组
* @param [out] cmd 获得解析后命令
* @param [out] cmdLen 命令长度
*/
#define _PARSE_LOGROTATE(cmd, length, paraItem)\
do{\
  	if(NULL != paraItem.paraDetial)\
	  	if(paraItem.paraName != LOGROTATE_PROSTROTATE && paraItem.paraName != LOGROTATE_PREROTATE)\
      		length += snprintf(cmd+length, 500-length, "%s %s\n", logrotateTable[paraItem.paraName], paraItem.paraDetial);\
		else\
			length += snprintf(cmd+length, 500-length, "%s \n%s\nendscript\n", logrotateTable[paraItem.paraName], paraItem.paraDetial);\
	else\
	 	return LOG_FAIL;\
}while(0)

/**
* @brief 将level转成bit位
*/
#define _LEVEL_TO_BIT(level) 1<<(level)


#endif/*__INC_LOG_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */