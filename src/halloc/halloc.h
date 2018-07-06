/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: thrad.h
@brief:  halloc 所有用到的宏和结构体
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/03/07  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __INC_HALLOC_H__
#define __INC_HALLOC_H__

/**  @brief 成功返回 */
#define HALLOC_SUCC FUNC_SUCC
/**  @brief 失败返回 */
#define HALLOC_FAIL FUNC_FAIL
/**  @brief 失败返回 */
#define HALLOC_NULL FUNC_NULL
/**  @brief KB大小 */
#define KB 1024
/**  @brief halloc log函数 */
#define halloc_log printf
/**  @brief 校验码 */
#define CHECKCODE 0xabcd
/**  @brief 定时器时间*/
#define SUPERVISOR_TIME 5
/**  @brief 内存位移 指针一次位移四字节*/ 
#define MEM_OFFSET(addr, offset) (char *)(addr)+offset

/**  @brief 页结构体 */
typedef struct Page page_t;
struct Page{
    /* 公共参数 */
    uint8_t brotherflag;  /**< 哥哥标志 */
    uint8_t usedflag;     /**< 内存页被使用标志 */
    uint8_t mallocpage;   /**< malloc申请页 */
    list_node_t pageNode; /**< 页节点 */
    void *unusedAddr;     /**< 指向未被使用的地址*/
    void *headAddr;       /**< 地址头 */
    /* 系统层使用参数 */
    int_t pageSize;          /**< 纸张大小*/
    int_t unusedSize;        /**< 未被使用的大小 单位字节*/
    page_t *brother;         /**< 另一张裁剪处理的兄弟纸张 */
    list_node_t destroyNode; /**< 销毁节点 销毁内存池使用*/
    /* 资源管理层使用参数 */
    uint_t maxTime;         /**< 当前占用时间最长的内存块时间值 */
    list_head_t blocksList; /**< 内存块链表 */
    uint_t freeBlocks;      /**< 闲置内存块数量 */
    thread_mutex_t lock;    /**< 页锁 */
};

/**  @brief 内存块结构体 */
typedef struct{
    /* 公共参数 */
    uint_t blockSize;      /**< 内存块大小 */
    void *headAddr;        /**< 地址头 */
    uint16_t lease;        /**< 内存块租用时间 租赁算法使用*/
    int8_t color;          /**< 绿色-等待被回收 黄色-分配层使用 红色-外部模块使用 白色-无人使用*/
    int8_t type;           /**< 内存块类型*/
    list_node_t blockNode; /**< 页节点 位于其他链表 */
    page_t *page;          /**< 隶属的内存页 */
    /* 资源管理层使用参数 */
    list_node_t node; /**< 页节点 位于blocksList内存块链表*/
    /* 资源分配层使用参数 */
    uint16_t module;       /**< 模块 */
    uint16_t line;         /**< 行号 */
    string_t file;         /**< 文件名 */
    string_t func;         /**< 函数名 */
    /* 内存泄漏使用 */
    uint8_t checkSign;     /**< 泄漏检查标记 */
    list_node_t blackNode; /**< 黑名单节点 */
} block_t;


/**  @brief 内存校验信息结构体 */
typedef struct{
    u16_t headcheck; /**< 校验码 默认CHECKCODE */
    block_t *block;  /**< 指向本内存的block */
    u16_t tailcheck; /**< 校验码 默认CHECKCODE */
} checkinfo_t;


/**  @brief 使用标志*/
enum init_flag{
    INITIALIZED = 1, /**< 已初始化*/
    NONINIT     = 2, /**< 未初始化 */
};

/**  @brief 使用标志*/
enum mem_Info{
    MLAYER_STATIC_MEM = 1,      /**< 静态内存*/
    MLAYER_STATIC_FREEMEM,      /**< 静态闲置内存*/
    MLAYER_THREADSTACK_MEM,     /**< 线程栈使用内存*/
    MLAYER_THREADSTACK_FREEMEM, /**< 线程栈闲置内存 */
    ALAYER_STATIC_MEM,          /**< 静态内存 */
    ALAYER_STACK_MEM,           /**< 线程栈内存 */
    ALAYER_DYNAMIC_MEM,         /**< 动态内存 */
};
/******************************************************************
*   Name：垃圾回收层相关结构体
*   Desc：系统层中将会使用这部分结构体
******************************************************************/
/**  @brief 挂起线程 */
#define STOP_THREAD SIGPWR
/**  @brief gc管理*/
typedef struct{
    uint8_t GCInit : 1;          /**< GC初始化*/
    uint8_t GCStart : 1;         /**< 开始GC*/
    uint8_t readySign : 1;       /**< 准备标志*/
    uint8_t wakeupSign : 1;      /**< 苏醒标志*/
    uint16_t totalThread : 14;   /**< 总线程数 */
    uint16_t currentThread : 14; /**< 当前线程数 */
    uint8_t timerOpen;           /**< 定时器打开 */
    Timer_t supervisor;          /**< 监管员 */
    list_head_t blackList;       /**< 黑名单 */
    void *hallocHeadAddr;        /**< halloc首地址 */
    void *hallocTailAddr;        /**< halloc尾地址 */
    void *startData;             /**< 程序数据段起始地址 */
    void *endData;               /**< 数据段尾部地址 */
    void *stackbottom;           /**< 主进程栈底地址 */
    pid_t mainPid;               /**< 主进程id */
    thread_cond_t wakeup;        /**< 唤醒条件变量 */
    thread_mutex_t lock;         /**< 唤醒锁 */
    thread_mutex_t listLock;     /**< 链表锁 */
}gc_t;
/******************************************************************
*   Name：内存分配层相关结构体
*   Desc：系统层中将会使用这部分结构体
******************************************************************/
/**  @brief size转换*/
#define SWITCH_TO_KB(size) (size%KB >0?size/KB+1:size/KB)
/**  @brief 申请内存大小转化为内存块大小 */
#define SWITCH_TO_BLOCKSIZE(size) (size+sizeof(checkinfo_t))
/**  @brief 向管理层租赁时间 默认1分钟 */
#define LEASE_TIME (1*60/SUPERVISOR_TIME)
/**  @brief 获得内存地址 */
#define get_malloc_addr(addr) MEM_OFFSET(addr,sizeof(checkinfo_t))

/**  @brief 货架*/
typedef struct{
    int_t memSize;               /**< 内存大小 */
    int_t freeMemSize;           /**< 闲置内存大小 */
    uint_t returnBlocks;         /**< 归还内存块数量 */
    uint8_t layer;               /**< 货架层 */
    uint16_t giveBack;           /**< 归还数量 租赁算法使用 */
    uint8_t applyLevel;          /**< 申请等级 租赁算法使用 */
    stack_head_t freegood;       /**< 空闲货物 */
    stack_head_t returngood;     /**< 归还货物 */
    pthread_mutex_t returnLock;  /**< 归还内存块锁 */
    pthread_mutex_t freeLock;    /**< 闲置内存块锁 */
}shelf_t;

/**  @brief 模块内存管理*/
typedef struct{
    uint16_t module;            /**< 模块 */
    uint_t dynamicMem;          /**< 动态内存大小 */
    uint_t staticMem;           /**< 静态内存大小 */
    uint_t threadMem;           /**< 线程内存大小 kb*/
    pthread_mutex_t moduleLock; /**< 模块锁 */
    list_head_t staticList;     /**< 静态链表*/
    list_head_t threadList;     /**< 线程栈链表*/
    shelf_t small[6];           /**< small货架 */
    shelf_t middle[6];          /**< middle货架 */
    shelf_t big[6];             /**< big货架 */
    shelf_t huge;               /**< huge货架 */
}moduleMem_t;

/**  @brief 模块池*/
typedef struct{
    Timer_t supervisor;           /**< 监管员 租赁算法使用 */
    moduleMem_t memory[MOD_BUTT]; /**< 模块内存池 */
} modulePool_t;

