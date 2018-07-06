/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thrad.h
@brief:  halloc �����õ��ĺ�ͽṹ��
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_HALLOC_H__
#define __INC_HALLOC_H__

/**  @brief �ɹ����� */
#define HALLOC_SUCC FUNC_SUCC
/**  @brief ʧ�ܷ��� */
#define HALLOC_FAIL FUNC_FAIL
/**  @brief ʧ�ܷ��� */
#define HALLOC_NULL FUNC_NULL
/**  @brief KB��С */
#define KB 1024
/**  @brief halloc log���� */
#define halloc_log printf
/**  @brief У���� */
#define CHECKCODE 0xabcd
/**  @brief ��ʱ��ʱ��*/
#define SUPERVISOR_TIME 5
/**  @brief �ڴ�λ�� ָ��һ��λ�����ֽ�*/ 
#define MEM_OFFSET(addr, offset) (char *)(addr)+offset

/**  @brief ҳ�ṹ�� */
typedef struct Page page_t;
struct Page{
    /* �������� */
    uint8_t brotherflag;  /**< ����־ */
    uint8_t usedflag;     /**< �ڴ�ҳ��ʹ�ñ�־ */
    uint8_t mallocpage;   /**< malloc����ҳ */
    list_node_t pageNode; /**< ҳ�ڵ� */
    void *unusedAddr;     /**< ָ��δ��ʹ�õĵ�ַ*/
    void *headAddr;       /**< ��ַͷ */
    /* ϵͳ��ʹ�ò��� */
    int_t pageSize;          /**< ֽ�Ŵ�С*/
    int_t unusedSize;        /**< δ��ʹ�õĴ�С ��λ�ֽ�*/
    page_t *brother;         /**< ��һ�Ųü�������ֵ�ֽ�� */
    list_node_t destroyNode; /**< ���ٽڵ� �����ڴ��ʹ��*/
    /* ��Դ�����ʹ�ò��� */
    uint_t maxTime;         /**< ��ǰռ��ʱ������ڴ��ʱ��ֵ */
    list_head_t blocksList; /**< �ڴ������ */
    uint_t freeBlocks;      /**< �����ڴ������ */
    thread_mutex_t lock;    /**< ҳ�� */
};

/**  @brief �ڴ��ṹ�� */
typedef struct{
    /* �������� */
    uint_t blockSize;      /**< �ڴ���С */
    void *headAddr;        /**< ��ַͷ */
    uint16_t lease;        /**< �ڴ������ʱ�� �����㷨ʹ��*/
    int8_t color;          /**< ��ɫ-�ȴ������� ��ɫ-�����ʹ�� ��ɫ-�ⲿģ��ʹ�� ��ɫ-����ʹ��*/
    int8_t type;           /**< �ڴ������*/
    list_node_t blockNode; /**< ҳ�ڵ� λ���������� */
    page_t *page;          /**< �������ڴ�ҳ */
    /* ��Դ�����ʹ�ò��� */
    list_node_t node; /**< ҳ�ڵ� λ��blocksList�ڴ������*/
    /* ��Դ�����ʹ�ò��� */
    uint16_t module;       /**< ģ�� */
    uint16_t line;         /**< �к� */
    string_t file;         /**< �ļ��� */
    string_t func;         /**< ������ */
    /* �ڴ�й©ʹ�� */
    uint8_t checkSign;     /**< й©����� */
    list_node_t blackNode; /**< �������ڵ� */
} block_t;


/**  @brief �ڴ�У����Ϣ�ṹ�� */
typedef struct{
    u16_t headcheck; /**< У���� Ĭ��CHECKCODE */
    block_t *block;  /**< ָ���ڴ��block */
    u16_t tailcheck; /**< У���� Ĭ��CHECKCODE */
} checkinfo_t;


/**  @brief ʹ�ñ�־*/
enum init_flag{
    INITIALIZED = 1, /**< �ѳ�ʼ��*/
    NONINIT     = 2, /**< δ��ʼ�� */
};

/**  @brief ʹ�ñ�־*/
enum mem_Info{
    MLAYER_STATIC_MEM = 1,      /**< ��̬�ڴ�*/
    MLAYER_STATIC_FREEMEM,      /**< ��̬�����ڴ�*/
    MLAYER_THREADSTACK_MEM,     /**< �߳�ջʹ���ڴ�*/
    MLAYER_THREADSTACK_FREEMEM, /**< �߳�ջ�����ڴ� */
    ALAYER_STATIC_MEM,          /**< ��̬�ڴ� */
    ALAYER_STACK_MEM,           /**< �߳�ջ�ڴ� */
    ALAYER_DYNAMIC_MEM,         /**< ��̬�ڴ� */
};
/******************************************************************
*   Name���������ղ���ؽṹ��
*   Desc��ϵͳ���н���ʹ���ⲿ�ֽṹ��
******************************************************************/
/**  @brief �����߳� */
#define STOP_THREAD SIGPWR
/**  @brief gc����*/
typedef struct{
    uint8_t GCInit : 1;          /**< GC��ʼ��*/
    uint8_t GCStart : 1;         /**< ��ʼGC*/
    uint8_t readySign : 1;       /**< ׼����־*/
    uint8_t wakeupSign : 1;      /**< ���ѱ�־*/
    uint16_t totalThread : 14;   /**< ���߳��� */
    uint16_t currentThread : 14; /**< ��ǰ�߳��� */
    uint8_t timerOpen;           /**< ��ʱ���� */
    Timer_t supervisor;          /**< ���Ա */
    list_head_t blackList;       /**< ������ */
    void *hallocHeadAddr;        /**< halloc�׵�ַ */
    void *hallocTailAddr;        /**< hallocβ��ַ */
    void *startData;             /**< �������ݶ���ʼ��ַ */
    void *endData;               /**< ���ݶ�β����ַ */
    void *stackbottom;           /**< ������ջ�׵�ַ */
    pid_t mainPid;               /**< ������id */
    thread_cond_t wakeup;        /**< ������������ */
    thread_mutex_t lock;         /**< ������ */
    thread_mutex_t listLock;     /**< ������ */
}gc_t;
/******************************************************************
*   Name���ڴ�������ؽṹ��
*   Desc��ϵͳ���н���ʹ���ⲿ�ֽṹ��
******************************************************************/
/**  @brief sizeת��*/
#define SWITCH_TO_KB(size) (size%KB >0?size/KB+1:size/KB)
/**  @brief �����ڴ��Сת��Ϊ�ڴ���С */
#define SWITCH_TO_BLOCKSIZE(size) (size+sizeof(checkinfo_t))
/**  @brief ����������ʱ�� Ĭ��1���� */
#define LEASE_TIME (1*60/SUPERVISOR_TIME)
/**  @brief ����ڴ��ַ */
#define get_malloc_addr(addr) MEM_OFFSET(addr,sizeof(checkinfo_t))

/**  @brief ����*/
typedef struct{
    int_t memSize;               /**< �ڴ��С */
    int_t freeMemSize;           /**< �����ڴ��С */
    uint_t returnBlocks;         /**< �黹�ڴ������ */
    uint8_t layer;               /**< ���ܲ� */
    uint16_t giveBack;           /**< �黹���� �����㷨ʹ�� */
    uint8_t applyLevel;          /**< ����ȼ� �����㷨ʹ�� */
    stack_head_t freegood;       /**< ���л��� */
    stack_head_t returngood;     /**< �黹���� */
    pthread_mutex_t returnLock;  /**< �黹�ڴ���� */
    pthread_mutex_t freeLock;    /**< �����ڴ���� */
}shelf_t;

/**  @brief ģ���ڴ����*/
typedef struct{
    uint16_t module;            /**< ģ�� */
    uint_t dynamicMem;          /**< ��̬�ڴ��С */
    uint_t staticMem;           /**< ��̬�ڴ��С */
    uint_t threadMem;           /**< �߳��ڴ��С kb*/
    pthread_mutex_t moduleLock; /**< ģ���� */
    list_head_t staticList;     /**< ��̬����*/
    list_head_t threadList;     /**< �߳�ջ����*/
    shelf_t small[6];           /**< small���� */
    shelf_t middle[6];          /**< middle���� */
    shelf_t big[6];             /**< big���� */
    shelf_t huge;               /**< huge���� */
}moduleMem_t;

/**  @brief ģ���*/
typedef struct{
    Timer_t supervisor;           /**< ���Ա �����㷨ʹ�� */
    moduleMem_t memory[MOD_BUTT]; /**< ģ���ڴ�� */
} modulePool_t;

/**  @brief �ڴ������ */
enum MemType{
  MEM_NONE,         /**< ������ */
  MEM_DYNAMIC,      /**< ��̬�ڴ� */
  MEM_STATIC,       /**< ��̬�ڴ� */
  MEM_THREAD_STACK, /**< �߳�ջ�ڴ� */
};
/******************************************************************
*   Name���ڴ�������ؽṹ��
*   Desc��ϵͳ���н���ʹ���ⲿ�ֽṹ��
******************************************************************/
/**  @brief huge�ֿ��ڴ��ͷ���ֵ Ĭ��10����*/
#define RELEASE_HUGE_THRESH (10*60/SUPERVISOR_TIME)
/**  @brief й©���ʱ�� 2Сʱ*/
#define LEAK_CHECK (2*60*60/SUPERVISOR_TIME)
/**  @brief ����ڴ�Խ��*/
#define MEMISOVERFLOW(checkinfo) (checkinfo->headcheck != CHECKCODE || checkinfo->tailcheck != CHECKCODE)
/**  @brief ��̬�ڴ�ҳ */
#define STATIC_PAGE 4*1024

    /**  @brief �ֺŽṹ�� */
typedef struct{
    int_t memSize;              /**< �ڴ��С */
    int_t freeMemSize;          /**< �����ڴ��С */
    uint8_t storageType;        /**< �ֿ����� */
    uint8_t timerOpen;          /**< ��ʱ���� */
    stack_head_t freegood;      /**< ���л��� */
    list_head_t goodsArea;      /**< �ֿ���� */
    Timer_t supervisor;         /**< ���Ա �����㷨ʹ�� */
    pthread_mutex_t blocksLock; /**< �ڴ���� */
}storage_t;

/**  @brief ��̬�ڴ�ṹ�� */
typedef struct{
    int_t memSize;               /**< �ڴ��С */
    int_t freeMemSize;           /**< �����ڴ��С */
    int_t threadMem;             /**< �߳��ڴ��С kb*/
    int_t threadFreemem;         /**< �߳������ڴ��С kb*/
    uint8_t staticInit;          /**< ��ʼ����־ */
    list_head_t pagesList;       /**< ҳ���� ÿ��ҳ�̶�4k*/
    list_head_t threadList;      /**< �߳�ջ����*/
    list_head_t freeblockList;   /**< �����ڴ������*/
    list_head_t freethreadList;  /**< �����߳�ջ����*/
    Timer_t supervisor;          /**< ���Ա ����ڴ�Խ��� */
    pthread_mutex_t staticLock;  /**< ��̬�ڴ��� */
    pthread_mutex_t threadLock;  /**< �߳��ڴ��� */
} staticmem_t;

/**  @brief ��̬�ڴ�ṹ�� */
typedef struct{
    int_t memSize;               /**< �ڴ��С 1k��������*/
    int_t freeMemSize;           /**< �����ڴ��С */
    uint8_t dynamicInit;         /**< ��ʼ����־ */
    uint8_t timerSum;            /**< ��ʱ������ */
    storage_t small[6];          /**< small�ֿ� */
    storage_t middle[6];         /**< middle�ֿ�  */
    storage_t big[6];            /**< big�ֿ�  */
    storage_t huge;              /**< huge�ֿ�  */
    pthread_mutex_t dynamicLock; /**< ��̬�ڴ��� */
}dynamicmem_t;

/**  @brief �ڴ����ɫ*/
enum color_type{
    BLOCK_WHITE  = 0,  /**< ��ɫ-����ʹ��  */
    BLOCK_GREEN  = 1,  /**< ��ɫ-�ȴ������� */
    BLOCK_YELLOW = 2,  /**< ��ɫ-�����ʹ�� */
    BLOCK_RED    = 3,  /**< ��ɫ-�ⲿģ��ʹ��*/
};

/**  @brief �ڴ����Ϣ */
enum block_info{
    BLOCK_STORAGE = 0,      /**< �ڴ��ֺ� */
    BLOCK_SIZE    = 1,      /**< �ڴ���С */
    BLOCK_MSIZE   = 2,      /**< �ڴ��������malloc��С*/
    PGAE_BSUM     = 3,      /**< һ���ڴ�ҳ���ڴ������ */
    PGAE_SIZE     = 4,      /**< �ڴ�ҳ��С*/
};

/**  @brief �ڴ����Ϣ */
enum collect_user{
    SUPERVISOR = 0,      /**< ����Ա */
    GUEST      = 1,      /**< ���� */
};
/******************************************************************
*   Name��ϵͳ����ؽṹ��
*   Desc��ϵͳ���н���ʹ���ⲿ�ֽṹ��
******************************************************************/
/**  @brief ҳ���С ����Ϊ14kb������Դ�����ʹ�õó���ֵ */
#define PAGE_TABLE_SIZE 15
/**  @brief ҳ����� 100mb*/
#define PAGE_MAX_SIZE 100*1024
/**  @brief �ڴ����� 64*/
#define BLOCK_MAX_SIZE 64
/**  @brief pabջ��� 64*/
#define PAB_STACK_SIZE 8*1024

/**  @brief page_t and block_t */
typedef struct{
    list_head_t PABStackList;   /**< page_t and block_t ջ���� */
    stack_head_t freePageStack; /**< ���� page_t ջ */
    stack_head_t freeBlockStack; /**<  ���� block_t ջ */
}pab_t;

/**  @brief �������ڴ���� */
typedef struct{
    uint_t used;      /**<��ʹ���ڴ� */
    void *unusedAddr; /**< ָ��δ��ʹ�õĵ�ַ*/
    void *headAddr;   /**< ��ַͷ */
}baseLayer_t;

/**  @brief �ڴ�ؽṹ�� */
typedef struct{
    int_t memSize;                /**< �ڴ��С kb*/
    int_t freeMemSize;            /**< �����ڴ��С kb*/
    uint8_t slayerInit;           /**< ��ʼ����־ */
    pthread_mutex_t poolLock;     /**< �ڴ���� */
    list_head_t destroyHeadList;  /**< ������ʹ�õ�ҳͷ���� */
    pab_t pageAndBlock;           /**< page_t and block_t ջ���� */
    baseLayer_t baseMem;          /**< �������ڴ���� */
} mempool_t;


/**  @brief page_t and block_t ջ */
typedef struct{
    int_t used;          /**<��ʹ���ڴ� */
    list_node_t PABnode; /**<PAB�ڵ� */
    void *stackSP;       /**<ջָ�� */
}pabStack_t;

/**  @brief ҳ��� */
typedef struct{
    int_t pagesNum;            /**< ��ǰҳ���� */
    stack_head_t freePagesList;/**< ����ҳ���� */
    uint_t popular;            /**< �ܻ�ӭ�� */
} layering_t;

/**  @brief �����ڴ�ҳ����*/
typedef struct{
    layering_t pagesTable[PAGE_TABLE_SIZE]; /**< ҳ������ */
    int_t topMenu[5];                       /**< ��ֽ�㷨�õ������Ű� */
} freepages_t;

/**  @brief malloc ����*/
enum malloc_type{
    MALLOC_INIT = 0,        /**< malloc��ʼ�� */
    MALLOC_FROM_SYSTEM = 1, /**< �ڴ��Ǵ�ϵͳmalloc*/
};

/**  @brief ʹ�ñ�־*/
enum used_flag{
    USED = 0,     /**< ��ʹ��*/
    NON_USED = 1, /**< δʹ��*/
};
/*******************************************************************************************************************
*   Name��˽�к����ӿ�
*   Desc���ڲ����õ�һЩ����
*******************************************************************************************************************/
/******************************************************************
*   Name��gc�㺯���ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief �ϱ���ʱblock
* @param [in]  block   �ϱ��ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_report_overtime_block(block_t *block);
/**
* @brief �ڴ�ɨ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_halloc_memory_scan(list_head_t *pageList);
/**
* @brief ��ʼ��gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_init(void);
#define gc_init gclayer_init
/**
* @brief �ݻ�gc��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
gclayer_destroy(void);
#define gc_destroy gclayer_destroy
/******************************************************************
*   Name��ϵͳ�㺯���ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ϵͳ������һ���ڴ�ҳ
* @param [in]  pageSize   �ڴ�ҳ��С
* @param [in]  blockSum   �ڴ������
* @retval ����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_NULL ��ʾʧ��
*/
page_t *
slayer_malloc(uint_t pageSize, uint_t blockSum);

/**
* @brief ��ϵͳ���ͷ�һ���ڴ�ҳ
* @param [in]  pageSize   �ڴ�ҳ��С
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
slayer_free(page_t *page);
/**
* @brief ��ȡ��̬�ڴ��
* @param [in]  page      �ڴ�ҳ
* @param [in]  blockSize �ڴ���С
* @retval �����ڴ��    ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
block_t *
slayer_get_static_block(page_t * page, uint_t blockSize);
/**
* @brief ���ȫ���ڴ��ͷ��ַ
* @retval �����ڴ��׵�ַ ��ʾ�ɹ�
* @retval HALLOC_FAIL   ��ʾʧ��
*/
void *
slayer_get_headaddr(void);
/******************************************************************
*   Name���ڴ����㺯���ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ���ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_init(void);

/**
* @brief �ݻ��ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_destroy(void);
/**
* @brief ��ȡ����㶯̬�ڴ��
* @param [out]  blockList �ڴ������
* @param [in]  blockinfo   �ڴ����Ϣ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t 
mlayer_dynamic_malloc(list_head_t *blockList, uint_t blockinfo[5]);

/**
* @brief ��ȡ��̬�ڴ�
* @param [in]  blockSize  �ڴ���С
* @retval �����ڴ��    ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
block_t * 
mlayer_static_malloc(uint_t blockSize);

/**
* @brief malloc�߳�ջ
* @param [in]  stackSzie   �߳�ջ��С kb
* @retval �����߳��ڴ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t * 
mlayer_malloc_thread_stack(uint_t stackSzie);
/**
* @brief �ͷŹ�����ڴ��
* @param [in]  block     �ͷ��ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_free(block_t *block);
/**
* @brief ����ڴ�Խ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_check_overflow(void);
/**
* @brief ��ȡmlayer���ڴ���Ϣ
* @retval �ڴ���Ϣ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
uint_t
mlayer_get_meminfo(uint infoType);
/**
* @brief ��ȡ��ʱ������
* @retval �ڴ���Ϣ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
uint_t
mlayer_get_timersum(void);
/******************************************************************
*   Name���ڴ����㺯���ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĺ���
******************************************************************/
/**
* @brief ��ʼ���ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
alayer_init(void);
/**
* @brief �ݻ��ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
alayer_destroy(void);
/**
* @brief ��ȡ�ڴ�
* @param [out]  msize   �ڴ��С
* @param [out]  mtype   �ڴ�����
	- MEM_DYNAMIC ��̬�ڴ�
	- MEM_STATIC ��̬�ڴ�
	- MEM_THREAD_STACK �߳�ջ�ڴ�
* @param [out]  module  ģ������
* @param [out]  file    �ļ���
* @param [out]  func    ������
* @param [out]  line    �к�
* @retval �����ڴ�ָ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
void * 
alayer_malloc(uint_t msize, uint_t mtype, uint_t module, const char * file, const char * func, int_t line);
#define hmalloc alayer_malloc
/**
* @brief �ͷ��ڴ�
	�ڴ���ͷ������¼�������
	1. ���checkinfo_t �������ڴ������Խ�絽���ڴ�飬������ϱ���������
	2. ���checkinfo_t������Խ�絽�����ڴ�飬������ϱ���������
	3. ���ڴ��Żص�Ŀ�괦
* @param [in]  memory  �ͷ��ڴ�
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
alayer_free(void * memory);
#define hfree alayer_free
/**
* @brief ��ȡmlayer���ڴ���Ϣ
* @param [in]  module  ģ������
* @param [in]  infoType     ��Ϣ����
 - ALAYER_STATIC_MEM ��̬�ڴ�
 - ALAYER_STACK_MEM �߳�ջ�ڴ�
 - ALAYER_DYNAMIC_MEM ��̬�ڴ�
* @retval �ڴ���Ϣ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
uint_t
alayer_get_meminfo(uint_t module, uint infoType);
/*******************************************************************************************************************
*   Name��˽�нӿں� 
*   Desc���ڲ����õ�һЩ�꣬��Ҫ�ǻ���쳣�������ö�ջ
*******************************************************************************************************************/
/**
* @brief ����ѭ��
* @param [in]  type    ��������block/page
* @param [in]  list    ��������
* @param [in] member   �ڵ��ڽṹ����ڶ���Ľṹ����
*/
#define traversal_list_do(type, list, member)\
{\
    int_t nodeIndex;\
    int_t nodeSum = stack_get_current_member(list);\
    list_node_t  *tmp = NULL;\
    tmp = list_get_head_node(list);\
    for(nodeIndex=0; nodeIndex<nodeSum;nodeIndex++){\
		type = list_takeout_struct(tmp, type##_t, member);\
		tmp = list_get_next_node(tmp);

#define while_traversal_list(list)\
    }\
}

/**
* @brief ��ӡ�ڴ������
* @param [in] list     ��ӡ�ڴ������
* @param [in] member   �ڵ��ڽṹ����ڶ���Ľṹ����
*/

#define slayer_print_blocklist(list, member)\
do{\
    block_t *block= NULL;\
    halloc_log("\n\n\nprint the blocklist\n");\
	traversal_list_do(block, list, member){\
        halloc_log("****************%d block info*****************\n", nodeIndex);\
		halloc_log("headAddr=%p\n", block->headAddr);\
		halloc_log("blockSize=%d\n",block->blockSize);\
        halloc_log("color=%d\n",block->color);\
	}while_traversal_list(list);\
}while(0)

/**
* @brief ��ӡ�ڴ�ҳ����
* @param [in] print     ��ӡ�ڴ�ҳ����
*/
#define slayer_print_pagelist(list, member)\
do{\
	page_t 	*page = NULL;\
    halloc_log("\n\n\nprint the pagelist\n");\
	traversal_list_do(page, list, member){\
        halloc_log("****************%d page info*****************\n", nodeIndex);\
        halloc_log("headAddr=%p\n", page->headAddr);\
		halloc_log("maxTime=%d\n",page->maxTime);\
        halloc_log("pageSize=%d\n",page->pageSize);\
	}while_traversal_list(list);\
}while(0)

#endif /*__INC_HALLOC_H__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */