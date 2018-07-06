/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: system_layer.c
@brief: ϵͳ������Ҫ����ϵͳ�����ڴ�ҳ���Լ���������ڴ�ҳ��������Դ�������շ��ڴ�ҳ
        ���ڴ�غͷ����ڴ�ҳʹ�á�������Ҫ�ĸ����������������ϲ���ú��������г�ʼ����
        �ͷ�slayer�ڴ��������һ���˸���Ҫ���֡�
        ��һ�����֣������ֽ�㷨��������Ҫ�Ƕ���ҳ�Ĳü�����
        �ڶ������֣�������ֽ�㷨��������Ҫ�Ƕ��ڴ�ҳ�Ĳ���
        ���������֣�ҳ�ṹ��������ͷ�
        ���ĸ����֣��ڴ��ṹ������
        ��������֣��ϲ���õ�����ҳ����
        ���������֣��ϲ���õ��ͷ�ҳ����
        ���߸����֣�halloc��ʼ������
        �ڰ˸����֣�halloc�ݻٺ���
@attention�� ��ģ������ⲿ�ļ���ģ�� util/utildef.h,utilinc.h errģ��
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/02
-------------------------------------------------------------------------------
  Change History :                                                              
                 | <Date>      | <Version> | <Author> | <Description>                     
-------------------------------------------------------------------------------
  @todo          | 2017/12/18  | 0.0.0.1   | D.C.P    | �����ļ�            
******************************************************************************/
#include ".././util/util_inc.h"
#include "halloc.h"

/**  @brief �ڴ�ع����� */
mempool_t Pool;

/**  @brief �����ڴ�ҳ����*/
freepages_t FreeTable;
/******************************************************************
*   Name��ϵͳ�㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief ��ֽ
* @param [in]  newPage  ��ҳ
* @param [in]  tatterAddr  ��ҳ��ַ
* @param [in]  newpageSize ��ҳ��С
* @param [in]  pageSize    ���и�ҳ��С
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
* @brief ��ʼ���ڴ�
* @param [in]  block  �ڴ��
* @param [in]  addr  �ڴ��ַ
*/
#define _mem_init(block, addr)\
do{\
    checkinfo_t * info = (checkinfo_t *)addr;\
    info->headcheck = CHECKCODE;\
    info->block = block;\
	info->tailcheck = CHECKCODE;\
}while(0)
/******************************************************************
*   Name���ײ㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief ��ײ������ڴ�
* @param [in]  size     �ڴ��С
* @retval �����ڴ��׵�ַ ��ʾ�ɹ�
* @retval HALLOC_NULL   ��ʾʧ��
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
*   Name��ϵͳ�㺯�ӿ�
*   Desc�����Ա��ڲ����ⲿ���õĽӿ�
******************************************************************/
/**
* @brief ���ȫ���ڴ��ͷ��ַ
* @retval �����ڴ��׵�ַ ��ʾ�ɹ�
* @retval HALLOC_NULL   ��ʾʧ��
*/
void *
slayer_get_headaddr(void)
{
	return Pool.baseMem.headAddr;
}

/**
* @brief ��ʼ���ڴ��
* @param [in]  block    �ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_init_block(block_t *block)
{
	/* �������� */
	block->blockSize= 0;			   // �ڴ���С
    block->headAddr = NULL;            // ��ַͷ
    block->lease    = 0;        	   // �ڴ������ʱ�� �����㷨ʹ��
    block->color    = BLOCK_WHITE;     // ��ɫ-�ȴ������� ��ɫ-�����ʹ�� ��ɫ-�ⲿģ��ʹ�� ��ɫ-����ʹ��
	block->type     = MEM_NONE;		   // �ڴ������
    list_node_init(&block->blockNode); // �ڴ��ڵ� λ����������
    /* ��Դ�����ʹ�ò��� */
    /* ��Դ�����ʹ�ò��� */
    /* �ڴ�й©ʹ�� */
	block->checkSign = 0;			   // й©�����
	list_node_init(&block->blackNode); // �������ڵ�
	return HALLOC_SUCC;
}


