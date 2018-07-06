/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file:  platform.h
@brief: 平台兼容头文件为了兼容各个平台代码
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2016/03/02  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_PLATFORM_H__
#define __INC_PLATFORM_H__
#include <stdio.h>
  
#if defined(__APPLE__) && defined(__GNUC__)   
    #define  OS_MACX   //MACX   - Mac OS X 是苹果公司为Mac系列产品开发的专属操作系统
#elif defined(__MACOSX__)   
    #define  OS_MACX   //MACX   - Mac OS X 是苹果公司为Mac系列产品开发的专属操作系统
#elif defined(macintosh)   
    #define  OS_MAC9   //MAC9   - Mac OS 9 苹果Classic Mac OS系列的最后一个主版本
#elif defined(__CYGWIN__)   
    #define  OS_CYGWIN  //CYGWIN - Cygwin Cygwin是一个在windows平台上运行的类UNIX模拟环境 
#elif defined(MSDOS) || defined(_MSDOS)   
    #define  OS_MSDOS  //MSDOS  - MS-DOS and Windows  微软磁盘操作系统
#elif defined(__OS2__)   
    #if defined(__EMX__)   
        #define  OS_OS2EMX  //OS2EMX - XFree86 on OS/2 (not PM) XFree86是 X Window系统的一个实现。最初是为 IBM PC兼容机上的 Unix类 OS 开发
    #else   
        #define  OS_OS2   //OS2 - OS/2  OS/2是由微软和IBM公司共同创造,后来由IBM单独开发的一套操作系统
    #endif   
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))   
    #define  OS_WIN32   //WIN32  - Win32 (Windows 95/98/ME and Windows NT/2000/XP) Win32是指Microsoft Windows操作系统的32位环境
    #define  OS_WIN64   //WIN64 Win64是指Microsoft Windows操作系统的64位环境
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))   
    #define  OS_WIN32   //WIN32  - Win32 (Windows 95/98/ME and Windows NT/2000/XP) Win32是指Microsoft Windows操作系统的32位环境
#elif defined(__MWERKS__) && defined(__INTEL__)   
    #define  OS_WIN32   //WIN32  - Win32 (Windows 95/98/ME and Windows NT/2000/XP) Win32是指Microsoft Windows操作系统的32位环境
#elif defined(__sun) || defined(sun)   
    #define  OS_SOLARIS  //SOLARIS- Sun Solaris 是UNIX操作系统的衍生版本之一。
#elif defined(hpux) || defined(__hpux)   
    #define  OS_HPUX   //HPUX   - HP-UX 是惠普9000系列服务器的操作系统
#elif defined(__ultrix) || defined(ultrix)   
    #define  OS_ULTRIX  //ULTRIX - DEC Ultrix  ULTRIX-Ultrix 是基于 Berkeley 版本的 Unix 的一个版本
#elif defined(sinix)   
    #define  OS_RELIANT   //RELIANT- Reliant UNIX
#elif defined(__linux__) || defined(__linux)   
    #define  OS_LINUX   //LINUX  - Linux Linux是一套免费使用和自由传播的类Unix操作系统
#elif defined(__FreeBSD__)   
    #define  OS_FREEBSD //FREEBSD-  FreeBSD是一种类UNIX操作系统，是由经过BSD、386BSD和4.4BSD发展而来的Unix的一个重要分支
    #define  OS_BSD4    //BSD4   - Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统
#elif defined(__NetBSD__)   
    #define  OS_NETBSD //NETBSD - NetBSD NetBSD是一个免费的，具有高度移植性的 UNIX-like 操作系统，是现行可移植平台最多的操作系统
    #define  OS_BSD4   //BSD4   - Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统
#elif defined(__OpenBSD__)   
    #define  OS_OPENBSD//OPENBSD- OpenBSD  OpenBSD是一个多平台的，基于4.4BSD的类UNIX操作系统 
    #define  OS_BSD4   //BSD4   - Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统
#elif defined(__bsdi__)   
    #define  OS_BSDI   //BSDI   - BSD/OS BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统
    #define  OS_BSD4   //BSD4   - Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统
