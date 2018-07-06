/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log_pub.h
@brief:  log����ͷ�ļ�
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_LOG_PUB_H__
#define __INC_LOG_PUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "log.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief log �����ڴ�����Ϣ�����ֵ */
#define MSG_SIZE 1024

typedef log * log_t;

/**  @brief log ����ṹ�� */
typedef struct {
    u32_t    paraName;   /**< ������*/
    string_t paraDetial; /**< ��������*/
} logrotate_t;


/**  @brief log ����ṹ�� */
typedef struct {
  u32_t module;   /**< ģ����*/
  u8_t bitLevel;  /**< ���˵�log�ȼ�*/
  u8_t print;     /**< ��ӡ   ENABLEDʹ�� DISABLE�ر�ʹ��*/
} logMod_t;

/**  @brief LOG��Ϣ�ȼ� */
enum log_level{
  NONE    = LOG_NONE,        /** �ر����д�ӡ */
  EMERG   = LOG_EMERG,       /** �ں˱�����������Ϣ   */
  ALERT   = LOG_ALERT,       /** ��Ҫ�����޸ĵ���Ϣ   */
  CRIT    = LOG_CRIT,        /** ���ؼ�����ֹ����ϵͳ���������������������������Ϣ */
  ERR     = LOG_ERR,         /** ���󼶱���ֹĳ�����ܻ���ģ�鲻��������������Ϣ */
  WARNING = LOG_WARNING,     /** ���漶�����Ϣ     */
  NOTICE  = LOG_NOTICE,      /** �������Ҫ�Ե���ͨ��������Ϣ */
  INFO    = LOG_INFO,        /** һ����Ϣ����־����� */
  DEBUG   = LOG_DEBUG,       /** �е�ʽ��Ϣ�ģ���־��Ϣ��� */
};


/**  @brief LOG������*/
enum log_option{
CONS_OPTION   = LOG_CONS,   /**  �������Ϣ���͸�syslogd�ػ�����ʱ��������ֱ�ӽ������Ϣ������ն�*/
NDELAY_OPTION = LOG_NDELAY, /**  ��������ϵͳ��־�����ӣ�ͨ������£�ֻ���ڲ�����һ����־��Ϣ������²Ż������־ϵͳ�����ӣ�*/
NOWAIT_OPTION = LOG_NOWAIT, /**  �ڼ�¼��־��Ϣʱ�����ȴ����ܵ��ӽ��̵Ĵ���*/
ODELAY_OPTION = LOG_ODELAY, /**  ������LOG_NDELAY��������ϵͳ��־������ֻ����syslog��������ʱ�Żᴴ��*/
PERROR_OPTION = LOG_PERROR, /**  �ڽ���Ϣд����־��ͬʱ������Ϣ���͵���׼���������POSIX.1-2001��֧�ָò�����*/
PID_OPTION    = LOG_PID,    /**  ÿ����־��Ϣ�ж��������̺�*/
};


/**  @brief LOG����*/
enum log_type{
  KERN     = LOG_KERN,          /**  kern     �ں˲�������Ϣ*/
  USER     = LOG_USER,          /**  user     �û���������������Ϣ*/
  MAIL     = LOG_MAIL,          /**  mail     �ʼ�*/
  DAEMON   = LOG_DAEMON,        /**  daemon   ����ϵͳ�ػ����̣���routed */
  AUTH     = LOG_AUTH,          /**  auth     ��֤ϵͳ��login��su��getty��*/
  SYSLOG   = LOG_SYSLOG,        /**  syslog   rsyslog�����ڲ�����Ϣ,ʱ���ʶ*/
  LPR      = LOG_LPR,           /**  lpr      ϵͳ��ӡ������أ�lpr��lpd */
  NEWS     = LOG_NEWS,          /**  news     ������*/
  UUCP     = LOG_UUCP,          /**  uucp     unix to unix copy, unix����֮����ص�ͨѶ*/
  CRON     = LOG_CRON,          /**  cron    cron�ػ�����*/
  AUTHPRIV = LOG_AUTHPRIV,      /**  authpriv ssh,ftp�ȵ�¼��Ϣ����֤��Ϣ*/
  FTP      = LOG_FTP,           /**  ftp      �ļ�����Э�飺ftpd��tftpd */
  LOCAL0   = LOG_LOCAL0,        /**  �Զ������־�豸 local0*/
  LOCAL1   = LOG_LOCAL1,        /**  �Զ������־�豸 local1*/
  LOCAL2   = LOG_LOCAL2,        /**  �Զ������־�豸 local2*/
  LOCAL3   = LOG_LOCAL3,        /**  �Զ������־�豸 local3*/
  LOCAL4   = LOG_LOCAL4,        /**  �Զ������־�豸 local4*/
  LOCAL5   = LOG_LOCAL5,        /**  �Զ������־�豸 local5*/
  LOCAL6   = LOG_LOCAL6,        /**  �Զ������־�豸 local6*/
  LOCAL7   = LOG_LOCAL7,        /**  �Զ������־�豸 local7*/
};

