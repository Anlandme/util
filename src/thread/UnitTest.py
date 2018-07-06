#!/usr/bin/python
# -*- coding: UTF-8 -*-
#！！！！！将本脚本放在需要单元测试的文件目录下

#/**************************需要修改部分************************/
UnitPath="../TEST/" #单元测试文件路径
FilePath="../" #待编译文件以及编译后生成的文件所在的路径(一般不用修改)
GccFile = ["errno/err.c", "except/except.c", "thread/thread.c", "list/list.c", "thread/test_thread.c"] #编译关联文件
TestFile = ["thread/thread.c"]; #待测试测试文件
#/************************************************************/

import sys
sys.path.append(UnitPath+"python")
import common
import performance
import coverage
import index



#处理命令行
common.HandleCmdArg()
#执行代码
try:
    try:
        File = common.GenerateCmd(FilePath, UnitPath, GccFile)#组装编译命令
        GccCmd =  common.Gcc + " " + common.Args + " " + File + " -o test " + common.Libs
        common.Compile(GccCmd) # 编译文件
        common.CreateDir() #生存文件夹
        common.ExecuteTest() #执行程序
        HtmlCover = coverage.Coverage(FilePath, UnitPath, common.ResultDir, common.Gcov, TestFile ) #生成覆盖率文件
        performance.Performance(UnitPath, common.ResultDir, common.Gprof) #生成性能文件
        index.Index(UnitPath, common.ResultDir, HtmlCover)  #生成index文件
    except UserWarning:
        print "编译出错(complie was fault!)"
finally:
    common.CleanUp(UnitPath) #清理现场