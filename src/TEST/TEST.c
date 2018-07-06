/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: TEST.c
@brief: 这是一个单元测试模块，进行程序的函数，模块的测试
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P
@version  0.0.0.1
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :
                 | <Date>      | <Version> | <Author> | <Description>
-------------------------------------------------------------------------------
  @todo          | 2017/04/06  | 0.0.0.1   | D.C.P    | 创建文件
******************************************************************************/
#include ".././util/util_inc.h"
#include "TEST.h"
#define TEST_FILE "./test.tmp"
/**  @brief 打印框架 */
#define SUITE_LINE "======================================"
#define CASE_LINE  "==>==>==>=>==>==>==>==>==>==>==>=>==>==>==>==>"
#define PRINT_LINE1 "****************************************自定义输出内容****************************************"

/**  @brief 内存泄漏表单框架 */
#define LEAK_SIZE 20000


/**
* @brief 段错误处理函数
*/
void _test_unit_test_handler(int signal)   
{  
    EXCEPTION(ERR_ENOSPC, "发生段错误");
}   


/**
* @brief 单元测试初始化
*/
int_t
_test_start_unit_test(void)
{
    printf("开始初始化单元测试\n");
    signal(SIGSEGV,_test_unit_test_handler);//段错误信号
    printf("欢迎进行单元测试\n");
    return TEST_SUCC;
}




/**
* @brief 计算时间差
* @param [in] start    开始时间
* @param [in] end      结束时间
*/
struct timespec
_test_gettime(struct timespec start, struct timespec end)
{
    struct timespec tmp;
    tmp.tv_sec = end.tv_sec - start.tv_sec;
    tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
    return tmp;
}


/**
* @brief 计算时间
* @param [in] testTime    时间
*/
double
_test_showtime(struct timespec testTime)
{
    double tmp = testTime.tv_sec + (double)testTime.tv_nsec/1000000000;//1秒 = 1 000 000 000 纳秒
    return tmp;
}



/**
* @brief 测试内容写入文件
* @param [in] filePath    文件路径
* @param [in] text        写入内容
*/
static int_t
_test_file(string_t filePath, string_t text)
{
    if (NULL == filePath || NULL == text) RETURN_ERR(ERR_PARA, TEST_FAIL);

    FILE *fp = NULL;
    fp = fopen(filePath, "a");
    if (fp == NULL) RETURN_ERRDESC( THE_ERRNO, TEST_FAIL, "文件打开错误(open file was fault)--%d", THE_ERRNO );

    fwrite(text, 1, strlen(text), fp); /* 写入文件*/

    fclose(fp);
    fp = NULL; //需要指向空，否则会指向原打开文件地址

    return TEST_SUCC;
}



/**
* @brief memResult上清楚数据
* @param [in] clearMem   被清除的内容
* @param [in] memResult   存储被清除内容的空间
* @param [in] leakCount   存储被清除内容的数量
*/
static int_t
_test_clear_memResult( char * clearMem, char_t memResult[LEAK_SIZE][40], u16_t * leakCount )
{
    u32_t count;                  //循环计数
    char_t freeMem[40] = {'\0'}; //被释放的内存
    strcpy(freeMem, clearMem);
    freeMem[strlen(freeMem)-1] = '\0';

    for (count = 0; count < LEAK_SIZE; count++)
        if (NULL != strstr(memResult[count], freeMem))
        {
            (*leakCount)--;
            memset(memResult[count],'\0',20);//清除
            return TEST_SUCC;
        }
    return TEST_FAIL;
}
/**
* @brief memResult上填充数据
* @param [in] fillMem   被填充的内容
* @param [in] memResult   存储填充内容的空间
* @param [in] leakCount   存储填充内容的数量
*/
static int_t
_test_fill_memResult( char * fillMem, char_t memResult[LEAK_SIZE][40], u16_t * leakCount )
{
    u32_t count;                  //循环计数

    (*leakCount)++;
    for (count = 0; count < LEAK_SIZE; count++)
        if( '\0' ==  memResult[count][0] ) 
        {
            strcpy(memResult[count], fillMem);
            return TEST_SUCC; 
        }
    return TEST_FAIL;
}

/**
* @brief 输出memResult内容
* @param [in] memResult   需要输出的内容
* @param [in] leakCount   内容的数量
*/
static int_t
_test_print_report( char_t memResult[50][40], u16_t leakCount )
{
    u32_t count;                     //循环计数
    char_t leakList[LEAK_SIZE*20] = {'\0'}; //内存泄漏表单
    u16_t listLen = 0;              //内容长度

    if( leakCount == 0 )
        listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,"-测试的内存泄漏数：%d\n\n", leakCount);
    else
    {
        listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,"-测试的内存泄漏数：%d", leakCount);
        listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,  "\n@@@@@@@@@@@@@@@@@@@@@@@");
        listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,  "\n@     泄漏地址  泄漏大小 \n");
    }

    for( count=0; (count <LEAK_SIZE)&&(leakCount>0); count++ )
        if( '\0' != memResult[count][0] ) 
        {
            listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,"@(%d)  %s", count, memResult[count]);
            leakCount--;
        }

    listLen += snprintf(leakList+listLen, LEAK_SIZE*20-listLen,"\n");
#ifdef PRINT_OUT
    printf("%s", leakList);
#endif
    _test_file(TEST_FILE, leakList);
    return TEST_SUCC;
}



/**
* @brief 打印内存监控报告
*/
static int_t
_test_malloc_monitor_report( void )
{
    FILE *fp = NULL;                 //读取的监控内存文件
    char_t line[100];                //逐行读取text并保存的缓冲区
    char_t *offset = NULL;           //缓冲区内的内存位移
    char_t memResult[LEAK_SIZE][40]; //存储申请内存结果
    u16_t leakCount = 0;             //申请内存计数

    memset(memResult,'\0',sizeof(memResult));//初始化memResult
    fp=fopen(MONITOR_MALLOC_FILE,"r");
    if (fp == NULL) RETURN_ERRDESC( THE_ERRNO, TEST_FAIL, "文件打开错误(open file was fault)--%d", THE_ERRNO );

    fseek(fp,0,SEEK_SET);//位移到文件头部
    /** 输出的模板
      *  = Start
      *  @ ./test:[0x804936a] + 0x8c5d830 0x190 
      *  @ ./test:[0x8049387] - 0x8c5d830
      *  = End
      **/
    while( NULL != fgets(line, 100, fp) )//逐行读取
    {
        if( '=' == line[0] ) continue;//当为首行(= Start)或者尾行(= End)时候抛弃本条内容
        offset = strchr(line+3, ' ');//位置到空格位置
        switch(*(offset+1))//判断+ 或者 -
        {
            case '+': _test_fill_memResult(offset+3, memResult, &leakCount);  break;
            case '-': _test_clear_memResult(offset+3, memResult, &leakCount); break;
        }
    }

    fclose(fp);
    remove(MONITOR_MALLOC_FILE);
    _test_print_report( memResult, leakCount );
    return TEST_SUCC;

}

/**
* @brief 初始化测试套件
* @param [in] testSuite   测试套件
*/
int_t
_test_init_suite(struct suite *testSuite)
{
    if (NULL == testSuite) RETURN_ERR(ERR_PARA, TEST_FAIL);
    testSuite->SuiteName = NULL;      //套件名
    testSuite->SuiteSum = 0;          //测试实例数
    testSuite->SuiteErrSum = 0;       //测试实例出错数
    testSuite->SuiteTime.tv_sec = 0;  //套件时间 秒
    testSuite->SuiteTime.tv_nsec = 0; //套件时间 纳秒
    testSuite->SuiteSetup = NULL;     //case装载函数
    testSuite->SuiteTeardown = NULL;  //case 卸载函数
    testSuite->TestGroupFree = NULL;  //测试组结构体释放函数
    return TEST_SUCC;
}

/**
* @brief 开始一个套件测试
* @param [in] testSuite 套件结构体
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t
_test_suite_start(struct suite *testSuite)
{
    if (NULL == testSuite) RETURN_ERR(ERR_PARA, TEST_FAIL);
    char_t text[120] = {'\0'};;
    setenv("MALLOC_TRACE", MONITOR_MALLOC_FILE, 1);//设定内存监控的全局变量
#if !defined( OS_MAC)
    mtrace();//启动内存监控
#endif

    snprintf(text, 120, "\n%sTEST %s GROUP START%s\n", SUITE_LINE ,testSuite->SuiteName ,SUITE_LINE);
    
#ifdef PRINT_OUT
    printf("%s", text);
#endif
    _test_file(TEST_FILE, text);
    return TEST_SUCC;
}

/**
* @brief 结束一个套件测试
* @param [in] testSuite 套件结构体
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t
_test_suite_end(struct suite *testSuite)
{
    if (NULL == testSuite) RETURN_ERR(ERR_PARA, TEST_FAIL);
    char_t text[250] = {'\0'};
    u32_t textLen = 0; //内容长度

    textLen += snprintf(text+textLen, 250, "\n%sTEST %s GROUP  END %s\n", SUITE_LINE ,testSuite->SuiteName ,SUITE_LINE);
    textLen += snprintf(text+textLen, 250-textLen, "-测试case数:%d \n-测试case出错数:%d \n-测试时长:%.5lf seconds\n", 
                                testSuite->SuiteSum, testSuite->SuiteErrSum, _test_showtime(testSuite->SuiteTime) );
    
#ifdef PRINT_OUT
    printf("%s", text);
#endif
    _test_file(TEST_FILE, text);

#if !defined( OS_MAC)
    muntrace();//结束内存监控
    _test_malloc_monitor_report();
#else
    _test_file(TEST_FILE, "\n");
#endif
    printf("%s suite [Finish]\n", testSuite->SuiteName);
    return TEST_SUCC;
}

/**
* @brief 初始化套件的测试实例
* @param [in] testCase   实例
*/
int_t
_test_init_case(struct Case *testCase)
{
    if (NULL == testCase) RETURN_ERR(ERR_PARA, TEST_FAIL);
    testCase->MySuite          = NULL;                   //我的套件初始化
    testCase->Line             = 0;                      //实例出错所在行
    testCase->Function         = NULL;                   //实例出错所在函数
    testCase->File             = NULL;                   //实例出错所在文件
    testCase->CaseName         = NULL;                   //实例名
    testCase->CaseTime.tv_sec  = 0;                      //计数器 秒
    testCase->CaseTime.tv_nsec = 0;                      //计数器 纳秒
    testCase->ErrText[0]       = '\0';                   //出错描述打印
    testCase->PrintLen         = 0;                      //需要打印的内容的长度
    testCase->PrintText[0]     = '\0';                   //需要打印的内容
    testCase->TESTGroupSP      = CLOSE_TESTGROUP;        //测试组的参数栈指针
    return TEST_SUCC;
}

/**
* @brief 处理测试的实例结果
* @param [in] testCase  实例结构体
* @param [in] desc      实例描述
* @param [in] result    实例测试结果
* @attention 当定义了TEST_FILE,就将测试内容写入文件
*/
int_t
_test_show_case_result(struct suite *testSuite, struct Case *testCase, string_t desc, u32_t result)
{
    if (NULL == testCase  || NULL == testSuite) RETURN_ERR(ERR_PARA, TEST_FAIL);
    char_t printText[2048] = {'\0'}; //打印的内容
    u32_t textLen = 0;               //内容长度

    testSuite->SuiteSum++;//测试实例计数
        if( NULL != testCase->CaseName )
            textLen += snprintf( printText+textLen, 1024, "\n%s CASE %s\n",CASE_LINE, testCase->CaseName);
        if( NULL != desc )
            textLen += snprintf( printText+textLen, 1024-textLen, "[ Desc   ]%s\n",desc);
    switch (result)
    {
    case TEST_SUCC:
        if( NULL != testCase->CaseName )
            textLen += snprintf( printText+textLen, 1024-textLen, "[ OK     ]%s: test sucess!! 测试通过\n",testCase->CaseName);
        if( 0 != testCase->CaseTime.tv_sec || 0 != testCase->CaseTime.tv_nsec )
            textLen += snprintf( printText+textLen, 1024-textLen, "[ Time   ]计时的代码段时间: %.5lf seconds\n",_test_showtime(testCase->CaseTime));
        break;
    case TEST_FAIL:
    testSuite->SuiteErrSum++;//测试出错实例计数
        if( NULL != testCase->CaseName )
            textLen += snprintf( printText+textLen, 1024-textLen, "[ ERR    ]%s: test was fault！！ 测试出错\n",testCase->CaseName);
            textLen += snprintf( printText+textLen, 1024-textLen, "[ ERR    ]出错位置: file(%s) function(%s) line(%d)\n",testCase->File,testCase->Function,testCase->Line);
            textLen += snprintf( printText+textLen, 1024-textLen, "%s\n", testCase->ErrText );                                                        
        break;
    default:
            testSuite->SuiteErrSum++;//测试出错实例计数
            textLen += snprintf( printText+textLen, 1024-textLen, "[ Emer   ]返回结果是错误的，无法处理\n");
    }

    if( '\0' != testCase->PrintText[0])//打印自定义输出内容
        textLen += snprintf( printText+textLen, 1024-textLen, "%s\n\n%s\n%s\n",PRINT_LINE1, testCase->PrintText,PRINT_LINE1); 
    
#ifdef PRINT_OUT
    printf("%s",printText);
#endif
    _test_file(TEST_FILE, printText);
    return TEST_SUCC;
}
