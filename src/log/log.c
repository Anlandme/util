/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log.c
@brief:  logģ��
        ��Ҫ�ǽ���Ҫ��log��Ϣ���ݲ�ͬ�ȼ�����ͬ������д�ӡ�����Ե���log_config_rsyslogd
        ����rsyslogdѡ������Ҫ��ѡ�����log_config_logrotate��log����ת��ʽѹ���Ƚ�������
            ͬʱlog���������˺���log_set_filter��log_set_modules��log_set_filter��һ������
        �Ĺ��ˣ����������ȼ�����log_set_modules����˼�����������������ò�ͬд��log�ȼ������
        �Ƿ�д��logȡ����log_set_filter
            log_set_modules��һ�־�ϸ�Ĺ��ˣ�������ĳ��ģ������д��log�ȼ���ͬʱҲ֧�ִ�ӡ��
        �ն�ҳ�档�ڵ���ʱ����Ը�������������ԵĽ��д���log
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@attention�� ��Ҫϵͳ֧��rsyslog ��logrotate����Ӧ�ã�����һ��cenOS ubuntu��Ĭ�϶�֧������Ӧ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#include ".././util/util_inc.h"
#include "log.h"

/**  @brief log ����ṹ�� */
log Logger;

/**  @brief logrotate�ַ�����*/
string_t logrotateTable[] = {
    "compress",        /* ����ѹ����ָ���������ľ���־������Ĭ���õ���gzipѹ����*/
    "compressoptions", /* ��gzip -9��ģʽѹ��*/
    "uncompresscmd",   /* ��ѹ��־��Ĭ����gunzip*/
    "dateext",         /* �������־�ļ��ḽ����һ���̺��ߺ�YYYYMMDD��ʽ��ʱ���*/
    "delaycompress",   /* ����ǰ����־�ļ�ѹ���Ƴٵ���һ������*/
    "ifempty",         /* ��ʹ��־�ļ��ǿյ�Ҳ����*/
    "mail",            /* ���������ļ����͵�ָ��E-mail��ַ*/
    "copytruncate",    /* ���ڻ��ڴ��е���־�ļ����ѵ�ǰ��־���ݲ��ضϣ���ʼ����*/
    "mailfirst",       /* ���ʼ����������ļ�*/
    "maillast",        /* �������ʷ�ļ���Ĭ�ϣ�*/
    "nocompress",      /* �����logrotate.conf��������ѹ����������������ѹ���Ĳ���*/
    "nomail",          /* �������ʼ����κε�ַ*/
    "olddir",          /* �������־�ļ�����ָ����Ŀ¼������͵�ǰ��־�ļ���ͬһ���ļ�ϵͳ*/
    "postrotate",      /* postrotate/endscript ��������������������ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
    "prerotate",       /* prerotate/endscript	��������ǰ������,�����ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
    "rotate",          /* ������ౣ��֮ǰ�����ݼ��Σ������Ľ���ɾ�����ʼ����գ���Ϊ0�򲻱���*/
    "size",            /* ����־������ָ����С��ʱ��ʼ���棬�����ῼ��*/
    "start",           /* �����ļ�������������֡����磬ָ��0ʱ��ԭ��־�ļ�����ı����ļ���.0Ϊ��չ�������ָ��9����ֱ�Ӵ�.9��ʼ����0-8Ȼ���ټ����������rotateָ���Ĵ�����*/
    "daily",           /* ÿ������ѡ��*/
    "monthly",         /* һ��������һ��*/
    "weekly",          /* �����ǰ�����ڼ����ϴ���������ڼ��٣����߹���һ�������ڣ��ͻᷢ������ͨ������ÿ�ܵĵ�һ�����棬���logrotate����ÿ�����еģ����ڵ�һ���л���ʱ�������档*/
    "yearly",          /* �����ǰ��ݲ�ͬ���ϴ��������ݣ��������־����*/
};

/**  @brief LOG��Ϣ�ȼ��ַ�����*/
string_t levelTable[] = {
    "none",    /*�Cʲô������¼*/
    "emerg",   /*�C�ں˱�����������Ϣ*/
    "alert",   /*�C��Ҫ�����޸ĵ���Ϣ*/
    "crit",    /*�C���ؼ�����ֹ����ϵͳ���������������������������Ϣ*/
    "err",     /*�C���󼶱���ֹĳ�����ܻ���ģ�鲻��������������Ϣ*/
    "warning", /*�C���漶��*/
    "notice",  /*�C�������Ҫ�Ե���ͨ��������Ϣ*/
    "info",    /*�Cһ����Ϣ����־�����*/
    "debug",   /*�C�е�ʽ��Ϣ�ģ���־��Ϣ���*/
};