/**  @brief logrotate����*/
enum logrotate_para{
  LOGROTATE_COMPRESS = 0,   /** ����ѹ����ָ���������ľ���־������Ĭ���õ���gzipѹ����*/
  LOGROTATE_COMPRESSOPTION, /** ��gzip -9��ģʽѹ��*/
  LOGROTATE_UNCOMPRESS,     /** ��ѹ��־��Ĭ����gunzip*/
  LOGROTATE_DATEEXT,        /** �������־�ļ��ḽ����һ���̺��ߺ�YYYYMMDD��ʽ��ʱ���*/
  LOGROTATE_DELAYCOMPRESS,  /** ����ǰ����־�ļ�ѹ���Ƴٵ���һ������*/
  LOGROTATE_IFEMPTY,        /** ��ʹ��־�ļ��ǿյ�Ҳ����*/
  LOGROTATE_MAIL,           /** ���������ļ����͵�ָ��E-mail��ַ*/
  LOGROTATE_COPYTRUNCATE,   /** ���ڻ��ڴ��е���־�ļ����ѵ�ǰ��־���ݲ��ضϣ���ʼ����*/
  LOGROTATE_MAILFIRST,      /** ���ʼ����������ļ�*/
  LOGROTATE_MAILLAST,       /** �������ʷ�ļ���Ĭ�ϣ�*/
  LOGROTATE_NOCOMPRESS,     /** �����logrotate.conf��������ѹ����������������ѹ���Ĳ���*/
  LOGROTATE_NOMAIL,         /** �������ʼ����κε�ַ*/
  LOGROTATE_OLDDIR,         /** �������־�ļ�����ָ����Ŀ¼������͵�ǰ��־�ļ���ͬһ���ļ�ϵͳ*/
  LOGROTATE_PROSTROTATE,    /** postrotate/endscript ��������������������ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
  LOGROTATE_PREROTATE,      /** prerotate/endscript	��������ǰ������,�����ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
  LOGROTATE_ROTATE,         /** ������ౣ��֮ǰ�����ݼ��Σ������Ľ���ɾ�����ʼ����գ���Ϊ0�򲻱���*/
  LOGROTATE_SIZE,           /** ����־������ָ����С��ʱ��ʼ���棬�����ῼ��*/
  LOGROTATE_START,          /** �����ļ�������������֡����磬ָ��0ʱ��ԭ��־�ļ�����ı����ļ���.0Ϊ��չ�������ָ��9����ֱ�Ӵ�.9��ʼ����0-8Ȼ���ټ����������rotateָ���Ĵ�����*/
  LOGROTATE_DAILY,          /** ÿ������ѡ��*/
  LOGROTATE_MONTHLY,        /** һ��������һ��*/
  LOGROTATE_WEEKLY,         /** �����ǰ�����ڼ����ϴ���������ڼ��٣����߹���һ�������ڣ��ͻᷢ������ͨ������ÿ�ܵĵ�һ�����棬���logrotate����ÿ�����еģ����ڵ�һ���л���ʱ�������档*/
  LOGROTATE_YEARLY,         /** �����ǰ��ݲ�ͬ���ϴ��������ݣ��������־����*/
  LOGROTATE_BUTT
};
/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��syslogd�������ļ��ڼ����Լ���������Ϣ����
* @note  logType.msgLevel  logPath --->   ����mail.info      /flash/var/maillog_info
* @param [in] logType  ��Ϣ���ͣ�syslogd֧�ֵ����ͣ�
* @param [in] logLevel ��Ϣ�ȼ���syslogd֧�ֵĵȼ���
* @param [in] logName  log����
* @param [in] logPath  log�ļ�·��
* @param [in] logPath  logԶ��·��
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
log_t
log_config_rsyslogd(int_t logType, int_t logLevel,  string_t logName, string_t logPath, string_t logRemote);


