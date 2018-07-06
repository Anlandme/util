#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os.path
import sys
import os

Gcc="gcc" #gcc 编译工具
Gcov="gcov"  #gcov 覆盖率工具
Gprof="gprof"  #gprof 性能工具
Args="-fprofile-arcs -ftest-coverage -pg -rdynamic -g" # gcc 编译参数 主要是gprof 和 gcov的编译参数
Libs="-lpthread -lrt" #关联库

Makefile="" #当使用makefile 修改这个变量
ResultFile="./test" #gcc或者makefile生成文件

ResultDir = "./result" #生成结果存放目录

UnitTestFile = ["TEST.c"]; #单元测试文件

Arg = "" #arg命令行




# 定义清理编译结果函数
def CleanUp( path ):
   pyc=path+"python/*.pyc"
   cmd= "rm " + pyc + " test *.gcda *.gcno *.gcov sysmacros.* *.tmp gmon.out"
   os.system(cmd)  
   print cmd 
   return;





# 定义编译函数
def Compile(cmd):
    global Arg, Makefile, ResultFile
    if 'a' == Arg:
        os.system(cmd)
    if 'e'== Arg:
        os.system(Makefile)

    if False == os.path.isfile(ResultFile): #如果不存在就返回False 
        raise UserWarning
    return;





# 定义清理编译结果函数
def CreateDir():
    global ResultDir
    if False == os.path.isdir(ResultDir ): #如果不存在就创建文件夹
        os.mkdir(ResultDir)
    return;





# 定义编译所需的命令
def GenerateCmd( filepath ,unitpath, testfile):
    global UnitTestFile
    cmd ="" #定义命令
    for File in testfile: # 处理文件
        cmd += filepath + File +" "
    for File in UnitTestFile: # 处理文件
        cmd += unitpath + File +" "

    return cmd;




#  执行测试文件test
def ExecuteTest():
    global Arg, ResultFile
    if 'e' != Arg:
        result = os.system(ResultFile)
        if( 0 != result and 256 != result):
            print "运行程序失败!"
            sys.exit(0);#直接退出程序
    return;





#定义 test.py文件是否在源代码文件里面
def filePosition( unitpath ):
    if True == os.path.isfile("./TEST.c") and True == os.path.isfile("./TEST.h") \
            and True == os.path.isdir("./python") and True == os.path.isdir("./html"):
        return "../";
    return "../"+unitpath;


    


# 展示支持命令
def ShwoCmd():
    print "\n -h show all cmd  [example: python UnitTest.py -h ]"
    print "\n -a auto Compile UnitTest File  [example: python UnitTest.py -a ]"
    print "\n -e Compile UnitTest File on embedded systerm  [example: python UnitTest.py -e ResultsFile ]"
    print "\n -m use the Makefile Compile UnitTest File  [example: python UnitTest.py -m Makefile ResultsFile ]"
    return;




# 处理命令行参数
def HandleCmdArg():
    global Arg, Makefile, ResultFile

    if 1 >= len(sys.argv) or "-h" == sys.argv[1]:
        ShwoCmd()
        sys.exit(0);#直接退出程序
    #处理 -a命令
    if "-a" == sys.argv[1]:
        if 2 != len(sys.argv):
            print "\n -a auto Compile UnitTest File  [example: python UnitTest.py -a ]"
            sys.exit(0);#直接退出程序
        Arg = "a"; return;


    #处理 -e命令
    if "-e" == sys.argv[1]:
        if 3 != len(sys.argv):
            print "\n -e Compile UnitTest File on embedded systerm  [example: python UnitTest.py -e ResultsFile ]"
            sys.exit(0);#直接退出程序
        ResultFile =sys.argv[2];
        Arg = "a"; return;


    #处理 -m命令
    if "-m" == sys.argv[1]:
        if 4 != len(sys.argv):
            print "\n -m use the Makefile Compile UnitTest File  [example: python UnitTest.py -m Makefile ResultsFile ]"
            sys.exit(0);#直接退出程序
        Makefile =sys.argv[2]; 
        ResultFile =sys.argv[3]; 
        Arg = "m"; return;


    ShwoCmd()
    sys.exit(0);#直接退出程序