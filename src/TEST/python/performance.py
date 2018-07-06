#!/usr/bin/python
# -*- coding: UTF-8 -*-
performancceFile = "./performance.tmp"

import os.path
import common

#定义向html文件追加头部
def _AppendHtmlHead(unitpath, html):
    HtmlPath = common.filePosition(unitpath)
    html.append("<!DOCTYPE html>\n\n")
    html.append("<html>\n")
    html.append("<head>\n")
    html.append("   <meta charset=\"utf-8\">\n")
    html.append("   <title>性能报告</title>\n")
    html.append("   <link rel=\"stylesheet\" href=\"%shtml/css/performance.css\">\n" % (HtmlPath))
    html.append("</head>\n\n")
    html.append("<body class=\"performance\">\n")
    html.append("<h1>性能报告</h1>\n")
    html.append("<span>\n")
    html.append("    <input class='buttom' id='tag' type='checkbox'>\n")
    html.append("   <label class='content' pageA='函数性能表' pageB='函数关系表' for='tag'></label>\n")
    html.append("</span>\n")
    return;


#定义向html文件追加头部
def _AppendHtmlTail(unitpath,html):
    HtmlPath = common.filePosition(unitpath)
    html.append("</body>\n\n")
    html.append("   <script type=\"text/javascript\" src=\"%shtml/js/buildPerformance.js\"></script>\n" % (HtmlPath))
    html.append("</html>\n")
    return;



#定义向html的FlatProfile表格追加头部
def _AppendFlatProfileHead(html):
    html.append("<div id='fun-performance' >\n")
    html.append("   <table rules=\"rows\">\n")
    html.append("   <tr>\n")
    html.append("       <th>函数消耗时间占程序所有时间百分比</th>\n")
    html.append("       <th>程序累积执行时间</th>\n")
    html.append("       <th>函数所有被调用次数的合共时间</th> \n")
    html.append("       <th>函数被调用次数</th>\n")
    html.append("       <th>函数单次执行时间(不包括被调用时间)</th> \n")
    html.append("       <th>函数单次执行时间(包括被调用时间)</th> \n")
    html.append("       <th>函数名</th>\n")
    html.append("   </tr>\n")
    return;


#定义向html的FlatProfile表格追加尾部
def _AppendFlatProfileTail(html):
    html.append("   </table>\n")
    html.append("</div>\n")
    return;

#解析一行表格
def _AppendALine(html,line):
    words=line.split()
    if 7 == len(words):
        html.append("   <tr><td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td></tr> \n" \
                                                        % (words[0], words[1], words[2], words[3], words[4], words[5], words[6]))
    else:
        html.append("   <tr><td>%s</td>  <td>%s</td>  <td>%s</td>  <td></td>  <td></td>  <td></td>  <td>%s</td></tr> \n" \
                                                                                      % (words[0], words[1], words[2], words[3]))
    return;


#定义解析FlatProfile文件函数
def _PraseFlatProfile(html, lines, offset):
    _AppendFlatProfileHead(html) #添加FlatProfile头部
    while "\f\n" != lines[offset]:
        if '\n' == lines[offset]:
            offset+=1;continue;
        _AppendALine(html, lines[offset])
        offset+=1

    _AppendFlatProfileTail(html) #添加FlatProfile尾部
    return offset;




#定义向html的CallGraph表格追加头部
def _AppendCallGraphHead(html):
    html.append("<div id='fun-relation'>\n\n")
    html.append("<table rules=\"rows\">\n")
    html.append("   <tr>\n")
    html.append("       <th>索引值</th>\n")
    html.append("       <th>函数消耗时间占程序所有时间百分比</th>\n")
    html.append("       <th>函数本身执行时间</th>\n")
    html.append("       <th>执行子函数所用时间</th>\n")
    html.append("       <th>函数被调用次数</th>\n")
    html.append("       <th>函数名</th>\n")
    html.append("   </tr>\n")
    return;




#定义向html的CallGraph表格追加尾部
def _AppendCallGraphTail(html):
    del html[len(html)-1-8:len(html)-1] #消除多余的table语句
    html.append("</div>\n\n")
    return;

#定义向html的CallGraph表格子表格追加头尾
def _AppendCallGraphSonTable(html):
    html.append("</table>\n\n")  
    html.append("<table rules=\"rows\">\n")
    html.append("   <tr>\n")
    html.append("       <th>索引值</th>\n")
    html.append("       <th>函数消耗时间占程序所有时间百分比</th>\n")
    html.append("       <th>函数本身执行时间</th>\n")
    html.append("       <th>执行子函数所用时间</th>\n")
    html.append("       <th>函数被调用次数</th>\n")
    html.append("       <th>函数名</th>\n")
    html.append("   </tr>\n")
    return;



#解析一行表格
def _AppendOneLine(html,line):
    words=line.split()
    if 5 == len(words): 
        html.append("<tr><td></td>  <td></td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s %s</td> </tr>\n" \
                                    % (words[0], words[1], words[2], words[3], words[4]))
    if 6 == len(words): 
        html.append("<tr><td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td></td>  <td>%s %s</td> </tr>\n" \
                                    % (words[0], words[1], words[2], words[3], words[4], words[5]))
    if 7 == len(words): 
        html.append("<tr><td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s</td>  <td>%s %s</td> </tr>\n" \
                                    % (words[0], words[1], words[2], words[3], words[4], words[5], words[5]))
    return;


#定义解析CallGraph文件函数
def _PraseCallGraph( html, lines, offset):
    _AppendCallGraphHead(html) #添加FlatProfile头部
    while "\f\n" != lines[offset]:
        if '\n' == lines[offset]:
            offset+=1;continue;
        if -1 != lines[offset].find("------------------------"):#子表格头部
            _AppendCallGraphSonTable(html)
        else:
            _AppendOneLine(html, lines[offset])
        offset+=1

    _AppendCallGraphTail(html) #添加FlatProfile尾部
    return offset;




#定义解析performance文件函数
def _ParseGprof(unitpath, gproffd, htmlfd):
    html=[]#创建html文本
    Offset = 0
    _AppendHtmlHead(unitpath, html)
    lines = gproffd.readlines()
    #解析文件-----------------------------------
    while len(lines) > Offset:
        #解析
        if -1 != lines[Offset].find("time   seconds   seconds"): #FlatProfile表开始位置
            Offset+=1
            Offset= _PraseFlatProfile(html, lines, Offset) #FlatProfile表格
        if -1 != lines[Offset].find("self  children    called     name"): #CallGraph表开始位置
            if -1 != lines[Offset].find("<spontaneous>"):
                Offset+=1
            Offset = _PraseCallGraph(html, lines, Offset) #CallGraph表格
        Offset+=1
    #解析完成----------------------------------
    _AppendHtmlTail(unitpath, html)  
    htmlfd.writelines(html) #生成html文件
    del html,Offset, lines
    return;

#定义生成性能文件函数
def Performance(unitpath, resultdir, gprof):
    result = os.system(gprof + " -b test " + "./gmon.out > " + performancceFile) #生成性能文件
    if 0 != result:
        print "没有找到gprof命令(gprof command not found)"
        return;
    #打开和创建所需文件
    GprofFd = open(performancceFile, "r" )
    HtmlFd  = open(resultdir + "/" + "performance.html" , "w+" )
    _ParseGprof(unitpath, GprofFd, HtmlFd) #解析文件
    print "完成性能分析！(parse the performance success!)"
    del result, GprofFd, HtmlFd
    return;