/**
* @brief ��ʼ���ڴ�ҳ
* @param [in]  page      �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_init_page(page_t * page)
{
    /* �������� */
	page->brotherflag= 0;			  // ����־
	page->usedflag   = NON_USED;	  // ʹ�ñ�־
    page->mallocpage = MALLOC_INIT;   // malloc����ҳ
    page->unusedAddr = NULL;          // ָ��δ��ʹ�õĵ�ַ
    page->headAddr   = NULL;          // ��ַͷ
    list_node_init(&page->pageNode);  // ҳ�ڵ�
    /* ϵͳ��ʹ�ò��� */
    page->pageSize   = 0;          		// ֽ�Ŵ�С
    page->unusedSize = 0;        		// δ��ʹ�õĴ�С ��λ�ֽ�
    page->brother    = NULL;         	// ��һ�Ųü�������ֵ�ֽ�� 
    list_node_init(&page->destroyNode);  // ���ٽڵ� �����ڴ��ʹ��
    /* ��Դ�����ʹ�ò��� */
    page->maxTime    = 0;         // ��ǰռ��ʱ������ڴ��ʱ��ֵ 
	page->freeBlocks = 0;		  // �����ڴ������
	pthread_mutex_init(&page->lock,NULL); //ҳ��
    list_init(&page->blocksList); // �ڴ������ 
	return HALLOC_SUCC;
}

