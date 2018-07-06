#!/usr/bin/python
# -*- coding: UTF-8 -*-
#！！！！！将本脚本放在需要单元测试的文件目录下

#/**************************需要修改部分************************/
UnitPath="./" #单元测试文件路径
FilePath="./" #待编译文件以及编译后生成的文件所在的路径(一般不用修改)
TestFile = ["test_TEST.c"]; #待测试测试文件
#/************************************************************/

import os.path
import sys
sys.path.append(UnitPath+"python")
import common
import performance
import coverage
import index




# 定义清理编译结果函数
def CleanUp( path ):
   pyc=path+"python/*.pyc"
   cmd= "rm " + pyc + " test *.gcda *.gcno *.gcov sysmacros.* *.tmp gmon.out"
   os.system(cmd)  
   print cmd 
   return;

# 定义编译函数
def Compile( makefile, cmd, resultfile):
    if "" == makefile:
        os.system(cmd)
    else:
        os.system(makefile)

    if False == os.path.isfile(resultfile): #如果不存在就返回False 
        raise UserWarning
    return;

# 定义清理编译结果函数
def CreateDir( resultdir):
    if False == os.path.isdir( resultdir ): #如果不存在就创建文件夹
        os.mkdir(resultdir)
    return;

# 定义编译所需的命令
def GenerateCmd( filepath ,unitpath, testfile, unittestfile ):
    cmd ="" #定义命令
    for File in testfile: # 处理文件
        cmd += filepath + File +" "
    for File in unittestfile: # 处理文件
        cmd += unitpath + File +" "

    return cmd;

#执行代码
try:
    try:
        File = GenerateCmd(FilePath, UnitPath, TestFile, common.UnitTestFile )#组装编译命令
        GccCmd =  common.Gcc + " " + common.Args + " " + File + " -o test " + common.Libs
        Compile(common.Makefile, GccCmd, FilePath + common.ResultFile) # 编译文件
        CreateDir(FilePath + common.ResultDir) #生存文件夹
        os.system(FilePath + common.ResultFile) #执行程序
        HtmlCover = coverage.Coverage(FilePath, UnitPath, common.ResultDir, common.Gcov, TestFile+common.UnitTestFile ) #生成覆盖率文件
        performance.Performance(FilePath, UnitPath, common.ResultDir, common.Gprof) #生成性能文件
        index.Index(FilePath, UnitPath, common.ResultDir, HtmlCover)  #生成index文件
    except UserWarning:
        print "编译出错(complie was fault!)"
finally:
    CleanUp(UnitPath) #清理现场