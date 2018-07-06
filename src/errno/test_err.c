/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_err.c
@brief: ��Ҫ�Ǵ���ģ����в��ԣ���ģ���ڵĺ����������ú��쳣���ý��в���
        ����ʱ�� ��Ҫ��
        #define RETURN_ERR( Errno, Return )\
        {\
            errno = Errno;\
            return ERR_FAIL;\
        }
        ��Ϊ��
        #define RETURN_ERR( Errno, Return )\
        {\
            errno = Errno;\
        }
        ��������澯warning: return makes pointer from integer without a cast
        �Լ�RETURN_ERR����������޷�ִ��
@attention gcc err.c test_err.c -o test -lpthread
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
******************************************************************************/
#include ".././util/util_inc.h"

void *child1 (void *arg)
{
    err_t *errinfo1;
    u32_t errCode;

    printf("-----------------------pthread1-----------------------------\n");
    RETURN_ERR(10, FUNC_NULL);
    printf("child1: err1=%d ErrNo%d : %s\n",THE_ERRNO, errno, err_strerr(errno));
    errinfo1 = err_get_info( errno );
    errCode = err_get_errno(errinfo1);
    if( NULL != errinfo1 )
    {
        printf("test err_get_info() -> ErrNo%d : %s\n", errCode, err_strerr(errCode));
    }
}

void *child2 (void *arg)
{
    err_t *errinfo2;
    u32_t errCode;

    printf("-----------------------pthread2------------------------------\n");
    RETURN_ERR(20, FUNC_NULL);
    printf("child2: err2=%d ErrNo%d : %s\n",THE_ERRNO , errno, err_strerr(errno));
    errinfo2 = err_get_info( errno );
    errCode = err_get_errno(errinfo2);
    if( NULL != errinfo2 )
    {
        printf("test err_get_info() -> ErrNo%d : %s\n", errCode, err_strerr(errCode));
    }
}

/**
* @brief ���Դ�����
*/
int main (void)
{
    int i;
    pthread_t tid1, tid2;//�߳�id

    for(i = 0; i < ERR_BUTT+1; i++ ){
        printf("ErrNo%d : %s\n", i, err_strerr(i));//���Ժ������óɹ����Լ���ERR_BUTT���ǲ鿴��������ʧ�ܵ����
    }
    //�����̲߳��Զ��߳��ȷ��ش��������
    pthread_create (&tid1, NULL, (void *)child1, NULL);
    pthread_create (&tid2, NULL, (void *)child2, NULL);
    pthread_join (tid1, NULL);
    pthread_join (tid2, NULL);
    return (0);
}