/**
* @brief ������־��ת����
* @param [in] logger     log������
* @param [in] paraitem ��������
* @param [in] paraNum  ������������
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_config_logrotate(log_t logger, logrotate_t *paraItem, int_t itemNum );


/**
* @brief ��log
* @param [in] logger     log������
* @param [in] option     logѡ��
	- CONS_OPTION   �������Ϣ���͸�syslogd�ػ�����ʱ��������ֱ�ӽ������Ϣ������ն�
	- NDELAY_OPTION ��������ϵͳ��־�����ӣ�ͨ������£�ֻ���ڲ�����һ����־��Ϣ������²Ż������־ϵͳ�����ӣ�
	- NOWAIT_OPTION �ڼ�¼��־��Ϣʱ�����ȴ����ܵ��ӽ��̵Ĵ���
	- ODELAY_OPTION ������NDELAY_OPTION��������ϵͳ��־������ֻ����syslog��������ʱ�Żᴴ��
	- PERROR_OPTION �ڽ���Ϣд����־��ͬʱ������Ϣ���͵���׼���������POSIX.1-2001��֧�ָò�����
	- PID_OPTION    ÿ����־��Ϣ�ж��������̺�
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_open(log_t logger, int_t option);

/**
* @brief �ر�log
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t 
log_close(void);

/**
* @brief log��Ϣ����
* @param [in] bitLevel     ���˳���Ҫ�� �ȼ� 
 - BIT_EMERG
 - BIT_ALERT
 - BIT_CRIT
 - BIT_ERR
 - BIT_WARNING
 - BIT_NOTICE
 - BIT_INFO
 - BIT_DEBUG
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_set_filter(u8_t bitLevel);

/**
* @brief ����module��ӡlog����
* @param [in] modules   ���õ�ģ������
* @param [in] sum       ��������
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_set_modules(logMod_t *modules, int_t sum );

/**
* @brief ����log
* @param [in] file     �ļ���
* @param [in] func     ������
* @param [in] line     �к�
* @param [in] module   ʹ�õ�ģ��
* @param [in] level    log�ȼ�
 - EMERG   �ں˱�����������Ϣ
 - ALERT   ��Ҫ�����޸ĵ���Ϣ 
 - CRIT    ���ؼ�����ֹ����ϵͳ���������������������������Ϣ 
 - ERR     ���󼶱���ֹĳ�����ܻ���ģ�鲻��������������Ϣ 
 - WARNING ���漶�����Ϣ 
 - NOTICE  �������Ҫ�Ե���ͨ��������Ϣ 
 - INFO    һ����Ϣ����־����� 
 - DEBUG   �е�ʽ��Ϣ�ģ���־��Ϣ��� 
* @param [in] format ��ʽ����Ϣ
* @param [in] ...    log��Ϣ����
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
#define log(module, level, format, ...) _log( __FILE__,  __FUNCTION__, __LINE__, module, level, format,  ##__VA_ARGS__ )
/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/
/**
* @brief ��levelת��bitλ
*/
#define BIT(level) _LEVEL_TO_BIT(level)



#endif/*__INC_LOG_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */