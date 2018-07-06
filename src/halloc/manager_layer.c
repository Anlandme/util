/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: manager_layer.c
@brief: �������Ҫ����ϵͳ�����ڴ�ҳ���Լ�����Դ������ṩ�ڴ��ʹ�á�������Ҫ����������
        ���ϲ���ú��������г�ʼ�����ͷ�mlayer�ڴ��������һ���Ÿ���Ҫ���֡�
        ��һ�����֣�����ֿ��㷨����
        �ڶ������֣����������㷨����
        ���������֣���̬�ڴ����--�߳�ջ����
        ���ĸ����֣���̬�ڴ����--���Ա
        ��������֣���̬�ڴ����--���Ա
        ���������֣��ڴ�Խ����
        ���߸����֣��ϲ���õ�����ҳ����
        �ڰ˸����֣�mslayer��ʼ������
        �ھŸ����֣�mslayer�ݻٺ���
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

/**  @brief ��̬�ڴ���� */
staticmem_t StaticMem;

/**  @brief ��̬�ڴ���� */
dynamicmem_t DynamicMem;

/******************************************************************
*   Name���ڴ����㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief ��òֿ�
* @param [in]  storage  �ֿ�ָ��
* @param [in]  index  �ֿ�����
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
* @brief ���Խ��
* @param [in]  block      �ڴ��
* @param [in]  overflocw  �ڴ�Խ�����
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
* @brief ���Ի�þ�̬�ڴ��
* @param [in]  list      �ڴ������
* @param [in]  block      �ڴ��
* @param [in]  size      �ڴ���С
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
* @brief �����ڴ��
* @param [in]  list       �ڴ������
* @param [in]  page       �ڴ�ҳ
* @param [in]  block      �ڴ��
* @param [in]  freelist   �����ڴ������
* @param [in]  freesum    �����ڴ�
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
*   Name���ڴ����㺯�ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief ����ڴ�
* @param [in]  page    �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_check_memory(page_t *page)
{
	block_t 	*block 	     = NULL;//ָ����Ҫ�������ڴ��
    stack_head_t *blocksList = &page->blocksList; //�ڴ�ҳ���ڴ������
	uint_t     overflow = 0;//�ڴ�Խ�����

	traversal_list_do(block, blocksList, node){	//����ڴ�Խ����й©
		_mlayer_check_overflow(block, overflow);
		if(block->lease >= LEAK_CHECK && BLOCK_RED==block->color) CHECK_FL(gclayer_report_overtime_block(block));
	}while_traversal_list(blocksList);
	if(overflow != 0) return HALLOC_FAIL;
	return HALLOC_SUCC;
}

/**
* @brief ����ڴ�Խ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_check_overflow(void)
{
	page_t *page = NULL;//�ڴ�ҳ
	uint_t overflow = 0;//�ڴ�Խ�����
	int_t index;
	storage_t *storage = NULL;

	//��̬�ڴ沿�ּ��
	traversal_list_do(page, &StaticMem.pagesList, pageNode){
		if(HALLOC_FAIL == _mlayer_check_memory(page)) overflow++;
	}while_traversal_list(&StaticMem.pagesList);
	//�߳��ڴ���
	traversal_list_do(page, &StaticMem.threadList, pageNode){
		if(HALLOC_FAIL == _mlayer_check_memory(page)) overflow++;
	}while_traversal_list(&StaticMem.threadList);
	//��̬�ڴ���
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
* @brief �ڴ�ҳ����
* @param [in] goodsArea ����
* @param [in] page     �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_sort_page(stack_head_t *goodsArea, page_t *page)
{
    list_node_t  *tmp = NULL;
	page_t *prevPage  = NULL;//��һ��ҳ�ڵ�
	//���ڵ�����
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
* @brief �ռ��ڴ��
* @param [in]  user    �û�
 - SUPERVISOR  ����Ա
 - GUSET       ����
* @param [in]  storage �ֿ�
* @param [in]  page    �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t 
_mlayer_collect_block(uint_t user, storage_t * storage, page_t * page)
{
	//��û�������ڴ��ʱ��ֱ�ӷ���
	stack_head_t *blocksList  = &page->blocksList; //�ڴ�ҳ���ڴ������
	block_t 	*block    = NULL;//ָ����Ҫ�������ڴ��
	uint_t     usedBlock  = 0;//��ʹ�õĵ��ڴ�����
	uint_t     oldmaxTime = page->maxTime;//����ɵ�maxTimeֵ
	stack_head_t blockList;//�ռ����ڴ�������Ŵ�
	uint_t  blockSize = 0; //�ڴ�ҳ��С
	stack_init(&blockList);

	page->maxTime = 0;
	//�����ڴ��
	traversal_list_do(block, blocksList, node){//����ѭ���ռ��ڴ��
		//��Ӧ�û��յ�ȫ���ռ�����˳�
		if( GUEST == user && 0 == page->freeBlocks) goto success;
		//�ҳ��ڴ�������޵����ʱ��
		if(SUPERVISOR  == user && BLOCK_YELLOW <= block->color) {
			block->lease++;
			page->maxTime = block->lease > page->maxTime?block->lease:page->maxTime;
			if(BLOCK_RED==block->color || BLOCK_YELLOW==block->color) usedBlock++;
		}
		//�����������ڴ���ռ�
		if(BLOCK_GREEN == block->color){
			block->color = BLOCK_WHITE;
			CHECK_FF(stack_push(&storage->freegood, &block->blockNode));
			blockSize = blockSize + block->blockSize;
			pthread_mutex_lock(&page->lock); 
			page->freeBlocks--;
			pthread_mutex_unlock(&page->lock); 
		}
	}while_traversal_list(blocksList);
	//�����ڴ�ҳ
	if(SUPERVISOR  == user && 0 == usedBlock){
		traversal_list_do(block, blocksList, node){//����ѭ�������ڴ�ҳ
			if(BLOCK_WHITE == block->color) CHECK_FL(list_delete_node(&storage->freegood, &block->blockNode));
		}while_traversal_list(blocksList);
		CHECK_FL(list_delete_node(&storage->goodsArea, &page->pageNode));
		CHECK_FL(slayer_free(page));
		//���¶�̬�ڴ��вֿ���ܶ�̬�ڴ�ֵ
		storage->freeMemSize   = storage->freeMemSize   - page->pageSize*KB + blockSize;
		pthread_mutex_lock(&DynamicMem.dynamicLock); //����
		DynamicMem.memSize 	   = DynamicMem.memSize     - page->pageSize*KB;
		DynamicMem.freeMemSize = DynamicMem.freeMemSize - page->pageSize*KB + blockSize;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
		goto success;
	}
	//��maxTime���º󣬽����½�������
	if(SUPERVISOR  == user && page->maxTime != oldmaxTime+1 ) _mlayer_sort_page(&storage->goodsArea, page);
	success:
		return HALLOC_SUCC;
}


/**
* @brief huge�ֿ����
* @param [in]  storage �ֿ�
* @param [in]  page    �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_huge_storage_supervisor(storage_t *storage, page_t * page)
{
	block_t *recover   = NULL; //Ƕ�뵽��������Ĵ������ڴ��
	block_t *block     = NULL; //�ڴ��
	stack_head_t *freegood = &storage->freegood; //�ڴ������
	uint_t blockSize = 0; //block��С
	//�ڴ���
	CHECK_FF(_mlayer_check_memory(page));
	//�ڴ����
	recover = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//����ڴ��ʱ���Ƿ�ʱ
	if(recover->lease++ >= RELEASE_HUGE_THRESH){
		CHECK_FF(list_delete_node(&storage->goodsArea, &page->pageNode));//ɾ���ڵ�
		slayer_free(page);
		goto success;
	}

	if(BLOCK_GREEN != recover->color) goto success;

	//���ǿ��п�ʱ����л��յ�freegood��
	traversal_list_do(block, freegood, blockNode){//��������
		if(block->blockSize >= recover->blockSize){//�ҵ��ʺϻ��յ��ڴ��λ�ú� ִ��Ƕ�����
			CHECK_FF(list_embed_node(INSERT_TO_HEAD, freegood, &block->blockNode, &recover->blockNode));//Ƕ�뵽����
			recover->color = BLOCK_WHITE;
			goto mem_update;
		}
	}while_traversal_list(freegood);
	//�����ڴ��������û���ڴ�飬����ҪѰ�Һ���λ�ã�ֱ��Ƕ��
	CHECK_FF(list_insert_node(INSERT_TO_TAIL, freegood, &recover->blockNode));
	recover->color = BLOCK_WHITE;
	

	mem_update:
		storage->freeMemSize = storage->freeMemSize + recover->blockSize;
		pthread_mutex_lock(&DynamicMem.dynamicLock); //����
		DynamicMem.freeMemSize = DynamicMem.freeMemSize + recover->blockSize;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
	success:
		return HALLOC_SUCC;
}


/**
* @brief huge�ֿ����
* @param [in] storage �ֿ�
* @param [in] goodsArea ����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_small_middle_big_storage_supervisor(storage_t * storage, stack_head_t *goodsArea)
{
	page_t 	*page = NULL; //�ڴ�ҳ

	//mlayer_check_overflow();
	traversal_list_do(page, goodsArea, pageNode){//��������
		//�ڴ���
		CHECK_FF(_mlayer_check_memory(page));
		//���ڴ�������뵽freegood
		CHECK_FL(_mlayer_collect_block(SUPERVISOR, storage, page));
	}while_traversal_list(goodsArea);
	return HALLOC_SUCC;
}


/**
* @brief ��̬�ڴ涨ʱ��������������Ա��
		 �ڴ���Ա��Ҫ�Ǽ���ڴ���Ƿ�����ڴ�Խ�磬�����ڴ��ṹ�����֮��ὲ����
		 ������Ҫ�ǽ����Ա��μ���ڴ档���ھ�̬�ڴ�д�벻���ر�Ƶ�������Բ���ҪƵ
		 ���Ľ���鿴��������10����б������е��ڴ��鿴�ڴ���Ƿ�Խ�磬Ϊ�˷�ֹ
		 һ���ڴ��Խ���д����ڴ�����ݡ�һ������Խ�磬�����ϲ鿴��һ���ڴ���Ƿ�
		 Խ�磬һֱ�鵽û��Խ����ڴ�顣Ȼ����������ڴ�ʱ����ļ���������������Ϣ��
		 �����ϱ���������
* @param [in]  value    ����ֵ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
void
mlayer_dynamicmem_supervisor(union sigval value)
{
	halloc_log("dynamicmem supervisor-%d start\n", value.sival_int);
	stack_head_t *goodsArea = NULL; //����
	page_t 	*page 			= NULL; //�ڴ�ҳ
    storage_t * storage 	= NULL; //��������
    _mlayer_get_storage(storage, value.sival_int);
	goodsArea = &storage->goodsArea;

	pthread_mutex_lock(&storage->blocksLock); //����
	if(stack_get_current_member(goodsArea) == 0 && 1 == storage->timerOpen) {//����û���ڴ�ҳʱ��
		CHECK_FL(timer_stop(storage->supervisor));//�رն�ʱ��
		storage->timerOpen = 0;
		goto unlock;
	}
    if(value.sival_int == 18){//huge�ֿ����
		traversal_list_do(page, goodsArea, pageNode){ 
			CHECK_FL(_mlayer_huge_storage_supervisor(storage, page));
		}while_traversal_list(goodsArea);
	}else{//small�ֿ� middle�ֿ� big�ֿ����
        CHECK_FL(_mlayer_small_middle_big_storage_supervisor(storage, goodsArea));
    }
	pthread_mutex_unlock(&storage->blocksLock); //����
    //gc��
	//gclayer_halloc_memory_scan(goodsArea);
	return;
	unlock:
		pthread_mutex_lock(&DynamicMem.dynamicLock); //����
		DynamicMem.timerSum--;
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
		pthread_mutex_unlock(&storage->blocksLock); //����
}


/**
* @brief �ڴ�ҳ����
* @param [in] list     ����
* @param [in] page     �ڴ�ҳ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_sort_block(stack_head_t *list, block_t *block)
{
    list_node_t  *tmp = NULL;
	block_t *prevBlock  = NULL;//��һ���ڴ��ڵ�
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
* @brief ��̬�ڴ涨ʱ��������������Ա��
		 �ڴ���Ա��Ҫ�Ǽ���ڴ���Ƿ�����ڴ�Խ�磬�����ڴ��ṹ�����֮��ὲ����
		 ������Ҫ�ǽ����Ա��μ���ڴ档���ھ�̬�ڴ�д�벻���ر�Ƶ�������Բ���ҪƵ
		 ���Ľ���鿴��������10����б������е��ڴ��鿴�ڴ���Ƿ�Խ�磬Ϊ�˷�ֹ
		 һ���ڴ��Խ���д����ڴ�����ݡ�һ������Խ�磬�����ϲ鿴��һ���ڴ���Ƿ�
		 Խ�磬һֱ�鵽û��Խ����ڴ�顣Ȼ����������ڴ�ʱ����ļ���������������Ϣ��
		 �����ϱ���������
* @param [in]  value    ����ֵ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
void
mlayer_staticmem_supervisor(union sigval value)
{
	page_t 	*page = NULL; //�ڴ�ҳ
	block_t *block= NULL; //�ڴ��
	stack_head_t *list  = NULL; //�洢ҳ��������߳�ջ����
	halloc_log("staticmem supervisor start\n");
	pthread_mutex_lock(&StaticMem.staticLock); //����
	//��̬�ڴ���
	list = &StaticMem.pagesList; //ҳ����
	_mlayer_recover_staticblock(list, page, block, &StaticMem.freeblockList, StaticMem.freeMemSize);
	//�߳��ڴ���
	list = &StaticMem.threadList; //�߳�ջ����
	_mlayer_recover_staticblock(list, page, block, &StaticMem.freethreadList, StaticMem.threadFreemem);
	pthread_mutex_unlock(&StaticMem.staticLock); //����
	//gc��
	gclayer_halloc_memory_scan(&StaticMem.pagesList);
	return;
}


/**
* @brief ��ȡ�ڴ��
* @param [in]  storage    �ֺ�
* @param [in]  blockinfo   �ڴ����Ϣ
* @param [in]  blockSum   �ڴ������
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t 
_mlayer_malloc_small_middle_big_block(storage_t * storage, list_head_t *blockList, uint_t blockinfo[5])
{
	page_t * page = NULL; //�ڴ�ҳ
	block_t *block= NULL; //�ڴ��
	uint_t  pageSize = 0; //�ڴ�ҳ��С
	uint_t   number = storage->storageType; //�ֺ�
	int_t mallocSum = blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE];//�����ڴ������
	stack_head_t *goodsArea  = &storage->goodsArea; //����
	stack_head_t *freegood  = &storage->freegood; //��������

	//�鿴����freegood�������� ���ڻ��ߵ�����������ֱ�ӻ�ȡ���˳�
	if(stack_get_current_member(freegood)>=mallocSum) goto success;
	//ǰ��������Ѱ�� ���Ż�����
	traversal_list_do(page, goodsArea, pageNode){//��������
		if(page->freeBlocks != 0) CHECK_FL(_mlayer_collect_block(GUEST, storage, page));
		if(stack_get_current_member(freegood) >= mallocSum) goto success;
	}while_traversal_list(goodsArea);
	//��Ϊ���������¼�������
	mallocSum= blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE]-stack_get_current_member(freegood);
	//��������û���ڴ�ҳ����ϵͳ������
	for(;mallocSum>=0; mallocSum=mallocSum-blockinfo[PGAE_BSUM]){
		CHECK_NF(page = slayer_malloc(blockinfo[PGAE_SIZE], blockinfo[PGAE_BSUM]));
		CHECK_FG(list_insert_node(INSERT_TO_TAIL, goodsArea, &page->pageNode), list_fail);
		//����freegood����
		traversal_list_do(block, &page->blocksList, node){ 
			CHECK_FG(stack_push(freegood, &block->blockNode), list_fail);
		}while_traversal_list(&page->blocksList);
		pthread_mutex_lock(&page->lock); 
		page->freeBlocks = 0;
		pthread_mutex_unlock(&page->lock); 
	}
	//���¶�̬�ڴ��вֿ���ܶ�̬�ڴ�ֵ
	pageSize = blockinfo[PGAE_SIZE]*KB;

	success:
		mallocSum= blockinfo[BLOCK_MSIZE]/blockinfo[BLOCK_SIZE];
		CHECK_FL(stack_push_list(blockList, freegood, mallocSum));
		//���¶�̬�ڴ��вֿ���ܶ�̬�ڴ�ֵ
		storage->memSize 	   = storage->memSize + pageSize;
		storage->freeMemSize   = storage->freeMemSize + pageSize -blockinfo[BLOCK_MSIZE];
		pthread_mutex_lock(&DynamicMem.dynamicLock); //����
		DynamicMem.memSize     = DynamicMem.memSize + pageSize;
    	DynamicMem.freeMemSize = DynamicMem.freeMemSize + pageSize -blockinfo[BLOCK_MSIZE];
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
		return HALLOC_SUCC;
	list_fail:
		slayer_free(page);
		return HALLOC_FAIL;
}

/**
* @brief ��ȡhuge�ֿ��ڴ��
* @param [in]  storage    �ֺ�
* @param [out]  blockList �ڴ������
* @param [in]  blockinfo   �ڴ����Ϣ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t 
_mlayer_malloc_huge_block(storage_t * storage, list_head_t *blockList, uint_t blockinfo[5])
{
    page_t       *page     = NULL;			     //��ȡ�ڴ�ҳ
    block_t      *block    = NULL;			     //��ȡ�ڴ��ָ��
    stack_head_t *freegood = &storage->freegood; //�ڴ������
    uint_t       pageSize  = 0;			         //�ڴ�ҳ��С

    //˳ʱ������ڵ�
	traversal_list_do(block, freegood, blockNode){//ѭ������Ѱ���ʺϵ��ڴ��
		if(blockinfo[BLOCK_SIZE] <= block->blockSize && block->blockSize <= blockinfo[BLOCK_SIZE] + 1*KB) {
			CHECK_FF(list_delete_node(freegood, &block->blockNode));//��С���С��1kb ȡ��
			goto success;
		}
	}while_traversal_list(freegood);
	//û�к��ʵ�ֱ�Ӵ�ϵͳ���ȡ
	CHECK_NF(page = slayer_malloc(blockinfo[PGAE_SIZE], 1));
	CHECK_FG(stack_push(&storage->goodsArea, &page->pageNode),fail);//��������
	block = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//���¶�̬�ڴ��вֿ���ܶ�̬�ڴ�ֵ
	pageSize = blockinfo[PGAE_SIZE]*KB;

	success:
		//���¶�̬�ڴ��вֿ���ܶ�̬�ڴ�ֵ
		pthread_mutex_lock(&DynamicMem.dynamicLock); //����
		DynamicMem.memSize = DynamicMem.memSize + pageSize;
		DynamicMem.freeMemSize = DynamicMem.freeMemSize + pageSize - blockinfo[BLOCK_MSIZE];
		pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
		CHECK_FF(stack_push(blockList, &block->blockNode));
		return HALLOC_SUCC;	

	fail:
		slayer_free(page);
		return HALLOC_FAIL;
}



/**
* @brief �����ڴ��
* @param [in]  storage    �ֺ�
* @param [out] blockList  �ڴ������
* @param [in]  blockinfo   �ڴ����Ϣ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_mlayer_lease_block(storage_t *storage, list_head_t *blockList, uint_t blockinfo[5])
{
	block_t *block = NULL;

	if(18 == storage->storageType){//huge�ֿ����
		CHECK_FF(_mlayer_malloc_huge_block(storage, blockList, blockinfo));
	}else{//small�ֿ� middle�ֿ� big�ֿ����
		CHECK_FF(_mlayer_malloc_small_middle_big_block(storage, blockList, blockinfo));
	}
	traversal_list_do(block, blockList, blockNode){ 
		block->color = BLOCK_YELLOW; 
		block->type  = MEM_DYNAMIC;
	}while_traversal_list(blockList);//����ѭ��
	return HALLOC_SUCC;
}

/**
* @brief �򿪹���Ա
* @param [in]  storage    �ֺ�
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_start_supervisor(storage_t *storage)
{
	struct timespec times;//��ʱ������ʱ��
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_start(storage->supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	storage->timerOpen == 1;
	pthread_mutex_lock(&DynamicMem.dynamicLock); //����
	DynamicMem.timerSum++;
	pthread_mutex_unlock(&DynamicMem.dynamicLock); //����
	return HALLOC_SUCC;
}


/**
* @brief ��ȡ����㶯̬�ڴ��
* @param [out]  blockList �ڴ������
* @param [in]  blockinfo   �ڴ����Ϣ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t 
mlayer_dynamic_malloc(list_head_t *blockList, uint_t blockinfo[5])
{
	if(INITIALIZED != DynamicMem.dynamicInit) RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	if(PARAMISNULL(blockList))  RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	
	storage_t * storage = NULL;
	int_t ret = HALLOC_FAIL;
	//��òֿ�
	_mlayer_get_storage(storage, blockinfo[BLOCK_STORAGE]);
	pthread_mutex_lock(&storage->blocksLock); 
	//��û�п�����ʱ������Աʱ��������ʱ������Ա
	if(storage->timerOpen == 0) mlayer_start_supervisor(storage);
	//�����ڴ��
	ret = _mlayer_lease_block(storage, blockList, blockinfo);
	pthread_mutex_unlock(&storage->blocksLock); 
	return ret;
}


/**
* @brief ��ȡ��̬�ڴ�
* @param [in]  blockSize  �ڴ���С
* @retval �����ڴ��    ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t * 
mlayer_static_malloc(uint_t blockSize)
{
	if(INITIALIZED != StaticMem.staticInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	if(blockSize > STATIC_PAGE) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t 	*page = NULL; //�ڴ�ҳ
	block_t *block= NULL;
	stack_head_t *list  = NULL; //�����ڴ���������ҳ����
	uint_t pageSize = 0;		//�ڴ�ҳ��С

	pthread_mutex_lock(&StaticMem.staticLock); //����
	//��̬�ڴ���
	list = &StaticMem.freeblockList; //�����ڴ������
	_mlayer_tryto_get_staticblock(list, block, blockSize);
	//��ҳ��ջ��ȡ
	list = &StaticMem.pagesList; //ҳ����
	traversal_list_do(page, list, pageNode){//ѭ�������ҵ��㹻�ռ��ȡ�ڴ����ڴ�ҳ
		block = slayer_get_static_block(page, blockSize);
		if(block != NULL) goto success;
	}while_traversal_list(list);
	//���»�ȡһ���ڴ�ҳ
	CHECK_NG(page = slayer_malloc(STATIC_PAGE, 0), malloc_fail);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &StaticMem.pagesList, &page->pageNode), list_fail);
	CHECK_NG(block = slayer_get_static_block(page, blockSize), malloc_fail);
	//�����ܶ�̬�������ڴ�ֵ
	pageSize = STATIC_PAGE;

	success:
		//�����ܶ�̬�������ڴ�ֵ
		StaticMem.memSize = StaticMem.memSize + pageSize;
		StaticMem.freeMemSize = StaticMem.freeMemSize - block->blockSize + pageSize;
		block->color = BLOCK_YELLOW;
		block->type  = MEM_STATIC;
		pthread_mutex_unlock(&StaticMem.staticLock); //����
		return block;
	list_fail:
		slayer_free(page);
	malloc_fail:
	get_fail:
		pthread_mutex_unlock(&StaticMem.staticLock); //����
		return HALLOC_NULL;	
}

/**
* @brief malloc�߳�ջ
* @param [in]  stackSzie   �߳�ջ��С kb
* @retval �����߳��ڴ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t * 
mlayer_malloc_thread_stack(uint_t stackSize)
{
	if(INITIALIZED != StaticMem.staticInit) RETURN_ERR(ERR_PARA, HALLOC_NULL);
	page_t  *page = NULL;
	block_t *block= NULL;

	pthread_mutex_lock(&StaticMem.staticLock); //����
	//���Ի�ȡ�߳�ջ
	_mlayer_tryto_get_staticblock(&StaticMem.freethreadList, block, stackSize*KB);
	//û�к��ʾ�����
	CHECK_NG(page = slayer_malloc(stackSize, 1), malloc_fail);
	CHECK_FG(list_insert_node(INSERT_TO_HEAD, &StaticMem.threadList, &page->pageNode), list_fail);
	//ת�����ڴ��
	block = list_takeout_struct(list_get_head_node(&page->blocksList), block_t, node);
	//�����ܶ�̬�������ڴ�ֵ
	StaticMem.threadMem = StaticMem.threadMem + page->pageSize*KB;
	StaticMem.threadFreemem = StaticMem.threadFreemem + page->pageSize*KB; 

	success:
		//�����ܶ�̬�������ڴ�ֵ
		StaticMem.threadFreemem = StaticMem.threadFreemem - block->blockSize;
		block->color = BLOCK_YELLOW;
		block->type  = MEM_THREAD_STACK;
		pthread_mutex_unlock(&StaticMem.staticLock); //����
		return block;
	list_fail:
		slayer_free(page);
	malloc_fail:
	get_fail:
		pthread_mutex_unlock(&StaticMem.staticLock); //����
		return HALLOC_NULL;
}

/**
* @brief �ͷŹ�����ڴ��
* @param [in]  block     �ͷ��ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
* @brief ��ʼ���ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
	//��̬�ڴ��ʼ��
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	StaticMem.memSize       = 0;          //�ڴ��С 
    StaticMem.freeMemSize   = 0;          //�����ڴ��С
	StaticMem.threadMem     = 0;          //�ڴ��С 
    StaticMem.threadFreemem = 0;          //�����ڴ��С
	StaticMem.staticInit  = INITIALIZED;  //��ʼ����־
	event.callbackFunc    = mlayer_staticmem_supervisor;
	pthread_mutex_init(&StaticMem.staticLock,NULL);//��̬�ڴ��� 
	pthread_mutex_init(&StaticMem.threadLock,NULL);//�߳�ջ�ڴ��� 
	list_init(&StaticMem.pagesList); //ҳ���� ÿ��ҳ�̶�4k 
	list_init(&StaticMem.threadList);//�߳�ջ����
	list_init(&StaticMem.freeblockList); //�����ڴ������ 
	list_init(&StaticMem.freethreadList);//�����߳�ջ����
	CHECK_FF(timer_open(&StaticMem.supervisor,&event)); //���Ա ����ڴ�Խ���
	CHECK_FF(timer_start(StaticMem.supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	//��̬�ڴ��ʼ��
	pthread_mutex_init(&DynamicMem.dynamicLock,NULL);//��̬�ڴ��� 
	DynamicMem.timerSum	   = 0;          //��ʱ������
	DynamicMem.memSize     = 0;          //�ڴ��С 
    DynamicMem.freeMemSize = 0;          //�����ڴ��С
	DynamicMem.dynamicInit = INITIALIZED;//��ʼ����־
	event.callbackFunc     = mlayer_dynamicmem_supervisor;
	for(index=0;index<19;index++){
		event.signalValue.sival_int = index;
		_mlayer_get_storage(storage, index);
		storage->storageType = index; //�ֿ�����
		list_init(&storage->freegood);//���л���
		stack_init(&storage->goodsArea);//�ֿ����  
		CHECK_FF(timer_open(&storage->supervisor,&event)); //���Ա �����㷨ʹ�� 
		storage->timerOpen = 0; //�ֿ�����
		pthread_mutex_init(&storage->blocksLock,NULL);//�ڴ���� 
	}
	return HALLOC_SUCC;
}



/**
* @brief �ݻ��ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
mlayer_destroy(void)
{
	storage_t * storage = NULL;
	int_t index;
	//��̬�ڴ�ݻ�
	CHECK_FL(timer_stop(StaticMem.supervisor));//�رն�ʱ��
	CHECK_FF(timer_close(&StaticMem.supervisor)); //���Ա ����ڴ�Խ���
	//��̬�ڴ�ݻ�
	for(index=0;index<19;index++){
		_mlayer_get_storage(storage, index);
		CHECK_FL(timer_stop(storage->supervisor));//�رն�ʱ��
		CHECK_FF(timer_close(&storage->supervisor)); //���Ա �����㷨ʹ��	
	}
    return HALLOC_SUCC;
}

/**
* @brief ��ȡmlayer���ڴ���Ϣ
* @param [in]  infoType     ��Ϣ����
 - MLAYER_STATIC_MEM ��̬�ڴ�
 - MLAYER_STATIC_FREEMEM ��̬�����ڴ�
 - MLAYER_THREADSTACK_MEM �߳�ջʹ���ڴ�
 - MLAYER_THREADSTACK_FREEMEM �߳�ջ�����ڴ�
* @retval �ڴ���Ϣ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
* @brief ��ȡ��ʱ������
* @retval �ڴ���Ϣ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
uint_t
mlayer_get_timersum(void)
{
	return DynamicMem.timerSum +1;//��̬��ʱ�� + ��̬��ʱ��
}