/**  @brief 内存块类型 */
enum MemType{
  MEM_NONE,         /**< 无类型 */
  MEM_DYNAMIC,      /**< 动态内存 */
  MEM_STATIC,       /**< 静态内存 */
  MEM_THREAD_STACK, /**< 线程栈内存 */
};
/******************************************************************
*   Name：内存管理层相关结构体
*   Desc：系统层中将会使用这部分结构体
******************************************************************/
/**  @brief huge仓库内存释放阈值 默认10分钟*/
#define RELEASE_HUGE_THRESH (10*60/SUPERVISOR_TIME)
/**  @brief 泄漏检查时间 2小时*/
#define LEAK_CHECK (2*60*60/SUPERVISOR_TIME)
/**  @brief 检查内存越界*/
#define MEMISOVERFLOW(checkinfo) (checkinfo->headcheck != CHECKCODE || checkinfo->tailcheck != CHECKCODE)
/**  @brief 静态内存页 */
#define STATIC_PAGE 4*1024

    /**  @brief 仓号结构体 */
typedef struct{
    int_t memSize;              /**< 内存大小 */
    int_t freeMemSize;          /**< 闲置内存大小 */
    uint8_t storageType;        /**< 仓库类型 */
    uint8_t timerOpen;          /**< 定时器打开 */
    stack_head_t freegood;      /**< 空闲货物 */
    list_head_t goodsArea;      /**< 仓库货区 */
    Timer_t supervisor;         /**< 监管员 租赁算法使用 */
    pthread_mutex_t blocksLock; /**< 内存块锁 */
}storage_t;

/**  @brief 静态内存结构体 */
typedef struct{
    int_t memSize;               /**< 内存大小 */
    int_t freeMemSize;           /**< 闲置内存大小 */
    int_t threadMem;             /**< 线程内存大小 kb*/
    int_t threadFreemem;         /**< 线程闲置内存大小 kb*/
    uint8_t staticInit;          /**< 初始化标志 */
    list_head_t pagesList;       /**< 页链表 每个页固定4k*/
    list_head_t threadList;      /**< 线程栈链表*/
    list_head_t freeblockList;   /**< 闲置内存块链表*/
    list_head_t freethreadList;  /**< 闲置线程栈链表*/
    Timer_t supervisor;          /**< 监管员 监管内存越界等 */
    pthread_mutex_t staticLock;  /**< 静态内存锁 */
    pthread_mutex_t threadLock;  /**< 线程内存锁 */
} staticmem_t;

/**  @brief 动态内存结构体 */
typedef struct{
    int_t memSize;               /**< 内存大小 1k的整数倍*/
    int_t freeMemSize;           /**< 闲置内存大小 */
    uint8_t dynamicInit;         /**< 初始化标志 */
    uint8_t timerSum;            /**< 定时器数量 */
    storage_t small[6];          /**< small仓库 */
    storage_t middle[6];         /**< middle仓库  */
    storage_t big[6];            /**< big仓库  */
    storage_t huge;              /**< huge仓库  */
    pthread_mutex_t dynamicLock; /**< 动态内存锁 */
}dynamicmem_t;

/**  @brief 内存块颜色*/
enum color_type{
    BLOCK_WHITE  = 0,  /**< 白色-无人使用  */
    BLOCK_GREEN  = 1,  /**< 绿色-等待被回收 */
    BLOCK_YELLOW = 2,  /**< 黄色-分配层使用 */
    BLOCK_RED    = 3,  /**< 红色-外部模块使用*/
};

/**  @brief 内存块信息 */
enum block_info{
    BLOCK_STORAGE = 0,      /**< 内存块仓号 */
    BLOCK_SIZE    = 1,      /**< 内存块大小 */
    BLOCK_MSIZE   = 2,      /**< 内存块向管理层malloc大小*/
    PGAE_BSUM     = 3,      /**< 一个内存页的内存块数量 */
    PGAE_SIZE     = 4,      /**< 内存页大小*/
};

/**  @brief 内存块信息 */
enum collect_user{
    SUPERVISOR = 0,      /**< 管理员 */
    GUEST      = 1,      /**< 客人 */
};
/******************************************************************
*   Name：系统层相关结构体
*   Desc：系统层中将会使用这部分结构体
******************************************************************/
/**  @brief 页表大小 定义为14kb是由资源管理层使用得出的值 */
#define PAGE_TABLE_SIZE 15
/**  @brief 页表最大 100mb*/
#define PAGE_MAX_SIZE 100*1024
/**  @brief 内存块最大 64*/
#define BLOCK_MAX_SIZE 64
/**  @brief pab栈最大 64*/
#define PAB_STACK_SIZE 8*1024

/**  @brief page_t and block_t */
typedef struct{
    list_head_t PABStackList;   /**< page_t and block_t 栈链表 */
    stack_head_t freePageStack; /**< 闲置 page_t 栈 */
    stack_head_t freeBlockStack; /**<  闲置 block_t 栈 */
}pab_t;

/**  @brief 基础层内存管理 */
typedef struct{
    uint_t used;      /**<已使用内存 */
    void *unusedAddr; /**< 指向未被使用的地址*/
    void *headAddr;   /**< 地址头 */
}baseLayer_t;

/**  @brief 内存池结构体 */
typedef struct{
    int_t memSize;                /**< 内存大小 kb*/
    int_t freeMemSize;            /**< 闲置内存大小 kb*/
    uint8_t slayerInit;           /**< 初始化标志 */
    pthread_mutex_t poolLock;     /**< 内存池锁 */
    list_head_t destroyHeadList;  /**< 销毁所使用的页头链表 */
    pab_t pageAndBlock;           /**< page_t and block_t 栈链表 */
    baseLayer_t baseMem;          /**< 基础层内存管理 */
} mempool_t;


/**  @brief page_t and block_t 栈 */
typedef struct{
    int_t used;          /**<已使用内存 */
    list_node_t PABnode; /**<PAB节点 */
    void *stackSP;       /**<栈指针 */
}pabStack_t;

/**  @brief 页表层 */
typedef struct{
    int_t pagesNum;            /**< 当前页数量 */
    stack_head_t freePagesList;/**< 闲置页链表 */
    uint_t popular;            /**< 受欢迎度 */
} layering_t;

/**  @brief 闲置内存页管理*/
typedef struct{
    layering_t pagesTable[PAGE_TABLE_SIZE]; /**< 页表数组 */
    int_t topMenu[5];                       /**< 剪纸算法用到的热门榜单 */
} freepages_t;

/**  @brief malloc 处理*/
enum malloc_type{
    MALLOC_INIT = 0,        /**< malloc初始化 */
    MALLOC_FROM_SYSTEM = 1, /**< 内存是从系统malloc*/
};

/**  @brief 使用标志*/
enum used_flag{
    USED = 0,     /**< 已使用*/
    NON_USED = 1, /**< 未使用*/
};
/*******************************************************************************************************************
*   Name：私有函数接口
*   Desc：内部调用的一些函数
*******************************************************************************************************************/
/******************************************************************
*   Name：gc层函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 上报超时block
* @param [in]  block   上报内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_report_overtime_block(block_t *block);
/**
* @brief 内存扫描
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_halloc_memory_scan(list_head_t *pageList);
/**
* @brief 初始化gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_init(void);
#define gc_init gclayer_init
/**
* @brief 摧毁gc层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
gclayer_destroy(void);
#define gc_destroy gclayer_destroy
/******************************************************************
*   Name：系统层函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 向系统层申请一个内存页
* @param [in]  pageSize   内存页大小
* @param [in]  blockSum   内存块数量
* @retval 获得内存页    表示成功
* @retval HALLOC_NULL 表示失败
*/
page_t *
slayer_malloc(uint_t pageSize, uint_t blockSum);

/**
* @brief 向系统层释放一个内存页
* @param [in]  pageSize   内存页大小
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
slayer_free(page_t *page);
/**
* @brief 获取静态内存块
* @param [in]  page      内存页
* @param [in]  blockSize 内存块大小
* @retval 返回内存块    表示成功
* @retval HALLOC_FAIL  表示失败
*/
block_t *
slayer_get_static_block(page_t * page, uint_t blockSize);
/**
* @brief 获得全部内存的头地址
* @retval 返回内存首地址 表示成功
* @retval HALLOC_FAIL   表示失败
*/
void *
slayer_get_headaddr(void);
/******************************************************************
*   Name：内存管理层函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化内存管理层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_init(void);

/**
* @brief 摧毁内存管理层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_destroy(void);
/**
* @brief 获取管理层动态内存块
* @param [out]  blockList 内存块链表
* @param [in]  blockinfo   内存块信息
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t 
mlayer_dynamic_malloc(list_head_t *blockList, uint_t blockinfo[5]);

/**
* @brief 获取静态内存
* @param [in]  blockSize  内存块大小
* @retval 返回内存块    表示成功
* @retval HALLOC_FAIL  表示失败
*/
block_t * 
mlayer_static_malloc(uint_t blockSize);

/**
* @brief malloc线程栈
* @param [in]  stackSzie   线程栈大小 kb
* @retval 返回线程内存块  表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t * 
mlayer_malloc_thread_stack(uint_t stackSzie);
/**
* @brief 释放管理层内存块
* @param [in]  block     释放内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_free(block_t *block);
/**
* @brief 检测内存越界
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_check_overflow(void);
/**
* @brief 获取mlayer层内存信息
* @retval 内存信息  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
mlayer_get_meminfo(uint infoType);
/**
* @brief 获取定时器数量
* @retval 内存信息  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
mlayer_get_timersum(void);
/******************************************************************
*   Name：内存分配层函数接口
*   Desc：可以被内部和外部调用的函数
******************************************************************/
/**
* @brief 初始化内存分配层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_init(void);
/**
* @brief 摧毁内存分配层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_destroy(void);
/**
* @brief 获取内存
* @param [out]  msize   内存大小
* @param [out]  mtype   内存类型
	- MEM_DYNAMIC 动态内存
	- MEM_STATIC 静态内存
	- MEM_THREAD_STACK 线程栈内存
* @param [out]  module  模块类型
* @param [out]  file    文件名
* @param [out]  func    函数名
* @param [out]  line    行号
* @retval 返回内存指针  表示成功
* @retval HALLOC_NULL  表示失败
*/
void * 
alayer_malloc(uint_t msize, uint_t mtype, uint_t module, const char * file, const char * func, int_t line);
#define hmalloc alayer_malloc
/**
* @brief 释放内存
	内存块释放由以下几个步骤
	1. 检查checkinfo_t 看上面内存块有无越界到本内存块，如果有上报到管理器
	2. 检查checkinfo_t看有无越界到下面内存块，如果有上报到管理器
	3. 将内存块放回到目标处
* @param [in]  memory  释放内存
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_free(void * memory);
#define hfree alayer_free
/**
* @brief 获取mlayer层内存信息
* @param [in]  module  模块类型
* @param [in]  infoType     消息类型
 - ALAYER_STATIC_MEM 静态内存
 - ALAYER_STACK_MEM 线程栈内存
 - ALAYER_DYNAMIC_MEM 动态内存
* @retval 内存信息  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
alayer_get_meminfo(uint_t module, uint infoType);
/*******************************************************************************************************************
*   Name：私有接口宏 
*   Desc：内部调用的一些宏，主要是获得异常函数调用堆栈
*******************************************************************************************************************/
/**
* @brief 遍历循环
* @param [in]  type    操作类型block/page
* @param [in]  list    遍历链表
* @param [in] member   节点在结构体的内定义的结构体名
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
* @brief 打印内存块链表
* @param [in] list     打印内存块链表
* @param [in] member   节点在结构体的内定义的结构体名
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
* @brief 打印内存页链表
* @param [in] print     打印内存页链表
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