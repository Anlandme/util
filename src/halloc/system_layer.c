/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: system_layer.c
@brief: 系统层上主要是向系统申请内存页，以及管理空闲内存页，和向资源管理层回收发内存页
        回内存池和发放内存页使用。本层主要四个函数，和两个被上层调用函数，还有初始化和
        释放slayer内存管理函数，一共八个主要部分。
        第一个部分：处理剪纸算法函数，主要是对于页的裁剪操作
        第二个部分：处理碎纸算法函数，主要是对于大页的操作
        第三个部分：页结构体申请和释放
        第四个部分：内存块结构体申请
        第五个部分：上层调用的申请页函数
        第六个部分：上层调用的释放页函数
        第七个部分：halloc初始化函数
        第八个部分：halloc摧毁函数
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

/**  @brief 内存池管理器 */
mempool_t Pool;

/**  @brief 闲置内存页管理*/
freepages_t FreeTable;
/******************************************************************
*   Name：系统层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 碎纸
* @param [in]  newPage  新页
* @param [in]  tatterAddr  新页地址
* @param [in]  newpageSize 新页大小
* @param [in]  pageSize    待切割页大小
*/
#define _slayer_tatter_page(newPage, tatterAddr, newpageSize, pagesize)\
do{\
    int_t tatterSize= pagesize < newpageSize?pagesize:newpageSize;\
	newPage->headAddr   = tatterAddr;\
	newPage->pageSize   = tatterSize;\
	newPage->unusedSize = tatterSize;\
	CHECK_FF(_slayer_recover_page(newPage));\
	if(pagesize < newpageSize) return HALLOC_SUCC;\
	CHECK_NF(newPage = _slayer_get_page());\
	tatterAddr = MEM_OFFSET(tatterAddr, newpageSize*KB);\
	pagesize = pagesize - newpageSize;\
}while(0)

/**
* @brief 初始化内存
* @param [in]  block  内存块
* @param [in]  addr  内存地址
*/
#define _mem_init(block, addr)\
do{\
    checkinfo_t * info = (checkinfo_t *)addr;\
    info->headcheck = CHECKCODE;\
    info->block = block;\
	info->tailcheck = CHECKCODE;\
}while(0)
/******************************************************************
*   Name：底层函接口宏
*   Desc：可以被内部和外部调用的接口宏
******************************************************************/
/**
* @brief 向底层申请内存
* @param [in]  size     内存大小
* @retval 返回内存首地址 表示成功
* @retval HALLOC_NULL   表示失败
*/
void *
blayer_malloc(uint size)
{
	void *memory;
	if( size > Pool.baseMem.used) {
		halloc_log("not memory could malloc\n");
		return HALLOC_NULL;
	}
	memory = Pool.baseMem.unusedAddr;
	Pool.baseMem.unusedAddr = MEM_OFFSET(Pool.baseMem.unusedAddr ,size);
	Pool.baseMem.used = Pool.baseMem.used - size;
	return memory;
}
/******************************************************************
*   Name：系统层函接口
*   Desc：可以被内部和外部调用的接口
******************************************************************/
/**
* @brief 获得全部内存的头地址
* @retval 返回内存首地址 表示成功
* @retval HALLOC_NULL   表示失败
*/
void *
slayer_get_headaddr(void)
{
	return Pool.baseMem.headAddr;
}

/**
* @brief 初始化内存块
* @param [in]  block    内存块
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_init_block(block_t *block)
{
	/* 公共参数 */
	block->blockSize= 0;			   // 内存块大小
    block->headAddr = NULL;            // 地址头
    block->lease    = 0;        	   // 内存块租用时间 租赁算法使用
    block->color    = BLOCK_WHITE;     // 绿色-等待被回收 黄色-分配层使用 红色-外部模块使用 白色-无人使用
	block->type     = MEM_NONE;		   // 内存块类型
    list_node_init(&block->blockNode); // 内存块节点 位于其他链表
    /* 资源管理层使用参数 */
    /* 资源分配层使用参数 */
    /* 内存泄漏使用 */
	block->checkSign = 0;			   // 泄漏检查标记
	list_node_init(&block->blackNode); // 黑名单节点
	return HALLOC_SUCC;
}


