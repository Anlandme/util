#!/usr/bin/python
# -*- coding: UTF-8 -*-
import common

Color =[1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0]#suite 颜色
ColorIndex = 0

#定义追加index文件头
def _AppendHtmlHead(unitpath, indexhtml):
    HtmlPath = common.filePosition(unitpath)
    indexhtml.append("<!DOCTYPE html>\n")
    indexhtml.append("<html>\n\n")
    indexhtml.append("<head>\n")
    indexhtml.append("    <meta charset=\"utf-8\">\n")
    indexhtml.append("    <title>单元测试报告</title>\n")
    indexhtml.append("    <link rel=\"stylesheet\" href=\"%shtml/css/style.css\">\n" % (HtmlPath))
    indexhtml.append("</head>\n\n")
    indexhtml.append("<body class=\"body\">\n")
    indexhtml.append("    <div id=\"page\" class=\"page\">\n")
    indexhtml.append("        <div id=\"panels\" class=\"panels\">\n")
    indexhtml.append("            <span class=\"panel color0\" onmousemove=\"MouseArrive(this)\" onmouseout=\"MouseLeave(this)\">\n")
    indexhtml.append("                <div class=\"panel-content\">\n")
    indexhtml.append("                    <h2>单</h2>\n")
    indexhtml.append("                    <h2>元</h2>\n")
    indexhtml.append("                    <h2>测</h2>\n")
    indexhtml.append("                    <h2>试</h2>\n")
    indexhtml.append("                    <h2>报</h2>\n")
    indexhtml.append("                    <h2>告</h2>\n")
    indexhtml.append("                    <a class=\"file\" href=\"./performance.html\">性能</br>报告</a>\n")
    indexhtml.append("                </div>\n")
    indexhtml.append("                <div class=\"panel-text\">\n")
    indexhtml.append("                    <h1>测试的结果</h1>\n")
    indexhtml.append("                    <h4>suite展示</h4>\n")
    indexhtml.append("                        <ul>测试的case数量</ul>\n")
    indexhtml.append("                        <ul>测试的case出错数量</ul>\n")
    indexhtml.append("                        <ul>一个suite测试时长</ul>\n")
    indexhtml.append("                        <ul>内存泄漏展示内存泄漏位置大小</ul>\n")
    indexhtml.append("                    <h4>case展示</h4>\n")
    indexhtml.append("                        <ul>case的成功结果</ul>\n")
    indexhtml.append("                        <ul>case失败结果，失败位置失败内容</ul>\n")
    indexhtml.append("                        <ul>case内打印结果</ul>\n")
    indexhtml.append("                    <h4>性能展示</h4>\n")
    indexhtml.append("                        <ul>函数运行时间</ul>\n")
    indexhtml.append("                        <ul>函数调用次数</ul>\n")
    indexhtml.append("                        <ul>函数调用关系</ul>\n")
    indexhtml.append("                    <h4>覆盖率展示</h4>\n")
    indexhtml.append("                        <ul>文件和文件内函数的语句覆盖率</ul>\n")
    indexhtml.append("                        <ul>文件和文件内函数的分支覆盖率</ul>\n")
    indexhtml.append("                        <ul>文件和文件内函数的条件覆盖率</ul>\n")
    indexhtml.append("                        <ul>文件和文件内函数的函数覆盖率</ul>\n")
    indexhtml.append("                </div>\n")
    indexhtml.append("            </span>\n")
    return;

#定义追加index文件尾
def _AppendHtmlTail(unitpath, indexhtml):
    HtmlPath = common.filePosition(unitpath)
    indexhtml.append("</div>\n")
    indexhtml.append("</div>\n\n")
    indexhtml.append("</body>\n")
    indexhtml.append("    <script type=\"text/javascript\" src=\"%shtml/js/buildBase.js\"></script>\n" % (HtmlPath))
    indexhtml.append("    <script type=\"text/javascript\" src=\"%shtml/js/buildScroll.js\"></script>\n" % (HtmlPath))
    indexhtml.append("    <script type=\"text/javascript\" src=\"%shtml/js/buildText.js\"></script>\n" % (HtmlPath))
    indexhtml.append("    <script type=\"text/javascript\" src=\"%shtml/js/buildCanvas.js\"></script>\n"  % (HtmlPath))
    indexhtml.append("</html>\n")
    return;

#定义解析结果
def _ParseResult(offset, lines, resultblock, suitehead):
    offset+=1
    resultblock.append("                <div class=\"panel-content\">\n")
    resultblock.append("                    <h1>%s</h1>\n" % suitehead[1]);
    lines[offset] = lines[offset].strip('\n') #消除换行符
    resultblock.append("                    <h5>%s</h5>\n" % (lines[offset])); offset+=1
    lines[offset] = lines[offset].strip('\n') #消除换行符
    resultblock.append("                    <h5>%s</h5>\n" % (lines[offset])); offset+=1
    lines[offset] = lines[offset].strip('\n') #消除换行符
    resultblock.append("                    <h5>%s</h5>\n" % (lines[offset])); offset+=1
    lines[offset] = lines[offset].strip('\n') #消除换行符
    resultblock.append("                    <h5>%s</h5>\n" % (lines[offset])); offset+=1
    return offset;





