/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log.c
@brief:  log模块
        主要是将需要的log信息根据不同等级，不同需求进行打印，可以调用log_config_rsyslogd
        配置rsyslogd选择所需要的选项，调用log_config_logrotate对log的轮转方式压缩等进行配置
            同时log有两个过滤函数log_set_filter和log_set_modules，log_set_filter是一个粗略
        的过滤，但是其优先级高于log_set_modules，意思是在两个函数内设置不同写入log等级，最后
        是否写入log取决于log_set_filter
            log_set_modules是一种精细的过滤，单独对某个模块设置写入log等级，同时也支持打印到
        终端页面。在调试时候可以更加灵活更有针对性的进行处理log
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@attention： 需要系统支持rsyslog 和logrotate两个应用，不过一般cenOS ubuntu等默认都支持两个应用
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "log.h"

/**  @brief log 管理结构体 */
log Logger;

/**  @brief logrotate字符串表*/
string_t logrotateTable[] = {
    "compress",        /* 启用压缩，指的是轮替后的旧日志，这里默认用的是gzip压缩的*/
    "compressoptions", /* 以gzip -9的模式压缩*/
    "uncompresscmd",   /* 解压日志，默认是gunzip*/
    "dateext",         /* 轮替的日志文件会附加上一个短横线和YYYYMMDD格式的时间戳*/
    "delaycompress",   /* 将以前的日志文件压缩推迟到下一次轮替*/
    "ifempty",         /* 即使日志文件是空的也轮替*/
    "mail",            /* 将轮替后的文件发送到指定E-mail地址*/
    "copytruncate",    /* 用于还在打开中的日志文件，把当前日志备份并截断，开始轮替*/
    "mailfirst",       /* 向邮件发送轮替文件*/
    "maillast",        /* 轮替后历史文件（默认）*/
    "nocompress",      /* 如果在logrotate.conf中启用了压缩，这里是做不用压缩的参数*/
    "nomail",          /* 不发送邮件到任何地址*/
    "olddir",          /* 轮替后日志文件放入指定的目录，必须和当前日志文件在同一个文件系统*/
    "postrotate",      /* postrotate/endscript 在做完轮替后的命令，两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
    "prerotate",       /* prerotate/endscript	在做轮替前的命令,两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
    "rotate",          /* 轮替最多保留之前的数据几次，超出的将被删除或邮件接收，设为0则不保存*/
    "size",            /* 当日志增长到指定大小的时候开始轮替，它不会考虑*/
    "start",           /* 轮替文件名基于这个数字。例如，指定0时，原日志文件轮替的备份文件以.0为扩展名，如果指定9，就直接从.9开始跳过0-8然后再继续向后轮替rotate指定的次数。*/
    "daily",           /* 每天轮替选项*/
    "monthly",         /* 一个月轮替一次*/
    "weekly",          /* 如果当前的星期几比上次轮替的星期几少，或者过了一个多星期，就会发生轮替通常是在每周的第一天轮替，如果logrotate不是每天运行的，会在第一次有机会时进行轮替。*/
    "yearly",          /* 如果当前年份不同于上次轮替的年份，则进行日志轮替*/
};

/**  @brief LOG消息等级字符串表*/
string_t levelTable[] = {
    "none",    /*–什么都不记录*/
    "emerg",   /*–内核崩溃等严重信息*/
    "alert",   /*–需要立刻修改的信息*/
    "crit",    /*–严重级别，阻止整个系统或者整个软件不能正常工作的信息*/
    "err",     /*–错误级别，阻止某个功能或者模块不能正常工作的信息*/
    "warning", /*–警告级别*/
    "notice",  /*–最具有重要性的普通条件的信息*/
    "info",    /*–一般信息的日志，最常用*/
    "debug",   /*–有调式信息的，日志信息最多*/
};