/**
* @brief 初始化内存页
* @param [in]  page      内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_init_page(page_t * page)
{
    /* 公共参数 */
	page->brotherflag= 0;			  // 哥哥标志
	page->usedflag   = NON_USED;	  // 使用标志
    page->mallocpage = MALLOC_INIT;   // malloc申请页
    page->unusedAddr = NULL;          // 指向未被使用的地址
    page->headAddr   = NULL;          // 地址头
    list_node_init(&page->pageNode);  // 页节点
    /* 系统层使用参数 */
    page->pageSize   = 0;          		// 纸张大小
    page->unusedSize = 0;        		// 未被使用的大小 单位字节
    page->brother    = NULL;         	// 另一张裁剪处理的兄弟纸张 
    list_node_init(&page->destroyNode);  // 销毁节点 销毁内存池使用
    /* 资源管理层使用参数 */
    page->maxTime    = 0;         // 当前占用时间最长的内存块时间值 
	page->freeBlocks = 0;		  // 闲置内存块数量
	pthread_mutex_init(&page->lock,NULL); //页锁
    list_init(&page->blocksList); // 内存块链表 
	return HALLOC_SUCC;
}

/**
* @brief 解析出page_t 或者 block_t
* @param [in]  stack     pab栈
* @param [in]  pageSum   内存页数量
* @param [in]  blockSum  内存块数量
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_parse_pad(pabStack_t * stack, uint_t *mallocSize, uint_t pageSum, uint_t blockSum)
{
	int_t index = 0;
	
	if(1 == pageSum){
		page_t *page = (page_t *)stack->stackSP;
		_slayer_init_page(page);
		CHECK_FF(stack_push(&Pool.pageAndBlock.freePageStack, &page->pageNode));
	}else if(0 != blockSum){
		block_t *block = (block_t *)stack->stackSP;//获取地址中第一个内存块
		list_head_t list; //存储?新的内存块的链表
		list_init(&list);
		for(index=0; index<blockSum; index++) {//初始化内存块后放到链表中
			_slayer_init_block(&block[index]);
			list_node_init(&block[index].node);
			CHECK_FF(list_insert_node(INSERT_TO_HEAD, &list, &block[index].node));
		}
		//将获得的所有链表放入到闲置内存块栈当中
		CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
	}
	stack->stackSP = MEM_OFFSET(stack->stackSP ,*mallocSize);
	stack->used    = stack->used + blockSum*sizeof(block_t) + pageSum*sizeof(page_t);
	*mallocSize    = *mallocSize - blockSum*sizeof(block_t)- pageSum*sizeof(page_t);
	return HALLOC_SUCC;
}

/**
* @brief 向内核获取内存 用于PAB栈获取8kb大小栈
* @retval 返回新栈  表示成功
* @retval HALLOC_FAIL  表示失败
*/
pabStack_t *
_slayer_malloc_stack(void)
{
	pabStack_t *stack = NULL; //pad栈结构体指针
	stack = (pabStack_t *)malloc(PAB_STACK_SIZE);
	stack->stackSP = MEM_OFFSET(stack, sizeof(pabStack_t));
	stack->used    = sizeof(pabStack_t);
	list_node_init(&stack->PABnode);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &Pool.pageAndBlock.PABStackList, &(stack->PABnode)),insert_fail);//插入节点
	return stack;
	insert_fail:
		free(stack);
		return HALLOC_NULL;
}


/**
* @brief 向pageAndBlock栈获取page 或者 block
* @param [in]  pageSum   内存页数量
* @param [in]  blockSum  内存块数量
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
#define stack_block_sum(used) (PAB_STACK_SIZE-used)/sizeof(block_t) //当前栈可获取block数量
#define stack_page_sum(used) (PAB_STACK_SIZE-used)/sizeof(page_t) //当前栈可获取page数量
int_t
_slayer_malloc_pab(uint_t pageSum, uint_t blockSum)
{
	uint_t mallocSize  = sizeof(page_t)*pageSum + sizeof(block_t)*blockSum;//申请内存的大小 因为其中一个为0，所以可以计算另一个值
	uint_t curBlock	   = blockSum; //block数量
	uint_t mallocBlock = 0; //申请block数量
    pabStack_t *stack  = NULL; //pab栈结构体指针
    list_node_t *tmp   = NULL; //内存块链表的节点
    stack_head_t *list = &Pool.pageAndBlock.PABStackList; //pab栈链表
	//获取内存块或者内存页结构体
	if( stack_get_current_member(list) != 0 ){
		tmp = list_get_head_node(list); 
		do{
			stack = list_takeout_struct(tmp, pabStack_t, PABnode);
			if(stack_block_sum(stack->used)!=0 && stack_page_sum(stack->used)!=0){
				if( 0 != blockSum ) {
					//计算出当前还有向内部申请的内存块数量
					mallocBlock = stack_block_sum(stack->used) >= curBlock?curBlock:stack_block_sum(stack->used);
					curBlock = curBlock - mallocBlock;
				}
				CHECK_FL(_slayer_parse_pad(stack, &mallocSize, pageSum, mallocBlock));
			}
			if(0 == mallocSize) goto malloc_succ;
			tmp = list_get_next_node(tmp);
		}while(tmp != list_get_head_node(list));
	}
	//如果没有则申请新的栈
	mallocBlock = curBlock;
	CHECK_NF(stack = _slayer_malloc_stack());
	CHECK_FF(_slayer_parse_pad(stack, &mallocSize, pageSum, mallocBlock));


	malloc_succ:
		return HALLOC_SUCC;
}


/**
* @brief 初始化内存块链表
* @param [in]  list     内存块链表
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_init_block_list(page_t * page)
{
    block_t *block     = NULL;	//获取内存块指针
    list_node_t *tmp   = NULL; //内存块链表的节点
    stack_head_t *list = &page->blocksList; //内存块链表
	void *offsetAddr   = page->headAddr; //内存块位移地址
	int_t offset 	   = (page->pageSize*KB)/stack_get_current_member(&page->blocksList); //计算每次偏移距离
    //顺时针遍历内存块并初始化
    tmp = list_get_head_node(list); 
    do{
        block = list_takeout_struct(tmp, block_t, node);
        _slayer_init_block(block);
		block->headAddr = offsetAddr;
		block->blockSize= offset;
		block->page = page;
		_mem_init(block, offsetAddr);
		offsetAddr = MEM_OFFSET(offsetAddr ,offset); //地址偏移
        tmp = list_get_next_node(tmp);
    }while(tmp != list_get_head_node(list));
	return HALLOC_SUCC;
}



/**
* @brief 获归还内存块链表
* @param [in]  page      内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_return_block_list(page_t * page)
{
	uint_t number =stack_get_current_member(&page->blocksList);//内存块栈上内存块数量
	CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &page->blocksList, number));
	return HALLOC_SUCC;
}

/**
* @brief 获取内存块链表
* @param [in]  page      内存页
* @param [in]  blockSum  内存数量
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_get_block_list(page_t * page, uint_t blockSum)
{
	if(0 == blockSum) goto success;
	uint_t number   = stack_get_current_member(&Pool.pageAndBlock.freeBlockStack);//闲置内存块栈上内存块数量
	stack_head_t list;
	list_init(&list);
	if( number < blockSum ) CHECK_FF(_slayer_malloc_pab(0,blockSum-number));//需要向PABStackList栈内获取剩下的内存块
	//获取内存块到内存页中冰初始化
	CHECK_FF(stack_pop_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
	CHECK_FG(stack_push_list(&page->blocksList, &list, blockSum), list_fail);
	_slayer_init_block_list(page);
	//将内存块地址初始化
	success:
		page->freeBlocks = blockSum;
		return HALLOC_SUCC;

	list_fail:
		CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
		return HALLOC_FAIL;
}


/**
* @brief 获取静态内存块
* @param [in]  page      内存页
* @param [in]  blockSize 内存块大小
* @retval 返回内存块    表示成功
* @retval HALLOC_NULL  表示失败
*/
block_t *
slayer_get_static_block(page_t * page, uint_t blockSize)
{
	if(HALLOC_NULL == page|| 0 == blockSize) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if( page->unusedSize < blockSize ) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	uint_t number      = stack_get_current_member(&Pool.pageAndBlock.freeBlockStack);//闲置内存块栈上内存块数量
    block_t *block     = NULL;	//获取内存块指针
    list_node_t *tmp   = NULL; //内存块链表的节点
	void *offsetAddr   = page->unusedAddr; //内存块位移地址
	int_t offset 	   = blockSize; //计算每次偏移距离

	if( number == 0 ) CHECK_FN(_slayer_malloc_pab(0,1));//需要向PABStackList栈内获取剩下的内存块
	CHECK_NN(tmp = stack_pop(&Pool.pageAndBlock.freeBlockStack));//出栈
	//内存块初始化
	block = list_takeout_struct(tmp, block_t, node);
    _slayer_init_block(block);
	block->headAddr = offsetAddr;
	block->blockSize= offset;
	block->page = page;
	_mem_init(block, offsetAddr);
	page->unusedAddr = MEM_OFFSET(offsetAddr ,offset); //地址偏移
	CHECK_FG(stack_push(&page->blocksList, tmp), list_fail);
	page->freeBlocks++;
	return block;

	list_fail:
		CHECK_FN(stack_push(&Pool.pageAndBlock.freeBlockStack, tmp));
		return HALLOC_NULL;
}

