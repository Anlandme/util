/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: err.c
@brief: 本模块主要是当出现错误时，等正确描述错因，为了方便可以自定义错误原因，可以在
        err_number上加入错误码并在Err_String对错误码进行描述。调用linux官方库函数或者
        自定义函数需要获得错误码时候，则将<errno.h>下的errno放入到异常，当自定义函数
        需要上报错误码时候，调用宏RETURN_ERR上报错误码，宏会将错误码放入到<errno.h>
        下的errno中，等待获取。
        err_strerr函数：支持官方glib库errno.h 文件下errno 错误码调用err_strerr函数
                        转换成错误描述，也可以使用err_strerr进行自定义错误码转换成自
                        定义错误描述字符串。
@attention： 本模块关联外部文件模块 util/utildef.h,utilinc.h
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/02  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "err.h"

/**  @brief 错误码对应的结构体数组*/
const static Err ErrText[] ={
      { SUCCESS            /*0*/,    "Success " },
      /*  错误码 */
      { ERR_EPERM          /*1*/,    "ERR_EPERM Operation not permitted 操作不允许" },
      { ERR_ENOENT         /*2*/,    "ERR_ENOENT No such file or directory 文件/路径不存在" },
      { ERR_ESRCH          /*3*/,    "ERR_ESRCH No such process 进程不存在" },
      { ERR_EINTR          /*4*/,    "ERR_EINTR Interrupted system call 中断的系统调用" },
      { ERR_EIO            /*5*/,    "ERR_EIO I/O error I/O错误" },
      { ERR_ENXIO          /*6*/,    "ERR_ENXIO No such device or address 设备/地址不存在" },
      { ERR_E2BIG          /*7*/,    "ERR_E2BIG Arg list too long 参数列表过长" },
      { ERR_ENOEXEC        /*8*/,    "ERR_ENOEXEC Exec format error 执行格式错误" },
      { ERR_EBADF          /*9*/,    "ERR_EBADF Bad file number 错误文件编号" },
      { ERR_ECHILD         /*10*/,   "ERR_ECHILD No child processes 子进程不存在" },
      { ERR_EAGAIN         /*11*/,   "ERR_EAGAIN Try again 重试" },
      { ERR_ENOMEM         /*12*/,   "ERR_ENOMEM Out of memory 内存不足" },
      { ERR_EACCES         /*13*/,   "ERR_EACCES Permission denied 无权限" },
      { ERR_EFAULT         /*14*/,   "ERR_EFAULT Bad address 地址错误" },
      { ERR_ENOTBLK        /*15*/,   "ERR_ENOTBLK Block device required 需要块设备" },
      { ERR_EBUSY          /*16*/,   "ERR_EBUSY Device or resource busy 设备或资源忙" },
      { ERR_EEXIST         /*17*/,   "ERR_EEXIST File exists 文件已存在" },
      { ERR_EXDEV          /*18*/,   "ERR_EXDEV Cross-device link 跨设备链路" },
      { ERR_ENODEV         /*19*/,   "ERR_ENODEV No such device 设备不存在" },
      { ERR_ENOTDIR        /*20*/,   "ERR_ENOTDIR Not a directory 路径不存在" },
      { ERR_EISDIR         /*21*/,   "ERR_EISDIR Is a directory 是路径" },
      { ERR_EINVAL         /*22*/,   "ERR_EINVAL Invalid argument 无效参数" },
      { ERR_ENFILE         /*23*/,   "ERR_ENFILE File table overflow 文件表溢出 " },
      { ERR_EMFILE         /*24*/,   "ERR_EMFILE Too many open files 打开的文件过多" },
      { ERR_ENOTTY         /*25*/,   "ERR_ENOTTY Not a typewriter 非打字机" },
      { ERR_ETXTBSY        /*26*/,   "ERR_ETXTBSY Text file busy 文本文件忙" },
      { ERR_EFBIG          /*27*/,   "ERR_EFBIG File too large 文件太大" },
      { ERR_ENOSPC         /*28*/,   "ERR_ENOSPC No space left on device 设备无空间" },
      { ERR_ESPIPE         /*29*/,   "ERR_ESPIPE Illegal seek 非法查询" },
      { ERR_EROFS          /*30*/,   "ERR_EROFS Read-only file system 只读文件系统" },
      { ERR_EMLINK         /*31*/,   "ERR_EMLINK Too many links 链接太多" },
      { ERR_EPIPE          /*32*/,   "ERR_EPIPE Broken pipe 管道破裂" },
      { ERR_EDOM           /*33*/,   "ERR_EDOM Math argument out of domain of func 参数超出函数域" },
      { ERR_ERANGE         /*34*/,   "ERR_ERANGE Math result not representable 结果无法表示" },
      { ERR_EDEADLK        /*35*/,   "ERR_EDEADLK Resource deadlock would occur 资源将发生死锁" },
      { ERR_ENAMETOOLONG   /*36*/,   "ERR_ENAMETOOLONG File name too long 文件名太长" },
      { ERR_ENOLCK         /*37*/,   "ERR_ENOLCK No record locks available 没有可用的记录锁" },
      { ERR_ENOSYS         /*38*/,   "ERR_ENOSYS Function not implemented 函数未实现" },
      { ERR_ENOTEMPTY      /*39*/,   "ERR_ENOTEMPTY Directory not empty 目录非空" },
      { ERR_ELOOP          /*40*/,   "ERR_ELOOP Too many symbolic links encountered 遇到太多符号链接" },
      { ERR_EWOULDBLOCK    /*41*/,   "ERR_EWOULDBLOCK Try again 重试" },
      { ERR_ENOMSG         /*42*/,   "ERR_ENOMSG No message of desired type 没有符合需求类型的消息" },
      { ERR_EIDRM          /*43*/,   "ERR_EIDRM Identifier removed 标识符已删除" },
      { ERR_ECHRNG         /*44*/,   "ERR_ECHRNG Channel number out of range 通道编号超出范围" },
      { ERR_EL2NSYNC       /*45*/,   "ERR_EL2NSYNC Level 2 not synchronized level2不同步" },
      { ERR_EL3HLT         /*46*/,   "ERR_EL3HLT Level 3 halted 3级停止" },
      { ERR_EL3RST         /*47*/,   "ERR_EL3RST Level 3 reset 3级重置" },
      { ERR_ELNRNG         /*48*/,   "ERR_ELNRNG Link number out of range 链接编号超出范围" },
      { ERR_EUNATCH        /*49*/,   "ERR_EUNATCH Protocol driver not attached 协议驱动程序没有连接" },
      { ERR_ENOCSI         /*50*/,   "ERR_ENOCSI No CSI structure available 没有可用的CSI结构" },
      { ERR_EL2HLT         /*51*/,   "ERR_EL2HLT Level 2 halted 2级停止" },
      { ERR_EBADE          /*52*/,   "ERR_EBADE Invalid exchange 无效交换" },
      { ERR_EBADR          /*53*/,   "ERR_EBADR Invalid request descriptor 无效请求描述" },
      { ERR_EXFULL         /*54*/,   "ERR_EXFULL Exchange full 交换完全" },
      { ERR_ENOANO         /*55*/,   "ERR_ENOANO No anode 无阳极" },
      { ERR_EBADRQC        /*56*/,   "ERR_EBADRQC Invalid request code 无效请求码" },
      { ERR_EBADSLT        /*57*/,   "ERR_EBADSLT Invalid slot 无效插槽" },
      { ERR_EDEADLOCK      /*58*/,   "ERR_EDEADLOCK EAGAIN  Operation would block 操作会阻塞" },
      { ERR_EBFONT         /*59*/,   "ERR_EBFONT Bad font file format 错误的字体文件格式" },
      { ERR_ENOSTR         /*60*/,   "ERR_ENOSTR Device not a stream 设备不是流" },
      { ERR_ENODATA        /*61*/,   "ERR_ENODATA No data available 无数据" },
      { ERR_ETIME          /*62*/,   "ERR_ETIME Timer expired 计时器到期" },
      { ERR_ENOSR          /*63*/,   "ERR_ENOSR Out of streams resources 流资源不足" },
      { ERR_ENONET         /*64*/,   "ERR_ENONET Machine is not on the network 机器不在网络上" },
      { ERR_ENOPKG         /*65*/,   "ERR_ENOPKG Package not installed 包未安装" },
      { ERR_EREMOTE        /*66*/,   "ERR_EREMOTE Object is remote 对象是远程" },
      { ERR_ENOLINK        /*67*/,   "ERR_ENOLINK Link has been severed 链接正在服务中" },
      { ERR_EADV           /*68*/,   "ERR_EADV Advertise error 广告错误" },
      { ERR_ESRMNT         /*69*/,   "ERR_ESRMNT Srmount error 源文件挂载错误" },
      { ERR_ECOMM          /*70*/,   "ERR_ECOMM Communication error on send 发送过程中通讯错误" },
      { ERR_EPROTO         /*71*/,   "ERR_EPROTO Protocol error 协议错误" },
      { ERR_EMULTIHOP      /*72*/,   "ERR_EMULTIHOP Multihop attempted 多跳尝试" },
      { ERR_EDOTDOT        /*73*/,   "ERR_EDOTDOT RFS specific error RFS特定错误" },
      { ERR_EBADMSG        /*74*/,   "ERR_EBADMSG Not a data message 不是数据类型消息" },
      { ERR_EOVERFLOW      /*75*/,   "ERR_EOVERFLOW Value too large for defined data type 对指定的数据类型来说值太大" },
      { ERR_ENOTUNIQ       /*76*/,   "ERR_ENOTUNIQ Name not unique on network 网络上名字不唯一" },
      { ERR_EBADFD         /*77*/,   "ERR_EBADFD File descriptor in bad state 文件描述符状态错误" },
      { ERR_EREMCHG        /*78*/,   "ERR_EREMCHG Remote address changed 远程地址改变" },
      { ERR_ELIBACC        /*79*/,   "ERR_ELIBACC Can not access a needed shared library 无法访问需要的共享库" },
      { ERR_ELIBBAD        /*80*/,   "ERR_ELIBBAD Accessing a corrupted shared library 访问损坏的共享库" },
      { ERR_ELIBSCN        /*81*/,   "ERR_ELIBSCN .lib section in a.out corrupted 库部分在a.out损坏" },
      { ERR_ELIBMAX        /*82*/,   "ERR_ELIBMAX Attempting to link in too many shared libraries 试图链接太多的共享库" },
      { ERR_ELIBEXEC       /*83*/,   "ERR_ELIBEXEC Cannot exec a shared library directly 不能直接运行共享库" },
      { ERR_EILSEQ         /*84*/,   "ERR_EILSEQ Illegal byte sequence 非法字节序" },
      { ERR_ERESTART       /*85*/,   "ERR_ERESTART Interrupted system call should be restarted 应重新启动被中断的系统调用" },
      { ERR_ESTRPIPE       /*86*/,   "ERR_ESTRPIPE Streams pipe error 流管错误" },
      { ERR_EUSERS         /*87*/,   "ERR_EUSERS Too many users 用户太多" },
      { ERR_ENOTSOCK       /*88*/,   "ERR_ENOTSOCK Socket operation on non-socket 在非套接字上进行套接字操作" },
      { ERR_EDESTADDRREQ   /*89*/,   "ERR_EDESTADDRREQ Destination address required 需要目的地址" },
      { ERR_EMSGSIZE       /*90*/,   "ERR_EMSGSIZE Message too long 消息太长" },
      { ERR_EPROTOTYPE     /*91*/,   "ERR_EPROTOTYPE Protocol wrong type for socket 错误协议类型" },
      { ERR_ENOPROTOOPT    /*92*/,   "ERR_ENOPROTOOPT Protocol not available 协议不可用" },
      { ERR_PROTONOSUPPORT /*93*/,   "ERR_PROTONOSUPPORT Protocol not supported 不支持协议" },
      { ERR_SOCKTNOSUPPORT /*94*/,   "ERR_SOCKTNOSUPPORT Socket type not supported 不支持套接字类型" },
      { ERR_EOPNOTSUPP     /*95*/,   "ERR_EOPNOTSUPP Operation not supported on transport endpoint 操作上不支持传输端点" },
      { ERR_EPFNOSUPPORT   /*96*/,   "ERR_EPFNOSUPPORT Protocol family not supported 不支持协议族" },
      { ERR_EAFNOSUPPORT   /*97*/,   "ERR_EAFNOSUPPORT Address family not supported by protocol 协议不支持地址群" },
      { ERR_EADDRINUSE     /*98*/,   "ERR_EADDRINUSE Address already in use 地址已被使用" },
      { ERR_EADDRNOTAVAIL  /*99*/,   "ERR_EADDRNOTAVAIL Cannot assign requested address 无法分配请求的地址" },
      { ERR_ENETDOWN       /*100*/,  "ERR_ENETDOWN Network is down 网络已关闭" },
      { ERR_ENETUNREACH    /*101*/,  "ERR_ENETUNREACH Network is unreachable 网络不可达" },
      { ERR_ENETRESET      /*102*/,  "ERR_ENETRESET Network dropped connection because of reset 网络由于复位断开连接" },
      { ERR_ECONNABORTED   /*103*/,  "ERR_ECONNABORTED Software caused connection abort 软件导致连接终止" },
      { ERR_ECONNRESET     /*104*/,  "ERR_ECONNRESET Connection reset by peer 连接被对方复位" },
      { ERR_ENOBUFS        /*105*/,  "ERR_ENOBUFS No buffer space available 没有可用的缓存空间" },
      { ERR_EISCONN        /*106*/,  "ERR_EISCONN Transport endpoint is already connected 传输端点已连接" },
      { ERR_ENOTCONN       /*107*/,  "ERR_ENOTCONN Transport endpoint is not connected 传输端点未连接" },
      { ERR_ESHUTDOWN      /*108*/,  "ERR_ESHUTDOWN Cannot send after transport endpoint shutdown 传输端点关闭后不能在发送" },
      { ERR_ETOOMANYREFS   /*109*/,  "ERR_ETOOMANYREFS Too many references: cannot splice 太多的引用：无法接合" },
      { ERR_ETIMEDOUT      /*110*/,  "ERR_ETIMEDOUT Connection timed out 连接超时" },
      { ERR_ECONNREFUSED   /*111*/,  "ERR_ECONNREFUSED Connection refused 连接被拒绝" },
      { ERR_EHOSTDOWN      /*112*/,  "ERR_EHOSTDOWN Host is down 主机已关闭" },
      { ERR_EHOSTUNREACH   /*113*/,  "ERR_EHOSTUNREACH No route to host 无法路由到主机" },
      { ERR_EALREADY       /*114*/,  "ERR_EALREADY Operation already in progress 操作已在进程中" },
      { ERR_EINPROGRESS    /*115*/,  "ERR_EINPROGRESS Operation now in progress 进程中正在进行的操作" },
      { ERR_ESTALE         /*116*/,  "ERR_ESTALE Stale NFS file handle " },
      { ERR_EUCLEAN        /*117*/,  "ERR_EUCLEAN Structure needs cleaning 结构需要清理" },
      { ERR_ENOTNAM        /*118*/,  "ERR_ENOTNAM Not a XENIX named type file 没有XENIX类型的文件" },
      { ERR_ENAVAIL        /*119*/,  "ERR_ENAVAIL No XENIX semaphores available 没有有效的XENIX信号量" },
      { ERR_EISNAM         /*120*/,  "ERR_EISNAM Is a named type file 有名类型文件" },
      { ERR_EREMOTEIO      /*121*/,  "ERR_EREMOTEIO Remote I/O error 远端io错误" },
      { ERR_EDQUOT         /*122*/,  "ERR_EDQUOT Quota exceeded 超出配额" },
      { ERR_ENOMEDIUM      /*123*/,  "ERR_ENOMEDIUM No medium found 没有发现介质" },
      { ERR_EMEDIUMTYPE    /*124*/,  "ERR_EMEDIUMTYPE Wrong medium type 错误的介质类型" },
      /* 不能修改前面的错误码，只能在后面重新添加错误码*/
      { ERR_LINE           /*125*/,  "ERR_LINE was useless 公共和自定义错误码分界用" },
      { ERR_ASSERT         /*200*/,  "ERR_ASSERT The assert was happened 产生断言" },
      { ERR_PARA           /*201*/,  "ERR_PARA The formal-parameter pointer was fault 错误的形参" },
      { ERR_VARIATE        /*202*/,  "ERR_VARIATE The variable was fault 错误的变量" },
      { ERR_USER           /*203*/,  "ERR_USER The errno detial was user-defined 自定义错误内容" },
      { ERR_BUTT                  ,  " Invalid Err Number 无效的错误码 " },
};

/**  @brief 错误码不对应*/
const static Err ErrNoNotEqual ={ ERR_BUTT,  "err_number number couldn't equal to ErrText error cede ,  err_number错误码与ErrText错误码不相等" };
/**  @brief 错误码表过大*/
const static Err ErrListOversize ={ ERR_BUTT,  "err_number list too big， may be create the segment fault,  err_number错误码表过大,可能致使ErrText产生段错误" };


/**
* @brief 返回错误码对应的字符串，字符串内容是错误原因描述(支持glib库errno.h 文件下errno 错误码转换)
* @param [in] number 错误码编号
* @return 返回错误码描述字符串
* @enum err_number
*/
string_t 
err_strerr( const u32_t number )
{
        s32_t offset = ERR_LINE + 1 + number - 200;//计算出ErrText数组内自定义的错误码偏移
        if( ERR_BUTT <= number )
                return ErrText[ERR_BUTT].ErrString;
        if( offset > 0 && ErrText[offset].ErrCode != number )
                return ErrNoNotEqual.ErrString;
        
        if( number > ERR_LINE)
                return ErrText[offset].ErrString;
        else
                return ErrText[number].ErrString;

}

/**
* @brief 返回错误码对应结构体
* @param [in] number 错误码编号
* @return 返回错误码结构体
* @enum err_number
*/
Err *
err_get_info( const u32_t number )
{
        if( ERR_BUTT <= number )
                return ( Err * )&ErrText[ERR_BUTT];
        //查看错误码表是否过大,ErrText使用时将可能产生段错误
        if( ERR_BUTT < (sizeof(ErrText) / sizeof(ErrText[0]) -1) )//-1为减去数组success
                return ( Err * )&ErrListOversize;
        //查看错误码跟错误码结构体数组内错误码不对应，将使得错误消息不准确
        if( ErrText[number].ErrCode != number )
                return ( Err * )&ErrNoNotEqual;

        return ( Err * )&ErrText[number];
}

/**
* @brief 返回错误码结构体对应错误码
* @param [in] err 错误码结构体
* @return 返回错误码
* @enum err_number
*/
int_t
err_get_errno( const Err *err )
{
        if( NULL == err ) return FUNC_FAIL;

        return err->ErrCode;
}
