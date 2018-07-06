/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_log.c
@brief:  log模块单元测试
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/08/01  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include ".././TEST/pub_TEST.h"

/**********************************************************************************************
*   Name：测试log的 函数错误返回
**********************************************************************************************/
//mem函数类
DEFINE_TEST_CODE_START(test_log_false_case)
    log logger;
    logrotate_t  paraItem;
    logMod_t mod;
    TEST_FUNC_RETURN_NULL(log_config_rsyslogd(USER,9,"test","/tmp/log",NULL),   查看 log_config_rsyslogd   错误等级返回错误情况);
    TEST_FUNC_RETURN_NULL(log_config_rsyslogd(USER,NONE,"test","/tmp/log",NULL),   查看 log_config_rsyslogd   错误等级返回错误情况);
    TEST_FUNC_RETURN_NULL(log_config_rsyslogd(USER,DEBUG,NULL,"/tmp/log",NULL), 查看 log_config_rsyslogd   空名字返回错误情况);
    TEST_FUNC_RETURN_NULL(log_config_rsyslogd(USER,DEBUG,"test",NULL,NULL),     查看 log_config_rsyslogd   路径返回错误情况);
    TEST_FUNC_RETURN_FAIL(log_config_logrotate(&logger,NULL,2),                 查看 log_config_logrotate  返回错误情况);
    TEST_FUNC_RETURN_FAIL(log_config_logrotate(NULL,&paraItem,2),               查看 log_config_logrotate  返回错误情况);
    TEST_FUNC_RETURN_FAIL(log_open(NULL,CONS_OPTION),                           查看 log_open              返回错误情况);
    TEST_FUNC_RETURN_FAIL(log_set_modules(NULL,2),                              查看 log_set_modules       过滤操作错误返回错误情况);
    TEST_FUNC_RETURN_FAIL(log_set_modules(&mod,MOD_BUTT+1),                     查看 log_set_modules       过滤操作错误返回错误情况);
    TEST_FUNC_RETURN_FAIL(log(0,LOG_EMERG-1,"TEST"),                            查看 log                   等级低于LOG_EMERG返回错误情况);
    TEST_FUNC_RETURN_FAIL(log(0,LOG_EMERG+1,"TEST"),                            查看 log                   等级高于LOG_EMERG返回错误情况);
    TEST_FUNC_RETURN_FAIL(log(0,LOG_EMERG,"TEST"),                              查看 log                   未初始化情况);
    TEST_FUNC_RETURN_FAIL(log(MOD_BUTT+1,LOG_EMERG,"TEST"),                     查看 log                   module非法情况);
DEFINE_TEST_CODE_END