/**  @brief LOG��Ϣ�����ַ�����*/
string_t logTypeTable[] = {
    "kern",     /*  kern     �ں˲�������Ϣ*/
    "user",     /*  user     �û���������������Ϣ*/
    "mail",     /*  mail     �ʼ�*/
    "daemon",   /*  daemon   ����ϵͳ�ػ����̣���routed */
    "auth",     /*  auth     ��֤ϵͳ��login��su��getty��*/
    "syslog",   /*  syslog   rsyslog�����ڲ�����Ϣ,ʱ���ʶ*/
    "lpr",      /*  lpr      ϵͳ��ӡ������أ�lpr��lpd */
    "news",     /*  news     ������*/
    "uucp",     /*  uucp     unix to unix copy, unix����֮����ص�ͨѶ*/
    "cron",     /*  cron     cron�ػ�����*/
    "authpriv", /*  authpriv ssh,ftp�ȵ�¼��Ϣ����֤��Ϣ*/
    "ftp",      /*  ftp      �ļ�����Э�飺ftpd��tftpd */
    "local0",   /*  �Զ������־�豸 local0*/
    "local1",   /*  �Զ������־�豸 local1*/
    "local2",   /*  �Զ������־�豸 local2*/
    "local3",   /*  �Զ������־�豸 local3*/
    "local4",   /*  �Զ������־�豸 local4*/
    "local5",   /*  �Զ������־�豸 local5*/
    "local6",   /*  �Զ������־�豸 local6*/
    "local7",   /*  �Զ������־�豸 local7*/
};

/**
* @brief ��syslogd�������ļ��ڼ����Լ���������Ϣ����
* @param [in] logType    ��Ϣ���ͣ�syslogd֧�ֵ����ͣ�@see log_type
* @param [in] logName  log����
* @param [in] logPath  log�ļ�·��
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_NULL  ��ʾʧ��
*/
static log * 
_log_init(int_t logType, string_t logName, string_t logPath)
{
    if(Logger.InitFlag == LOG_IS_INITIALIZED) return &Logger;
    int_t index;

    if(!STRINGISNULL(logPath))  strncpy(Logger.logPath, logPath, strlen(logPath));
    strncpy(Logger.logName, logName, strlen(logName));
    Logger.logType   = logType;
    Logger.logOption = 0;
    Logger.InitFlag  = LOG_IS_INITIALIZED;
    for(index=0; index<MOD_BUTT; index++){
        Logger.module[index].bitLevel=0xff;
        Logger.module[index].print=0;
        memset(Logger.module[index].logNumber, 0, sizeof(Logger.module[index].logNumber));
    }
    return &Logger;
}

/**
* @brief ���log�����ַ���
* @param [in] logType  ��Ϣ���ͣ�syslogd֧�ֵ����ͣ�@see log_type
* @retval �����ַ���  ��ʾ�ɹ�
* @retval LOG_NULL  ��ʾʧ��
*/
static string_t 
_log_getLogType(int_t logType)
{
    switch(logType){
        case KERN     : return logTypeTable[0];  /*  kern     �ں˲�������Ϣ*/
        case USER     : return logTypeTable[1];  /*  user     �û���������������Ϣ*/
        case MAIL     : return logTypeTable[2];  /*  mail     �ʼ�*/
        case DAEMON   : return logTypeTable[3];  /*  daemon   ����ϵͳ�ػ����̣���routed  */
        case AUTH     : return logTypeTable[4];  /*  auth     ��֤ϵͳ��login��su��getty��*/
        case SYSLOG   : return logTypeTable[5];  /*  syslog   rsyslog�����ڲ�����Ϣ,ʱ���ʶ*/
        case LPR      : return logTypeTable[6];  /*  lpr      ϵͳ��ӡ������أ�lpr��lpd  */
        case NEWS     : return logTypeTable[7];  /*  news     ������*/
        case UUCP     : return logTypeTable[8];  /*  uucp     unix              to unix copy, unix����֮����ص�ͨѶ*/
        case CRON     : return logTypeTable[9];  /*  cron     cron�ػ�����*/
        case AUTHPRIV : return logTypeTable[10]; /*  authpriv ssh,ftp�ȵ�¼��Ϣ����֤��Ϣ*/
        case FTP      : return logTypeTable[11]; /*  ftp      �ļ�����Э�飺ftpd��tftpd */
        case LOCAL0   : return logTypeTable[12]; /*  �Զ������־�豸 local0*/
        case LOCAL1   : return logTypeTable[13]; /*  �Զ������־�豸 local1*/
        case LOCAL2   : return logTypeTable[14]; /*  �Զ������־�豸 local2*/
        case LOCAL3   : return logTypeTable[15]; /*  �Զ������־�豸 local3*/
        case LOCAL4   : return logTypeTable[16]; /*  �Զ������־�豸 local4*/
        case LOCAL5   : return logTypeTable[17]; /*  �Զ������־�豸 local5*/
        case LOCAL6   : return logTypeTable[18]; /*  �Զ������־�豸 local6*/
        case LOCAL7   : return logTypeTable[19]; /*  �Զ������־�豸 local7*/
        default: return LOG_NULL;
    }
}

