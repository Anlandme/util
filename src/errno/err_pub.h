/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: errpub.h
@brief:对于错误码公共函数 公共结构体
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/02  | 0.0.0.1   | D.C.P    | 创建文件             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_ERRPUB_H__
#define __INC_ERRPUB_H__
/******************************************************************
*   Name：头文件
*   Desc：主要是被外部调用的变量函数需要
******************************************************************/
#include "err.h"
/******************************************************************
*   Name：公共变量
*   Desc：主要是被外部调用的变量
******************************************************************/
/**  @brief 错误码类型 */
typedef Err err_t;

/**  @brief 错误码定义 */
enum err_number{
    SUCCESS            = 0,
    /* 错误码 */
    ERR_EPERM          = 1,       /**< Operation not permitted 操作不允许*/
    ERR_ENOENT         = 2,       /**< No such file or directory 文件/路径不存在*/
    ERR_ESRCH          = 3,       /**< No such process 进程不存在*/
    ERR_EINTR          = 4,       /**< Interrupted system call 中断的系统调用*/
    ERR_EIO            = 5,       /**< I/O error I/O错误*/
    ERR_ENXIO          = 6,       /**< No such device or address 设备/地址不存在*/
    ERR_E2BIG          = 7,       /**< Arg list too long 参数列表过长*/
    ERR_ENOEXEC        = 8,       /**< Exec format error 执行格式错误*/
    ERR_EBADF          = 9,       /**< Bad file number 错误文件编号*/
    ERR_ECHILD         = 10,      /**< No child processes 子进程不存在*/
    ERR_EAGAIN         = 11,      /**< Try again 重试*/
    ERR_ENOMEM         = 12,      /**< Out of memory 内存不足*/
    ERR_EACCES         = 13,      /**< Permission denied 无权限*/
    ERR_EFAULT         = 14,      /**< Bad address 地址错误*/
    ERR_ENOTBLK        = 15,      /**< Block device required 需要块设备*/
    ERR_EBUSY          = 16,      /**< Device or resource busy 设备或资源忙*/
    ERR_EEXIST         = 17,      /**< File exists 文件已存在*/
    ERR_EXDEV          = 18,      /**< Cross-device link 跨设备链路*/
    ERR_ENODEV         = 19,      /**< No such device 设备不存在*/
    ERR_ENOTDIR        = 20,      /**< Not a directory 路径不存在*/
    ERR_EISDIR         = 21,      /**< Is a directory 是路径*/
    ERR_EINVAL         = 22,      /**< Invalid argument 无效参数*/
    ERR_ENFILE         = 23,      /**< File table overflow 文件表溢出 */
    ERR_EMFILE         = 24,      /**< Too many open files 打开的文件过多*/
    ERR_ENOTTY         = 25,      /**< Not a typewriter 非打字机*/
    ERR_ETXTBSY        = 26,      /**< Text file busy 文本文件忙*/
    ERR_EFBIG          = 27,      /**< File too large 文件太大*/
    ERR_ENOSPC         = 28,      /**< No space left on device 设备无空间*/
    ERR_ESPIPE         = 29,      /**< Illegal seek 非法查询*/
    ERR_EROFS          = 30,      /**< Read-only file system 只读文件系统*/
    ERR_EMLINK         = 31,      /**< Too many links 链接太多*/
    ERR_EPIPE          = 32,      /**< Broken pipe 管道破裂*/
    ERR_EDOM           = 33,      /**< Math argument out of domain of func 参数超出函数域*/
    ERR_ERANGE         = 34,      /**< Math result not representable 结果无法表示*/
    ERR_EDEADLK        = 35,      /**< Resource deadlock would occur 资源将发生死锁*/
    ERR_ENAMETOOLONG   = 36,      /**< File name too long 文件名太长*/
    ERR_ENOLCK         = 37,      /**< No record locks available 没有可用的记录锁*/
    ERR_ENOSYS         = 38,      /**< Function not implemented 函数未实现*/
    ERR_ENOTEMPTY      = 39,      /**< Directory not empty 目录非空*/
    ERR_ELOOP          = 40,      /**< Too many symbolic links encountered 遇到太多符号链接*/
    ERR_EWOULDBLOCK    = 41,
    ERR_ENOMSG         = 42,      /**< No message of desired type 没有符合需求类型的消息*/
    ERR_EIDRM          = 43,      /**< Identifier removed 标识符已删除*/
    ERR_ECHRNG         = 44,      /**< Channel number out of range 通道编号超出范围*/
    ERR_EL2NSYNC       = 45,      /**< Level 2 not synchronized level2不同步*/
    ERR_EL3HLT         = 46,      /**< Level 3 halted 3级停止*/
    ERR_EL3RST         = 47,      /**< Level 3 reset 3级重置*/
    ERR_ELNRNG         = 48,      /**< Link number out of range 链接编号超出范围*/
    ERR_EUNATCH        = 49,      /**< Protocol driver not attached 协议驱动程序没有连接*/
    ERR_ENOCSI         = 50,      /**< No CSI structure available 没有可用的CSI结构*/
    ERR_EL2HLT         = 51,      /**< Level 2 halted 2级停止*/
    ERR_EBADE          = 52,      /**< Invalid exchange 无效交换*/
    ERR_EBADR          = 53,      /**< Invalid request descriptor 无效请求描述*/
    ERR_EXFULL         = 54,      /**< Exchange full 交换完全*/
    ERR_ENOANO         = 55,      /**< No anode 无阳极*/
    ERR_EBADRQC        = 56,      /**< Invalid request code 无效请求码*/
    ERR_EBADSLT        = 57,      /**< Invalid slot 无效插槽*/
    ERR_EDEADLOCK      = 58,      /**< EAGAIN  Operation would block 操作会阻塞*/
    ERR_EBFONT         = 59,      /**< Bad font file format 错误的字体文件格式*/
    ERR_ENOSTR         = 60,      /**< Device not a stream 设备不是流*/
    ERR_ENODATA        = 61,      /**< No data available 无数据*/
    ERR_ETIME          = 62,      /**< Timer expired 计时器到期*/
    ERR_ENOSR          = 63,      /**< Out of streams resources 流资源不足*/
    ERR_ENONET         = 64,      /**< Machine is not on the network 机器不在网络上*/
    ERR_ENOPKG         = 65,      /**< Package not installed 包未安装*/
    ERR_EREMOTE        = 66,      /**< Object is remote 对象是远程*/
    ERR_ENOLINK        = 67,      /**< Link has been severed 链接正在服务中*/
    ERR_EADV           = 68,      /**< Advertise error 广告错误*/
    ERR_ESRMNT         = 69,      /**< Srmount error 源文件挂载错误*/
    ERR_ECOMM          = 70,      /**< Communication error on send 发送过程中通讯错误*/
    ERR_EPROTO         = 71,      /**< Protocol error 协议错误*/
    ERR_EMULTIHOP      = 72,      /**< Multihop attempted 多跳尝试*/
    ERR_EDOTDOT        = 73,      /**< RFS specific error RFS特定错误*/
    ERR_EBADMSG        = 74,      /**< Not a data message 不是数据类型消息*/
    ERR_EOVERFLOW      = 75,      /**< Value too large for defined data type 对指定的数据类型来说值太大*/
    ERR_ENOTUNIQ       = 76,      /**< Name not unique on network 网络上名字不唯一*/
    ERR_EBADFD         = 77,      /**< File descriptor in bad state 文件描述符状态错误*/
    ERR_EREMCHG        = 78,      /**< Remote address changed 远程地址改变*/
    ERR_ELIBACC        = 79,      /**< Can not access a needed shared library 无法访问需要的共享库*/
    ERR_ELIBBAD        = 80,      /**< Accessing a corrupted shared library 访问损坏的共享库*/
    ERR_ELIBSCN        = 81,      /**< .lib section in a.out corrupted 库部分在a.out损坏*/
    ERR_ELIBMAX        = 82,      /**< Attempting to link in too many shared libraries 试图链接太多的共享库*/
    ERR_ELIBEXEC       = 83,      /**< Cannot exec a shared library directly 不能直接运行共享库*/
    ERR_EILSEQ         = 84,      /**< Illegal byte sequence 非法字节序*/
    ERR_ERESTART       = 85,      /**< Interrupted system call should be restarted 应重新启动被中断的系统调用*/
    ERR_ESTRPIPE       = 86,      /**< Streams pipe error 流管错误*/
    ERR_EUSERS         = 87,      /**< Too many users 用户太多*/
    ERR_ENOTSOCK       = 88,      /**< Socket operation on non-socket 在非套接字上进行套接字操作*/
    ERR_EDESTADDRREQ   = 89,      /**< Destination address required 需要目的地址*/
    ERR_EMSGSIZE       = 90,      /**< Message too long 消息太长*/
    ERR_EPROTOTYPE     = 91,      /**< Protocol wrong type for socket 错误协议类型*/
    ERR_ENOPROTOOPT    = 92,      /**< Protocol not available 协议不可用*/
    ERR_PROTONOSUPPORT = 93,      /**< Protocol not supported 不支持协议*/
    ERR_SOCKTNOSUPPORT = 94,      /**< Socket type not supported 不支持套接字类型*/
    ERR_EOPNOTSUPP     = 95,      /**< Operation not supported on transport endpoint 操作上不支持传输端点*/
    ERR_EPFNOSUPPORT   = 96,      /**< Protocol family not supported 不支持协议族*/
    ERR_EAFNOSUPPORT   = 97,      /**< Address family not supported by protocol 协议不支持地址群*/
    ERR_EADDRINUSE     = 98,      /**< Address already in use 地址已被使用*/
    ERR_EADDRNOTAVAIL  = 99,      /**< Cannot assign requested address 无法分配请求的地址*/
    ERR_ENETDOWN       = 100,     /**< Network is down 网络已关闭*/
    ERR_ENETUNREACH    = 101,     /**< Network is unreachable 网络不可达*/
    ERR_ENETRESET      = 102,     /**< Network dropped connection because of reset 网络由于复位断开连接*/
    ERR_ECONNABORTED   = 103,     /**< Software caused connection abort 软件导致连接终止*/
    ERR_ECONNRESET     = 104,     /**< Connection reset by peer 连接被对方复位*/
    ERR_ENOBUFS        = 105,     /**< No buffer space available 没有可用的缓存空间*/
    ERR_EISCONN        = 106,     /**< Transport endpoint is already connected 传输端点已连接*/
    ERR_ENOTCONN       = 107,     /**< Transport endpoint is not connected 传输端点未连接*/
    ERR_ESHUTDOWN      = 108,     /**< Cannot send after transport endpoint shutdown 传输端点关闭后不能在发送*/
    ERR_ETOOMANYREFS   = 109,     /**< Too many references: cannot splice 太多的引用：无法接合*/
    ERR_ETIMEDOUT      = 110,     /**< Connection timed out 连接超时*/
    ERR_ECONNREFUSED   = 111,     /**< Connection refused 连接被拒绝*/
    ERR_EHOSTDOWN      = 112,     /**< Host is down 主机已关闭*/
    ERR_EHOSTUNREACH   = 113,     /**< No route to host 无法路由到主机*/
    ERR_EALREADY       = 114,     /**< Operation already in progress 操作已在进程中*/
    ERR_EINPROGRESS    = 115,     /**< Operation now in progress 进程中正在进行的操作*/
    ERR_ESTALE         = 116,     /**< Stale NFS file handle */
    ERR_EUCLEAN        = 117,     /**< Structure needs cleaning 结构需要清理*/
    ERR_ENOTNAM        = 118,     /**< Not a XENIX named type file 没有XENIX类型的文件*/
    ERR_ENAVAIL        = 119,     /**< No XENIX semaphores available 没有有效的XENIX信号量*/
    ERR_EISNAM         = 120,     /**< Is a named type file 有名类型文件*/
    ERR_EREMOTEIO      = 121,     /**< Remote I/O error 远端io错误*/
    ERR_EDQUOT         = 122,     /**< Quota exceeded 超出配额*/
    ERR_ENOMEDIUM      = 123,     /**< No medium found 没有发现介质*/
    ERR_EMEDIUMTYPE    = 124,     /**< Wrong medium type 错误的介质类型*/
    /*不能修改前面的错误码，只能在后面重新添加错误码*/
    ERR_LINE           = 125,     /**< ERR_LINE was useless 公共和自定义错误码分界用*/
    ERR_ASSERT         = 200,     /**< The assert was happened 产生断言*/
    ERR_PARA           = 201,     /**< The formal-parameter pointer was fault 错误的形参*/
    ERR_VARIATE        = 202,     /**< The variable was fault 错误的变量 */
    ERR_USER           = 203,     /**< The errno detial was user-defined 自定义错误内容*/
    ERR_BUTT
};

