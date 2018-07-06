/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: log_pub.h
@brief:  log公共头文件
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_LOG_PUB_H__
#define __INC_LOG_PUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "log.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief log 函数内处理消息的最大值 */
#define MSG_SIZE 1024

typedef log * log_t;

/**  @brief log 管理结构体 */
typedef struct {
    u32_t    paraName;   /**< 参数名*/
    string_t paraDetial; /**< 参数内容*/
} logrotate_t;


/**  @brief log 管理结构体 */
typedef struct {
  u32_t module;   /**< 模块名*/
  u8_t bitLevel;  /**< 过滤的log等级*/
  u8_t print;     /**< 打印   ENABLED使能 DISABLE关闭使能*/
} logMod_t;

/**  @brief LOG消息等级 */
enum log_level{
  NONE    = LOG_NONE,        /** 关闭所有打印 */
  EMERG   = LOG_EMERG,       /** 内核崩溃等严重信息   */
  ALERT   = LOG_ALERT,       /** 需要立刻修改的信息   */
  CRIT    = LOG_CRIT,        /** 严重级别，阻止整个系统或者整个软件不能正常工作的信息 */
  ERR     = LOG_ERR,         /** 错误级别，阻止某个功能或者模块不能正常工作的信息 */
  WARNING = LOG_WARNING,     /** 警告级别的消息     */
  NOTICE  = LOG_NOTICE,      /** 最具有重要性的普通条件的信息 */
  INFO    = LOG_INFO,        /** 一般信息的日志，最常用 */
  DEBUG   = LOG_DEBUG,       /** 有调式信息的，日志信息最多 */
};


/**  @brief LOG操作项*/
enum log_option{
CONS_OPTION   = LOG_CONS,   /**  如果将信息发送给syslogd守护进程时发生错误，直接将相关信息输出到终端*/
NDELAY_OPTION = LOG_NDELAY, /**  立即打开与系统日志的连接（通常情况下，只有在产生第一条日志信息的情况下才会打开与日志系统的连接）*/
NOWAIT_OPTION = LOG_NOWAIT, /**  在记录日志信息时，不等待可能的子进程的创建*/
ODELAY_OPTION = LOG_ODELAY, /**  类似于LOG_NDELAY参数，与系统日志的连接只有在syslog函数调用时才会创建*/
PERROR_OPTION = LOG_PERROR, /**  在将信息写入日志的同时，将信息发送到标准错误输出（POSIX.1-2001不支持该参数）*/
PID_OPTION    = LOG_PID,    /**  每条日志信息中都包括进程号*/
};


/**  @brief LOG类型*/
enum log_type{
  KERN     = LOG_KERN,          /**  kern     内核产生的消息*/
  USER     = LOG_USER,          /**  user     用户程序产生的相关信息*/
  MAIL     = LOG_MAIL,          /**  mail     邮件*/
  DAEMON   = LOG_DAEMON,        /**  daemon   其他系统守护进程，如routed */
  AUTH     = LOG_AUTH,          /**  auth     认证系统：login、su、getty等*/
  SYSLOG   = LOG_SYSLOG,        /**  syslog   rsyslog服务内部的信息,时间标识*/
  LPR      = LOG_LPR,           /**  lpr      系统打印机缓冲池：lpr、lpd */
  NEWS     = LOG_NEWS,          /**  news     新闻组*/
  UUCP     = LOG_UUCP,          /**  uucp     unix to unix copy, unix主机之间相关的通讯*/
  CRON     = LOG_CRON,          /**  cron    cron守护进程*/
  AUTHPRIV = LOG_AUTHPRIV,      /**  authpriv ssh,ftp等登录信息的验证信息*/
  FTP      = LOG_FTP,           /**  ftp      文件传输协议：ftpd、tftpd */
  LOCAL0   = LOG_LOCAL0,        /**  自定义的日志设备 local0*/
  LOCAL1   = LOG_LOCAL1,        /**  自定义的日志设备 local1*/
  LOCAL2   = LOG_LOCAL2,        /**  自定义的日志设备 local2*/
  LOCAL3   = LOG_LOCAL3,        /**  自定义的日志设备 local3*/
  LOCAL4   = LOG_LOCAL4,        /**  自定义的日志设备 local4*/
  LOCAL5   = LOG_LOCAL5,        /**  自定义的日志设备 local5*/
  LOCAL6   = LOG_LOCAL6,        /**  自定义的日志设备 local6*/
  LOCAL7   = LOG_LOCAL7,        /**  自定义的日志设备 local7*/
};