/**
* @brief 获归还一个内存页
* @param [in]  page     内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_return_page(page_t *page)
{
	CHECK_FF(stack_push(&Pool.pageAndBlock.freePageStack, &page->pageNode));
	_slayer_init_page(page);
	return HALLOC_SUCC;
}

/**
* @brief 回收一个内存页到空闲链表
* @param [in]  page     内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_recover_page(page_t *page)
{
    /* 公共参数 */
	page->usedflag   = NON_USED;	  // 使用标志
    page->mallocpage = MALLOC_INIT;   // malloc申请页
    page->unusedAddr = page->headAddr;// 指向未被使用的地址
    /* 系统层使用参数 */
    page->unusedSize = page->pageSize;// 未被使用的大小 单位字节
    page->brother    = NULL;          // 另一张裁剪处理的兄弟纸张 
    /* 资源管理层使用参数 */
    page->maxTime    = 0;         // 当前占用时间最长的内存块时间值 
	if( 0 != stack_get_current_member(&page->blocksList)) CHECK_FF(_slayer_return_block_list(page));//归还内存块链表
    list_init(&page->blocksList);
	//将内存页放回到页表
	CHECK_FF(stack_push(&FreeTable.pagesTable[page->pageSize].freePagesList, &page->pageNode));
	return HALLOC_SUCC;
}

/**
* @brief 获取一个内存页
* @retval 获得内存页    表示成功
* @retval HALLOC_NULL  表示失败
*/
page_t *
_slayer_get_page(void)
{
	stack_node_t *node = NULL;
	if(stack_get_current_member(&Pool.pageAndBlock.freePageStack) == 0){
		CHECK_FN(_slayer_malloc_pab(1,0));//获取一个内存页
	}
	CHECK_NN(node = stack_pop(&Pool.pageAndBlock.freePageStack));
	return list_takeout_struct(node, page_t, pageNode);

}



/**
* @brief 向内核获取内存 用于内存的申请
* @param [in]  pageSize 页大小（kb）
* @param [in]  blockSum   内存块数量
* @retval 获得内存页    表示成功
* @retval HALLOC_NULL  表示失败
*/
page_t *
_slayer_malloc(uint_t pageSize, uint_t blockSum)
{
	page_t *page = NULL;
	CHECK_NN(page = _slayer_get_page());
	CHECK_NG(page->headAddr = blayer_malloc(pageSize*KB), malloc_fail);
	//完成页参数赋值
	page->unusedAddr = page->headAddr;
	page->pageSize   = pageSize;
	page->unusedSize = pageSize;
	page->mallocpage = MALLOC_FROM_SYSTEM;
	//将内存加入摧毁链表
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &Pool.destroyHeadList, &page->destroyNode), list_fail);
	CHECK_FG( _slayer_get_block_list(page, blockSum),list_fail);
	Pool.freeMemSize = Pool.freeMemSize + pageSize;
	Pool.memSize = Pool.memSize + pageSize;
	return page;

	list_fail:
		free(page->headAddr);
	malloc_fail:
		_slayer_return_page(page);
		return HALLOC_NULL;
}