/**
* @brief ������page_t ���� block_t
* @param [in]  stack     pabջ
* @param [in]  pageSum   �ڴ�ҳ����
* @param [in]  blockSum  �ڴ������
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
		block_t *block = (block_t *)stack->stackSP;//��ȡ��ַ�е�һ���ڴ��
		list_head_t list; //�洢?�µ��ڴ�������
		list_init(&list);
		for(index=0; index<blockSum; index++) {//��ʼ���ڴ���ŵ�������
			_slayer_init_block(&block[index]);
			list_node_init(&block[index].node);
			CHECK_FF(list_insert_node(INSERT_TO_HEAD, &list, &block[index].node));
		}
		//����õ�����������뵽�����ڴ��ջ����
		CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
	}
	stack->stackSP = MEM_OFFSET(stack->stackSP ,*mallocSize);
	stack->used    = stack->used + blockSum*sizeof(block_t) + pageSum*sizeof(page_t);
	*mallocSize    = *mallocSize - blockSum*sizeof(block_t)- pageSum*sizeof(page_t);
	return HALLOC_SUCC;
}

/**
* @brief ���ں˻�ȡ�ڴ� ����PABջ��ȡ8kb��Сջ
* @retval ������ջ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
pabStack_t *
_slayer_malloc_stack(void)
{
	pabStack_t *stack = NULL; //padջ�ṹ��ָ��
	stack = (pabStack_t *)malloc(PAB_STACK_SIZE);
	stack->stackSP = MEM_OFFSET(stack, sizeof(pabStack_t));
	stack->used    = sizeof(pabStack_t);
	list_node_init(&stack->PABnode);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &Pool.pageAndBlock.PABStackList, &(stack->PABnode)),insert_fail);//����ڵ�
	return stack;
	insert_fail:
		free(stack);
		return HALLOC_NULL;
}


/**
* @brief ��pageAndBlockջ��ȡpage ���� block
* @param [in]  pageSum   �ڴ�ҳ����
* @param [in]  blockSum  �ڴ������
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
#define stack_block_sum(used) (PAB_STACK_SIZE-used)/sizeof(block_t) //��ǰջ�ɻ�ȡblock����
#define stack_page_sum(used) (PAB_STACK_SIZE-used)/sizeof(page_t) //��ǰջ�ɻ�ȡpage����
int_t
_slayer_malloc_pab(uint_t pageSum, uint_t blockSum)
{
	uint_t mallocSize  = sizeof(page_t)*pageSum + sizeof(block_t)*blockSum;//�����ڴ�Ĵ�С ��Ϊ����һ��Ϊ0�����Կ��Լ�����һ��ֵ
	uint_t curBlock	   = blockSum; //block����
	uint_t mallocBlock = 0; //����block����
    pabStack_t *stack  = NULL; //pabջ�ṹ��ָ��
    list_node_t *tmp   = NULL; //�ڴ������Ľڵ�
    stack_head_t *list = &Pool.pageAndBlock.PABStackList; //pabջ����
	//��ȡ�ڴ������ڴ�ҳ�ṹ��
	if( stack_get_current_member(list) != 0 ){
		tmp = list_get_head_node(list); 
		do{
			stack = list_takeout_struct(tmp, pabStack_t, PABnode);
			if(stack_block_sum(stack->used)!=0 && stack_page_sum(stack->used)!=0){
				if( 0 != blockSum ) {
					//�������ǰ�������ڲ�������ڴ������
					mallocBlock = stack_block_sum(stack->used) >= curBlock?curBlock:stack_block_sum(stack->used);
					curBlock = curBlock - mallocBlock;
				}
				CHECK_FL(_slayer_parse_pad(stack, &mallocSize, pageSum, mallocBlock));
			}
			if(0 == mallocSize) goto malloc_succ;
			tmp = list_get_next_node(tmp);
		}while(tmp != list_get_head_node(list));
	}
	//���û���������µ�ջ
	mallocBlock = curBlock;
	CHECK_NF(stack = _slayer_malloc_stack());
	CHECK_FF(_slayer_parse_pad(stack, &mallocSize, pageSum, mallocBlock));


	malloc_succ:
		return HALLOC_SUCC;
}


/**
* @brief ��ʼ���ڴ������
* @param [in]  list     �ڴ������
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_init_block_list(page_t * page)
{
    block_t *block     = NULL;	//��ȡ�ڴ��ָ��
    list_node_t *tmp   = NULL; //�ڴ������Ľڵ�
    stack_head_t *list = &page->blocksList; //�ڴ������
	void *offsetAddr   = page->headAddr; //�ڴ��λ�Ƶ�ַ
	int_t offset 	   = (page->pageSize*KB)/stack_get_current_member(&page->blocksList); //����ÿ��ƫ�ƾ���
    //˳ʱ������ڴ�鲢��ʼ��
    tmp = list_get_head_node(list); 
    do{
        block = list_takeout_struct(tmp, block_t, node);
        _slayer_init_block(block);
		block->headAddr = offsetAddr;
		block->blockSize= offset;
		block->page = page;
		_mem_init(block, offsetAddr);
		offsetAddr = MEM_OFFSET(offsetAddr ,offset); //��ַƫ��
        tmp = list_get_next_node(tmp);
    }while(tmp != list_get_head_node(list));
	return HALLOC_SUCC;
}



/**
* @brief ��黹�ڴ������
* @param [in]  page      �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_return_block_list(page_t * page)
{
	uint_t number =stack_get_current_member(&page->blocksList);//�ڴ��ջ���ڴ������
	CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &page->blocksList, number));
	return HALLOC_SUCC;
}

/**
* @brief ��ȡ�ڴ������
* @param [in]  page      �ڴ�ҳ
* @param [in]  blockSum  �ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_get_block_list(page_t * page, uint_t blockSum)
{
	if(0 == blockSum) goto success;
	uint_t number   = stack_get_current_member(&Pool.pageAndBlock.freeBlockStack);//�����ڴ��ջ���ڴ������
	stack_head_t list;
	list_init(&list);
	if( number < blockSum ) CHECK_FF(_slayer_malloc_pab(0,blockSum-number));//��Ҫ��PABStackListջ�ڻ�ȡʣ�µ��ڴ��
	//��ȡ�ڴ�鵽�ڴ�ҳ�б���ʼ��
	CHECK_FF(stack_pop_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
	CHECK_FG(stack_push_list(&page->blocksList, &list, blockSum), list_fail);
	_slayer_init_block_list(page);
	//���ڴ���ַ��ʼ��
	success:
		page->freeBlocks = blockSum;
		return HALLOC_SUCC;

	list_fail:
		CHECK_FF(stack_push_list(&Pool.pageAndBlock.freeBlockStack, &list, blockSum));
		return HALLOC_FAIL;
}


/**
* @brief ��ȡ��̬�ڴ��
* @param [in]  page      �ڴ�ҳ
* @param [in]  blockSize �ڴ���С
* @retval �����ڴ��    ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t *
slayer_get_static_block(page_t * page, uint_t blockSize)
{
	if(HALLOC_NULL == page|| 0 == blockSize) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if( page->unusedSize < blockSize ) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	uint_t number      = stack_get_current_member(&Pool.pageAndBlock.freeBlockStack);//�����ڴ��ջ���ڴ������
    block_t *block     = NULL;	//��ȡ�ڴ��ָ��
    list_node_t *tmp   = NULL; //�ڴ������Ľڵ�
	void *offsetAddr   = page->unusedAddr; //�ڴ��λ�Ƶ�ַ
	int_t offset 	   = blockSize; //����ÿ��ƫ�ƾ���

	if( number == 0 ) CHECK_FN(_slayer_malloc_pab(0,1));//��Ҫ��PABStackListջ�ڻ�ȡʣ�µ��ڴ��
	CHECK_NN(tmp = stack_pop(&Pool.pageAndBlock.freeBlockStack));//��ջ
	//�ڴ���ʼ��
	block = list_takeout_struct(tmp, block_t, node);
    _slayer_init_block(block);
	block->headAddr = offsetAddr;
	block->blockSize= offset;
	block->page = page;
	_mem_init(block, offsetAddr);
	page->unusedAddr = MEM_OFFSET(offsetAddr ,offset); //��ַƫ��
	CHECK_FG(stack_push(&page->blocksList, tmp), list_fail);
	page->freeBlocks++;
	return block;

	list_fail:
		CHECK_FN(stack_push(&Pool.pageAndBlock.freeBlockStack, tmp));
		return HALLOC_NULL;
}

/**
* @brief ��黹һ���ڴ�ҳ
* @param [in]  page     �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_return_page(page_t *page)
{
	CHECK_FF(stack_push(&Pool.pageAndBlock.freePageStack, &page->pageNode));
	_slayer_init_page(page);
	return HALLOC_SUCC;
}

/**
* @brief ����һ���ڴ�ҳ����������
* @param [in]  page     �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_recover_page(page_t *page)
{
    /* �������� */
	page->usedflag   = NON_USED;	  // ʹ�ñ�־
    page->mallocpage = MALLOC_INIT;   // malloc����ҳ
    page->unusedAddr = page->headAddr;// ָ��δ��ʹ�õĵ�ַ
    /* ϵͳ��ʹ�ò��� */
    page->unusedSize = page->pageSize;// δ��ʹ�õĴ�С ��λ�ֽ�
    page->brother    = NULL;          // ��һ�Ųü�������ֵ�ֽ�� 
    /* ��Դ�����ʹ�ò��� */
    page->maxTime    = 0;         // ��ǰռ��ʱ������ڴ��ʱ��ֵ 
	if( 0 != stack_get_current_member(&page->blocksList)) CHECK_FF(_slayer_return_block_list(page));//�黹�ڴ������
    list_init(&page->blocksList);
	//���ڴ�ҳ�Żص�ҳ��
	CHECK_FF(stack_push(&FreeTable.pagesTable[page->pageSize].freePagesList, &page->pageNode));
	return HALLOC_SUCC;
}

