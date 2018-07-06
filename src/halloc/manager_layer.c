/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: manager_layer.c
@brief: 管理层主要是向系统申请内存页，以及向资源分配层提供内存块使用。本层主要六个函数，
        和上层调用函数，还有初始化和释放mlayer内存管理函数，一共九个主要部分。
        第一个部分：处理仓库算法函数
        第二个部分：处理租赁算法函数
        第三个部分：静态内存管理--线程栈申请
        第四个部分：静态内存管理--监管员
        第五个部分：动态内存管理--监管员
        第六个部分：内存越界监控
        第七个部分：上层调用的申请页函数
        第八个部分：mslayer初始化函数
        第九个部分：mslayer摧毁函数
@attention： 本模块关联外部文件和模块 util/utildef.h,utilinc.h err模块
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/12/18  | 0.0.0.1   | D.C.P    | 创建文件            
******************************************************************************/
#include ".././util/util_inc.h"
#include "halloc.h"

/**  @brief 静态内存管理 */
staticmem_t StaticMem;

/**  @brief 动态内存管理 */
dynamicmem_t DynamicMem;

/******************************************************************
*   Name：内存管理层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 获得仓库
* @param [in]  storage  仓库指针
* @param [in]  index  仓库索引
*/
#define _mlayer_get_storage(storage, index)\
do{\
    switch(index/6){\
        case 0: storage = &DynamicMem.small[index%6];break;\
        case 1: storage = &DynamicMem.middle[index%6];break;\
        case 2: storage = &DynamicMem.big[index%6];break;\
        case 3: storage = &DynamicMem.huge;break;\
    }\
}while(0)
/**
* @brief 检查越界
* @param [in]  block      内存块
* @param [in]  overflocw  内存越界计数
*/
#define _mlayer_check_overflow(block, overflow)\
do{\
    checkinfo_t  *headcheck = (checkinfo_t *)block->headAddr;\
    if(MEMISOVERFLOW(headcheck)){\
		overflow++;\
        halloc_log("the memory alloc place,module:%d, file:%s, line:%d, function:%s, ",block->module, block->file, block->line, block->func);\
		halloc_log("size:%d\n",block->blockSize);\
		slayer_print_blocklist(blocksList, node);\
	}\
}while(0)

/**
* @brief 尝试获得静态内存块
* @param [in]  list      内存块链表
* @param [in]  block      内存块
* @param [in]  size      内存块大小
*/
#define _mlayer_tryto_get_staticblock(list, block, size)\
do{\
	if( stack_get_current_member(list) != 0 ){\
		block = list_takeout_struct(list_get_tail_node(list), block_t, blockNode);\
		if(block->blockSize >= size ){\
			traversal_list_do(block, list, blockNode){\
				if(block->blockSize >= size ) {\
					CHECK_FG(list_delete_node(list, &block->blockNode), get_fail);\
					goto success;\
				}\
			}while_traversal_list(list);\
		}\
	}\
}while(0)

/**
* @brief 回收内存块
* @param [in]  list       内存块链表
* @param [in]  page       内存页
* @param [in]  block      内存块
* @param [in]  freelist   闲置内存块链表
* @param [in]  freesum    闲置内存
*/
#define _mlayer_recover_staticblock(list, page, block, freelist, freemem)\
do{\
	traversal_list_do(page, list, pageNode){\
		if(HALLOC_FAIL ==  _mlayer_check_memory(page)) break;\
		if(page->freeBlocks != 0){\
			traversal_list_do(block, &page->blocksList, node){\
				if(BLOCK_GREEN == block->color){\
					CHECK_FL(_mlayer_sort_block(freelist, block));\
					block->color = BLOCK_WHITE;\
					freemem = freemem + block->blockSize; \
				}\
			}while_traversal_list(&page->blocksList);\
		}\
	}while_traversal_list(list);\
}while(0)

/******************************************************************
*   Name：内存管理层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 检测内存
* @param [in]  page    内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_check_memory(page_t *page)
{
	block_t 	*block 	     = NULL;//指向需要操作的内存块
    stack_head_t *blocksList = &page->blocksList; //内存页的内存块链表
	uint_t     overflow = 0;//内存越界计数

	traversal_list_do(block, blocksList, node){	//检查内存越界与泄漏
		_mlayer_check_overflow(block, overflow);
		if(block->lease >= LEAK_CHECK && BLOCK_RED==block->color) CHECK_FL(gclayer_report_overtime_block(block));
	}while_traversal_list(blocksList);
	if(overflow != 0) return HALLOC_FAIL;
	return HALLOC_SUCC;
}

/**
* @brief 检测内存越界
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_check_overflow(void)
{
	page_t *page = NULL;//内存页
	uint_t overflow = 0;//内存越界计数
	int_t index;
	storage_t *storage = NULL;

	//静态内存部分检测
	traversal_list_do(page, &StaticMem.pagesList, pageNode){
		if(HALLOC_FAIL == _mlayer_check_memory(page)) overflow++;
	}while_traversal_list(&StaticMem.pagesList);
	//线程内存检测
	traversal_list_do(page, &StaticMem.threadList, pageNode){
		if(HALLOC_FAIL == _mlayer_check_memory(page)) overflow++;
	}while_traversal_list(&StaticMem.threadList);
	//动态内存检测
	for(index=0;index<19;index++){
		_mlayer_get_storage(storage, index);
		traversal_list_do(page, &storage->goodsArea, pageNode){
			if(HALLOC_FAIL == _mlayer_check_memory(page)) overflow++;
		}while_traversal_list(&storage->goodsArea);
	}
	if(overflow != 0) return HALLOC_FAIL;
	return HALLOC_SUCC;
}


/**
* @brief 内存页排序
* @param [in] goodsArea 货区
* @param [in] page     内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_sort_page(stack_head_t *goodsArea, page_t *page)
{
    list_node_t  *tmp = NULL;
	page_t *prevPage  = NULL;//上一个页节点
	//将节点排序
	if(list_get_head_node(goodsArea) != &page->pageNode){
		tmp = list_get_prev_node(&page->pageNode);
		do{
			prevPage = list_takeout_struct(tmp, page_t, pageNode);
			tmp = list_get_prev_node(tmp);
			if(prevPage->maxTime < page->maxTime){
				CHECK_FF(list_delete_node(goodsArea, &page->pageNode));
				CHECK_FG(list_embed_node(INSERT_TO_HEAD, goodsArea, &prevPage->pageNode, &page->pageNode), embed_fail);
			}
		}while(tmp != list_get_tail_node(goodsArea));
	}

	success:
		return HALLOC_SUCC;
	embed_fail:
		CHECK_FF(list_insert_node(INSERT_TO_TAIL, goodsArea, &page->pageNode));
		return HALLOC_FAIL;
}

/**
* @brief 收集内存块
* @param [in]  user    用户
 - SUPERVISOR  管理员
 - GUSET       客人
* @param [in]  storage 仓库
* @param [in]  page    内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t 
_mlayer_collect_block(uint_t user, storage_t * storage, page_t * page)
{
	//当没有闲置内存块时候，直接返回
	stack_head_t *blocksList  = &page->blocksList; //内存页的内存块链表
	block_t 	*block    = NULL;//指向需要操作的内存块
	uint_t     usedBlock  = 0;//被使用的的内存块计数
	uint_t     oldmaxTime = page->maxTime;//保存旧的maxTime值
	stack_head_t blockList;//收集的内存块链表存放处
	uint_t  blockSize = 0; //内存页大小
	stack_init(&blockList);

	page->maxTime = 0;
	//回收内存块
	traversal_list_do(block, blocksList, node){//链表循环收集内存块
		//将应该回收的全部收集完后退出
		if( GUEST == user && 0 == page->freeBlocks) goto success;
		//找出内存块中租赁的最长的时间
		if(SUPERVISOR  == user && BLOCK_YELLOW <= block->color) {
			block->lease++;
			page->maxTime = block->lease > page->maxTime?block->lease:page->maxTime;
			if(BLOCK_RED==block->color || BLOCK_YELLOW==block->color) usedBlock++;
		}
		//将符合条件内存块收集
		if(BLOCK_GREEN == block->color){
			block->color = BLOCK_WHITE;
			CHECK_FF(stack_push(&storage->freegood, &block->blockNode));
			blockSize = blockSize + block->blockSize;
			pthread_mutex_lock(&page->lock); 
			page->freeBlocks--;
			pthread_mutex_unlock(&page->lock); 
		}
	}while_traversal_list(blocksList);
	//回收内存页
	if(SUPERVISOR  == user && 0 == usedBlock){
		traversal_list_do(block, blocksList, node){//链表循环回收内存页
			if(BLOCK_WHITE == block->color) CHECK_FL(list_delete_node(&storage->freegood, &block->blockNode));
		}while_traversal_list(blocksList);
		CHECK_FL(list_delete_node(&storage->goodsArea, &page->pageNode));
		CHECK_FL(slayer_free(page));
		//更新动态内存中仓库和总动态内存值
		storage->freeMemSize   = storage->freeMemSize   - page->pageSize*KB + blockSize;
		pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
		DynamicMem.memSize 	   = DynamicMem.memSize     - page->pageSize*KB;
		DynamicMem.freeMemSize = DynamicMem.freeMemSize - page->pageSize*KB + blockSize;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
		goto success;
	}
	//当maxTime更新后，将重新进行排序
	if(SUPERVISOR  == user && page->maxTime != oldmaxTime+1 ) _mlayer_sort_page(&storage->goodsArea, page);
	success:
		return HALLOC_SUCC;
}


/**
* @brief huge仓库管理
* @param [in]  storage 仓库
* @param [in]  page    内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_huge_storage_supervisor(storage_t *storage, page_t * page)
{
	block_t *recover   = NULL; //嵌入到闲置链表的待回收内存块
	block_t *block     = NULL; //内存块
	stack_head_t *freegood = &storage->freegood; //内存块链表
	uint_t blockSize = 0; //block大小
	//内存检测
	CHECK_FF(_mlayer_check_memory(page));
	//内存管理
	recover = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//检查内存块时间是否到时
	if(recover->lease++ >= RELEASE_HUGE_THRESH){
		CHECK_FF(list_delete_node(&storage->goodsArea, &page->pageNode));//删除节点
		slayer_free(page);
		goto success;
	}

	if(BLOCK_GREEN != recover->color) goto success;

	//当是空闲块时候进行回收到freegood中
	traversal_list_do(block, freegood, blockNode){//遍历链表
		if(block->blockSize >= recover->blockSize){//找到适合回收的内存块位置后 执行嵌入操作
			CHECK_FF(list_embed_node(INSERT_TO_HEAD, freegood, &block->blockNode, &recover->blockNode));//嵌入到队列
			recover->color = BLOCK_WHITE;
			goto mem_update;
		}
	}while_traversal_list(freegood);
	//闲置内存块链表中没有内存块，不需要寻找合适位置，直接嵌入
	CHECK_FF(list_insert_node(INSERT_TO_TAIL, freegood, &recover->blockNode));
	recover->color = BLOCK_WHITE;
	

	mem_update:
		storage->freeMemSize = storage->freeMemSize + recover->blockSize;
		pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
		DynamicMem.freeMemSize = DynamicMem.freeMemSize + recover->blockSize;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
	success:
		return HALLOC_SUCC;
}


/**
* @brief huge仓库管理
* @param [in] storage 仓库
* @param [in] goodsArea 货区
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_small_middle_big_storage_supervisor(storage_t * storage, stack_head_t *goodsArea)
{
	page_t 	*page = NULL; //内存页

	//mlayer_check_overflow();
	traversal_list_do(page, goodsArea, pageNode){//遍历链表
		//内存检测
		CHECK_FF(_mlayer_check_memory(page));
		//将内存链表放入到freegood
		CHECK_FL(_mlayer_collect_block(SUPERVISOR, storage, page));
	}while_traversal_list(goodsArea);
	return HALLOC_SUCC;
}


/**
* @brief 动态内存定时器处理函数（管理员）
		 内存监管员主要是监管内存块是否出现内存越界，具体内存块结构体设计之后会讲到，
		 这里主要是讲监管员如何监管内存。由于静态内存写入不是特别频繁，所以不需要频
		 繁的今天查看。大概设计10秒进行遍历所有的内存块查看内存块是否越界，为了防止
		 一个内存块越界改写多个内存块内容。一旦发现越界，则向上查看上一个内存块是否
		 越界，一直查到没有越界的内存块。然后根据申请内存时候的文件，行数，函数信息，
		 进行上报到管理器
* @param [in]  value    传入值
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
void
mlayer_dynamicmem_supervisor(union sigval value)
{
	halloc_log("dynamicmem supervisor-%d start\n", value.sival_int);
	stack_head_t *goodsArea = NULL; //货区
	page_t 	*page 			= NULL; //内存页
    storage_t * storage 	= NULL; //货区链表
    _mlayer_get_storage(storage, value.sival_int);
	goodsArea = &storage->goodsArea;

	pthread_mutex_lock(&storage->blocksLock); //上锁
	if(stack_get_current_member(goodsArea) == 0 && 1 == storage->timerOpen) {//货区没有内存页时候
		CHECK_FL(timer_stop(storage->supervisor));//关闭定时器
		storage->timerOpen = 0;
		goto unlock;
	}
    if(value.sival_int == 18){//huge仓库管理
		traversal_list_do(page, goodsArea, pageNode){ 
			CHECK_FL(_mlayer_huge_storage_supervisor(storage, page));
		}while_traversal_list(goodsArea);
	}else{//small仓库 middle仓库 big仓库管理
        CHECK_FL(_mlayer_small_middle_big_storage_supervisor(storage, goodsArea));
    }
	pthread_mutex_unlock(&storage->blocksLock); //解锁
    //gc层
	//gclayer_halloc_memory_scan(goodsArea);
	return;
	unlock:
		pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
		DynamicMem.timerSum--;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
		pthread_mutex_unlock(&storage->blocksLock); //解锁
}


/**
* @brief 内存页排序
* @param [in] list     链表
* @param [in] page     内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_sort_block(stack_head_t *list, block_t *block)
{
    list_node_t  *tmp = NULL;
	block_t *prevBlock  = NULL;//上一个内存块节点
	if(stack_get_current_member(list) != 0){
		tmp = list_get_tail_node(list);
		do{
			prevBlock = list_takeout_struct(tmp, block_t, blockNode);
			if(prevBlock->blockSize <= block->blockSize){
				CHECK_FL(list_embed_node(INSERT_TO_TAIL, list, &prevBlock->blockNode, &block->blockNode));
				goto success;
			}else{
				if(tmp == list_get_head_node(list)){
					CHECK_FL(list_embed_node(INSERT_TO_HEAD, list, &prevBlock->blockNode, &block->blockNode));
					goto success;
				}
			}
			tmp = list_get_prev_node(tmp);
		}while(tmp != list_get_tail_node(list));
	}else{
		CHECK_FF(list_insert_node(INSERT_TO_HEAD, list, &block->blockNode));
	}
	success:
		return HALLOC_SUCC;
}


/**
* @brief 静态内存定时器处理函数（管理员）
		 内存监管员主要是监管内存块是否出现内存越界，具体内存块结构体设计之后会讲到，
		 这里主要是讲监管员如何监管内存。由于静态内存写入不是特别频繁，所以不需要频
		 繁的今天查看。大概设计10秒进行遍历所有的内存块查看内存块是否越界，为了防止
		 一个内存块越界改写多个内存块内容。一旦发现越界，则向上查看上一个内存块是否
		 越界，一直查到没有越界的内存块。然后根据申请内存时候的文件，行数，函数信息，
		 进行上报到管理器
* @param [in]  value    传入值
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
void
mlayer_staticmem_supervisor(union sigval value)
{
	page_t 	*page = NULL; //内存页
	block_t *block= NULL; //内存块
	stack_head_t *list  = NULL; //存储页链表或者线程栈链表
	halloc_log("staticmem supervisor start\n");
	pthread_mutex_lock(&StaticMem.staticLock); //上锁
	//静态内存检测
	list = &StaticMem.pagesList; //页链表
	_mlayer_recover_staticblock(list, page, block, &StaticMem.freeblockList, StaticMem.freeMemSize);
	//线程内存检测
	list = &StaticMem.threadList; //线程栈链表
	_mlayer_recover_staticblock(list, page, block, &StaticMem.freethreadList, StaticMem.threadFreemem);
	pthread_mutex_unlock(&StaticMem.staticLock); //解锁
	//gc层
	gclayer_halloc_memory_scan(&StaticMem.pagesList);
	return;
}


/**
* @brief 获取内存块
* @param [in]  storage    仓号
* @param [in]  blockinfo   内存块信息
* @param [in]  blockSum   内存块数量
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t 
_mlayer_malloc_small_middle_big_block(storage_t * storage, list_head_t *blockList, uint_t blockinfo[5])
{
	page_t * page = NULL; //内存页
	block_t *block= NULL; //内存块
	uint_t  pageSize = 0; //内存页大小
	uint_t   number = storage->storageType; //仓号
	int_t mallocSum = blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE];//申请内存块数量
	stack_head_t *goodsArea  = &storage->goodsArea; //货区
	stack_head_t *freegood  = &storage->freegood; //闲置链表

	//查看闲置freegood存在数量 大于或者等于需求量，直接获取后退出
	if(stack_get_current_member(freegood)>=mallocSum) goto success;
	//前往货区中寻找 可优化部分
	traversal_list_do(page, goodsArea, pageNode){//遍历链表
		if(page->freeBlocks != 0) CHECK_FL(_mlayer_collect_block(GUEST, storage, page));
		if(stack_get_current_member(freegood) >= mallocSum) goto success;
	}while_traversal_list(goodsArea);
	//因为不够，重新计算差距量
	mallocSum= blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE]-stack_get_current_member(freegood);
	//不够或者没有内存页则向系统层申请
	for(;mallocSum>=0; mallocSum=mallocSum-blockinfo[PGAE_BSUM]){
		CHECK_NF(page = slayer_malloc(blockinfo[PGAE_SIZE], blockinfo[PGAE_BSUM]));
		CHECK_FG(list_insert_node(INSERT_TO_TAIL, goodsArea, &page->pageNode), list_fail);
		//加入freegood链表
		traversal_list_do(block, &page->blocksList, node){ 
			CHECK_FG(stack_push(freegood, &block->blockNode), list_fail);
		}while_traversal_list(&page->blocksList);
		pthread_mutex_lock(&page->lock); 
		page->freeBlocks = 0;
		pthread_mutex_unlock(&page->lock); 
	}
	//更新动态内存中仓库和总动态内存值
	pageSize = blockinfo[PGAE_SIZE]*KB;

	success:
		mallocSum= blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE];
		CHECK_FL(stack_push_list(blockList, freegood, mallocSum));
		//更新动态内存中仓库和总动态内存值
		storage->memSize 	   = storage->memSize + pageSize;
		storage->freeMemSize   = storage->freeMemSize + pageSize -blockinfo[BLOCK_MSIZE];
		pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
		DynamicMem.memSize     = DynamicMem.memSize + pageSize;
    	DynamicMem.freeMemSize = DynamicMem.freeMemSize + pageSize -blockinfo[BLOCK_MSIZE];
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
		return HALLOC_SUCC;
	list_fail:
		slayer_free(page);
		return HALLOC_FAIL;
}

/**
* @brief 获取huge仓库内存块
* @param [in]  storage    仓号
* @param [out]  blockList 内存块链表
* @param [in]  blockinfo   内存块信息
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t 
_mlayer_malloc_huge_block(storage_t * storage, list_head_t *blockList, uint_t blockinfo[5])
{
    page_t       *page     = NULL;			     //获取内存页
    block_t      *block    = NULL;			     //获取内存块指针
    stack_head_t *freegood = &storage->freegood; //内存块链表
    uint_t       pageSize  = 0;			         //内存页大小

    //顺时针遍历节点
	traversal_list_do(block, freegood, blockNode){//循环链表寻找适合的内存块
		if(blockinfo[BLOCK_SIZE] <= block->blockSize && block->blockSize <= blockinfo[BLOCK_SIZE] + 1*KB) {
			CHECK_FF(list_delete_node(freegood, &block->blockNode));//大小间隔小于1kb 取出
			goto success;
		}
	}while_traversal_list(freegood);
	//没有合适的直接从系统层获取
	CHECK_NF(page = slayer_malloc(blockinfo[PGAE_SIZE], 1));
	CHECK_FG(stack_push(&storage->goodsArea, &page->pageNode),fail);//加入链表
	block = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//更新动态内存中仓库和总动态内存值
	pageSize = blockinfo[PGAE_SIZE]*KB;

	success:
		//更新动态内存中仓库和总动态内存值
		pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
		DynamicMem.memSize = DynamicMem.memSize + pageSize;
		DynamicMem.freeMemSize = DynamicMem.freeMemSize + pageSize - blockinfo[BLOCK_MSIZE];
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
		CHECK_FF(stack_push(blockList, &block->blockNode));
		return HALLOC_SUCC;	

	fail:
		slayer_free(page);
		return HALLOC_FAIL;
}



/**
* @brief 租用内存块
* @param [in]  storage    仓号
* @param [out] blockList  内存块链表
* @param [in]  blockinfo   内存块信息
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_mlayer_lease_block(storage_t *storage, list_head_t *blockList, uint_t blockinfo[5])
{
	block_t *block = NULL;

	if(18 == storage->storageType){//huge仓库操作
		CHECK_FF(_mlayer_malloc_huge_block(storage, blockList, blockinfo));
	}else{//small仓库 middle仓库 big仓库操作
		CHECK_FF(_mlayer_malloc_small_middle_big_block(storage, blockList, blockinfo));
	}
	traversal_list_do(block, blockList, blockNode){ 
		block->color = BLOCK_YELLOW; 
		block->type  = MEM_DYNAMIC;
	}while_traversal_list(blockList);//链表循环
	return HALLOC_SUCC;
}

/**
* @brief 打开管理员
* @param [in]  storage    仓号
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_start_supervisor(storage_t *storage)
{
	struct timespec times;//定时器激活时间
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_start(storage->supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	storage->timerOpen == 1;
	pthread_mutex_lock(&DynamicMem.dynamicLock); //加锁
	DynamicMem.timerSum++;
	pthread_mutex_unlock(&DynamicMem.dynamicLock); //解锁
	return HALLOC_SUCC;
}


/**
* @brief 获取管理层动态内存块
* @param [out]  blockList 内存块链表
* @param [in]  blockinfo   内存块信息
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t 
mlayer_dynamic_malloc(list_head_t *blockList, uint_t blockinfo[5])
{
	if(INITIALIZED != DynamicMem.dynamicInit) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	if(PARAMISNULL(blockList))  RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	
	storage_t * storage = NULL;
	int_t ret = HALLOC_FAIL;
	//获得仓库
	_mlayer_get_storage(storage, blockinfo[BLOCK_STORAGE]);
	pthread_mutex_lock(&storage->blocksLock); 
	//当没有开启定时器管理员时候，启动定时器管理员
	if(storage->timerOpen == 0) mlayer_start_supervisor(storage);
	//租赁内存块
	ret = _mlayer_lease_block(storage, blockList, blockinfo);
	pthread_mutex_unlock(&storage->blocksLock); 
	return ret;
}


/**
* @brief 获取静态内存
* @param [in]  blockSize  内存块大小
* @retval 返回内存块    表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t * 
mlayer_static_malloc(uint_t blockSize)
{
	if(INITIALIZED != StaticMem.staticInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if(blockSize > STATIC_PAGE) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t 	*page = NULL; //内存页
	block_t *block= NULL;
	stack_head_t *list  = NULL; //闲置内存块链表或者页链表
	uint_t pageSize = 0;		//内存页大小

	pthread_mutex_lock(&StaticMem.staticLock); //上锁
	//静态内存检测
	list = &StaticMem.freeblockList; //闲置内存块链表
	_mlayer_tryto_get_staticblock(list, block, blockSize);
	//从页堆栈获取
	list = &StaticMem.pagesList; //页链表
	traversal_list_do(page, list, pageNode){//循环链表找到足够空间获取内存块的内存页
		block = slayer_get_static_block(page, blockSize);
		if(block != NULL) goto success;
	}while_traversal_list(list);
	//重新获取一个内存页
	CHECK_NG(page = slayer_malloc(STATIC_PAGE, 0), malloc_fail);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &StaticMem.pagesList, &page->pageNode), list_fail);
	CHECK_NG(block = slayer_get_static_block(page, blockSize), malloc_fail);
	//更新总动态和闲置内存值
	pageSize = STATIC_PAGE;

	success:
		//更新总动态和闲置内存值
		StaticMem.memSize = StaticMem.memSize + pageSize;
		StaticMem.freeMemSize = StaticMem.freeMemSize - block->blockSize + pageSize;
		block->color = BLOCK_YELLOW;
		block->type  = MEM_STATIC;
		pthread_mutex_unlock(&StaticMem.staticLock); //解锁
		return block;
	list_fail:
		slayer_free(page);
	malloc_fail:
	get_fail:
		pthread_mutex_unlock(&StaticMem.staticLock); //解锁
		return HALLOC_NULL;	
}

/**
* @brief malloc线程栈
* @param [in]  stackSzie   线程栈大小 kb
* @retval 返回线程内存块  表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t * 
mlayer_malloc_thread_stack(uint_t stackSize)
{
	if(INITIALIZED != StaticMem.staticInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t  *page = NULL;
	block_t *block= NULL;

	pthread_mutex_lock(&StaticMem.staticLock); //上锁
	//尝试获取线程栈
	_mlayer_tryto_get_staticblock(&StaticMem.freethreadList, block, stackSize*KB);
	//没有合适就申请
	CHECK_NG(page = slayer_malloc(stackSize, 1), malloc_fail);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &StaticMem.threadList, &page->pageNode), list_fail);
	//转化成内存块
	block = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//更新总动态和闲置内存值
	StaticMem.threadMem = StaticMem.threadMem + page->pageSize*KB;
	StaticMem.threadFreemem = StaticMem.threadFreemem + page->pageSize*KB; 

	success:
		//更新总动态和闲置内存值
		StaticMem.threadFreemem = StaticMem.threadFreemem - block->blockSize;
		block->color = BLOCK_YELLOW;
		block->type  = MEM_THREAD_STACK;
		pthread_mutex_unlock(&StaticMem.staticLock); //解锁
		return block;
	list_fail:
		slayer_free(page);
	malloc_fail:
	get_fail:
		pthread_mutex_unlock(&StaticMem.staticLock); //解锁
		return HALLOC_NULL;
}

/**
* @brief 释放管理层内存块
* @param [in]  block     释放内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_free(block_t *block)
{
	if(PARAMISNULL(block))  RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	page_t *page = block->page;
	block->color = BLOCK_GREEN;
	block->lease = 0;
	pthread_mutex_lock(&page->lock); 
	page->freeBlocks++;
	pthread_mutex_unlock(&page->lock); 
	return HALLOC_SUCC;
}


/**
* @brief 初始化内存管理层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_init(void)
{
    timerEvent_t event;
    struct timespec times;
	int_t index;
	storage_t *storage = NULL;


    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
	//静态内存初始化
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	StaticMem.memSize       = 0;          //内存大小 
    StaticMem.freeMemSize   = 0;          //闲置内存大小
	StaticMem.threadMem     = 0;          //内存大小 
    StaticMem.threadFreemem = 0;          //闲置内存大小
	StaticMem.staticInit  = INITIALIZED;  //初始化标志
	event.callbackFunc    = mlayer_staticmem_supervisor;
	pthread_mutex_init(&StaticMem.staticLock,NULL);//静态内存锁 
	pthread_mutex_init(&StaticMem.threadLock,NULL);//线程栈内存锁 
	list_init(&StaticMem.pagesList); //页链表 每个页固定4k 
	list_init(&StaticMem.threadList);//线程栈链表
	list_init(&StaticMem.freeblockList); //闲置内存块链表 
	list_init(&StaticMem.freethreadList);//闲置线程栈链表
	CHECK_FF(timer_open(&StaticMem.supervisor,&event)); //监管员 监管内存越界等
	CHECK_FF(timer_start(StaticMem.supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	//动态内存初始化
	pthread_mutex_init(&DynamicMem.dynamicLock,NULL);//动态内存锁 
	DynamicMem.timerSum	   = 0;          //定时器数量
	DynamicMem.memSize     = 0;          //内存大小 
    DynamicMem.freeMemSize = 0;          //闲置内存大小
	DynamicMem.dynamicInit = INITIALIZED;//初始化标志
	event.callbackFunc     = mlayer_dynamicmem_supervisor;
	for(index=0;index<19;index++){
		event.signalValue.sival_int = index;
		_mlayer_get_storage(storage, index);
		storage->storageType = index; //仓库类型
		list_init(&storage->freegood);//空闲货物
		stack_init(&storage->goodsArea);//仓库货区  
		CHECK_FF(timer_open(&storage->supervisor,&event)); //监管员 租赁算法使用 
		storage->timerOpen = 0; //仓库类型
		pthread_mutex_init(&storage->blocksLock,NULL);//内存块锁 
	}
	return HALLOC_SUCC;
}



/**
* @brief 摧毁内存管理层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
mlayer_destroy(void)
{
	storage_t * storage = NULL;
	int_t index;
	//静态内存摧毁
	CHECK_FL(timer_stop(StaticMem.supervisor));//关闭定时器
	CHECK_FF(timer_close(&StaticMem.supervisor)); //监管员 监管内存越界等
	//动态内存摧毁
	for(index=0;index<19;index++){
		_mlayer_get_storage(storage, index);
		CHECK_FL(timer_stop(storage->supervisor));//关闭定时器
		CHECK_FF(timer_close(&storage->supervisor)); //监管员 租赁算法使用	
	}
    return HALLOC_SUCC;
}

/**
* @brief 获取mlayer层内存信息
* @param [in]  infoType     消息类型
 - MLAYER_STATIC_MEM 静态内存
 - MLAYER_STATIC_FREEMEM 静态闲置内存
 - MLAYER_THREADSTACK_MEM 线程栈使用内存
 - MLAYER_THREADSTACK_FREEMEM 线程栈闲置内存
* @retval 内存信息  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
mlayer_get_meminfo(uint infoType)
{
	switch(infoType){
		case MLAYER_STATIC_MEM:return StaticMem.memSize;
		case MLAYER_STATIC_FREEMEM:return StaticMem.freeMemSize;
		case MLAYER_THREADSTACK_MEM:return StaticMem.threadMem*KB;
		case MLAYER_THREADSTACK_FREEMEM:return StaticMem.threadFreemem*KB;
	}
	return HALLOC_FAIL;
}


/**
* @brief 获取定时器数量
* @retval 内存信息  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
mlayer_get_timersum(void)
{
	return DynamicMem.timerSum +1;//动态定时器 + 静态定时器
}