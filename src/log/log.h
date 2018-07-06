/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log.h
@brief:����logģ��ĵĽṹ�� �궨��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/29
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/29  | 0.0.0.1   | D.C.P    | �����ļ�             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_LOG_H__
#define __INC_LOG_H__

/**  @brief �ɹ����� */
#define LOG_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define LOG_FAIL FUNC_FAIL
/**  @brief ʧ�ܷ��� */
#define LOG_NULL FUNC_NULL
/**  @brief LOG �ȼ�Ϊ��Ч�� */
#define LOG_LEVEL_IS_INVALID(level) ( (level > LOG_DEBUG) || ( level < LOG_EMERG) )
/**  @brief ����ṹ���ѳ�ʼ���� */
#define LOG_IS_INITIALIZED 1
/**  @brief log��Ϣ�ȼ�Ϊ�� */
#define LOG_NONE LOG_EMERG-1

/**  @brief log ����ṹ�� */
typedef struct {
  u8_t bitLevel;      /**< ���˵�log�ȼ�*/
  u8_t print;         /**< ��ӡ   ENABLEDʹ�� DISABLE�ر�ʹ��*/
  u32_t logNumber[7]; /**< ��ͬ��Ϣ�ȼ�����log����*/
} mod_t;

/**  @brief log ����ṹ�� */
typedef struct {
  u8_t     InitFlag;               /**< ��ʼ����־λ   */
  char_t   logName[255];           /**< log����    */
  u8_t     logOption;              /**< log����    */
  u8_t     logType;                /**< log����    */
  char_t   logPath[255];           /**< logд��·��*/
  mod_t    module[MOD_BUTT];       /**< module   log����ͳ�� */
} log;


/******************************************************************
*   Name��˽�нӿں� 
*   Desc���ڲ����õ�һЩ�꣬��Ҫ�ǻ���쳣�������ö�ջ
******************************************************************/
/**
* @brief ������־��ת����
* @param [in] paraitem ��������
* @param [out] cmd ��ý���������
* @param [out] cmdLen �����
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
* @brief ��levelת��bitλ
*/
#define _LEVEL_TO_BIT(level) 1<<(level)


#endif/*__INC_LOG_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */