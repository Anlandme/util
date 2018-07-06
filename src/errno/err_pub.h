/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: errpub.h
@brief:���ڴ����빫������ �����ṹ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/02  | 0.0.0.1   | D.C.P    | �����ļ�             
******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef __INC_ERRPUB_H__
#define __INC_ERRPUB_H__
/******************************************************************
*   Name��ͷ�ļ�
*   Desc����Ҫ�Ǳ��ⲿ���õı���������Ҫ
******************************************************************/
#include "err.h"
/******************************************************************
*   Name����������
*   Desc����Ҫ�Ǳ��ⲿ���õı���
******************************************************************/
/**  @brief ���������� */
typedef Err err_t;

/**  @brief �����붨�� */
enum err_number{
    SUCCESS            = 0,
    /* ������ */
    ERR_EPERM          = 1,       /**< Operation not permitted ����������*/
    ERR_ENOENT         = 2,       /**< No such file or directory �ļ�/·��������*/
    ERR_ESRCH          = 3,       /**< No such process ���̲�����*/
    ERR_EINTR          = 4,       /**< Interrupted system call �жϵ�ϵͳ����*/
    ERR_EIO            = 5,       /**< I/O error I/O����*/
    ERR_ENXIO          = 6,       /**< No such device or address �豸/��ַ������*/
    ERR_E2BIG          = 7,       /**< Arg list too long �����б����*/
    ERR_ENOEXEC        = 8,       /**< Exec format error ִ�и�ʽ����*/
    ERR_EBADF          = 9,       /**< Bad file number �����ļ����*/
    ERR_ECHILD         = 10,      /**< No child processes �ӽ��̲�����*/
    ERR_EAGAIN         = 11,      /**< Try again ����*/
    ERR_ENOMEM         = 12,      /**< Out of memory �ڴ治��*/
    ERR_EACCES         = 13,      /**< Permission denied ��Ȩ��*/
    ERR_EFAULT         = 14,      /**< Bad address ��ַ����*/
    ERR_ENOTBLK        = 15,      /**< Block device required ��Ҫ���豸*/
    ERR_EBUSY          = 16,      /**< Device or resource busy �豸����Դæ*/
    ERR_EEXIST         = 17,      /**< File exists �ļ��Ѵ���*/
    ERR_EXDEV          = 18,      /**< Cross-device link ���豸��·*/
    ERR_ENODEV         = 19,      /**< No such device �豸������*/
    ERR_ENOTDIR        = 20,      /**< Not a directory ·��������*/
    ERR_EISDIR         = 21,      /**< Is a directory ��·��*/
    ERR_EINVAL         = 22,      /**< Invalid argument ��Ч����*/
    ERR_ENFILE         = 23,      /**< File table overflow �ļ������ */
    ERR_EMFILE         = 24,      /**< Too many open files �򿪵��ļ�����*/
    ERR_ENOTTY         = 25,      /**< Not a typewriter �Ǵ��ֻ�*/
    ERR_ETXTBSY        = 26,      /**< Text file busy �ı��ļ�æ*/
    ERR_EFBIG          = 27,      /**< File too large �ļ�̫��*/
    ERR_ENOSPC         = 28,      /**< No space left on device �豸�޿ռ�*/
    ERR_ESPIPE         = 29,      /**< Illegal seek �Ƿ���ѯ*/
    ERR_EROFS          = 30,      /**< Read-only file system ֻ���ļ�ϵͳ*/
    ERR_EMLINK         = 31,      /**< Too many links ����̫��*/
    ERR_EPIPE          = 32,      /**< Broken pipe �ܵ�����*/
    ERR_EDOM           = 33,      /**< Math argument out of domain of func ��������������*/
    ERR_ERANGE         = 34,      /**< Math result not representable ����޷���ʾ*/
    ERR_EDEADLK        = 35,      /**< Resource deadlock would occur ��Դ����������*/
    ERR_ENAMETOOLONG   = 36,      /**< File name too long �ļ���̫��*/
    ERR_ENOLCK         = 37,      /**< No record locks available û�п��õļ�¼��*/
    ERR_ENOSYS         = 38,      /**< Function not implemented ����δʵ��*/
    ERR_ENOTEMPTY      = 39,      /**< Directory not empty Ŀ¼�ǿ�*/
    ERR_ELOOP          = 40,      /**< Too many symbolic links encountered ����̫���������*/
    ERR_EWOULDBLOCK    = 41,
    ERR_ENOMSG         = 42,      /**< No message of desired type û�з����������͵���Ϣ*/
    ERR_EIDRM          = 43,      /**< Identifier removed ��ʶ����ɾ��*/
    ERR_ECHRNG         = 44,      /**< Channel number out of range ͨ����ų�����Χ*/
    ERR_EL2NSYNC       = 45,      /**< Level 2 not synchronized level2��ͬ��*/
    ERR_EL3HLT         = 46,      /**< Level 3 halted 3��ֹͣ*/
    ERR_EL3RST         = 47,      /**< Level 3 reset 3������*/
    ERR_ELNRNG         = 48,      /**< Link number out of range ���ӱ�ų�����Χ*/
    ERR_EUNATCH        = 49,      /**< Protocol driver not attached Э����������û������*/
    ERR_ENOCSI         = 50,      /**< No CSI structure available û�п��õ�CSI�ṹ*/
    ERR_EL2HLT         = 51,      /**< Level 2 halted 2��ֹͣ*/
    ERR_EBADE          = 52,      /**< Invalid exchange ��Ч����*/
    ERR_EBADR          = 53,      /**< Invalid request descriptor ��Ч��������*/
    ERR_EXFULL         = 54,      /**< Exchange full ������ȫ*/
    ERR_ENOANO         = 55,      /**< No anode ������*/
    ERR_EBADRQC        = 56,      /**< Invalid request code ��Ч������*/
    ERR_EBADSLT        = 57,      /**< Invalid slot ��Ч���*/
    ERR_EDEADLOCK      = 58,      /**< EAGAIN  Operation would block ����������*/
    ERR_EBFONT         = 59,      /**< Bad font file format ����������ļ���ʽ*/
    ERR_ENOSTR         = 60,      /**< Device not a stream �豸������*/
    ERR_ENODATA        = 61,      /**< No data available ������*/
    ERR_ETIME          = 62,      /**< Timer expired ��ʱ������*/
    ERR_ENOSR          = 63,      /**< Out of streams resources ����Դ����*/
    ERR_ENONET         = 64,      /**< Machine is not on the network ��������������*/
    ERR_ENOPKG         = 65,      /**< Package not installed ��δ��װ*/
    ERR_EREMOTE        = 66,      /**< Object is remote ������Զ��*/
    ERR_ENOLINK        = 67,      /**< Link has been severed �������ڷ�����*/
    ERR_EADV           = 68,      /**< Advertise error ������*/
    ERR_ESRMNT         = 69,      /**< Srmount error Դ�ļ����ش���*/
    ERR_ECOMM          = 70,      /**< Communication error on send ���͹�����ͨѶ����*/
    ERR_EPROTO         = 71,      /**< Protocol error Э�����*/
    ERR_EMULTIHOP      = 72,      /**< Multihop attempted ��������*/
    ERR_EDOTDOT        = 73,      /**< RFS specific error RFS�ض�����*/
    ERR_EBADMSG        = 74,      /**< Not a data message ��������������Ϣ*/
    ERR_EOVERFLOW      = 75,      /**< Value too large for defined data type ��ָ��������������˵ֵ̫��*/
    ERR_ENOTUNIQ       = 76,      /**< Name not unique on network ���������ֲ�Ψһ*/
    ERR_EBADFD         = 77,      /**< File descriptor in bad state �ļ�������״̬����*/
    ERR_EREMCHG        = 78,      /**< Remote address changed Զ�̵�ַ�ı�*/
    ERR_ELIBACC        = 79,      /**< Can not access a needed shared library �޷�������Ҫ�Ĺ����*/
    ERR_ELIBBAD        = 80,      /**< Accessing a corrupted shared library �����𻵵Ĺ����*/
    ERR_ELIBSCN        = 81,      /**< .lib section in a.out corrupted �ⲿ����a.out��*/
    ERR_ELIBMAX        = 82,      /**< Attempting to link in too many shared libraries ��ͼ����̫��Ĺ����*/
    ERR_ELIBEXEC       = 83,      /**< Cannot exec a shared library directly ����ֱ�����й����*/
    ERR_EILSEQ         = 84,      /**< Illegal byte sequence �Ƿ��ֽ���*/
    ERR_ERESTART       = 85,      /**< Interrupted system call should be restarted Ӧ�����������жϵ�ϵͳ����*/
    ERR_ESTRPIPE       = 86,      /**< Streams pipe error ���ܴ���*/
    ERR_EUSERS         = 87,      /**< Too many users �û�̫��*/
    ERR_ENOTSOCK       = 88,      /**< Socket operation on non-socket �ڷ��׽����Ͻ����׽��ֲ���*/
    ERR_EDESTADDRREQ   = 89,      /**< Destination address required ��ҪĿ�ĵ�ַ*/
    ERR_EMSGSIZE       = 90,      /**< Message too long ��Ϣ̫��*/
    ERR_EPROTOTYPE     = 91,      /**< Protocol wrong type for socket ����Э������*/
    ERR_ENOPROTOOPT    = 92,      /**< Protocol not available Э�鲻����*/
    ERR_PROTONOSUPPORT = 93,      /**< Protocol not supported ��֧��Э��*/
    ERR_SOCKTNOSUPPORT = 94,      /**< Socket type not supported ��֧���׽�������*/
    ERR_EOPNOTSUPP     = 95,      /**< Operation not supported on transport endpoint �����ϲ�֧�ִ���˵�*/
    ERR_EPFNOSUPPORT   = 96,      /**< Protocol family not supported ��֧��Э����*/
    ERR_EAFNOSUPPORT   = 97,      /**< Address family not supported by protocol Э�鲻֧�ֵ�ַȺ*/
    ERR_EADDRINUSE     = 98,      /**< Address already in use ��ַ�ѱ�ʹ��*/
    ERR_EADDRNOTAVAIL  = 99,      /**< Cannot assign requested address �޷���������ĵ�ַ*/
    ERR_ENETDOWN       = 100,     /**< Network is down �����ѹر�*/
    ERR_ENETUNREACH    = 101,     /**< Network is unreachable ���粻�ɴ�*/
    ERR_ENETRESET      = 102,     /**< Network dropped connection because of reset �������ڸ�λ�Ͽ�����*/
    ERR_ECONNABORTED   = 103,     /**< Software caused connection abort �������������ֹ*/
    ERR_ECONNRESET     = 104,     /**< Connection reset by peer ���ӱ��Է���λ*/
    ERR_ENOBUFS        = 105,     /**< No buffer space available û�п��õĻ���ռ�*/
    ERR_EISCONN        = 106,     /**< Transport endpoint is already connected ����˵�������*/
    ERR_ENOTCONN       = 107,     /**< Transport endpoint is not connected ����˵�δ����*/
    ERR_ESHUTDOWN      = 108,     /**< Cannot send after transport endpoint shutdown ����˵�رպ����ڷ���*/
    ERR_ETOOMANYREFS   = 109,     /**< Too many references: cannot splice ̫������ã��޷��Ӻ�*/
    ERR_ETIMEDOUT      = 110,     /**< Connection timed out ���ӳ�ʱ*/
    ERR_ECONNREFUSED   = 111,     /**< Connection refused ���ӱ��ܾ�*/
    ERR_EHOSTDOWN      = 112,     /**< Host is down �����ѹر�*/
    ERR_EHOSTUNREACH   = 113,     /**< No route to host �޷�·�ɵ�����*/
    ERR_EALREADY       = 114,     /**< Operation already in progress �������ڽ�����*/
    ERR_EINPROGRESS    = 115,     /**< Operation now in progress ���������ڽ��еĲ���*/
    ERR_ESTALE         = 116,     /**< Stale NFS file handle */
    ERR_EUCLEAN        = 117,     /**< Structure needs cleaning �ṹ��Ҫ����*/
    ERR_ENOTNAM        = 118,     /**< Not a XENIX named type file û��XENIX���͵��ļ�*/
    ERR_ENAVAIL        = 119,     /**< No XENIX semaphores available û����Ч��XENIX�ź���*/
    ERR_EISNAM         = 120,     /**< Is a named type file ���������ļ�*/
    ERR_EREMOTEIO      = 121,     /**< Remote I/O error Զ��io����*/
    ERR_EDQUOT         = 122,     /**< Quota exceeded �������*/
    ERR_ENOMEDIUM      = 123,     /**< No medium found û�з��ֽ���*/
    ERR_EMEDIUMTYPE    = 124,     /**< Wrong medium type ����Ľ�������*/
    /*�����޸�ǰ��Ĵ����룬ֻ���ں���������Ӵ�����*/
    ERR_LINE           = 125,     /**< ERR_LINE was useless �������Զ��������ֽ���*/
    ERR_ASSERT         = 200,     /**< The assert was happened ��������*/
    ERR_PARA           = 201,     /**< The formal-parameter pointer was fault ������β�*/
    ERR_VARIATE        = 202,     /**< The variable was fault ����ı��� */
    ERR_USER           = 203,     /**< The errno detial was user-defined �Զ����������*/
    ERR_BUTT
};

/******************************************************************
*   Name�������ӿں�
*   Desc����Ҫ�Ǳ��ⲿ���õĺ�ӿڣ����ԣ������쳣
******************************************************************/
/**  
* @brief ���ش���� 
* @param [in] Errno ������
* @param [in] Return ����ERR_FAIL����ERR_NULL
* @attention glibc�Ⲣ����ʼ��errno��Ĭ��ΪEWOULDBLOCK or EAGAIN
*/
#define RETURN_ERR( Errno, Return ) _RETURN_ERR( Errno, Return )
/**  
* @brief ���ش���� 
* @param [in] Desc  ֱ�Ӵ�ӡ�Ĵ����Զ�������
* @param [in] Errno ������
* @param [in] Return ����ERR_FAIL����ERR_NULL
*/
#define RETURN_ERRDESC( Errno, Return, Desc...) _RETURN_ERRDESC( Errno, Return, ##Desc )

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL ���򷵻�FUNC_FAIL/FUNC_NULL
* @param [in] result ����ֵ
*/
#define CHECK_FF( result ) _CHECK_FF( result )
#define CHECK_NF( result ) _CHECK_NF( result )
#define CHECK_FN( result ) _CHECK_FN( result )
#define CHECK_NN( result ) _CHECK_NN( result )

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL ����goto ��ĳ��
* @param [in] result ����ֵ
* @param [in] Goto  goto���Ŀ�ĵ�
*/
#define CHECK_FG( result, Goto ) _CHECK_FG( result, Goto )
#define CHECK_NG( result, Goto ) _CHECK_NG( result, Goto )

/**  
* @brief �����Ƿ񷵻�FUNC_FAIL/FUNC_NULL �Ǽ�¼log
* @param [in] result ����ֵ
*/
#define CHECK_FL( result ) _CHECK_FL( result )
#define CHECK_NL( result ) _CHECK_NL( result )


/**  @brief ��ȡ�ϱ��Ĵ�����*/
#define THE_ERRNO (errno)

/******************************************************************
*   Name�����������ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ���ش������Ӧ���ַ������ַ��������Ǵ���ԭ������(֧��glib��errno.h �ļ���errno ������ת��)
* @param [in] number ��������
* @return ���ش����������ַ���
*/
string_t 
err_strerr( const u32_t number );

/**
* @brief ���ش������Ӧ�ṹ��
* @param [in] number ��������
* @return ���ش�����ṹ��
*/
err_t *
err_get_info( const u32_t number );

/**
* @brief ���ش�����ṹ���Ӧ������
* @param [in] err ������ṹ��
* @return ���ش�����
* @enum err_number
*/
int_t
err_get_errno( const err_t *err );

#endif/*__INC_ERRPUB_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
