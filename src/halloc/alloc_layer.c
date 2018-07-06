/******************************************************************************
        (c) COPYRIGHT 2017-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: alloc_layer.c
@brief: �������Ҫ�Ƕ����ڴ��ַ�������ģ�鵱�У������ڸ���ģ����ڴ����й�����֤ģ���ڵ�
		�ڴ��ȵ�������䣬ģ����������Ҫ�ǿ������Ϊ���ܣ���Ӧģ�������ڴ�����Ӧģ���ͷ�
		������Ҫ�����㷨������alayer�����Ա���������������ú��������г�ʼ�����ͷ�alayer
		��Դ������һ��������Ҫ���֡�
        ��һ�����֣������㷨����
        �ڶ������֣�alayer�����Ա
        ���������֣�alayer��ʼ������
        ���ĸ����֣�alayer�ݻٺ���
        ��������֣������õ������ڴ溯��
        ���������֣������õ��ͷ��ڴ溯��

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

/**  @brief ģ���ڴ�� */
modulePool_t modulePool;

/**  @brief �ֿ��ڴ����� */
uint_t blockTable[19][5] = {
/*�ֺ� | �ڴ���С(hugeÿ����Ҫ��д) | ����������ڴ�������ܴ�С(ÿ����Ҫ��д) | �ڴ������ | �����ڴ�ҳ��С(hugeÿ����Ҫ��д) */
//small�ֿ�
{ 0,  48,   0, 64, 3},
{ 1,  64,   0, 16, 1},
{ 2,  80,   0, 64, 5},
{ 3,  96,   0, 32, 3},
{ 4,  112,  0, 64, 7},
{ 5,  128,  0, 8,  1},
//middle�ֿ�
{ 6,  192,  0, 16, 3},
{ 7,  256,  0, 8,  2},
{ 8,  320,  0, 16, 5},
{ 9,  384,  0, 8,  3},
{ 10, 448,  0, 16, 7},
{ 11, 512,  0, 4,  2},
//big�ֿ�
{ 12, 768,  0, 4,  3},
{ 13, 1024, 0, 4,  4},
{ 14, 1280, 0, 4,  5},
{ 15, 1536, 0, 2,  3},
{ 16, 1792, 0, 4,  7},
{ 17, 2048, 0, 2,  4},
//huge�ֿ�
{ 18, 0,    0, 1,  0}//
};