/**
* @brief 裁剪内存页
* @param [in]  page 待裁剪内存页
* @param [in]  pageSize 内存页大小
* @param [in]  blockSum   内存块数量
* @retval 获得裁剪后内存页    表示成功
* @retval HALLOC_FAIL  表示失败
*/
page_t *
_slayer_cut_page(page_t * cuted, uint_t pageSize, uint_t blockSum)
{
	page_t * page = NULL;//内存页指针
	CHECK_NN(page = _slayer_get_page());
	//初始化裁剪的页
	page->headAddr = MEM_OFFSET(cuted->headAddr, pageSize*KB);
	page->pageSize   = pageSize;
	page->unusedSize = pageSize;
	CHECK_FG(_slayer_get_block_list(page, blockSum) ,list_fail);
	//初始化被裁剪的页
	cuted->pageSize   = cuted->pageSize - pageSize;
	cuted->unusedSize = cuted->pageSize - pageSize;
	cuted->brotherflag =1;
	cuted->brother = page;
	page->brother = cuted;
	CHECK_FG(stack_push(&FreeTable.pagesTable[cuted->pageSize].freePagesList, &cuted->pageNode), push_fail);
	return page;

	push_fail:
		_slayer_return_block_list(page);//归还block链表
	list_fail:
		_slayer_return_page(page);//归还
		return HALLOC_NULL;
}