/**
* @brief ���log�ȼ��ַ���
* @param [in] logLevel ��Ϣ�ȼ���syslogd֧�ֵĵȼ���@see log_level
* @retval �����ַ���  ��ʾ�ɹ�
* @retval LOG_NULL  ��ʾʧ��
*/
static string_t 
_log_getLogLevel(int_t logLevel)
{
    switch(logLevel){
        case NONE    : return levelTable[0]; /** �ر����д�ӡ                     */
        case EMERG   : return levelTable[1]; /** �ں˱�����������Ϣ                  */
        case ALERT   : return levelTable[2]; /** ��Ҫ�����޸ĵ���Ϣ                  */
        case CRIT    : return levelTable[3]; /** ���ؼ�����ֹ����ϵͳ���������������������������Ϣ */
        case ERR     : return levelTable[4]; /** ���󼶱���ֹĳ�����ܻ���ģ�鲻��������������Ϣ   */
        case WARNING : return levelTable[5]; /** ���漶�����Ϣ                    */
        case NOTICE  : return levelTable[6]; /** �������Ҫ�Ե���ͨ��������Ϣ             */
        case INFO    : return levelTable[7]; /** һ����Ϣ����־�����                */
        case DEBUG   : return levelTable[8]; /** �е�ʽ��Ϣ�ģ���־��Ϣ��� */
        default: return LOG_NULL;  
    }
}