/**  @brief LOG消息类型字符串表*/
string_t logTypeTable[] = {
    "kern",     /*  kern     内核产生的消息*/
    "user",     /*  user     用户程序产生的相关信息*/
    "mail",     /*  mail     邮件*/
    "daemon",   /*  daemon   其他系统守护进程，如routed */
    "auth",     /*  auth     认证系统：login、su、getty等*/
    "syslog",   /*  syslog   rsyslog服务内部的信息,时间标识*/
    "lpr",      /*  lpr      系统打印机缓冲池：lpr、lpd */
    "news",     /*  news     新闻组*/
    "uucp",     /*  uucp     unix to unix copy, unix主机之间相关的通讯*/
    "cron",     /*  cron     cron守护进程*/
    "authpriv", /*  authpriv ssh,ftp等登录信息的验证信息*/
    "ftp",      /*  ftp      文件传输协议：ftpd、tftpd */
    "local0",   /*  自定义的日志设备 local0*/
    "local1",   /*  自定义的日志设备 local1*/
    "local2",   /*  自定义的日志设备 local2*/
    "local3",   /*  自定义的日志设备 local3*/
    "local4",   /*  自定义的日志设备 local4*/
    "local5",   /*  自定义的日志设备 local5*/
    "local6",   /*  自定义的日志设备 local6*/
    "local7",   /*  自定义的日志设备 local7*/
};

/**
* @brief 往syslogd的配置文件内加入自己的配置消息类型
* @param [in] logType    消息类型（syslogd支持的类型）@see log_type
* @param [in] logName  log名字
* @param [in] logPath  log文件路径
* @retval LOG_SUCC  表示成功
* @retval LOG_NULL  表示失败
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
* @brief 获得log类型字符串
* @param [in] logType  消息类型（syslogd支持的类型）@see log_type
* @retval 类型字符串  表示成功
* @retval LOG_NULL  表示失败
*/
static string_t 
_log_getLogType(int_t logType)
{
    switch(logType){
        case KERN     : return logTypeTable[0];  /*  kern     内核产生的消息*/
        case USER     : return logTypeTable[1];  /*  user     用户程序产生的相关信息*/
        case MAIL     : return logTypeTable[2];  /*  mail     邮件*/
        case DAEMON   : return logTypeTable[3];  /*  daemon   其他系统守护进程，如routed  */
        case AUTH     : return logTypeTable[4];  /*  auth     认证系统：login、su、getty等*/
        case SYSLOG   : return logTypeTable[5];  /*  syslog   rsyslog服务内部的信息,时间标识*/
        case LPR      : return logTypeTable[6];  /*  lpr      系统打印机缓冲池：lpr、lpd  */
        case NEWS     : return logTypeTable[7];  /*  news     新闻组*/
        case UUCP     : return logTypeTable[8];  /*  uucp     unix              to unix copy, unix主机之间相关的通讯*/
        case CRON     : return logTypeTable[9];  /*  cron     cron守护进程*/
        case AUTHPRIV : return logTypeTable[10]; /*  authpriv ssh,ftp等登录信息的验证信息*/
        case FTP      : return logTypeTable[11]; /*  ftp      文件传输协议：ftpd、tftpd */
        case LOCAL0   : return logTypeTable[12]; /*  自定义的日志设备 local0*/
        case LOCAL1   : return logTypeTable[13]; /*  自定义的日志设备 local1*/
        case LOCAL2   : return logTypeTable[14]; /*  自定义的日志设备 local2*/
        case LOCAL3   : return logTypeTable[15]; /*  自定义的日志设备 local3*/
        case LOCAL4   : return logTypeTable[16]; /*  自定义的日志设备 local4*/
        case LOCAL5   : return logTypeTable[17]; /*  自定义的日志设备 local5*/
        case LOCAL6   : return logTypeTable[18]; /*  自定义的日志设备 local6*/
        case LOCAL7   : return logTypeTable[19]; /*  自定义的日志设备 local7*/
        default: return LOG_NULL;
    }
}

/**
* @brief 获得log等级字符串
* @param [in] logLevel 消息等级（syslogd支持的等级）@see log_level
* @retval 类型字符串  表示成功
* @retval LOG_NULL  表示失败
*/
static string_t 
_log_getLogLevel(int_t logLevel)
{
    switch(logLevel){
        case NONE    : return levelTable[0]; /** 关闭所有打印                     */
        case EMERG   : return levelTable[1]; /** 内核崩溃等严重信息                  */
        case ALERT   : return levelTable[2]; /** 需要立刻修改的信息                  */
        case CRIT    : return levelTable[3]; /** 严重级别，阻止整个系统或者整个软件不能正常工作的信息 */
        case ERR     : return levelTable[4]; /** 错误级别，阻止某个功能或者模块不能正常工作的信息   */
        case WARNING : return levelTable[5]; /** 警告级别的消息                    */
        case NOTICE  : return levelTable[6]; /** 最具有重要性的普通条件的信息             */
        case INFO    : return levelTable[7]; /** 一般信息的日志，最常用                */
        case DEBUG   : return levelTable[8]; /** 有调式信息的，日志信息最多 */
        default: return LOG_NULL;  
    }
}