/**  @brief logrotate参数*/
enum logrotate_para{
  LOGROTATE_COMPRESS = 0,   /** 启用压缩，指的是轮替后的旧日志，这里默认用的是gzip压缩的*/
  LOGROTATE_COMPRESSOPTION, /** 以gzip -9的模式压缩*/
  LOGROTATE_UNCOMPRESS,     /** 解压日志，默认是gunzip*/
  LOGROTATE_DATEEXT,        /** 轮替的日志文件会附加上一个短横线和YYYYMMDD格式的时间戳*/
  LOGROTATE_DELAYCOMPRESS,  /** 将以前的日志文件压缩推迟到下一次轮替*/
  LOGROTATE_IFEMPTY,        /** 即使日志文件是空的也轮替*/
  LOGROTATE_MAIL,           /** 将轮替后的文件发送到指定E-mail地址*/
  LOGROTATE_COPYTRUNCATE,   /** 用于还在打开中的日志文件，把当前日志备份并截断，开始轮替*/
  LOGROTATE_MAILFIRST,      /** 向邮件发送轮替文件*/
  LOGROTATE_MAILLAST,       /** 轮替后历史文件（默认）*/
  LOGROTATE_NOCOMPRESS,     /** 如果在logrotate.conf中启用了压缩，这里是做不用压缩的参数*/
  LOGROTATE_NOMAIL,         /** 不发送邮件到任何地址*/
  LOGROTATE_OLDDIR,         /** 轮替后日志文件放入指定的目录，必须和当前日志文件在同一个文件系统*/
  LOGROTATE_PROSTROTATE,    /** postrotate/endscript 在做完轮替后的命令，两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
  LOGROTATE_PREROTATE,      /** prerotate/endscript	在做轮替前的命令,两个关键字必须单独成行，使用的操作在2者之间相当于分组“{}”，注意的使用外部指令时要用绝对路径*/
  LOGROTATE_ROTATE,         /** 轮替最多保留之前的数据几次，超出的将被删除或邮件接收，设为0则不保存*/
  LOGROTATE_SIZE,           /** 当日志增长到指定大小的时候开始轮替，它不会考虑*/
  LOGROTATE_START,          /** 轮替文件名基于这个数字。例如，指定0时，原日志文件轮替的备份文件以.0为扩展名，如果指定9，就直接从.9开始跳过0-8然后再继续向后轮替rotate指定的次数。*/
  LOGROTATE_DAILY,          /** 每天轮替选项*/
  LOGROTATE_MONTHLY,        /** 一个月轮替一次*/
  LOGROTATE_WEEKLY,         /** 如果当前的星期几比上次轮替的星期几少，或者过了一个多星期，就会发生轮替通常是在每周的第一天轮替，如果logrotate不是每天运行的，会在第一次有机会时进行轮替。*/
  LOGROTATE_YEARLY,         /** 如果当前年份不同于上次轮替的年份，则进行日志轮替*/
  LOGROTATE_BUTT
};
/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 往syslogd的配置文件内加入自己的配置消息类型
* @note  logType.msgLevel  logPath --->   例：mail.info      /flash/var/maillog_info
* @param [in] logType  消息类型（syslogd支持的类型）
* @param [in] logLevel 消息等级（syslogd支持的等级）
* @param [in] logName  log名字
* @param [in] logPath  log文件路径
* @param [in] logPath  log远端路径
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
log_t
log_config_rsyslogd(int_t logType, int_t logLevel,  string_t logName, string_t logPath, string_t logRemote);


/**
* @brief 加入日志轮转配置
* @param [in] logger     log处理器
* @param [in] paraitem 参数数组
* @param [in] paraNum  参数数组数量
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t
log_config_logrotate(log_t logger, logrotate_t *paraItem, int_t itemNum );


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
log_open(log_t logger, int_t option);

/**
* @brief 关闭log
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t 
log_close(void);

/**
* @brief log消息过滤
* @param [in] bitLevel     过滤出需要的 等级 
 - BIT_EMERG
 - BIT_ALERT
 - BIT_CRIT
 - BIT_ERR
 - BIT_WARNING
 - BIT_NOTICE
 - BIT_INFO
 - BIT_DEBUG
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t
log_set_filter(u8_t bitLevel);

/**
* @brief 设置module打印log开关
* @param [in] modules   配置的模块数组
* @param [in] sum       数组数量
* @retval LOG_SUCC  表示成功
* @retval LOG_FAIL  表示失败
*/
int_t
log_set_modules(logMod_t *modules, int_t sum );

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
#define log(module, level, format, ...) _log( __FILE__,  __FUNCTION__, __LINE__, module, level, format,  ##__VA_ARGS__ )
/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/
/**
* @brief 将level转成bit位
*/
#define BIT(level) _LEVEL_TO_BIT(level)



#endif/*__INC_LOG_PUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */