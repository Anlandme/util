/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: alloc_layer.c
@brief: 分配层主要是对于内存块分发到各个模块当中，并对于各个模块的内存块进行管理。保证模块内的
		内存块等到合理分配，模块这个概念，主要是可以理解为功能，对应模块申请内存必须对应模块释放
		本层主要租赁算法函数和alayer层管理员，和两个被外层调用函数，还有初始化和释放alayer
		资源函数，一共六个主要部分。
        第一个部分：租赁算法函数
        第二个部分：alayer层管理员
        第三个部分：alayer初始化函数
        第四个部分：alayer摧毁函数
        第五个部分：外层调用的申请内存函数
        第六个部分：外层调用的释放内存函数

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

/**  @brief 模块内存池 */
modulePool_t modulePool;

/**  @brief 仓库内存数组 */
uint_t blockTable[19][5] = {
/*仓号 | 内存块大小(huge每次需要填写) | 分配层申请内存块数量总大小(每次需要填写) | 内存块数量 | 所需内存页大小(huge每次需要填写) */
//small仓库
{ 0,  48,   0, 64, 3},
{ 1,  64,   0, 16, 1},
{ 2,  80,   0, 64, 5},
{ 3,  96,   0, 32, 3},
{ 4,  112,  0, 64, 7},
{ 5,  128,  0, 8,  1},
//middle仓库
{ 6,  192,  0, 16, 3},
{ 7,  256,  0, 8,  2},
{ 8,  320,  0, 16, 5},
{ 9,  384,  0, 8,  3},
{ 10, 448,  0, 16, 7},
{ 11, 512,  0, 4,  2},
//big仓库
{ 12, 768,  0, 4,  3},
{ 13, 1024, 0, 4,  4},
{ 14, 1280, 0, 4,  5},
{ 15, 1536, 0, 2,  3},
{ 16, 1792, 0, 4,  7},
{ 17, 2048, 0, 2,  4},
//huge仓库
{ 18, 0,    0, 1,  0}//
};


/******************************************************************
*   Name：内存分配层接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**  @brief huge shelf */
#define HUGE_SHELF 18
/**
* @brief 转化货架
* @param [in]  shelf  货架指针
* @param [in]  module 模块
* @param [in]  layer  货架层
*/
#define _alayer_return_shelf(shelf, module, layer)\
do{\
	switch(layer/6){\
			case 0: shelf = &modulePool.memory[module].small[layer%6];break;\
			case 1: shelf = &modulePool.memory[module].middle[layer%6];break;\
			case 2: shelf = &modulePool.memory[module].big[layer%6];break;\
			case 3: shelf = &modulePool.memory[module].huge;break;\
			default:halloc_log("get shelf was fault\n");break;\
	}\
}while(0)

/******************************************************************
*   Name：内存分配层接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 获得链表最大内存值 
* @param [in]  freegood  闲置内存块链表
* @retval 返回最大值  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_alayer_get_list_maximum(stack_head_t *list)
{
	if(stack_get_current_member(list) == 0) return 0;
	block_t *block = list_takeout_struct(list_get_tail_node(list), block_t, blockNode);
	return block->blockSize;
}

/**
* @brief 插入huge block
* @param [in]  freegood  货区
* @param [in]  insert  插入内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_alayer_insert_huge_block(shelf_t *shelf, block_t *insert)
{
	block_t *block = NULL;
	stack_head_t *freegood = &shelf->freegood;
	if(stack_get_current_member(freegood)==0 || insert->blockSize >= _alayer_get_list_maximum(freegood)){//链表为空或者插入的为最大值，则直接插入
		CHECK_FF(list_insert_node(INSERT_TO_TAIL, freegood, &insert->blockNode));
	}else{
		traversal_list_do(block, freegood, blockNode){ 
			if(block->blockSize >= insert->blockSize){
				CHECK_FF(list_embed_node(INSERT_TO_HEAD, freegood, &block->blockNode, &insert->blockNode));
				goto success;
			}
		}while_traversal_list(freegood);
	}
	success:
		shelf->freeMemSize = shelf->freeMemSize + insert->blockSize;
		return HALLOC_SUCC;
}

/**
* @brief 获得货架
* @param [in]  module  模块
* @param [in]  size    内存大小
* @retval 返回货架  表示成功
* @retval HALLOC_FAIL  表示失败
*/
shelf_t *
_alayer_get_shelf(uint module, uint_t size)
{
	shelf_t *shelf   = NULL;//货架
	int_t shelflayer = 0;//货架某层
	if(size > 2048) {//从huge层找到仓号
		shelflayer = 18;
	}else{
		for(shelflayer=0; shelflayer<18; shelflayer++)
			if(blockTable[shelflayer][BLOCK_SIZE] >= size) break;
	}
	//获得内存池中对应货架
	_alayer_return_shelf(shelf, module, shelflayer);
	return shelf;
}


/**
* @brief 获得申请内存块数量
		giveBack值和applyLevel值
		当对于内存块申请和释放是基于这两个值。applyLevel值每次增加等级申请数量将会翻倍，递增2->4->8，当等级大
		于20时候申请数量将位置不变，只递增等级。当每次释放完giveBack的值时候，递减一次等级。
			- 每次内存块被使用完了向管理层申请时候，先applyLevel获得内存块量再减去 giveBack获得已释放内存块数量
			- 更新applyLevel再向管理层申请内存块，申请成功后更新giveBack值，这个值为新applyLevel获得内存块量减去已释放内存块数量
* @param [in]  shelf  货架
* @retval 申请数量  表示成功
* @retval HALLOC_FAIL  表示失败
*/
uint_t
_alayer_malloc_sum(shelf_t *shelf)
{
	if(shelf->layer == HUGE_SHELF) return 1;
	switch(shelf->applyLevel/10){
		case 0:return 2;//giveBack=0-9 返回申请数量2
		case 1:return 4;//iveBack=10-19 返回申请数量4
		default:return 8;//iveBack=20~ 返回申请数量8
	}
}


/**
* @brief 释放内存块
* @param [in]  shelf  货架
* @param [in]  block  内存块
* @param [in]  freegood  闲置货物
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_alayer_free_block(shelf_t *shelf, stack_head_t *freegood, block_t * block)
{
	//释放内存块到管理层
	CHECK_FF(list_delete_node(freegood, &block->blockNode));
	//CHECK_FF(mlayer_free(block));
	shelf->memSize = shelf->memSize - block->blockSize;
	if(shelf->layer != HUGE_SHELF){
		if((shelf->giveBack-1) <= 0 && (shelf->giveBack-1) >= 0) shelf->applyLevel--;
		shelf->giveBack = shelf->giveBack-1>=0?shelf->giveBack-1:_alayer_malloc_sum(shelf)-1;
	}
	return HALLOC_SUCC;
}

/**
* @brief 管理货架
* @param [in]  shelf    货架
* @param [in]  memSize  模块内存大小
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_alayer_manager_shelf(shelf_t *shelf, uint_t *moduleMem)
{
	block_t *block = NULL;
	stack_head_t collect;//收集内存块
	stack_head_t *freegood = &shelf->freegood;
	stack_head_t *returngood = &shelf->returngood;
	stack_init(&collect);//收集内存块栈初始化

	//returngood处理
	//除了huge shelf，其他的都是用returngood接收处理外部返回参数
	if(shelf->layer != HUGE_SHELF && shelf->returnBlocks > 0){
		pthread_mutex_lock(&shelf->returnLock);
		traversal_list_do(block, returngood, blockNode){
			if(block->color == BLOCK_YELLOW){
				if(block->lease >= LEASE_TIME){//内存块超过租赁时间，释放回管理层
					if(HALLOC_SUCC != _alayer_free_block(shelf, returngood, block)) continue;
				}else{//回收内存块
					if(HALLOC_SUCC != list_delete_node(returngood, &block->blockNode)) continue;
					CHECK_FL(stack_push(&collect, &block->blockNode));
				}
				shelf->returnBlocks--;
			}	
		}while_traversal_list(returngood);
		pthread_mutex_unlock(&shelf->returnLock); 
	}
	//freegood处理
	pthread_mutex_lock(&shelf->freeLock);
	traversal_list_do(block, freegood, blockNode){ 
		if(block->lease >= LEASE_TIME) CHECK_FL(_alayer_free_block(shelf, freegood, block));
	}while_traversal_list(freegood);
	//将small middle big 的shelf的闲置block放入到freeblock
	if(shelf->layer != HUGE_SHELF && stack_get_current_member(&collect) != 0){
			CHECK_FL(stack_push_list(freegood, &collect, stack_get_current_member(&collect)));
			shelf->freeMemSize = stack_get_current_member(freegood)*blockTable[shelf->layer][BLOCK_SIZE];
	}
	pthread_mutex_unlock(&shelf->freeLock); 

	*moduleMem = *moduleMem + shelf->memSize;
	return HALLOC_SUCC;
}

/**
* @brief 定时器处理函数（管理员）
		监管员流程
		1.	查看归还链表是否有内存块，有则放入到的空闲链表
		2.	查看当前闲置内存块数量是否大于giveBack +1，如果是则释放giveBack值到管理层，aoolyLevel等级减一。
		3.	查看剩下的内存块是否有超时有超时则释放并更新giveBack值
		4. 如果是huge货架，不进行2 3步骤，而是直接遍历找到超时的就释放。
* @param [in]  value    传入值
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
void
alayer_supervisor(union sigval value)
{
	int_t module     = 0;//模块
	shelf_t *shelf   = NULL;//货架
	int_t shelflayer = 0;//货架某层
	halloc_log("alyer supervisor start\n");
	//循环遍历各模块
	for(module=0; module<MOD_BUTT; module++){
		if(modulePool.memory[module].dynamicMem == 0) continue;
		modulePool.memory[module].dynamicMem = 0;//清空重新计算
		for(shelflayer=0; shelflayer<19; shelflayer++){
			_alayer_return_shelf(shelf, module, shelflayer);
			if(shelf->memSize == 0) continue;
			CHECK_FL(_alayer_manager_shelf(shelf, &modulePool.memory[module].dynamicMem));
		}
	}

}


/**
* @brief 向系统层申请block 
* @param [in]  shelf  货架
* @param [in]  msize   内存大小
* @retval 返回内存块  表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t *
_alayer_malloc_block(shelf_t *shelf, uint_t msize)
{
	list_head_t blockList;
	block_t *block = NULL;//内存块
	list_node_t *node = NULL;
	uint_t blockinfo[5];//内存块信息
	stack_head_t *freegood = &shelf->freegood;
	uint_t index;
	stack_init(&blockList);
	//获得内存块基础信息
	for(index=0;index<5;index++) blockinfo[index] = blockTable[shelf->layer][index];

	//申请内存块
	if(shelf->layer == HUGE_SHELF){
		blockinfo[BLOCK_SIZE] = msize;
		blockinfo[BLOCK_MSIZE] = msize;
		blockinfo[PGAE_SIZE] = SWITCH_TO_KB(msize);
	}else{
		blockinfo[BLOCK_MSIZE] = blockTable[shelf->layer][BLOCK_SIZE]*_alayer_malloc_sum(shelf);
	}
	CHECK_FN(mlayer_dynamic_malloc(&blockList, blockinfo));
	//获得block
	node = stack_pop(&blockList);
	block = list_takeout_struct(node, block_t, blockNode);
	if(shelf->layer != HUGE_SHELF){
			CHECK_FG(stack_push_list(freegood, &blockList, stack_get_current_member(&blockList)), get_fail);
			shelf->applyLevel++;
	}

	shelf->memSize = shelf->memSize + _alayer_malloc_sum(shelf)*block->blockSize;
	shelf->freeMemSize = shelf->freeMemSize + block->blockSize*(_alayer_malloc_sum(shelf)-1);
	return block;

	get_fail:
		traversal_list_do(block, &blockList, blockNode){ 
			CHECK_FL(mlayer_free(block));
		}while_traversal_list(&blockList);
		return HALLOC_NULL;

}

/**
* @brief 租用内存
* @param [in]  shelf  货架
* @param [in]  msize   内存大小
* @retval 返回内存块  表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t *
_alayer_get_shelf_block(shelf_t *shelf, uint_t msize)
{
	block_t *block = NULL;//内存块
	stack_head_t *freegood = &shelf->freegood;
	stack_head_t *returngood = &shelf->returngood;

	//从回收链表中获取
	if(shelf->returnBlocks > 0){
		pthread_mutex_lock(&shelf->returnLock);
		traversal_list_do(block, returngood, blockNode){
			if(block->blockSize >= msize){
				if(HALLOC_SUCC != list_delete_node(returngood, &block->blockNode)) continue;
				shelf->freeMemSize = shelf->freeMemSize - block->blockSize;
				pthread_mutex_unlock(&shelf->returnLock); 
				goto success;
			}
		}while_traversal_list(returngood);
		pthread_mutex_unlock(&shelf->returnLock); 
	}
	//freegood寻找内存块
	pthread_mutex_lock(&shelf->freeLock);
	if( msize <= _alayer_get_list_maximum(freegood)){//当有内存块合适，进入寻找
		traversal_list_do(block, freegood, blockNode){
			if(msize <= block->blockSize && block->blockSize <= msize + 1*KB){
				if(HALLOC_SUCC != list_delete_node(freegood, &block->blockNode)) continue;
				shelf->freeMemSize = shelf->freeMemSize - block->blockSize;
				pthread_mutex_unlock(&shelf->freeLock); 
				goto success;
			}
		}while_traversal_list(freegood);
	}
	//从内存管理层申请
	CHECK_NN(block = _alayer_malloc_block(shelf, msize));
	pthread_mutex_unlock(&shelf->freeLock); 
	success:
		return block;
}



/**
* @brief 租用内存
* @param [in]  moduleMem  模块内存管理
* @param [in]  msize   内存大小
* @param [in]  mtype   内存类型
	- MEM_DYNAMIC 动态内存
	- MEM_STATIC 静态内存
	- MEM_THREAD_STACK 线程栈内存
* @retval 返回内存块  表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t *
_alayer_lease_block(moduleMem_t * moduleMem, uint_t msize, uint_t mtype)
{
	shelf_t *shelf = NULL;//货架
	block_t *block = NULL;//内存块
	switch(mtype){
		case MEM_DYNAMIC://获取动态内存
			CHECK_NN(shelf = _alayer_get_shelf(moduleMem->module, SWITCH_TO_BLOCKSIZE(msize)));
			uint_t memsize = shelf->memSize;
			CHECK_NN(block = _alayer_get_shelf_block(shelf, SWITCH_TO_BLOCKSIZE(msize)));
			pthread_mutex_lock(&moduleMem->moduleLock);
			moduleMem->dynamicMem = moduleMem->dynamicMem -memsize + shelf->memSize; 
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_STATIC://获取静态内存
			CHECK_NN(block =mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(msize)));
			pthread_mutex_lock(&moduleMem->moduleLock);
			moduleMem->staticMem = moduleMem->staticMem + block->blockSize; 
			CHECK_FG(stack_push(&moduleMem->staticList, &block->blockNode), push_fail);
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_THREAD_STACK://获取线程栈
			CHECK_NN(block =mlayer_malloc_thread_stack(SWITCH_TO_KB(msize)));
			pthread_mutex_lock(&moduleMem->moduleLock); 
    		moduleMem->threadMem = moduleMem->threadMem + block->blockSize;
			CHECK_FG(stack_push(&moduleMem->threadList, &block->blockNode), push_fail);
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
	}

	return block;
	push_fail:
			pthread_mutex_unlock(&moduleMem->moduleLock); 
			CHECK_FL(mlayer_free(block));
			return HALLOC_NULL;
}

/**
* @brief 获取内存
* @param [in]  msize   内存大小
* @param [in]  mtype   内存类型
	- MEM_DYNAMIC 动态内存
	- MEM_STATIC 静态内存
	- MEM_THREAD_STACK 线程栈内存
* @param [in]  module  模块类型
* @param [in]  file    文件名
* @param [in]  func    函数名
* @param [in]  line    行号
* @retval 返回内存指针  表示成功
* @retval HALLOC_NULL  表示失败
*/
void * 
alayer_malloc(uint_t msize, uint_t mtype, uint_t module, const char * file, const char * func, int_t line)
{
	if(msize >= (PAGE_MAX_SIZE)*KB-sizeof(checkinfo_t)) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if(module >= MOD_BUTT)  RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if(mtype > MEM_THREAD_STACK)  RETURN_ERR(ERR_PARA, HALLOC_NULL);
	block_t * block;
	CHECK_NN(block = _alayer_lease_block(&modulePool.memory[module], msize, mtype));
	block->module = module;
	block->line  = line;
	block->file  = (string_t)file;
	block->func  = (string_t)func;
	block->color = BLOCK_RED;
	return get_malloc_addr(block->headAddr);
}


/**
* @brief 检查越界
* @param [in]  addr     内存地址
* @retval 返回内存块     表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t *
_alayer_check_overflow(void *addr)
{
    checkinfo_t  *headcheck = (checkinfo_t *)((char *)addr - sizeof(checkinfo_t));
	block_t *block, *nextBlock;
    if(MEMISOVERFLOW(headcheck)) goto overflow;
	block = headcheck->block;
	//检查节点的下一个节点有无泄漏，有可能是本身内存块泄漏的
	nextBlock = list_takeout_struct(list_get_next_node(&block->node), block_t, node);
	headcheck = (checkinfo_t *)(nextBlock->headAddr);
	if(MEMISOVERFLOW(headcheck)) goto overflow;
	return block;
	overflow:
		if(HALLOC_SUCC ==  mlayer_check_overflow()){
			halloc_log("the pointer->%p was invalid\n", addr);
		}
		return HALLOC_NULL;

}

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
alayer_free(void * memory)
{
	if(memory == NULL)  RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	block_t * block         = NULL; //内存块
	shelf_t *shelf          = NULL;//货架
	moduleMem_t * moduleMem = NULL;//模块内存管理
	//内存越界检测
	CHECK_NF(block = _alayer_check_overflow(memory));
	//回收内存
	moduleMem = &modulePool.memory[block->module];
	switch(block->type){
		case MEM_DYNAMIC://处理动态内存
			CHECK_NF(shelf = _alayer_get_shelf(block->module, block->blockSize));
			block->color = BLOCK_YELLOW;
			//除了huge shelf，其他的都是用returnBlocks参数记录外层返回block数量
			if(shelf->layer != HUGE_SHELF){
				pthread_mutex_lock(&shelf->returnLock);
				CHECK_FG(stack_push(&shelf->returngood, &block->blockNode), push_fail);
				shelf->returnBlocks++;
				pthread_mutex_unlock(&shelf->returnLock); 
			}else{// huge shelf
				pthread_mutex_lock(&shelf->freeLock);
				_alayer_insert_huge_block(shelf, block);
				pthread_mutex_unlock(&shelf->freeLock); 
			}
		break;
		case MEM_STATIC://处理静态内存
			pthread_mutex_lock(&moduleMem->moduleLock);
			CHECK_FG(list_delete_node(&moduleMem->staticList, &block->blockNode), delete_fail);
			moduleMem->staticMem = moduleMem->staticMem - block->blockSize; 
			mlayer_free(block);
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_THREAD_STACK://处理线程栈
			pthread_mutex_lock(&moduleMem->moduleLock); 
			CHECK_FG(list_delete_node(&moduleMem->threadList, &block->blockNode), delete_fail);
    		moduleMem->threadMem = moduleMem->threadMem - block->blockSize;
			mlayer_free(block);
			pthread_mutex_unlock(&moduleMem->moduleLock);
		break;
	}
	return HALLOC_SUCC;
	push_fail:
	delete_fail:
		pthread_mutex_unlock(&moduleMem->moduleLock);
		halloc_log("free the block(module:%d file:%s line:%d func:%s) was fault\n", \
			block->module, block->file, block->line, block->func);
		return HALLOC_FAIL;
}

/**
* @brief 初始化内存分配层的单个模块
* @param [in]  module  初始化模块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_init_module(moduleMem_t * moduleMem)
{
	shelf_t *shelf   = NULL;//货架
	int_t layer;
	//模块内存管理初始化
	list_init(&moduleMem->staticList);//静态链表
	list_init(&moduleMem->staticList);//线程栈链表  
	pthread_mutex_init(&moduleMem->moduleLock,NULL);//内存块锁
	//货架初始化
	for(layer=0;layer<19;layer++) {
		_alayer_return_shelf(shelf, moduleMem->module, layer);
		shelf->layer       = layer; //货架层
		shelf->giveBack    = 0;     //归还数量
		shelf->applyLevel  = 0;     //申请等级
		shelf->returnBlocks= 0;     //归还内存块数量
		stack_init(&shelf->freegood);//空闲货物
		stack_init(&shelf->returngood);//归还货物
		pthread_mutex_init(&shelf->returnLock,NULL);//归还内存块锁 
		pthread_mutex_init(&shelf->freeLock,NULL);//闲置内存块锁 
	}
	return HALLOC_SUCC;
}


/**
* @brief 初始化内存分配层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_init(void)
{
    timerEvent_t event;
    struct timespec times;
	int_t module;

    event.clock = CLOCK_REALTIME;
    event.timerNotify = TIMER_NOTIFY_CALLBACK;
	event.callbackFunc= alayer_supervisor;
	//模块池初始化
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_open(&modulePool.supervisor,&event)); //监管员 监管内存越界等
	CHECK_FF(timer_start(modulePool.supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	//模块池每个内存管理初始化
	for(module=0;module<MOD_BUTT;module++) {
		modulePool.memory[module].dynamicMem = 0;
		modulePool.memory[module].module = module;
		alayer_init_module(&modulePool.memory[module]);
	}
	return HALLOC_SUCC;
}



/**
* @brief 摧毁内存分配层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
alayer_destroy(void)
{
	//摧毁管理员
	CHECK_FL(timer_stop(modulePool.supervisor));//关闭定时器
	CHECK_FF(timer_close(&modulePool.supervisor)); //监管员关闭
    return HALLOC_SUCC;
}

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
alayer_get_meminfo(uint_t module, uint infoType)
{
	switch(infoType){
		case ALAYER_STATIC_MEM:return modulePool.memory[module].staticMem;
		case ALAYER_STACK_MEM:return modulePool.memory[module].threadMem;
		case ALAYER_DYNAMIC_MEM:return modulePool.memory[module].dynamicMem;
	}
	return HALLOC_FAIL;
}