#elif defined(__sgi)   
    #define  OS_IRIX   //IRIX   - SGI Irix IRIX是由硅谷图形公司以System V与BSD延伸程序为基础所发展成的UNIX操作系统
#elif defined(__osf__)   
    #define  OS_OSF   //OSF    - HP Tru64 UNIX 开放软件基金/1(OSF/1)开放环境的UNIX操作系统
#elif defined(_AIX)   
    #define  OS_AIX   //AIX    - AIX 是IBM基于AT&T Unix System V开发的一套类UNIX操作系统
#elif defined(__Lynx__)   
    #define  OS_LYNX  //LYNX   - LynxOS LynxOS是由Lynx实时系统公司开发的操作系统
#elif defined(__GNU_HURD__)   
    #define  OS_HURD   //HURD   - GNU Hurd Hurd 都是一个现代的类 Unix 的内核
#elif defined(__DGUX__)   
    #define  OS_DGUX   //DGUX   - DG/UX
#elif defined(__QNXNTO__)   
    #define  OS_QNX6   //QNX6   - QNX RTP 6.1  
#elif defined(__QNX__)   
    #define  OS_QNX   //QNX    - QNX 是一种商用的遵从POSIX规范的类Unix实时操作系统，目标市场主要是面向嵌入式系统
#elif defined(_SEQUENT_)   
    #define  OS_DYNIX   //DYNIX  - DYNIX/ptx 
#elif defined(_SCO_DS)                   /* SCO OpenServer 5 + GCC */   
    #define  OS_SCO   //SCO    - SCO OpenServer 5 
#elif defined(__USLC__)                  /* all SCO platforms + UDK or OUDK */   
    #define  OS_UNIXWARE  //UNIXWARE- UnixWare 7, Open UNIX 8  UnixWare 7是一种先进的网络操作系统
    #define  OS_UNIXWARE7   
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */   
    #define  OS_UNIXWARE  //UNIXWARE- UnixWare 7, Open UNIX 8  UnixWare 7是一种先进的网络操作系统
    #define  OS_UNIXWARE7   
#else   
    #error "My program couldn't support the OS"
#endif   
  
#if defined( OS_MAC9) || defined( OS_MACX)   
    #define  OS_MAC   
#endif   
  
#if defined( OS_MAC9) || defined( OS_MSDOS) || defined( OS_OS2) || defined( OS_WIN32) || defined( OS_WIN64)   
    #undef  OS_UNIX   
#elif !defined( OS_UNIX)   
    #define  OS_UNIX   //UNIX   - Any UNIX BSD/SYSV system
#endif  


/**  @brief 打印当前系统*/
#if defined( OS_MACX)
    #define PRINT_OS()  printf(" The system was Mac OS X 是苹果公司为Mac系列产品开发的专属操作系统\n")
#elif defined( OS_MAC9) 
    #define PRINT_OS()  printf(" The system was Mac OS 9 苹果Classic Mac OS系列的最后一个主版本\n")
#elif defined( OS_CYGWIN) 
    #define PRINT_OS()  printf(" The system was Cygwin Cygwin是一个在windows平台上运行的类UNIX模拟环境 \n")
#elif defined( OS_MSDOS) 
    #define PRINT_OS()  printf(" The system was MS-DOS and Windows  微软磁盘操作系统 \n")
#elif defined( OS_OS2EMX) 
    #define PRINT_OS()  printf(" The system was XFree86 on OS/2 (not PM) XFree86是 X Window系统的一个实现。最初是为 IBM PC兼容机上的 Unix类 OS 开发\n")
#elif defined( OS_OS2) 
    #define PRINT_OS()  printf(" The system was OS/2是由微软和IBM公司共同创造,后来由IBM单独开发的一套操作系统\n")
#elif defined( OS_WIN32) 
    #define PRINT_OS()  printf(" The system was WIN32 Win32是指Microsoft Windows操作系统的32位环境\n")