/**
* @brief 裁剪内存页合并
* @param [in]  page 待合并内存页
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
_slayer_combine_page(page_t * page)
{
	if( 1 != page->brotherflag && 1 != page->brother->brotherflag) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	page_t * brother = page->brother;//内存页指针
	_slayer_return_block_list(page);//归还block链表
	//将兄弟从原来页表取出
	CHECK_FF(list_delete_node(&FreeTable.pagesTable[brother->pageSize].freePagesList, &brother->pageNode));
	if(1 == brother->brotherflag) {
		brother->pageSize = brother->pageSize + page->pageSize;
		brother->brotherflag =0;
		CHECK_FF(_slayer_recover_page(brother));
		CHECK_FF(_slayer_return_page(page));
	}else{
		page->pageSize = brother->pageSize + page->pageSize;
		page->brotherflag =0;
		CHECK_FF(_slayer_recover_page(page));
		CHECK_FF(_slayer_return_page(brother));
	}
	return HALLOC_SUCC;
}

/**
* @brief 当需要一定面积的page且page小于等于14kb时候，需要选择一张纸并将一张纸切分成两块，
		 为了使减剩下的纸能不被浪费，所以需要最优的裁剪方式。这个寻找最优的纸就是剪纸算法。
		 剪纸算法主要是用于对于空闲页表页的回收再利用，页的高效获取而存在。
		 （1）获取一个内存页时候
		 1. 当获取一个内存页时候，当前页表没有，则需要去查看其它页表有没内存页可以裁剪，
		 	选择哪个内存页呢？根据剪纸算法，需要获得最优纸张，需要查看 freepages_t 结
			构体下的 topMenu 热门榜单看到哪种纸张使用最多。
		 2. 热门榜单内有5个座号，查看榜单的座位是否被占用，如果被占用，则计算获得出最合
		 	适裁剪的纸，如果最适合裁剪的页表为空，则选择下一个榜单座位。遍历所有榜单都
			没有则向系统申请。
		 3. 裁剪页表，跟闲置页表管理结构体申请一个 page_t 结构体用在新的裁剪后的页，被
		 	裁剪的两个裁剪的页 需要 page_t 下的 brother 相互关联为兄弟，
	   	 4. 获取完内存页，然后更新 layering_t 结构体下的 popular 参数，并遍历 freepages_t 
			结构体下的 topMenu 热门榜单，如果 popular 比某个页表的 popular 大，则这个
			榜单下的座位。

		（2）一个内存页被释放回到空闲页表时候
		 1. 查看 page_t 结构体下的 brother 是否有兄弟纸张，有则查看这个纸张有没被使用，
			如果有使用则两个纸张合成一张
		 2. 合成时候比较page_t结构体下 brotherflag，0为弟弟，1为哥哥。将两个page_t结构
		 	体下的 pageSize 页大小相加获得合成纸所在页表，并将相加结果放入到哥哥pageSize 
			中。将弟弟的 page_t 结构体放回到 freepageheadList 链表中。

		（3）topMenu热门榜单
		 1. 榜单是从大到小排序，也就说topMenu[0]的热度大于topMenu[1]
		 2. 在搜索最适合裁剪纸时候是先从最热门的开始找
		 3. 在更新 popular 受欢迎度时候，通过冒泡法检索更新topMenu热门榜单
* @param [in]  pageSize 页大小（kb）
* @param [in]  blockSum   内存块数量
* @retval 获得内存页    表示成功
* @retval HALLOC_FAIL  表示失败
*/
page_t *
slayer_cut_page(uint_t pageSize, uint_t blockSum)
{
	page_t *page       = NULL;//内存页指针
	stack_node_t *node = NULL;//内存块指针
	int_t index        = 0;//topMenu 索引
	int_t topValue     = -1;//top值初始值为-1
	int_t insert       = 0;//插入topMenu榜单的值

	if( stack_get_current_member(&FreeTable.pagesTable[pageSize].freePagesList) == 0 ){//走剪纸算法流程
			for( index=0;index<5; index++ ){//流程1-2
				int_t value = FreeTable.topMenu[index];
				if( (value != -1) && (value+pageSize < PAGE_TABLE_SIZE) 
					&& (stack_get_current_member(&FreeTable.pagesTable[value+pageSize].freePagesList) != 0) ){
					topValue = value+pageSize;//计算最合适裁剪
				}
			}
			//遍历完topMenu
			if(topValue == -1){//没有找到则进行申请
				CHECK_NN(page = _slayer_malloc(pageSize, blockSum));
			}else{//找到则进行裁剪
				CHECK_NN(node = stack_pop(&FreeTable.pagesTable[topValue].freePagesList));
				page = list_takeout_struct(node, page_t, pageNode);//获得待裁剪页
				CHECK_NN(page = _slayer_cut_page(page, pageSize, blockSum));
			}
	}else{//存在则直接获取
		CHECK_NN(node = stack_pop(&FreeTable.pagesTable[pageSize].freePagesList));
		page = list_takeout_struct(node, page_t, pageNode);
		CHECK_FG(_slayer_get_block_list(page, blockSum),list_fail);//失败则goto list_fail
	}
	//更新popular
	FreeTable.pagesTable[pageSize].popular++;
	insert = pageSize;
	for( index=0;index<5; index++ ) {
		int_t value = FreeTable.topMenu[index];
		if( value == -1 || value == pageSize || FreeTable.pagesTable[value].popular<FreeTable.pagesTable[insert].popular){
			FreeTable.topMenu[index] = insert;//替换热门榜单
			if( value == -1 || value == pageSize ) break;//由于-1表示这个位置还是空，替换完后直接退出循环
			insert = value;
		}
	}

	return page;
	list_fail:
		CHECK_FN(stack_push(&FreeTable.pagesTable[pageSize].freePagesList, &page->pageNode));
		return HALLOC_NULL;
}