/**
* @brief ��syslogd�������ļ��ڼ����Լ���������Ϣ����
* @note  logType.msgLevel  logPath --->   ����mail.info      /flash/var/maillog_info
* @param [in] logType    ��Ϣ���ͣ�syslogd֧�ֵ����ͣ�@see log_type
* @param [in] logLevel   ��Ϣ�ȼ���syslogd֧�ֵĵȼ���@see log_level
* @param [in] logName    log����
* @param [in] logPath    log�ļ�·��
* @param [in] logRemote  logԶ��·��
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_NULL  ��ʾʧ��
*/
log *
log_config_rsyslogd(int_t logType, int_t logLevel,  string_t logName, string_t logPath, string_t logRemote)
{
	if(STRINGISNULL(logName))  RETURN_ERR(ERR_PARA, LOG_NULL);
	if(LOG_LEVEL_IS_INVALID(logLevel))  RETURN_ERR(ERR_PARA, LOG_NULL);
    if(STRINGISNULL(logPath) && STRINGISNULL(logRemote))  RETURN_ERR(ERR_PARA, LOG_NULL);
    FILE   *file       = NULL;//�ļ�ָ��
    char_t cmd[100]    = {'\0'};//���������ļ�������
    char_t rsyslogd[100] = {'\0'};//�����ļ�

    if(0 != access("/etc/rsyslog.d", F_OK) ) //�ж�Ŀ¼�Ƿ���� �����ڴ���Ŀ¼
        mkdir("/etc/rsyslog.d", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

    snprintf(rsyslogd, sizeof(rsyslogd), "/etc/rsyslog.d/%s.conf", logName);
    if(NULL == (file = fopen(rsyslogd, "w+")))//�����µ������ļ�
		RETURN_ERRDESC(THE_ERRNO, LOG_NULL, "can not open the rsyslog.d config file\n" );

    if(!STRINGISNULL(logPath)){
        snprintf(cmd, sizeof(cmd), "%s.%s %s\n", _log_getLogType(logType), _log_getLogLevel(logLevel), logPath);
        fwrite(cmd, strlen(cmd), 1, file);
    }
    if(!STRINGISNULL(logRemote)){
        snprintf(cmd, sizeof(cmd), "%s.%s @%s\n", _log_getLogType(logType), _log_getLogLevel(logLevel), logRemote);
        fwrite(cmd, strlen(cmd), 1, file);
    }
    fclose(file);
	system("service rsyslog restart");
    
    return _log_init(logType, logName, logPath);
} 

/**
* @brief ������־��ת����
* @param [in] paraitem ��������
* @param [out] cmd ��ý���������
* @param [out] cmdLen �����
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
static int_t 
_log_parse_logratate(logrotate_t paraItem, char_t *cmd, u16_t *cmdLen)
{
    u16_t lenght =*cmdLen;
    switch(paraItem.paraName){
        /* ����ѹ����ָ���������ľ���־������Ĭ���õ���gzipѹ����*/
        case LOGROTATE_COMPRESS:       lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ��gzip -9��ģʽѹ��*/
        case LOGROTATE_COMPRESSOPTION: lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ��ѹ��־��Ĭ����gunzip*/
        case LOGROTATE_UNCOMPRESS:     lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �������־�ļ��ḽ����һ���̺��ߺ�YYYYMMDD��ʽ��ʱ���*/
        case LOGROTATE_DATEEXT:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ����ǰ����־�ļ�ѹ���Ƴٵ���һ������*/
        case LOGROTATE_DELAYCOMPRESS:  lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ��ʹ��־�ļ��ǿյ�Ҳ����*/
        case LOGROTATE_IFEMPTY:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ���������ļ����͵�ָ��E-mail��ַ*/
        case LOGROTATE_MAIL:           _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* ���ڻ��ڴ��е���־�ļ����ѵ�ǰ��־���ݲ��ضϣ���ʼ����*/
        case LOGROTATE_COPYTRUNCATE:   lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* ���ʼ����������ļ�*/
        case LOGROTATE_MAILFIRST:      lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �������ʷ�ļ���Ĭ�ϣ�*/
        case LOGROTATE_MAILLAST:       lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �����logrotate.conf��������ѹ����������������ѹ���Ĳ���*/
        case LOGROTATE_NOCOMPRESS:     lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �������ʼ����κε�ַ*/
        case LOGROTATE_NOMAIL:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �������־�ļ�����ָ����Ŀ¼������͵�ǰ��־�ļ���ͬһ���ļ�ϵͳ*/
        case LOGROTATE_OLDDIR:         _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* prostrotate/endscript ��������������������ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
        case LOGROTATE_PROSTROTATE:    _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* prerotate/endscript	 ��������ǰ������:�����ؼ��ֱ��뵥�����У�ʹ�õĲ�����2��֮���൱�ڷ��顰{}����ע���ʹ���ⲿָ��ʱҪ�þ���·��*/
        case LOGROTATE_PREROTATE:      _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* ������ౣ��֮ǰ�����ݼ��Σ������Ľ���ɾ�����ʼ����գ���Ϊ0�򲻱���*/
        case LOGROTATE_ROTATE:         _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* ����־������ָ����С��ʱ��ʼ���棬�����ῼ��*/
        case LOGROTATE_SIZE:           _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* �����ļ�������������֡����磬ָ��0ʱ��ԭ��־�ļ�����ı����ļ���.0Ϊ��չ�������ָ��9����ֱ�Ӵ�.9��ʼ����0-8Ȼ���ټ����������rotateָ���Ĵ�����*/
        case LOGROTATE_START:          _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* ÿ������ѡ��*/
        case LOGROTATE_DAILY:          lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* һ��������һ��*/
        case LOGROTATE_MONTHLY:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �����ǰ�����ڼ����ϴ���������ڼ��٣����߹���һ�������ڣ��ͻᷢ������ͨ������ÿ�ܵĵ�һ�����棬���logrotate����ÿ�����еģ����ڵ�һ���л���ʱ�������档*/
        case LOGROTATE_WEEKLY:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* �����ǰ��ݲ�ͬ���ϴ��������ݣ��������־����*/
        case LOGROTATE_YEARLY:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        default: return LOG_FAIL; 
    } 
        *cmdLen = lenght;      
        return LOG_SUCC;


}