/**
* @brief ��ȡһ���ڴ�ҳ
* @retval ����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
page_t *
_slayer_get_page(void)
{
	stack_node_t *node = NULL;
	if(stack_get_current_member(&Pool.pageAndBlock.freePageStack) == 0){
		CHECK_FN(_slayer_malloc_pab(1,0));//��ȡһ���ڴ�ҳ
	}
	CHECK_NN(node = stack_pop(&Pool.pageAndBlock.freePageStack));
	return list_takeout_struct(node, page_t, pageNode);

}



/**
* @brief ���ں˻�ȡ�ڴ� �����ڴ������
* @param [in]  pageSize ҳ��С��kb��
* @param [in]  blockSum   �ڴ������
* @retval ����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
page_t *
_slayer_malloc(uint_t pageSize, uint_t blockSum)
{
	page_t *page = NULL;
	CHECK_NN(page = _slayer_get_page());
	CHECK_NG(page->headAddr = blayer_malloc(pageSize*KB), malloc_fail);
	//���ҳ������ֵ
	page->unusedAddr = page->headAddr;
	page->pageSize   = pageSize;
	page->unusedSize = pageSize;
	page->mallocpage = MALLOC_FROM_SYSTEM;
	//���ڴ����ݻ�����
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
* @brief �ü��ڴ�ҳ
* @param [in]  page ���ü��ڴ�ҳ
* @param [in]  pageSize �ڴ�ҳ��С
* @param [in]  blockSum   �ڴ������
* @retval ��òü����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
page_t *
_slayer_cut_page(page_t * cuted, uint_t pageSize, uint_t blockSum)
{
	page_t * page = NULL;//�ڴ�ҳָ��
	CHECK_NN(page = _slayer_get_page());
	//��ʼ���ü���ҳ
	page->headAddr = MEM_OFFSET(cuted->headAddr, pageSize*KB);
	page->pageSize   = pageSize;
	page->unusedSize = pageSize;
	CHECK_FG(_slayer_get_block_list(page, blockSum) ,list_fail);
	//��ʼ�����ü���ҳ
	cuted->pageSize   = cuted->pageSize - pageSize;
	cuted->unusedSize = cuted->pageSize - pageSize;
	cuted->brotherflag =1;
	cuted->brother = page;
	page->brother = cuted;
	CHECK_FG(stack_push(&FreeTable.pagesTable[cuted->pageSize].freePagesList, &cuted->pageNode), push_fail);
	return page;

	push_fail:
		_slayer_return_block_list(page);//�黹block����
	list_fail:
		_slayer_return_page(page);//�黹
		return HALLOC_NULL;
}


/**
* @brief �ü��ڴ�ҳ�ϲ�
* @param [in]  page ���ϲ��ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_slayer_combine_page(page_t * page)
{
	if( 1 != page->brotherflag && 1 != page->brother->brotherflag) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	page_t * brother = page->brother;//�ڴ�ҳָ��
	_slayer_return_block_list(page);//�黹block����
	//���ֵܴ�ԭ��ҳ��ȡ��
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
* @brief ����Ҫһ�������page��pageС�ڵ���14kbʱ����Ҫѡ��һ��ֽ����һ��ֽ�зֳ����飬
		 Ϊ��ʹ��ʣ�µ�ֽ�ܲ����˷ѣ�������Ҫ���ŵĲü���ʽ�����Ѱ�����ŵ�ֽ���Ǽ�ֽ�㷨��
		 ��ֽ�㷨��Ҫ�����ڶ��ڿ���ҳ��ҳ�Ļ��������ã�ҳ�ĸ�Ч��ȡ�����ڡ�
		 ��1����ȡһ���ڴ�ҳʱ��
		 1. ����ȡһ���ڴ�ҳʱ�򣬵�ǰҳ��û�У�����Ҫȥ�鿴����ҳ����û�ڴ�ҳ���Բü���
		 	ѡ���ĸ��ڴ�ҳ�أ����ݼ�ֽ�㷨����Ҫ�������ֽ�ţ���Ҫ�鿴 freepages_t ��
			�����µ� topMenu ���Ű񵥿�������ֽ��ʹ����ࡣ
		 2. ���Ű�����5�����ţ��鿴�񵥵���λ�Ƿ�ռ�ã������ռ�ã�������ó����
		 	�ʲü���ֽ��������ʺϲü���ҳ��Ϊ�գ���ѡ����һ������λ���������а񵥶�
			û������ϵͳ���롣
		 3. �ü�ҳ��������ҳ�����ṹ������һ�� page_t �ṹ�������µĲü����ҳ����
		 	�ü��������ü���ҳ ��Ҫ page_t �µ� brother �໥����Ϊ�ֵܣ�
	   	 4. ��ȡ���ڴ�ҳ��Ȼ����� layering_t �ṹ���µ� popular ������������ freepages_t 
			�ṹ���µ� topMenu ���Ű񵥣���� popular ��ĳ��ҳ��� popular �������
			���µ���λ��

		��2��һ���ڴ�ҳ���ͷŻص�����ҳ��ʱ��
		 1. �鿴 page_t �ṹ���µ� brother �Ƿ����ֵ�ֽ�ţ�����鿴���ֽ����û��ʹ�ã�
			�����ʹ��������ֽ�źϳ�һ��
		 2. �ϳ�ʱ��Ƚ�page_t�ṹ���� brotherflag��0Ϊ�ܵܣ�1Ϊ��硣������page_t�ṹ
		 	���µ� pageSize ҳ��С��ӻ�úϳ�ֽ����ҳ��������ӽ�����뵽���pageSize 
			�С����ܵܵ� page_t �ṹ��Żص� freepageheadList �����С�

		��3��topMenu���Ű�
		 1. ���ǴӴ�С����Ҳ��˵topMenu[0]���ȶȴ���topMenu[1]
		 2. ���������ʺϲü�ֽʱ�����ȴ������ŵĿ�ʼ��
		 3. �ڸ��� popular �ܻ�ӭ��ʱ��ͨ��ð�ݷ���������topMenu���Ű�
* @param [in]  pageSize ҳ��С��kb��
* @param [in]  blockSum   �ڴ������
* @retval ����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
page_t *
slayer_cut_page(uint_t pageSize, uint_t blockSum)
{
	page_t *page       = NULL;//�ڴ�ҳָ��
	stack_node_t *node = NULL;//�ڴ��ָ��
	int_t index        = 0;//topMenu ����
	int_t topValue     = -1;//topֵ��ʼֵΪ-1
	int_t insert       = 0;//����topMenu�񵥵�ֵ

	if( stack_get_current_member(&FreeTable.pagesTable[pageSize].freePagesList) == 0 ){//�߼�ֽ�㷨����
			for( index=0;index<5; index++ ){//����1-2
				int_t value = FreeTable.topMenu[index];
				if( (value != -1) && (value+pageSize < PAGE_TABLE_SIZE) 
					&& (stack_get_current_member(&FreeTable.pagesTable[value+pageSize].freePagesList) != 0) ){
					topValue = value+pageSize;//��������ʲü�
				}
			}
			//������topMenu
			if(topValue == -1){//û���ҵ����������
				CHECK_NN(page = _slayer_malloc(pageSize, blockSum));
			}else{//�ҵ�����вü�
				CHECK_NN(node = stack_pop(&FreeTable.pagesTable[topValue].freePagesList));
				page = list_takeout_struct(node, page_t, pageNode);//��ô��ü�ҳ
				CHECK_NN(page = _slayer_cut_page(page, pageSize, blockSum));
			}
	}else{//������ֱ�ӻ�ȡ
		CHECK_NN(node = stack_pop(&FreeTable.pagesTable[pageSize].freePagesList));
		page = list_takeout_struct(node, page_t, pageNode);
		CHECK_FG(_slayer_get_block_list(page, blockSum),list_fail);//ʧ����goto list_fail
	}
	//����popular
	FreeTable.pagesTable[pageSize].popular++;
	insert = pageSize;
	for( index=0;index<5; index++ ) {
		int_t value = FreeTable.topMenu[index];
		if( value == -1 || value == pageSize || FreeTable.pagesTable[value].popular<FreeTable.pagesTable[insert].popular){
			FreeTable.topMenu[index] = insert;//�滻���Ű�
			if( value == -1 || value == pageSize ) break;//����-1��ʾ���λ�û��ǿգ��滻���ֱ���˳�ѭ��
			insert = value;
		}
	}

	return page;
	list_fail:
		CHECK_FN(stack_push(&FreeTable.pagesTable[pageSize].freePagesList, &page->pageNode));
		return HALLOC_NULL;
}


/**
* @brief ��ֽ�㷨����Ҫ�ǵ�����14k���ڴ�ҳ�ص�ϵͳ��������Ƭ�ڴ�ҳ�ᱻ�˷ѣ���Ҫ�ֳ�
		 ���ŵĴ�С��������������ֽ�㷨�����ָ��ڴ�ҳ����Ҫ�ǽ����Ȱ񣬸�Ч�Ľ����ڴ�ҳ��
		 ���������ڴ�ҳ������������.
		����[4,5,6,7,8] 
		������Ҫ�ǿ���ϵͳ����㳣�õ��ڴ�ҳ��СΪ7,5,4,3,2,1,�໥��Ϻ��ϲֿ��㷨��Ҫ
		�Ĳ�ͬ�ڴ�ҳ�������Ƚϴ�ĳ��ָ��ʵ���������������������ָ�Ŀ���Ǹ���Ч������
		�ָ���ڴ棬����6���У�1��5����2��4����3��3����ϣ������еģ�1��5��������Сʱ��
		��2��4�����ߣ�3��3�����������ͼ����ܼӴ��������������ӱ�ʹ�ø��ʡ����ֱ���и�
		��1��5�����ܾ�û���Ǹ��õ�ʹ���ʡ�
		��ֽ��ʽ
		��Ϊ��Ч��ֽ����������topMenu������ϵó���ֽ��С������topMenu = [13,5,7,8,2]
		��������Ϊ�˷�ֹҳ������������޶�С�ڵ���7��ֵ������ϼ��㣬������tatter[13]++
		tatter[8]++��Ȼ��[5,7,2]��ϸ�����������[4,5,6,7,8] ԭ��ó���5��5������5��7��
		��5��2������7��7������7��2������2��2��������ϣ��ü�Ҳ��������Ĵ�С���б����ü�
* @param [in]  page    ҳ�ṹ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
slayer_tatter_page(page_t *page)
{
	char_t topGroup[5] = {4,5,6,7,8};//�����ϲ�ֿ������ڴ�������
	char_t tatter[14]  = {0};//���鳤��
	page_t * newPage   = page;//�ڴ�ҳָ��
	int_t index,j      = 0; //����
	void * tatterAddr  = page->headAddr;//��ֽ�ڴ�ҳ��ַ
	int_t tatterSize   = 0;//��ֽ�ڴ�ҳ��С
	int_t pageSize	   = page->pageSize;//�ڴ�ҳ��С

	_slayer_return_block_list(page);//�黹block����
	if(-5 == FreeTable.topMenu[0]+FreeTable.topMenu[1]+FreeTable.topMenu[2]+FreeTable.topMenu[3]+FreeTable.topMenu[4] ){//topMenuδʹ��
		for(index=0; index<5; index){
			index = index%5;
			//�и���ҳ
			_slayer_tatter_page(newPage, tatterAddr, topGroup[index], pageSize);
		}
	}else{//��ֽ
		//��ֽ����
		for(index=0;index<5;index++) if( FreeTable.topMenu[index]>=8 ) tatter[FreeTable.topMenu[index]]++;
		for(index=0;index<5;index++) 
			if( FreeTable.topMenu[index]<=7 ) for(j=0;j<index;j++) tatter[FreeTable.topMenu[index]+FreeTable.topMenu[j]]++;

		//��ʼ��ֽ
		for(index=0;index<14;){
			index = (++index)%14;
			if(tatter[index] == 0 ) continue;
			for( j=0; j<tatter[index]; j++) _slayer_tatter_page(newPage, tatterAddr, index, pageSize); //�и���ҳ
		}
	}
}



/**
* @brief ��ϵͳ������һ���ڴ�ҳ
* @param [in]  pageSize   �ڴ�ҳ��С
* @param [in]  blockSum   �ڴ������
* @retval ����ڴ�ҳ    ��ʾ�ɹ�
* @retval HALLOC_NULL ��ʾʧ��
*/
page_t *
slayer_malloc(uint_t pageSize, uint_t blockSum)
{
	if(INITIALIZED != Pool.slayerInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if((PAGE_MAX_SIZE <= pageSize)||(pageSize == 0 )) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if((BLOCK_MAX_SIZE < blockSum)) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t *page = NULL;
	pthread_mutex_lock(&Pool.poolLock);  
	if(pageSize >= PAGE_TABLE_SIZE){//���ڻ������ҳ���С ����ϵͳ�����ڴ�
		page = _slayer_malloc(pageSize, blockSum);
	}else if(pageSize < PAGE_TABLE_SIZE){//С��ҳ���С �����ڴ�ҳ����
		page = slayer_cut_page(pageSize, blockSum);
	}
	if(NULL != page) {
		page->usedflag = USED;//�ڴ�ҳ����ʹ��
		Pool.freeMemSize = Pool.freeMemSize - pageSize;
	}
    pthread_mutex_unlock(&Pool.poolLock); 
	return page;
}

/**
* @brief ��ϵͳ���ͷ�һ���ڴ�ҳ
* @param [in]  pageSize   �ڴ�ҳ��С
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
slayer_free(page_t *page)
{
	if(HALLOC_NULL == page) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	uint_t pageSize = page->pageSize;
	pthread_mutex_lock(&Pool.poolLock);
	if(PAGE_TABLE_SIZE > page->pageSize ){//С��ҳ��
		if( NULL != page->brother && NON_USED == page->brother->usedflag) {
			CHECK_FF(_slayer_combine_page(page));
		}else{
			CHECK_FF(_slayer_recover_page(page));
		}
	}else{//����ҳ��ʹ����ֽ�㷨
		CHECK_FF(slayer_tatter_page(page));
	}
	Pool.freeMemSize = Pool.freeMemSize + pageSize;
    pthread_mutex_unlock(&Pool.poolLock); 
	return HALLOC_SUCC;
}


/**
* @brief ��ʼ��ϵͳ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
slayer_init(void)
{
	int index; //������
	/* �ڴ�ؽṹ���ʼ�� */
	Pool.slayerInit  = INITIALIZED;
	Pool.memSize     = 0;
	Pool.freeMemSize = 0;
	pthread_mutex_init(&Pool.poolLock,NULL); 
	list_init(&Pool.destroyHeadList);
	list_init(&Pool.pageAndBlock.PABStackList);
	stack_init(&Pool.pageAndBlock.freePageStack);
	stack_init(&Pool.pageAndBlock.freeBlockStack);
	/* �����ڴ�ṹ���ʼ�� */
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
* @brief ��ʼ��halloc
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
halloc_init(void)
{
	/******************************************************************
	*   Name��ϵͳ���ʼ��
	******************************************************************/
	/* �ڴ�ؽṹ���ʼ�� */
	CHECK_FF(slayer_init());
	/******************************************************************
	*   Name���ڴ������ʼ��
	******************************************************************/
	/* �ڴ������ʼ�� */
	CHECK_FF(mlayer_init());
	/******************************************************************
	*   Name���ڴ������ʼ��
	******************************************************************/
	/* �ڴ������ʼ�� */
	CHECK_FF(alayer_init());
	return HALLOC_SUCC;
}


/**
* @brief �ݻ�ϵͳ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
slayer_destroy(void)
{
	int index, delSum; //������, ɾ������
	list_node_t *tmp   = NULL; //�ڴ������Ľڵ�
	pabStack_t *stack  = NULL; //��ȡջָ��
	page_t *page       = NULL; //�ڴ��ָ��
	pthread_mutex_lock(&Pool.poolLock); 
    //˳ʱ��ɾ���ڵ�
	delSum = list_get_current_member(&Pool.destroyHeadList);
    for(index=0; index<delSum; index++){
        tmp = list_get_head_node(&Pool.destroyHeadList);
        list_delete_node(&Pool.destroyHeadList, tmp);
        page = list_takeout_struct(tmp, page_t, destroyNode);
        //free(page->headAddr);
    }
    //˳ʱ��ɾ���ڵ�
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
* @brief �ݻ�halloc
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
halloc_destroy(void)
{
	/******************************************************************
	*   Name���ڴ������ͷ�
	******************************************************************/
	CHECK_FF(alayer_destroy());
	/******************************************************************
	*   Name���ڴ������ͷ�
	******************************************************************/
	CHECK_FF(mlayer_destroy());
	/******************************************************************
	*   Name��ϵͳ���ͷ�
	******************************************************************/
	CHECK_FF(slayer_destroy());
	return HALLOC_SUCC;
}