#定义解析打印部分
def _ParsePrint( offset, lines, caseblock):
    offset+=2 #消除print 修饰部分
    while -1 == lines[offset+1].find("**********自定义输出内容**********"): #到达底部
        lines[offset] = lines[offset].strip('\n') #消除换行符
        caseblock.append("                       <b>[ Print  ]</b>%s</br>\n" % (lines[offset]))
        offset+=1

    offset+=1 #消除print 修饰部分
    return offset;




#定义解析case
def _ParseCase(offset, lines, caseblock):
    TmpBlock = [] #临时使用块
    result = 0 #0表示case失败 0表示case成功
    head = lines[offset].split()
    true='√'
    false='X'

    offset+=1
    while '\n' != lines[offset]:
        lines[offset] = lines[offset].strip('\n') #消除换行符
        if -1 != lines[offset].find("[ OK     ]"):
            result=1 #结果执行为成功
        if -1 != lines[offset].find("**********自定义输出内容**********"): #分析打印部分
             offset = _ParsePrint( offset, lines, TmpBlock)
        else:
            word = lines[offset].split("]")
            TmpBlock.append("                       <b>%s]</b>%s</br>\n" % (word[0], word[1]))
        offset+=1

    if 1 == result:
        caseblock.append("                  <li>%s: %s %s</li>\n" % (head[1], head[2], true))
    else:
        caseblock.append("                  <li>%s: %s %s</li>\n" % (head[1], head[2], false))
    caseblock.append("                  <p>\n")
    caseblock.extend(TmpBlock)
    caseblock.append("                  </p>\n")
    
    del TmpBlock, result, head, word
    return offset;





#定义解析内存泄漏
def _ParseMemoryLeak(offset, lines, memleakblock):
    offset+=2 
    memleakblock.append("                    <table rules=\"rows\">\n")
    memleakblock.append("                        <tr><th>泄漏地址</th><th>泄漏大小</th></tr>\n")
    while '\n' != lines[offset]:
        word = lines[offset].split()
        if 3 == len(word):
            memleakblock.append("                    <tr><td>%s</td><td>%s</td></tr>\n" % (word[1], word[2]))
        offset+=1
    memleakblock.append("                    </table>\n")
    return offset;


#定义 将所有block组合成indexhtml函数
def _BuildIndexHtml(indexhtml, resultblock, memleakblock, caseblock):
    global ColorIndex  
    ColorIndex = (ColorIndex + 1)%len(Color) #计算color颜色
    indexhtml.append("          <span class=\"panel color%d\" onmousemove=\"MouseArrive(this)\" onmouseout=\"MouseLeave(this)\">\n" % (ColorIndex))
    indexhtml.extend(resultblock)
    indexhtml.extend(memleakblock)
    indexhtml.append("              </div>\n")
    indexhtml.append("              <div class=\"panel-text\">\n")
    indexhtml.append("                  <h1>测试的case项</h1>\n")
    indexhtml.extend(caseblock)
    indexhtml.append("              </div>\n")
    indexhtml.append("          </span>\n")
    return;



#定义解析suite部分
def _ParseSuite(indexhtml, offset, lines):
    ResultBlock = [] #存储结果块
    MemLeakBlock= [] #内存泄漏块
    CaseBlock   = [] #case块
    suiteHead = lines[offset].split()
    
    while -1 == lines[offset].find("GROUP  END"):#到达suite底部
        if -1 != lines[offset].find("==>==>==>=>==>==>"): #分析case
            offset = _ParseCase(offset, lines, CaseBlock, ) 
        offset+=1

    offset = _ParseResult(offset, lines, ResultBlock, suiteHead)
    if -1 != lines[offset].find("@@@@@@@@@@@@@"): #分析内存泄漏部分
        offset = _ParseMemoryLeak(offset, lines, MemLeakBlock)
    _BuildIndexHtml(indexhtml, ResultBlock, MemLeakBlock, CaseBlock)
    return offset;




#定义解析test.tmp文件
def _ParseTestTmp(testfd, indexhtml):
    Offset = 0
    lines = testfd.readlines()
    while len(lines) > Offset:
        if -1 != lines[Offset].find("GROUP START"): #CallGraph表开始位置
            Offset = _ParseSuite(indexhtml, Offset, lines)
        Offset+=1
    return;




#定义生成index文件函数
def Index(unitpath, resultdir, htmlcover):
    IndexHtml=[] #html 文件
    TestFd = open("./test.tmp" , "r" )
    HtmlFd = open(resultdir + "/index.html" , "w+" )
    #解析文件
    _AppendHtmlHead(unitpath, IndexHtml)
    _ParseTestTmp(TestFd, IndexHtml) #解析文件
    IndexHtml.extend(htmlcover) #加入覆盖率块
    _AppendHtmlTail(unitpath, IndexHtml)
    HtmlFd.writelines(IndexHtml) #生成html文件
    print "完成解析，并生成html文件到%s 目录！[Finish]" % (resultdir)
    del IndexHtml, TestFd, HtmlFd
    return;