/**
* @brief ������־��ת����
* @param [in] logger     log������
* @param [in] paraitem ��������
* @param [in] paraNum  ������������
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_config_logrotate(log * logger, logrotate_t * paraItem, int_t itemNum )
{
    if(PARAMISNULL(logger))  RETURN_ERR(ERR_PARA, LOG_FAIL);
    if(PARAMISNULL(paraItem)) RETURN_ERR(ERR_PARA, LOG_FAIL);

    FILE   *file          = NULL;//�ļ�ָ��
    char_t cmd[500]       = {'\0'};//���������ļ�������
    char_t logrotate[100] = {'\0'};//�����ļ�
    u16_t  cmdLen         = 0;
    u16_t  index          = 0;

    if(0 != access("/etc/logrotate.d", F_OK) ) //�ж�Ŀ¼�Ƿ���� �����ڴ���Ŀ¼
        mkdir("/etc/logrotate.d", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

    snprintf(logrotate, sizeof(logrotate), "/etc/logrotate.d/%s", logger->logName);
    if(NULL == (file = fopen(logrotate, "w+")))//�����µ������ļ�
		RETURN_ERRDESC( THE_ERRNO, LOG_FAIL, "can not open the logrotate.d config file\n" );

    cmdLen += snprintf(cmd+cmdLen, 500-cmdLen, "%s {\n", logger->logPath);
    for(index =0; index < itemNum; index++){
        if(LOG_FAIL == _log_parse_logratate(paraItem[index], cmd, &cmdLen)){
            RETURN_ERRDESC( ERR_USER, LOG_FAIL, "logrotate_t member's paraName was worng \n" );
            fclose(file);
        }
    }
    cmdLen += snprintf(cmd+cmdLen, 500-cmdLen, "}\n");
    fwrite(cmd, strlen(cmd), 1, file);
    fclose(file);
    snprintf(cmd, 500, "logrotate %s\n", logrotate);
    system(cmd);
	return LOG_SUCC;
}


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
log_open(log * logger, int_t option)
{
	if(PARAMISNULL(logger))  RETURN_ERR(ERR_PARA, LOG_FAIL);
    openlog(logger->logName, option ,logger->logType);
    logger->logOption = option;
    return LOG_SUCC;
}




/**
* @brief �ر�log
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t 
log_close(void)
{
    closelog();
	return LOG_SUCC;
}

/**
* @brief log��Ϣ����
* @param [in] bitLevel ���˳���Ҫ�� �ȼ� BIT�꽫levelת��bit level Ȼ��ֵ
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_set_filter(u8_t bitLevel)
{
    setlogmask(bitLevel);//��Ҫ����Ϣ������
	return LOG_SUCC;
}


/**
* @brief ����module��ӡlog����
* @param [in] modules   ���õ�ģ������
* @param [in] sum       ��������
* @retval LOG_SUCC  ��ʾ�ɹ�
* @retval LOG_FAIL  ��ʾʧ��
*/
int_t
log_set_modules(logMod_t *modules, int_t sum )
{
	if(STRINGISNULL(modules))  RETURN_ERR(ERR_PARA, LOG_FAIL);
    if(sum > MOD_BUTT)  RETURN_ERR(ERR_PARA, LOG_FAIL);
    int index;

    for( index =0; index<sum; index++ ){
        if(modules[index].module > MOD_BUTT)  RETURN_ERR(ERR_PARA, LOG_FAIL);
        Logger.module[modules[index].module].print  = modules[index].print;
        Logger.module[modules[index].module].bitLevel  = modules[index].bitLevel;
    }
	return LOG_SUCC;
}


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
int_t _log(string_t file, string_t func, int_t line, int_t module, int_t level, char* format, ... )
{
    if(module > MOD_BUTT)  RETURN_ERR(ERR_PARA, LOG_FAIL);
	if(LOG_LEVEL_IS_INVALID(level))  RETURN_ERR(ERR_PARA, LOG_FAIL);
	if(LOG_IS_INITIALIZED != Logger.InitFlag) RETURN_ERRDESC( ERR_USER, LOG_FAIL, "log do not init \n");
    char_t logMsg[MSG_SIZE];
    va_list vArgList; //����һ��va_list�͵ı���,���������ָ�������ָ��
    if(_LEVEL_TO_BIT(level) & Logger.module[module].bitLevel){
        va_start (vArgList, format);
        vsnprintf(logMsg, MSG_SIZE, format, vArgList);
        va_end(vArgList); 
        syslog(level, "[ --file:%s --function:%s --line:%d] %s\n",file, func, line, logMsg);
        if(Logger.module[module].print) printf("[--file:%s --function:%s --line:%d] %s\n",file, func, line, logMsg);
        return LOG_SUCC;
    }
        
	return LOG_SUCC;
}