/******************************************************************
*   Name���ڴ�����ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**  @brief huge shelf */
#define HUGE_SHELF 18
/**
* @brief ת������
* @param [in]  shelf  ����ָ��
* @param [in]  module ģ��
* @param [in]  layer  ���ܲ�
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
*   Name���ڴ�����ӿں�
*   Desc�����Ա��ڲ����ⲿ���õĽӿں�
******************************************************************/
/**
* @brief �����������ڴ�ֵ 
* @param [in]  freegood  �����ڴ������
* @retval �������ֵ  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_alayer_get_list_maximum(stack_head_t *list)
{
	if(stack_get_current_member(list) == 0) return 0;
	block_t *block = list_takeout_struct(list_get_tail_node(list), block_t, blockNode);
	return block->blockSize;
}

/**
* @brief ����huge block
* @param [in]  freegood  ����
* @param [in]  insert  �����ڴ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_alayer_insert_huge_block(shelf_t *shelf, block_t *insert)
{
	block_t *block = NULL;
	stack_head_t *freegood = &shelf->freegood;
	if(stack_get_current_member(freegood)==0 || insert->blockSize >= _alayer_get_list_maximum(freegood)){//����Ϊ�ջ��߲����Ϊ���ֵ����ֱ�Ӳ���
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
* @brief ��û���
* @param [in]  module  ģ��
* @param [in]  size    �ڴ��С
* @retval ���ػ���  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
shelf_t *
_alayer_get_shelf(uint module, uint_t size)
{
	shelf_t *shelf   = NULL;//����
	int_t shelflayer = 0;//����ĳ��
	if(size > 2048) {//��huge���ҵ��ֺ�
		shelflayer = 18;
	}else{
		for(shelflayer=0; shelflayer<18; shelflayer++)
			if(blockTable[shelflayer][BLOCK_SIZE] >= size) break;
	}
	//����ڴ���ж�Ӧ����
	_alayer_return_shelf(shelf, module, shelflayer);
	return shelf;
}


/**
* @brief ��������ڴ������
		giveBackֵ��applyLevelֵ
		�������ڴ��������ͷ��ǻ���������ֵ��applyLevelֵÿ�����ӵȼ������������ᷭ��������2->4->8�����ȼ���
		��20ʱ������������λ�ò��䣬ֻ�����ȼ�����ÿ���ͷ���giveBack��ֵʱ�򣬵ݼ�һ�εȼ���
			- ÿ���ڴ�鱻ʹ����������������ʱ����applyLevel����ڴ�����ټ�ȥ giveBack������ͷ��ڴ������
			- ����applyLevel�������������ڴ�飬����ɹ������giveBackֵ�����ֵΪ��applyLevel����ڴ������ȥ���ͷ��ڴ������
* @param [in]  shelf  ����
* @retval ��������  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
uint_t
_alayer_malloc_sum(shelf_t *shelf)
{
	if(shelf->layer == HUGE_SHELF) return 1;
	switch(shelf->applyLevel/10){
		case 0:return 2;//giveBack=0-9 ������������2
		case 1:return 4;//iveBack=10-19 ������������4
		default:return 8;//iveBack=20~ ������������8
	}
}


/**
* @brief �ͷ��ڴ��
* @param [in]  shelf  ����
* @param [in]  block  �ڴ��
* @param [in]  freegood  ���û���
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_alayer_free_block(shelf_t *shelf, stack_head_t *freegood, block_t * block)
{
	//�ͷ��ڴ�鵽�����
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
* @brief �������
* @param [in]  shelf    ����
* @param [in]  memSize  ģ���ڴ��С
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
_alayer_manager_shelf(shelf_t *shelf, uint_t *moduleMem)
{
	block_t *block = NULL;
	stack_head_t collect;//�ռ��ڴ��
	stack_head_t *freegood = &shelf->freegood;
	stack_head_t *returngood = &shelf->returngood;
	stack_init(&collect);//�ռ��ڴ��ջ��ʼ��

	//returngood����
	//����huge shelf�������Ķ�����returngood���մ����ⲿ���ز���
	if(shelf->layer != HUGE_SHELF && shelf->returnBlocks > 0){
		pthread_mutex_lock(&shelf->returnLock);
		traversal_list_do(block, returngood, blockNode){
			if(block->color == BLOCK_YELLOW){
				if(block->lease >= LEASE_TIME){//�ڴ�鳬������ʱ�䣬�ͷŻع����
					if(HALLOC_SUCC != _alayer_free_block(shelf, returngood, block)) continue;
				}else{//�����ڴ��
					if(HALLOC_SUCC != list_delete_node(returngood, &block->blockNode)) continue;
					CHECK_FL(stack_push(&collect, &block->blockNode));
				}
				shelf->returnBlocks--;
			}	
		}while_traversal_list(returngood);
		pthread_mutex_unlock(&shelf->returnLock); 
	}
	//freegood����
	pthread_mutex_lock(&shelf->freeLock);
	traversal_list_do(block, freegood, blockNode){ 
		if(block->lease >= LEASE_TIME) CHECK_FL(_alayer_free_block(shelf, freegood, block));
	}while_traversal_list(freegood);
	//��small middle big ��shelf������block���뵽freeblock
	if(shelf->layer != HUGE_SHELF && stack_get_current_member(&collect) != 0){
			CHECK_FL(stack_push_list(freegood, &collect, stack_get_current_member(&collect)));
			shelf->freeMemSize = stack_get_current_member(freegood)*blockTable[shelf->layer][BLOCK_SIZE];
	}
	pthread_mutex_unlock(&shelf->freeLock); 

	*moduleMem = *moduleMem + shelf->memSize;
	return HALLOC_SUCC;
}

/**
* @brief ��ʱ��������������Ա��
		���Ա����
		1.	�鿴�黹�����Ƿ����ڴ�飬������뵽�Ŀ�������
		2.	�鿴��ǰ�����ڴ�������Ƿ����giveBack +1����������ͷ�giveBackֵ������㣬aoolyLevel�ȼ���һ��
		3.	�鿴ʣ�µ��ڴ���Ƿ��г�ʱ�г�ʱ���ͷŲ�����giveBackֵ
		4. �����huge���ܣ�������2 3���裬����ֱ�ӱ����ҵ���ʱ�ľ��ͷš�
* @param [in]  value    ����ֵ
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
void
alayer_supervisor(union sigval value)
{
	int_t module     = 0;//ģ��
	shelf_t *shelf   = NULL;//����
	int_t shelflayer = 0;//����ĳ��
	halloc_log("alyer supervisor start\n");
	//ѭ��������ģ��
	for(module=0; module<MOD_BUTT; module++){
		if(modulePool.memory[module].dynamicMem == 0) continue;
		modulePool.memory[module].dynamicMem = 0;//������¼���
		for(shelflayer=0; shelflayer<19; shelflayer++){
			_alayer_return_shelf(shelf, module, shelflayer);
			if(shelf->memSize == 0) continue;
			CHECK_FL(_alayer_manager_shelf(shelf, &modulePool.memory[module].dynamicMem));
		}
	}

}


/**
* @brief ��ϵͳ������block 
* @param [in]  shelf  ����
* @param [in]  msize   �ڴ��С
* @retval �����ڴ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t *
_alayer_malloc_block(shelf_t *shelf, uint_t msize)
{
	list_head_t blockList;
	block_t *block = NULL;//�ڴ��
	list_node_t *node = NULL;
	uint_t blockinfo[5];//�ڴ����Ϣ
	stack_head_t *freegood = &shelf->freegood;
	uint_t index;
	stack_init(&blockList);
	//����ڴ�������Ϣ
	for(index=0;index<5;index++) blockinfo[index] = blockTable[shelf->layer][index];

	//�����ڴ��
	if(shelf->layer == HUGE_SHELF){
		blockinfo[BLOCK_SIZE] = msize;
		blockinfo[BLOCK_MSIZE] = msize;
		blockinfo[PGAE_SIZE] = SWITCH_TO_KB(msize);
	}else{
		blockinfo[BLOCK_MSIZE] = blockTable[shelf->layer][BLOCK_SIZE]*_alayer_malloc_sum(shelf);
	}
	CHECK_FN(mlayer_dynamic_malloc(&blockList, blockinfo));
	//���block
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
* @brief �����ڴ�
* @param [in]  shelf  ����
* @param [in]  msize   �ڴ��С
* @retval �����ڴ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t *
_alayer_get_shelf_block(shelf_t *shelf, uint_t msize)
{
	block_t *block = NULL;//�ڴ��
	stack_head_t *freegood = &shelf->freegood;
	stack_head_t *returngood = &shelf->returngood;

	//�ӻ��������л�ȡ
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
	//freegoodѰ���ڴ��
	pthread_mutex_lock(&shelf->freeLock);
	if( msize <= _alayer_get_list_maximum(freegood)){//�����ڴ����ʣ�����Ѱ��
		traversal_list_do(block, freegood, blockNode){
			if(msize <= block->blockSize && block->blockSize <= msize + 1*KB){
				if(HALLOC_SUCC != list_delete_node(freegood, &block->blockNode)) continue;
				shelf->freeMemSize = shelf->freeMemSize - block->blockSize;
				pthread_mutex_unlock(&shelf->freeLock); 
				goto success;
			}
		}while_traversal_list(freegood);
	}
	//���ڴ���������
	CHECK_NN(block = _alayer_malloc_block(shelf, msize));
	pthread_mutex_unlock(&shelf->freeLock); 
	success:
		return block;
}



/**
* @brief �����ڴ�
* @param [in]  moduleMem  ģ���ڴ����
* @param [in]  msize   �ڴ��С
* @param [in]  mtype   �ڴ�����
	- MEM_DYNAMIC ��̬�ڴ�
	- MEM_STATIC ��̬�ڴ�
	- MEM_THREAD_STACK �߳�ջ�ڴ�
* @retval �����ڴ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t *
_alayer_lease_block(moduleMem_t * moduleMem, uint_t msize, uint_t mtype)
{
	shelf_t *shelf = NULL;//����
	block_t *block = NULL;//�ڴ��
	switch(mtype){
		case MEM_DYNAMIC://��ȡ��̬�ڴ�
			CHECK_NN(shelf = _alayer_get_shelf(moduleMem->module, SWITCH_TO_BLOCKSIZE(msize)));
			uint_t memsize = shelf->memSize;
			CHECK_NN(block = _alayer_get_shelf_block(shelf, SWITCH_TO_BLOCKSIZE(msize)));
			pthread_mutex_lock(&moduleMem->moduleLock);
			moduleMem->dynamicMem = moduleMem->dynamicMem -memsize + shelf->memSize; 
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_STATIC://��ȡ��̬�ڴ�
			CHECK_NN(block =mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(msize)));
			pthread_mutex_lock(&moduleMem->moduleLock);
			moduleMem->staticMem = moduleMem->staticMem + block->blockSize; 
			CHECK_FG(stack_push(&moduleMem->staticList, &block->blockNode), push_fail);
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_THREAD_STACK://��ȡ�߳�ջ
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
* @brief ��ȡ�ڴ�
* @param [in]  msize   �ڴ��С
* @param [in]  mtype   �ڴ�����
	- MEM_DYNAMIC ��̬�ڴ�
	- MEM_STATIC ��̬�ڴ�
	- MEM_THREAD_STACK �߳�ջ�ڴ�
* @param [in]  module  ģ������
* @param [in]  file    �ļ���
* @param [in]  func    ������
* @param [in]  line    �к�
* @retval �����ڴ�ָ��  ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
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
* @brief ���Խ��
* @param [in]  addr     �ڴ��ַ
* @retval �����ڴ��     ��ʾ�ɹ�
* @retval HALLOC_NULL  ��ʾʧ��
*/
block_t *
_alayer_check_overflow(void *addr)
{
    checkinfo_t  *headcheck = (checkinfo_t *)((char *)addr - sizeof(checkinfo_t));
	block_t *block, *nextBlock;
    if(MEMISOVERFLOW(headcheck)) goto overflow;
	block = headcheck->block;
	//���ڵ����һ���ڵ�����й©���п����Ǳ����ڴ��й©��
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
alayer_free(void * memory)
{
	if(memory == NULL)  RETURN_ERR(ERR_PARA, HALLOC_FAIL);
	block_t * block         = NULL; //�ڴ��
	shelf_t *shelf          = NULL;//����
	moduleMem_t * moduleMem = NULL;//ģ���ڴ����
	//�ڴ�Խ����
	CHECK_NF(block = _alayer_check_overflow(memory));
	//�����ڴ�
	moduleMem = &modulePool.memory[block->module];
	switch(block->type){
		case MEM_DYNAMIC://����̬�ڴ�
			CHECK_NF(shelf = _alayer_get_shelf(block->module, block->blockSize));
			block->color = BLOCK_YELLOW;
			//����huge shelf�������Ķ�����returnBlocks������¼��㷵��block����
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
		case MEM_STATIC://����̬�ڴ�
			pthread_mutex_lock(&moduleMem->moduleLock);
			CHECK_FG(list_delete_node(&moduleMem->staticList, &block->blockNode), delete_fail);
			moduleMem->staticMem = moduleMem->staticMem - block->blockSize; 
			mlayer_free(block);
			pthread_mutex_unlock(&moduleMem->moduleLock); 
		break;
		case MEM_THREAD_STACK://�����߳�ջ
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
* @brief ��ʼ���ڴ�����ĵ���ģ��
* @param [in]  module  ��ʼ��ģ��
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
alayer_init_module(moduleMem_t * moduleMem)
{
	shelf_t *shelf   = NULL;//����
	int_t layer;
	//ģ���ڴ�����ʼ��
	list_init(&moduleMem->staticList);//��̬����
	list_init(&moduleMem->staticList);//�߳�ջ����  
	pthread_mutex_init(&moduleMem->moduleLock,NULL);//�ڴ����
	//���ܳ�ʼ��
	for(layer=0;layer<19;layer++) {
		_alayer_return_shelf(shelf, moduleMem->module, layer);
		shelf->layer       = layer; //���ܲ�
		shelf->giveBack    = 0;     //�黹����
		shelf->applyLevel  = 0;     //����ȼ�
		shelf->returnBlocks= 0;     //�黹�ڴ������
		stack_init(&shelf->freegood);//���л���
		stack_init(&shelf->returngood);//�黹����
		pthread_mutex_init(&shelf->returnLock,NULL);//�黹�ڴ���� 
		pthread_mutex_init(&shelf->freeLock,NULL);//�����ڴ���� 
	}
	return HALLOC_SUCC;
}


/**
* @brief ��ʼ���ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
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
	//ģ��س�ʼ��
	times.tv_sec = SUPERVISOR_TIME;
    times.tv_nsec = 0;
	CHECK_FF(timer_open(&modulePool.supervisor,&event)); //���Ա ����ڴ�Խ���
	CHECK_FF(timer_start(modulePool.supervisor,TIMER_MODE_NORMAL, TIMER_LOOP, &times));
	//ģ���ÿ���ڴ�����ʼ��
	for(module=0;module<MOD_BUTT;module++) {
		modulePool.memory[module].dynamicMem = 0;
		modulePool.memory[module].module = module;
		alayer_init_module(&modulePool.memory[module]);
	}
	return HALLOC_SUCC;
}



/**
* @brief �ݻ��ڴ�����
* @retval HALLOC_SUCC  ��ʾ�ɹ�
* @retval HALLOC_FAIL  ��ʾʧ��
*/
int_t
alayer_destroy(void)
{
	//�ݻٹ���Ա
	CHECK_FL(timer_stop(modulePool.supervisor));//�رն�ʱ��
	CHECK_FF(timer_close(&modulePool.supervisor)); //���Ա�ر�
    return HALLOC_SUCC;
}

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
alayer_get_meminfo(uint_t module, uint infoType)
{
	switch(infoType){
		case ALAYER_STATIC_MEM:return modulePool.memory[module].staticMem;
		case ALAYER_STACK_MEM:return modulePool.memory[module].threadMem;
		case ALAYER_DYNAMIC_MEM:return modulePool.memory[module].dynamicMem;
	}
	return HALLOC_FAIL;
}