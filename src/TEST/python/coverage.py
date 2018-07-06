#!/usr/bin/python
# -*- coding: UTF-8 -*-
import common


ConverageFile = "./coverage.tmp"

CoverageTable = {
    "function"    : "函数",
    "called"      : "被调用",
    "call"        : "调用",
    "branch"      : "分支",
    "fallthrough" : "失败",
    "never"       : "从未",
    "blocks"      : "代码块",
    "executed"    : "执行",
    "returned"    : "正常返回",
    "taken"       : "通过",
};


import os.path



#定义向html文件追加头部
def _AppendHtmlHead(unitpath, html):
    HtmlPath = common.filePosition(unitpath)
    html.append("<!DOCTYPE html>\n\n")
    html.append("<html>\n")
    html.append("<head>\n")
    html.append("   <meta charset=\"utf-8\">\n")
    html.append("   <title>代码覆盖率报告</title>\n")
    html.append("   <link rel=\"stylesheet\" href=\"%shtml/css/coverage.css\">\n" % (HtmlPath))
    html.append("</head>\n\n")
    html.append("<body class=\"coverage-page\">\n")
    html.append("<h1>TEST.C 覆盖率</h1>\n")
    html.append("<table rules=\"cols\" >\n")
    return;



#定义向html文件追加尾部
def _AppendHtmlTail(unitpath, html):
    HtmlPath = common.filePosition(unitpath)
    html.append("</table>\n")
    html.append("</body>\n\n")
    html.append("   <script type=\"text/javascript\" src=\"%shtml/js/buildCoverage.js\"></script>\n" % (HtmlPath))
    html.append("</html>\n")
    return;



#定义转换关键字为html格式文本函数
def _ChangeKeyToHtml(html, line, offset):
    key=line.split()
    result=""
    for word in key: #转换
        if None == CoverageTable.get(word):
            result = result + " " + word
        else:
            result = result + " " + CoverageTable[word]
    
    if 0 == offset:
        html.append("<tr class=\"comment\" ><td></td><td></td><td class=\"detail\">%s\n" % (result))
    else:
        html.append("%s\n" % (result))
    del result,key
    return;



#定义解析语句为html格式文本函数
def _ChangeCodeToHtml(html, line):
    blocks = line.split(":")
    result = ""
    for block in blocks[2:]:
        result = result + block + ":"
    result = result[:len(result)-2]
    html.append("<tr><td class=\"sum\">%s:</td>   <td class=\"line\">%s:</td><td class=\"code\">%s</td></tr>\n" \
                                                                                            % ( blocks[0], blocks[1], result ))
    del blocks, result
    return;


#定义解析.gcov文件
def _ParseGcov(unitpath, gcovfd, htmlfd):
    html=[]#创建html文本
    offset=0
    _AppendHtmlHead(unitpath, html)
    #解析文件-----------------------------------
    for  line in  gcovfd.readlines():
        if -1 == line.find(":"):#解析文件中的覆盖率注释
            _ChangeKeyToHtml(html, line, offset)
            offset+=1
            continue
        if 0 != offset:
            offset=0; html.append("</td></tr>\n")
        #解析文件中的代码
        _ChangeCodeToHtml(html, line)
    #解析完成----------------------------------
    _AppendHtmlTail(unitpath, html)  
    htmlfd.writelines(html) #生成html文件
    del html, offset
    return;



#定义解析Coverage文件
def _ParseCoverage(filepath , unitpath, resultdir, file):
    GcovFd = open(filepath + file + ".gcov" , "r" )
    fileName = file.split('/')
    htmlName = fileName[len(fileName)-1]
    HtmlFd = open(resultdir + "/" + htmlName[:len(htmlName)-2] + ".html" , "w+" )
    _ParseGcov(unitpath, GcovFd, HtmlFd) #解析生成覆盖率结果文件
    del GcovFd, HtmlFd, htmlName, fileName
    return;

#得到覆盖率参数值
def GetCoverageValue(lines, offset, des):
    if -1 !=  lines[offset].find(des):
        tmp = lines[offset].split(":") 
        return tmp[1].split(); 
    else:
        return ["0","0","None"];



#定义追加函数块
def _AppendFuncBlock(lines, offset, funcblock):
    FuncName = lines[offset].split()
    FuncName[1] = FuncName[1].strip('\'')
    #语句覆盖率
    LinesExecuted = GetCoverageValue(lines, offset+1, "Lines executed")
    #分支覆盖率
    BranchesExecuted = GetCoverageValue(lines, offset+2, "Branches executed")
    #条件覆盖率
    LeastOnce = GetCoverageValue(lines, offset+3, "Taken at least once")
    #函数覆盖率
    Callsexecuted = GetCoverageValue(lines, offset+4, "Calls executed")
    #生成文件
    funcblock.append("              <a>函数:%s</a></br>\n" % (FuncName[1]))
    funcblock.append("              <p>\n")
    funcblock.append("                  <b>语句覆盖率(%s):</b>%s</br>\n" % (LinesExecuted[0], LinesExecuted[2]))
    funcblock.append("                  <b>分支覆盖率(%s):</b>%s</br>\n" % (BranchesExecuted[0], BranchesExecuted[2]))
    funcblock.append("                  <b>条件覆盖率(%s):</b>%s</br>\n" % (LeastOnce[0], LeastOnce[2]))
    funcblock.append("                  <b>函数覆盖率(%s):</b>%s</br>\n" % (Callsexecuted[0], Callsexecuted[2]))
    funcblock.append("              </p>\n")

    offset+=5
    del FuncName, LinesExecuted, BranchesExecuted, LeastOnce, Callsexecuted
    return offset;