/**
* @brief 往syslogd的配置文件内加入自己的配置消息类型
* @note  logType.msgLevel  logPath --->   例：mail.info      /flash/var/maillog_info
* @param [in] logType    消息类型（syslogd支持的类型）@see log_type
* @param [in] logLevel   消息等级（syslogd支持的等级）@see log_level
* @param [in] logName    log名字
* @param [in] logPath    log文件路径
* @param [in] logRemote  log远端路径
* @retval LOG_SUCC  表示成功
* @retval LOG_NULL  表示失败
*/
log *
log_config_rsyslogd(int_t logType, int_t logLevel,  string_t logName, string_t logPath, string_t logRemote)
{
	if(STRINGISNULL(logName))  RETURN_ERR(ERR_PARA, LOG_NULL);
	if(LOG_LEVEL_IS_INVALID(logLevel))  RETURN_ERR(ERR_PARA, LOG_NULL);
    if(STRINGISNULL(logPath) && STRINGISNULL(logRemote))  RETURN_ERR(ERR_PARA, LOG_NULL);
    FILE   *file       = NULL;//文件指针
    char_t cmd[100]    = {'\0'};//插入配置文件的内容
    char_t rsyslogd[100] = {'\0'};//配置文件

    if(0 != access("/etc/rsyslog.d", F_OK) ) //判断目录是否存在 不存在创建目录
        mkdir("/etc/rsyslog.d", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

    snprintf(rsyslogd, sizeof(rsyslogd), "/etc/rsyslog.d/%s.conf", logName);
    if(NULL == (file = fopen(rsyslogd, "w+")))//创建新的配置文件
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
* @brief 解析日志轮转参数
* @param [in] paraitem 参数数组
* @param [out] cmd 获得解析后命令
* @param [out] cmdLen 命令长度
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
static int_t 
_log_parse_logratate(logrotate_t paraItem, char_t *cmd, u16_t *cmdLen)
{
    u16_t lenght =*cmdLen;
    switch(paraItem.paraName){
        /* 启用压缩，指的是轮替后的旧日志，这里默认用的是gzip压缩的*/
        case LOGROTATE_COMPRESS:       lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 以gzip -9的模式压缩*/
        case LOGROTATE_COMPRESSOPTION: lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 解压日志，默认是gunzip*/
        case LOGROTATE_UNCOMPRESS:     lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 轮替的日志文件会附加上一个短横线和YYYYMMDD格式的时间戳*/
        case LOGROTATE_DATEEXT:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 将以前的日志文件压缩推迟到下一次轮替*/
        case LOGROTATE_DELAYCOMPRESS:  lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 即使日志文件是空的也轮替*/
        case LOGROTATE_IFEMPTY:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 将轮替后的文件发送到指定E-mail地址*/
        case LOGROTATE_MAIL:           _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* 用于还在打开中的日志文件，把当前日志备份并截断，开始轮替*/
        case LOGROTATE_COPYTRUNCATE:   lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 向邮件发送轮替文件*/
        case LOGROTATE_MAILFIRST:      lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 轮替后历史文件（默认）*/
        case LOGROTATE_MAILLAST:       lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 如果在logrotate.conf中启用了压缩，这里是做不用压缩的参数*/
        case LOGROTATE_NOCOMPRESS:     lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 不发送邮件到任何地址*/
        case LOGROTATE_NOMAIL:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 轮替后日志文件放入指定的目录，必须和当前日志文件在同一个文件系统*/
        case LOGROTATE_OLDDIR:         _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* prostrotate/endscript 在做完轮替后的命令，两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
        case LOGROTATE_PROSTROTATE:    _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* prerotate/endscript	 在做轮替前的命令:两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
        case LOGROTATE_PREROTATE:      _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* 轮替最多保留之前的数据几次，超出的将被删除或邮件接收，设为0则不保存*/
        case LOGROTATE_ROTATE:         _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* 当日志增长到指定大小的时候开始轮替，它不会考虑*/
        case LOGROTATE_SIZE:           _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* 轮替文件名基于这个数字。例如，指定0时，原日志文件轮替的备份文件以.0为扩展名，如果指定9，就直接从.9开始跳过0-8然后再继续向后轮替rotate指定的次数。*/
        case LOGROTATE_START:          _PARSE_LOGROTATE(cmd, lenght, paraItem);break;
        /* 每天轮替选项*/
        case LOGROTATE_DAILY:          lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 一个月轮替一次*/
        case LOGROTATE_MONTHLY:        lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 如果当前的星期几比上次轮替的星期几少，或者过了一个多星期，就会发生轮替通常是在每周的第一天轮替，如果logrotate不是每天运行的，会在第一次有机会时进行轮替。*/
        case LOGROTATE_WEEKLY:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        /* 如果当前年份不同于上次轮替的年份，则进行日志轮替*/
        case LOGROTATE_YEARLY:         lenght += snprintf(cmd+lenght, 500-lenght, "%s\n", logrotateTable[paraItem.paraName]); break;
        default: return LOG_FAIL; 
    } 
        *cmdLen = lenght;      
        return LOG_SUCC;


}


/**
* @brief 加入日志轮转配置
* @param [in] logger     log处理器
* @param [in] paraitem 参数数组
* @param [in] paraNum  参数数组数量
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t
log_config_logrotate(log * logger, logrotate_t * paraItem, int_t itemNum )
{
    if(PARAMISNULL(logger))  RETURN_ERR(ERR_PARA, LOG_FAIL);
    if(PARAMISNULL(paraItem)) RETURN_ERR(ERR_PARA, LOG_FAIL);

    FILE   *file          = NULL;//文件指针
    char_t cmd[500]       = {'\0'};//插入配置文件的内容
    char_t logrotate[100] = {'\0'};//配置文件
    u16_t  cmdLen         = 0;
    u16_t  index          = 0;

    if(0 != access("/etc/logrotate.d", F_OK) ) //判断目录是否存在 不存在创建目录
        mkdir("/etc/logrotate.d", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);

    snprintf(logrotate, sizeof(logrotate), "/etc/logrotate.d/%s", logger->logName);
    if(NULL == (file = fopen(logrotate, "w+")))//创建新的配置文件
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
* @brief 打开log
* @param [in] logger     log处理器
* @param [in] option     log选项
	- CONS_OPTION   如果将信息发送给syslogd守护进程时发生错误，直接将相关信息输出到终端
	- NDELAY_OPTION 立即打开与系统日志的连接（通常情况下，只有在产生第一条日志信息的情况下才会打开与日志系统的连接）
	- NOWAIT_OPTION 在记录日志信息时，不等待可能的子进程的创建
	- ODELAY_OPTION 类似于NDELAY_OPTION参数，与系统日志的连接只有在syslog函数调用时才会创建
	- PERROR_OPTION 在将信息写入日志的同时，将信息发送到标准错误输出（POSIX.1-2001不支持该参数）
	- PID_OPTION    每条日志信息中都包括进程号
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
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
* @brief 关闭log
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t 
log_close(void)
{
    closelog();
	return LOG_SUCC;
}

/**
* @brief log消息过滤
* @param [in] bitLevel 过滤出需要的 等级 BIT宏将level转成bit level 然后赋值
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t
log_set_filter(u8_t bitLevel)
{
    setlogmask(bitLevel);//需要的消息被过滤
	return LOG_SUCC;
}


/**
* @brief 设置module打印log开关
* @param [in] modules   配置的模块数组
* @param [in] sum       数组数量
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
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
* @brief 发送log
* @param [in] file     文件名
* @param [in] func     函数名
* @param [in] line     行号
* @param [in] module   使用的模块
* @param [in] level    log等级
 - EMERG   内核崩溃等严重信息
 - ALERT   需要立刻修改的信息 
 - CRIT    严重级别，阻止整个系统或者整个软件不能正常工作的信息 
 - ERR     错误级别，阻止某个功能或者模块不能正常工作的信息 
 - WARNING 警告级别的消息 
 - NOTICE  最具有重要性的普通条件的信息 
 - INFO    一般信息的日志，最常用 
 - DEBUG   有调式信息的，日志信息最多 
* @param [in] format 格式化消息
* @param [in] ...    log消息内容
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t _log(string_t file, string_t func, int_t line, int_t module, int_t level, char* format, ... )
{
    if(module > MOD_BUTT)  RETURN_ERR(ERR_PARA, LOG_FAIL);
	if(LOG_LEVEL_IS_INVALID(level))  RETURN_ERR(ERR_PARA, LOG_FAIL);
	if(LOG_IS_INITIALIZED != Logger.InitFlag) RETURN_ERRDESC( ERR_USER, LOG_FAIL, "log do not init \n");
    char_t logMsg[MSG_SIZE];
    va_list vArgList; //定义一个va_list型的变量,这个变量是指向参数的指针
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