/**********************************************************************************************
*   Name：测试log的 函数正确返回
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_config_rsyslogd_true_case)
    TEST_FUNC_RETURN_IS_NOT_NULL(log_config_rsyslogd(LOCAL0,DEBUG,"test_log","/tmp/log","172.16.88.65"), 测试 log_config_rsyslogd   正常使用情况);
    FILE *file=NULL;//文件指针
    char_t buffer[200];//读取内容
    TEST_FUNC_RETURN_IS_NOT_NULL((file = fopen("/etc/rsyslog.d/test_log.conf", "r")), 返回为文件指针);
    fread(buffer, sizeof(buffer), 1, file);
    fclose(file);
    test_printf("test_log:%s\n", buffer);
DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_config_logrotate_true_case)
    log_t logger;
    logrotate_t  paraItem[4] = {{LOGROTATE_SIZE ,"10M" },{LOGROTATE_ROTATE, "9"},{LOGROTATE_WEEKLY, NULL},{LOGROTATE_PREROTATE, "echo \"hello\" "}};
    TEST_FUNC_RETURN_IS_NOT_NULL((logger = log_config_rsyslogd(LOCAL0,DEBUG,"test_log","/tmp/log",NULL)), 测试 log_config_rsyslogd   正常使用情况);
    TEST_FUNC_RETURN_SUCC(log_config_logrotate(logger, paraItem, 4), 测试 log_config_logrotate   正常使用情况);
    FILE *file=NULL;//文件指针
    char_t buffer[200];//读取内容
    TEST_FUNC_RETURN_IS_NOT_NULL((file = fopen("/etc/logrotate.d/test_log", "r")), 返回为文件指针);
    fread(buffer, sizeof(buffer), 1, file);
    fclose(file);
    test_printf("test_log:%s\n", buffer);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_log_true_case)
    remove("/tmp/log");
    log_t logger;
    TEST_FUNC_RETURN_IS_NOT_NULL((logger = log_config_rsyslogd(LOCAL0,DEBUG,"test_log","/tmp/log",NULL)), 测试 log_config_rsyslogd   正常使用情况);
    TEST_FUNC_RETURN_SUCC(log_open(logger, CONS_OPTION), 正常打开log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,DEBUG,"test_log_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log_close(), 正常关闭log功能);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_log_filter_true_case)
    remove("/tmp/log");
    log_t logger;
    int i;
    TEST_FUNC_RETURN_IS_NOT_NULL((logger = log_config_rsyslogd(LOCAL0,DEBUG,"test_log","/tmp/log",NULL)), 测试 log_config_rsyslogd   正常使用情况);
    TEST_FUNC_RETURN_SUCC(log_open(logger, CONS_OPTION), 正常打开log功能);
    TEST_FUNC_RETURN_SUCC(log_set_filter(BIT(NOTICE)|BIT(INFO)), 根据设置，过滤对应模块的log信息);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,DEBUG,  "DEBUG: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,INFO,   "INFO: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,NOTICE, "NOTICE: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,WARNING,"WARNING: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,ERR,    "ERR: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,CRIT,   "CRIT: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,ALERT,  "ALERT: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,EMERG,  "EMERG: test_log_filter_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log_set_filter(0xff), 恢复之前过滤的设置);
    TEST_FUNC_RETURN_SUCC(log_close(), 正常关闭log功能);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_log_modules_true_case)
    remove("/tmp/log");
    log_t logger;
    logMod_t module={ MOD_SYSREM, 0, ENABLED};
    module.bitLevel = BIT(INFO)|BIT(EMERG);
    TEST_FUNC_RETURN_IS_NOT_NULL((logger = log_config_rsyslogd(LOCAL0,DEBUG,"test_log","/tmp/log",NULL)), 测试 log_config_rsyslogd   正常使用情况);
    TEST_FUNC_RETURN_SUCC(log_open(logger, CONS_OPTION), 正常打开log功能);
    TEST_FUNC_RETURN_SUCC(log_set_modules(&module, 1), 根据设置，过滤对应模块的log信息);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,DEBUG,  "DEBUG: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,INFO,   "INFO: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,NOTICE, "NOTICE: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,WARNING,"WARNING: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,ERR,    "ERR: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,CRIT,   "CRIT: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,ALERT,  "ALERT: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log(MOD_SYSREM,EMERG,  "EMERG: test_log_modules_true_case TEST THE LOG!!"), 正常使用log功能);
    TEST_FUNC_RETURN_SUCC(log_close(), 正常关闭log功能);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试主函数
**********************************************************************************************/
int main (void)
{
    Start_UnitTest();//开始单元测试

    TEST_SUITE_START(测试函数调用的非正常情况)
        TEST_CASE(test_log_false_case,          调用log函数失败情况,          测试调用函数是否是预期失败);
    TEST_SUITE_END
    TEST_SUITE_START(测试配置syslogd和logrotate)
        TEST_CASE(test_config_rsyslogd_true_case,          syslogd配置成功情况,            测试syslogd配置是否是预期成功);
        TEST_CASE(test_config_logrotate_true_case,         logrotate配置成功情况,          测试logrotate配置是否是预期成功);
    TEST_SUITE_END
    TEST_SUITE_START(测试log正常写入)
        TEST_CASE(test_log_true_case,          调用log函数写入情况,          测试调用函数是否是预期成功);
        TEST_CASE(test_log_filter_true_case,   调用log过滤函数情况,          测试调用过滤函数是否是预期成功);
        TEST_CASE(test_log_modules_true_case,  调用log模块过滤函数情况,       测试调用模块过滤函数是否是预期成功);
    TEST_SUITE_END
    return 0;
}