/******************************************************************
*   Name：公共接口宏
*   Desc：主要是被外部调用的宏接口，断言，各类异常
******************************************************************/
/**  
* @brief 返回错误宏 
* @param [in] Errno 错误码
* @param [in] Return 返回ERR_FAIL或者ERR_NULL
* @attention glibc库并不初始化errno，默认为EWOULDBLOCK or EAGAIN
*/
#define RETURN_ERR( Errno, Return ) _RETURN_ERR( Errno, Return )
/**  
* @brief 返回错误宏 
* @param [in] Desc  直接打印的错误自定义描述
* @param [in] Errno 错误码
* @param [in] Return 返回ERR_FAIL或者ERR_NULL
*/
#define RETURN_ERRDESC( Errno, Return, Desc...) _RETURN_ERRDESC( Errno, Return, ##Desc )

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是则返回FUNC_FAIL/FUNC_NULL
* @param [in] result 返回值
*/
#define CHECK_FF( result ) _CHECK_FF( result )
#define CHECK_NF( result ) _CHECK_NF( result )
#define CHECK_FN( result ) _CHECK_FN( result )
#define CHECK_NN( result ) _CHECK_NN( result )

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是则goto 到某处
* @param [in] result 返回值
* @param [in] Goto  goto语句目的地
*/
#define CHECK_FG( result, Goto ) _CHECK_FG( result, Goto )
#define CHECK_NG( result, Goto ) _CHECK_NG( result, Goto )

/**  
* @brief 返回是否返回FUNC_FAIL/FUNC_NULL 是记录log
* @param [in] result 返回值
*/
#define CHECK_FL( result ) _CHECK_FL( result )
#define CHECK_NL( result ) _CHECK_NL( result )


/**  @brief 获取上报的错误码*/
#define THE_ERRNO (errno)

/******************************************************************
*   Name：公共函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 返回错误码对应的字符串，字符串内容是错误原因描述(支持glib库errno.h 文件下errno 错误码转换)
* @param [in] number 错误码编号
* @return 返回错误码描述字符串
*/
string_t 
err_strerr( const u32_t number );

/**
* @brief 返回错误码对应结构体
* @param [in] number 错误码编号
* @return 返回错误码结构体
*/
err_t *
err_get_info( const u32_t number );

/**
* @brief 返回错误码结构体对应错误码
* @param [in] err 错误码结构体
* @return 返回错误码
* @enum err_number
*/
int_t
err_get_errno( const err_t *err );

#endif/*__INC_ERRPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