#定义追加文件块
def _AppendFileBlock(lines, offset, fileblock):
    if -1 != lines[offset].find("sysmacros.h"):
        offset+=5; return;
        
    FuncName = lines[offset].split()
    FuncName[1] = FuncName[1].strip('\'')
    fileName = FuncName[1].split('/')
    #语句覆盖率
    LinesExecuted = GetCoverageValue(lines, offset+1, "Lines executed")
    #分支覆盖率
    BranchesExecuted = GetCoverageValue(lines, offset+2, "Branches executed")
    #条件覆盖率
    LeastOnce = GetCoverageValue(lines, offset+3, "Taken at least once")
    #函数覆盖率
    Callsexecuted = GetCoverageValue(lines, offset+4, "Calls executed")
    #生成文件
    fileblock.append("              <h1>%s 覆盖率</h1>\n" % (fileName[len(fileName)-1]))
    fileblock.append("              <h5>语句覆盖率:</h5>\n")
    fileblock.append("              <canvas class=\"ring-line\"  percent='%s/%s'></canvas>\n" % (LinesExecuted[2], LinesExecuted[0]))
    fileblock.append("              <h5>分支覆盖率:</h5>\n")
    fileblock.append("              <canvas class=\"ring-branch\" percent='%s/%s'></canvas>\n" % (BranchesExecuted[2], BranchesExecuted[0]))
    fileblock.append("              <h5>条件覆盖率:</h5>\n")
    fileblock.append("              <canvas class=\"ring-condition\" percent='%s/%s'></canvas>\n" % (LeastOnce[2], LeastOnce[0]))
    fileblock.append("              <h5>函数覆盖率:</h5>\n")
    fileblock.append("              <canvas class=\"ring-function\" percent='%s/%s'></canvas>\n" % (Callsexecuted[2], Callsexecuted[0]))

    offset+=6
    del FuncName, fileName, LinesExecuted, BranchesExecuted, LeastOnce, Callsexecuted        
    return offset;


                    


#定义追加文件块
def _Appendhtmlblock(htmllock, FuncBlock, FileBlock, file):
    fileName = file.split('/')
    htmlName = fileName[len(fileName)-1]
    htmllock.append("           <span class=\"panel color9\" onmousemove=\"MouseArrive(this)\" onmouseout=\"MouseLeave(this)\">\n")
    htmllock.append("               <div class=\"panel-coverage\">\n")
    htmllock.extend(FileBlock)
    htmllock.append("               <a class=\"file\" href=\"./%s.html\">进入</br>文件</a>\n" % (htmlName[:len(htmlName)-2])) 
    htmllock.append("               </div>\n") 
    htmllock.append("               <div class=\"panel-text\">\n")
    htmllock.extend(FuncBlock)
    htmllock.append("               </div>\n") 
    htmllock.append("           </span>\n") 
    return;



#定义解析Coverage.tmp文件每行
def _ParseLines(lines, htmlblock, file):
    FuncBlock=[] #存储函数块
    FileBlock=[] #存储文件块
    Offset=0
    if -1 != lines[Offset].find("gnu_dev_major"):
        Offset+=5
    if -1 != lines[Offset].find("gnu_dev_minor"):
        Offset+=5
    if -1 != lines[Offset].find("nu_dev_makedev"):
        Offset+=5

    while len(lines) > Offset:
        if -1 != lines[Offset].find("Function "):
            Offset = _AppendFuncBlock(lines, Offset, FuncBlock) #处理函数块
        if -1 != lines[Offset].find("File "):
            Offset = _AppendFileBlock(lines, Offset, FileBlock) #处理文件块
        Offset+=1
    _Appendhtmlblock(htmlblock, FuncBlock, FileBlock, file)
    del FuncBlock, FileBlock
    return;




#定义解析Coverage.tmp文件
def _ParseTmpFile(file, htmlblock):
    TmpFd = open(ConverageFile)
    #解析文件-----------------------------------
    lines = TmpFd.readlines()
    _ParseLines(lines, htmlblock, file)
    #解析完成----------------------------------
    del TmpFd
    return;

#定义覆盖率生成函数
def Coverage(filepath ,unitpath, resultdir, gcov, file):
    HtmlBlock=[] #创建html文本块
    for File in file: # 处理文件
        if -1 != File.find("test_"): #当文件为单元测试文件则跳过
            continue
        os.system(gcov + " -bf "  + File + " > " + ConverageFile ) #将执行结果写入到文件
        _ParseCoverage(filepath ,unitpath, resultdir, File) #解析Coverage文件
        _ParseTmpFile( File, HtmlBlock) #解析Coverage.tmp文件
    print "完成覆盖率分析！(parse the coverage success!)"

    return HtmlBlock;