/**
* @brief 碎纸算法，主要是当超过14k的内存页回到系统层后，这个大片内存页会被浪费，需要分成
		 最优的大小，所以引入了碎纸算法，来分割内存页。主要是借助热榜，高效的将大内存页分
		 割成所需的内存页，增加利用率.
		数组[4,5,6,7,8] 
		这里主要是考虑系统管理层常用的内存页大小为7,5,4,3,2,1,相互组合后结合仓库算法需要
		的不同内存页数量，比较大的出现概率的数，按照这个数组成数组分割目的是更高效的利用
		分割后内存，比如6，有（1，5）（2，4）（3，3）组合，当其中的（1，5）需求量小时候，
		（2，4）或者（3，3）的需求量就极可能加大。所以这样的增加被使用概率。如果直接切割
		成1和5，可能就没有那个好的使用率。
		碎纸方式
		因为高效用纸，所以利用topMenu数组组合得出碎纸大小。例如topMenu = [13,5,7,8,2]
		根据数组为了防止页表溢出，所以限定小于等于7的值参与组合计算，其他则tatter[13]++
		tatter[8]++，然后[5,7,2]组合根据上面数组[4,5,6,7,8] 原理得出（5，5），（5，7）
		（5，2），（7，7），（7，2），（2，2）集中组合，裁剪也按照这个的大小进行遍历裁剪
* @param [in]  page    页结构体
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
slayer_tatter_page(page_t *page)
{
	char_t topGroup[5] = {4,5,6,7,8};//是由上层仓库申请内存计算而来
	char_t tatter[14]  = {0};//切碎长度
	page_t * newPage   = page;//内存页指针
	int_t index,j      = 0; //索引
	void * tatterAddr  = page->headAddr;//碎纸内存页地址
	int_t tatterSize   = 0;//碎纸内存页大小
	int_t pageSize	   = page->pageSize;//内存页大小

	_slayer_return_block_list(page);//归还block链表
	if(-5 == FreeTable.topMenu[0]+FreeTable.topMenu[1]+FreeTable.topMenu[2]+FreeTable.topMenu[3]+FreeTable.topMenu[4] ){//topMenu未使用
		for(index=0; index<5; index){
			index = index%5;
			//切割新页
			_slayer_tatter_page(newPage, tatterAddr, topGroup[index], pageSize);
		}
	}else{//碎纸
		//碎纸计算
		for(index=0;index<5;index++) if( FreeTable.topMenu[index]>=8 ) tatter[FreeTable.topMenu[index]]++;
		for(index=0;index<5;index++) 
			if( FreeTable.topMenu[index]<=7 ) for(j=0;j<index;j++) tatter[FreeTable.topMenu[index]+FreeTable.topMenu[j]]++;

		//开始碎纸
		for(index=0;index<14;){
			index = (++index)%14;
			if(tatter[index] == 0 ) continue;
			for( j=0; j<tatter[index]; j++) _slayer_tatter_page(newPage, tatterAddr, index, pageSize); //切割新页
		}
	}
}



/**
* @brief 向系统层申请一个内存页
* @param [in]  pageSize   内存页大小
* @param [in]  blockSum   内存块数量
* @retval 获得内存页    表示成功
* @retval HALLOC_NULL 表示失败
*/
page_t *
slayer_malloc(uint_t pageSize, uint_t blockSum)
{
	if(INITIALIZED != Pool.slayerInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if((PAGE_MAX_SIZE <= pageSize)||(pageSize == 0 )) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if((BLOCK_MAX_SIZE < blockSum)) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t *page = NULL;
	pthread_mutex_lock(&Pool.poolLock);  
	if(pageSize >= PAGE_TABLE_SIZE){//大于或则等于页表大小 则向系统申请内存
		page = _slayer_malloc(pageSize, blockSum);
	}else if(pageSize < PAGE_TABLE_SIZE){//小于页表大小 则向内存页申请
		page = slayer_cut_page(pageSize, blockSum);
	}
	if(NULL != page) {
		page->usedflag = USED;//内存页将被使用
		Pool.freeMemSize = Pool.freeMemSize - pageSize;
	}
    pthread_mutex_unlock(&Pool.poolLock); 
	return page;
}

/**
* @brief 向系统层释放一个内存页
* @param [in]  pageSize   内存页大小
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
slayer_free(page_t *page)
{
	if(HALLOC_NULL == page) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	uint_t pageSize = page->pageSize;
	pthread_mutex_lock(&Pool.poolLock);
	if(PAGE_TABLE_SIZE > page->pageSize ){//小于页表
		if( NULL != page->brother && NON_USED == page->brother->usedflag) {
			CHECK_FF(_slayer_combine_page(page));
		}else{
			CHECK_FF(_slayer_recover_page(page));
		}
	}else{//大于页表使用碎纸算法
		CHECK_FF(slayer_tatter_page(page));
	}
	Pool.freeMemSize = Pool.freeMemSize + pageSize;
    pthread_mutex_unlock(&Pool.poolLock); 
	return HALLOC_SUCC;
}


/**
* @brief 初始化系统层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
slayer_init(void)
{
	int index; //索引号
	/* 内存池结构体初始化 */
	Pool.slayerInit  = INITIALIZED;
	Pool.memSize     = 0;
	Pool.freeMemSize = 0;
	pthread_mutex_init(&Pool.poolLock,NULL); 
	list_init(&Pool.destroyHeadList);
	list_init(&Pool.pageAndBlock.PABStackList);
	stack_init(&Pool.pageAndBlock.freePageStack);
	stack_init(&Pool.pageAndBlock.freeBlockStack);
	/* 闲置内存结构体初始化 */
	for(index=0; index<5; index++) FreeTable.topMenu[index] = -1;
	for(index=0; index<PAGE_TABLE_SIZE; index++){
		FreeTable.pagesTable[index].pagesNum = 0;
		FreeTable.pagesTable[index].popular  = 0;
		stack_init(&FreeTable.pagesTable[index].freePagesList);
	}
	Pool.baseMem.used = HALLOC_TOTAL_MEMORY;
	Pool.baseMem.headAddr   = malloc(HALLOC_TOTAL_MEMORY);
	Pool.baseMem.unusedAddr = Pool.baseMem.headAddr;
	return HALLOC_SUCC;
}

/**
* @brief 初始化halloc
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
halloc_init(void)
{
	/******************************************************************
	*   Name：系统层初始化
	******************************************************************/
	/* 内存池结构体初始化 */
	CHECK_FF(slayer_init());
	/******************************************************************
	*   Name：内存管理层初始化
	******************************************************************/
	/* 内存管理层初始化 */
	CHECK_FF(mlayer_init());
	/******************************************************************
	*   Name：内存分配层初始化
	******************************************************************/
	/* 内存分配层初始化 */
	CHECK_FF(alayer_init());
	return HALLOC_SUCC;
}


/**
* @brief 摧毁系统层
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
slayer_destroy(void)
{
	int index, delSum; //索引号, 删除数量
	list_node_t *tmp   = NULL; //内存块链表的节点
	pabStack_t *stack  = NULL; //获取栈指针
	page_t *page       = NULL; //内存块指针
	pthread_mutex_lock(&Pool.poolLock); 
    //顺时针删除节点
	delSum = list_get_current_member(&Pool.destroyHeadList);
    for(index=0; index<delSum; index++){
        tmp = list_get_head_node(&Pool.destroyHeadList);
        list_delete_node(&Pool.destroyHeadList, tmp);
        page = list_takeout_struct(tmp, page_t, destroyNode);
        //free(page->headAddr);
    }
    //顺时针删除节点
	delSum = stack_get_current_member(&Pool.pageAndBlock.PABStackList);
    for(index=0; index<delSum; index++){
        tmp = list_get_head_node(&Pool.pageAndBlock.PABStackList);
        list_delete_node(&Pool.pageAndBlock.PABStackList, tmp);
        stack = list_takeout_struct(tmp, pabStack_t, PABnode);
        //free(stack);
    }

	//free(Pool.baseMem.headAddr);
	for(index=0; index<5; index++) FreeTable.topMenu[index] = -1;
    pthread_mutex_unlock(&Pool.poolLock); 
	return HALLOC_SUCC;
}

/**
* @brief 摧毁halloc
* @retval HALLOC_SUCC  表示成功
* @retval HALLOC_FAIL  表示失败
*/
int_t
halloc_destroy(void)
{
	/******************************************************************
	*   Name：内存分配层释放
	******************************************************************/
	CHECK_FF(alayer_destroy());
	/******************************************************************
	*   Name：内存管理层释放
	******************************************************************/
	CHECK_FF(mlayer_destroy());
	/******************************************************************
	*   Name：系统层释放
	******************************************************************/
	CHECK_FF(slayer_destroy());
	return HALLOC_SUCC;
}