#elif defined( OS_WIN64) 
    #define PRINT_OS()  printf(" The system was WIN32 Win64是指Microsoft Windows操作系统的64位环境\n")
#elif defined( OS_SOLARIS) 
    #define PRINT_OS()  printf(" The system was Sun Solaris 是UNIX操作系统的衍生版本之一。\n")
#elif defined( OS_HPUX) 
    #define PRINT_OS()  printf(" The system was HP-UX 是惠普9000系列服务器的操作系统\n")
#elif defined( OS_ULTRIX) 
    #define PRINT_OS()  printf(" The system was HP-UX ULTRIX-Ultrix 是基于 Berkeley 版本的 Unix 的一个版本\n")
#elif defined( OS_RELIANT) 
    #define PRINT_OS()  printf(" The system was Reliant UNIX\n")
#elif defined( OS_LINUX) 
    #define PRINT_OS()  printf(" The system was Linux 是一套免费使用和自由传播的类Unix操作系统\n")
#elif defined( OS_IRIX)
    #define PRINT_OS()  printf(" The system was IRIX 是 由硅谷图形公司以System V与BSD延伸程序为基础所发展成的UNIX操作系统\n")
#elif defined( OS_OSF)
    #define PRINT_OS()  printf(" The system was HP Tru64 UNIX 开放软件基金/1(OSF/1)开放环境的UNIX操作系统\n")
#elif defined( OS_AIX)
    #define PRINT_OS()  printf(" The system was AIX 是 IBM基于AT&T Unix System V开发的一套类UNIX操作系统\n")
#elif defined( OS_LYNX)
    #define PRINT_OS()  printf(" The system was LynxOS 是 由Lynx实时系统公司开发的操作系统\n")
#elif defined( OS_HURD)
    #define PRINT_OS()  printf(" The system was GNU Hurd Hurd 都 是一个现代的类 Unix 的内核\n")
#elif defined( OS_QNX6)
    #define PRINT_OS()  printf(" The system was  QNX RTP 6.1 \n")
#elif defined( OS_QNX)
    #define PRINT_OS()  printf(" The system was QNX 是 一种商用的遵从POSIX规范的类Unix实时操作系统，目标市场主要是面向嵌入式系统\n")  
#elif defined( OS_DYNIX)
    #define PRINT_OS()  printf(" The system was DYNIX/ptx \n")  
#elif defined( OS_SCO)
    #define PRINT_OS()  printf(" The system was SCO OpenServer 5 \n")
#elif defined( OS_UNIXWARE) && defined( OS_UNIXWARE7)
    #define PRINT_OS()  printf(" The system was UnixWare 7, Open UNIX 8  UnixWare 7是一种先进的网络操作系统\n")
#elif defined( OS_FREEBSD) && defined( OS_BSD4) 
    #define PRINT_OS()  printf(" The system was FreeBSD 是一种类UNIX操作系统，是由经过BSD、386BSD和4.4BSD发展而来的Unix的一个重要分支\n")\
                        printf(" The system was Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统\n")
#elif defined( OS_NETBSD) && defined( OS_BSD4) 
    #define PRINT_OS()  printf(" The system was NetBSD NetBSD是一个免费的，具有高度移植性的 UNIX-like 操作系统，是现行可移植平台最多的操作系统\n")\
                        printf(" The system was Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统\n")
#elif defined( OS_OPENBSD) && defined( OS_BSD4) 
    #define PRINT_OS()  printf(" The system was OpenBSD是一个多平台的，基于4.4BSD的类UNIX操作系统 \n")\
                        printf(" The system was Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统\n")
#elif defined( OS_BSDI) && defined( OS_BSD4) 
    #define PRINT_OS()  printf(" The system was BSD/OS BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统\n")\
                        printf(" The system was Any BSD 4.4 system BSD (Berkeley Software Distribution，伯克利软件套件)是Unix的衍生系统\n")
#elif !defined( OS_UNIX)   
    #define PRINT_OS()  printf(" The system was any UNIX 操作系统\n")
#endif 


#endif/*__INC_PLATFORM_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
