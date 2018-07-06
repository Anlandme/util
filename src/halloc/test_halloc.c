/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_halloc.c
@brief: 
@attention gcc .././TEST/TEST.c .././errno/err.c list.c test_list.c -o test
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/14
******************************************************************************/
#include ".././util/util_inc.h"
#include ".././TEST/pub_TEST.h"
#include "halloc.h"

/**  @brief 测试链表排序 */
int_t
test_list_sort(stack_head_t *list)
{
	block_t *block = NULL;//内存块
    uint size= 0;

	traversal_list_do(block, list, blockNode){
		if(size > block->blockSize) return HALLOC_FAIL;
        size = block->blockSize;
	}while_traversal_list(list);

    return HALLOC_SUCC;
}
/**********************************************************************************************
*   Name：测试函数错误函数
**********************************************************************************************/
/**  @brief 测试申请释放的情况 */
DEFINE_TEST_CODE_START(test_halloc_false)
    list_head_t list;
    uint_t info[5] = { 0, 48, 0, 64, 3};
    TEST_FUNC_RETURN_NULL(slayer_malloc(4,10),               测试slayer_malloc未初始化情况);
    TEST_FUNC_RETURN_NULL(mlayer_malloc_thread_stack(10),    测试申请10k栈mlayer_malloc_thread_stack未初始化情况);
    TEST_FUNC_RETURN_NULL(mlayer_static_malloc(3*1024),      测试申请大于静态内存页mlayer_static_malloc未初始化情况); 
    TEST_FUNC_RETURN_FAIL(mlayer_dynamic_malloc(&list,info), 测试mlayer_dynamic_malloc未初始化情况); 

    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
        TEST_FUNC_RETURN_NULL(slayer_malloc(PAGE_MAX_SIZE,10),   测试slayer_malloc错误情况);
        TEST_FUNC_RETURN_NULL(slayer_malloc(0,10),               测试slayer_malloc错误情况);
        TEST_FUNC_RETURN_NULL(slayer_malloc(4,BLOCK_MAX_SIZE+1), 测试slayer_malloc错误情况);
        TEST_FUNC_RETURN_FAIL(slayer_free(NULL),                 测试slayer_free错误情况);
        TEST_FUNC_RETURN_NULL(mlayer_static_malloc(5*1024),      测试申请大于静态内存页mlayer_static_malloc错误情况);
        TEST_FUNC_RETURN_FAIL(mlayer_dynamic_malloc(NULL,info),  测试mlayer_dynamic_malloc错误情况); 
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
    TEST_FUNC_RETURN_NULL(hmalloc(102400*1024-sizeof(checkinfo_t), MEM_THREAD_STACK, MOD_SYSREM,__FILE__, __FUNCTION__,  __LINE__), 测试hmalloc错误情况);
    TEST_FUNC_RETURN_NULL(hmalloc(100, MEM_THREAD_STACK, MOD_BUTT, __FILE__, __FUNCTION__, __LINE__), 测试hmalloc错误情况);
    TEST_FUNC_RETURN_NULL(hmalloc(100, MEM_THREAD_STACK+1, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__), 测试hmalloc错误情况);
    TEST_FUNC_RETURN_FAIL(hfree(NULL), 测试hfree错误情况);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：系统层测试函数使用函数
**********************************************************************************************/
/**  @brief 测试申请释放的情况 */
DEFINE_TEST_CODE_START(test_halloc_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(3,16), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放内存页);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END

/**  @brief 测试pab栈的情况 */
DEFINE_TEST_CODE_START(test_pab_stack_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(5,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(3,32), 申请内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(3,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END


/**  @brief 测试页表的情况 */
DEFINE_TEST_CODE_START(test_page_table_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(5,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(1,8), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(9,9), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(3,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(5,64), 申请内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    //最后顺序 7 5 1 9 3
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END


/**  @brief 测试剪纸算法的情况 */
DEFINE_TEST_CODE_START(test_cut_page_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(5,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(2,8), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（2k，8）的内存页);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END

/**  @brief 测试碎纸算法的情况 */
DEFINE_TEST_CODE_START(test1_tatter_page_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(7,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(5,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(1,8), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(9,9), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(3,64), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（3k，64）的内存页);
    //最后顺序 7 5 1 9 3
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(100,1), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（100k，1）的内存页);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test2_tatter_page_system_layer)
    page_t * page;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化halloc);
    TEST_FUNC_RETURN_IS_NOT_NULL(page = slayer_malloc(100,1), 申请内存页);
    TEST_FUNC_RETURN_SUCC(slayer_free(page), 释放（100k，1）的内存页);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化halloc);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：资源管理层测试函数使用函数
**********************************************************************************************/
extern staticmem_t StaticMem;
extern dynamicmem_t DynamicMem;
DEFINE_TEST_CODE_START(test_mf_threadstack_manager_layer)
    block_t * block;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(10), 申请线程栈);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(10), 申请线程栈);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(30), 申请线程栈);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(90), 申请线程栈);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(20), 申请线程栈);
    mlayer_free(block);
    TEST_VALUE_IS_EQ(5, stack_get_current_member(&StaticMem.threadList), 线程栈数量);
    mlayer_staticmem_supervisor(0);
    //sleep(2);
    TEST_VALUE_IS_EQ(5, stack_get_current_member(&StaticMem.freethreadList), 线程栈内存);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(10), 申请线程栈);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(10), 申请线程栈);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(30), 申请线程栈);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(90), 申请线程栈);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_malloc_thread_stack(20), 申请线程栈);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&StaticMem.freethreadList), 线程栈内存);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_mf_staticmem_manager_layer)
    block_t * block;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(10)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(10)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(30)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(90)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(20)), 申请内存块);
    mlayer_free(block);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&StaticMem.freeblockList), 闲置内存块数量);
    mlayer_staticmem_supervisor(0);
    //sleep(2);
    TEST_VALUE_IS_EQ(5, stack_get_current_member(&StaticMem.freeblockList), 闲置内存块数量);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(10)), 申请内存块);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(10)), 申请内存块);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(30)), 申请内存块);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(90)), 申请内存块);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(20)), 申请内存块);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&StaticMem.freeblockList), 闲置内存块数量);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_staticpage_work_manager_layer)
    block_t * block;
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(1000)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(2000)), 申请内存块);
    mlayer_free(block);
    TEST_FUNC_RETURN_IS_NOT_NULL(block = mlayer_static_malloc(SWITCH_TO_BLOCKSIZE(3000)), 申请内存块);
    mlayer_free(block);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&StaticMem.freeblockList), 闲置内存块数量);
    mlayer_staticmem_supervisor(0);
    //sleep(2);
    TEST_VALUE_IS_EQ(3, stack_get_current_member(&StaticMem.freeblockList), 闲置内存块数量);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_mf_huge_dynamicmem_manager_layer)
    block_t * block;
    uint_t blockinfo[5] = { 18, 0, 0, 1,  0};
    list_head_t blockList;
    list_init(&blockList);
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(3000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(3000);blockinfo[4]=SWITCH_TO_KB(3000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(5000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(5000);blockinfo[4]=SWITCH_TO_KB(5000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(4000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(4000);blockinfo[4]=SWITCH_TO_KB(4000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    TEST_VALUE_IS_EQ(3, stack_get_current_member(&blockList), 闲置内存块数量);
    block = list_takeout_struct(list_get_head_node(&blockList), block_t, blockNode);
    block->color = BLOCK_GREEN;
    list_delete_node(&blockList, &block->blockNode);
    block = list_takeout_struct(list_get_head_node(&blockList), block_t, blockNode);
    block->color = BLOCK_GREEN;
    list_delete_node(&blockList, &block->blockNode);
    block = list_takeout_struct(list_get_head_node(&blockList), block_t, blockNode);
    block->color = BLOCK_GREEN;
    list_delete_node(&blockList, &block->blockNode);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&blockList), 闲置内存块数量);
    mlayer_dynamicmem_supervisor(18);
    TEST_VALUE_IS_EQ(3, stack_get_current_member(&DynamicMem.huge.freegood), 闲置内存块数量);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(3000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(3000);blockinfo[4]=SWITCH_TO_KB(3000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(5000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(5000);blockinfo[4]=SWITCH_TO_KB(5000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    blockinfo[1]=SWITCH_TO_BLOCKSIZE(4000);blockinfo[2]=SWITCH_TO_BLOCKSIZE(4000);blockinfo[4]=SWITCH_TO_KB(4000);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请动态内存块);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&DynamicMem.huge.freegood), 闲置内存块数量);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_mf_smb_dynamicmem_manager_layer)
    block_t * block;
    uint_t blockinfo[5] = { 0,  48,   20*48, 64, 3};
    list_head_t blockList;
    list_init(&blockList);
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_VALUE_IS_EQ(80, stack_get_current_member(&blockList), 内存块数量);
    traversal_list_do(block, &blockList, blockNode){//遍历链表
        TEST_FUNC_RETURN_SUCC(list_delete_node(&blockList, &block->blockNode), 从链表中删除block);
        TEST_FUNC_RETURN_SUCC(mlayer_free(block), 释放block到管理层);
	}while_traversal_list(&blockList);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    TEST_VALUE_IS_EQ(1, stack_get_current_member(&DynamicMem.small[0].goodsArea), 闲置内存块数量);
    traversal_list_do(block, &blockList, blockNode){//释放全部block
        TEST_FUNC_RETURN_SUCC(list_delete_node(&blockList, &block->blockNode), 从链表中删除block);
        TEST_FUNC_RETURN_SUCC(mlayer_free(block), 释放block到管理层);
	}while_traversal_list(&blockList);
    //再次申请
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList, blockinfo), 申请20个动态内存块);
    TEST_VALUE_IS_EQ(60, stack_get_current_member(&blockList), 闲置内存块数量);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_mf_smb_sort_manager_layer)
    block_t * block;
    page_t * page;
    uint_t index;
    uint_t result[2][5]= {{5,4,3,2,1}, {10,6,5,3,2}};
    uint_t blockinfo[5] = { 0,  48,   64*48, 64, 3};
    list_head_t blockList[5];
    list_init(&blockList[0]);list_init(&blockList[1]);list_init(&blockList[2]);list_init(&blockList[3]);list_init(&blockList[4]);
    TEST_FUNC_RETURN_SUCC(slayer_init(), 初始化slayer);
    TEST_FUNC_RETURN_SUCC(mlayer_init(), 初始化mlayer);
    mlayer_dynamicmem_supervisor(0);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList[0], blockinfo), 申请64个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList[1], blockinfo), 申请64个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList[2], blockinfo), 申请64个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList[3], blockinfo), 申请64个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    TEST_FUNC_RETURN_SUCC(mlayer_dynamic_malloc(&blockList[4], blockinfo), 申请64个动态内存块);
    mlayer_dynamicmem_supervisor(0);
    index =0;
    traversal_list_do(page, &DynamicMem.small[0].goodsArea, pageNode){
        TEST_VALUE_IS_EQ(result[0][index++], page->maxTime, 排序前数值);
    }while_traversal_list(&DynamicMem.small[0].goodsArea);
    traversal_list_do(block, &blockList[2], blockNode){block->lease= 9;}while_traversal_list(&blockList);
    mlayer_dynamicmem_supervisor(0);
    index =0;
    traversal_list_do(page, &DynamicMem.small[0].goodsArea, pageNode){
        TEST_VALUE_IS_EQ(result[1][index++], page->maxTime, 排序前数值);
    }while_traversal_list(&DynamicMem.small[0].goodsArea);
    TEST_FUNC_RETURN_SUCC(mlayer_destroy(), 初始化mlayer);
    TEST_FUNC_RETURN_SUCC(slayer_destroy(), 初始化slayer);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：资源管理层测试函数使用函数
**********************************************************************************************/
/**  @brief 模块内存池 */
extern modulePool_t modulePool;
DEFINE_TEST_CODE_START(test_static_stack_alloc_layer)
    void * memory;
    TEST_FUNC_RETURN_SUCC(halloc_init(), 初始化halloc);
    memory = hmalloc(100, MEM_STATIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory, 申请静态内存);
    TEST_VALUE_IS_EQ(124, alayer_get_meminfo(MOD_SYSREM, ALAYER_STATIC_MEM), 当前模块的静态内存大小);
    TEST_FUNC_RETURN_SUCC(hfree(memory), 释放静态内存);
    TEST_VALUE_IS_EQ(0, alayer_get_meminfo(MOD_SYSREM, ALAYER_STATIC_MEM), 当前模块的静态内存大小);
    //线程栈申请
    memory = hmalloc(10*1024, MEM_THREAD_STACK, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory, 申请线程栈);
    TEST_VALUE_IS_EQ(10240, alayer_get_meminfo(MOD_SYSREM, ALAYER_STACK_MEM), 当前模块的线程栈大小);
    TEST_FUNC_RETURN_SUCC(hfree(memory), 释放线程栈);
    TEST_VALUE_IS_EQ(0, alayer_get_meminfo(MOD_SYSREM, ALAYER_STACK_MEM), 当前模块的线程栈大小);
    TEST_FUNC_RETURN_SUCC(halloc_destroy(), 摧毁halloc);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_dynamic_alloc_layer)
    block_t * block;
    shelf_t *huge = &modulePool.memory[MOD_SYSREM].huge; 
    void *memory0, *memory1, *memory2, *memory3;
    TEST_FUNC_RETURN_SUCC(halloc_init(), 初始化halloc);
    //huge内存块管理
    memory0 = hmalloc(4000, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    memory1 = hmalloc(6000, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    memory2 = hmalloc(5000, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    memory3 = hmalloc(3000, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory0, 申请动态内存);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory1, 申请动态内存);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory2, 申请动态内存);
    TEST_FUNC_RETURN_IS_NOT_NULL(memory3, 申请动态内存);
    TEST_FUNC_RETURN_SUCC(test_list_sort(&huge->freegood), 检测申请后内存块排序是否正确);
    TEST_FUNC_RETURN_SUCC(hfree(memory0), 释放动态内存);
    TEST_FUNC_RETURN_SUCC(hfree(memory1), 释放动态内存);
    TEST_FUNC_RETURN_SUCC(hfree(memory2), 释放动态内存);
    TEST_FUNC_RETURN_SUCC(hfree(memory3), 释放动态内存);
    traversal_list_do(block, &huge->freegood, blockNode){
        block->lease = LEASE_TIME;
    }while_traversal_list(&huge->freegood);
    TEST_VALUE_IS_EQ(4, stack_get_current_member(&huge->freegood), 释放完huge内内存块);
    alayer_supervisor(0);
    TEST_VALUE_IS_EQ(0, stack_get_current_member(&huge->freegood), 释放完huge内内存块);
    //small middle big内存块管理
    void *mem[102];
    shelf_t *small = &modulePool.memory[MOD_SYSREM].small[5]; 
    int_t index;
    for(index=0; index<102; index++){
        mem[index] = hmalloc(100, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
        TEST_FUNC_RETURN_IS_NOT_NULL(mem[index], 申请动态内存);
    }
    TEST_VALUE_IS_EQ(6, stack_get_current_member(&small->freegood), 没释放small内内存块);
    for(index=0; index<102; index++) TEST_FUNC_RETURN_SUCC(hfree(mem[index]), 释放动态内存);
    alayer_supervisor(0);
    TEST_VALUE_IS_EQ(108, stack_get_current_member(&small->freegood), 释放完small内内存块);
    for(index=0; index<102; index++){
        mem[index] = hmalloc(100, MEM_DYNAMIC, MOD_SYSREM,__FILE__, __FUNCTION__, __LINE__);
        TEST_FUNC_RETURN_IS_NOT_NULL(mem[index], 申请动态内存);
    }
    TEST_VALUE_IS_EQ(6, stack_get_current_member(&small->freegood), 没释放small内内存块);
    for(index=0; index<102; index++) {
        TEST_FUNC_RETURN_SUCC(hfree(mem[index]), 释放动态内存);
        checkinfo_t  *headcheck = (checkinfo_t *)((char *)mem[index] - sizeof(checkinfo_t));
        block_t *block = headcheck->block;
        block->lease = LEASE_TIME;
    }
    alayer_supervisor(0);
    TEST_VALUE_IS_EQ(6, stack_get_current_member(&small->freegood), 释放完small内内存块);
        sleep(8);
    TEST_FUNC_RETURN_SUCC(halloc_destroy(), 摧毁halloc);
DEFINE_TEST_CODE_END

/**********************************************************************************************
*   Name：halloc 性能测试
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_malloc_free_performance)
        struct timespec start, end;
        void *memory;
        int_t index;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for(index=1; index<1024; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-1KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<2048; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-2KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<4096; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-4KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<8192; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-8KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<16*1024; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-16KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<32*1024; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-32KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<64*1024; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-64KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<128*1024; index++) {memory = malloc(index);free(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc and free 0-128KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_halloc_hfree_performance)
        struct timespec start, end;
        void *memory;
        int_t index;
        TEST_FUNC_RETURN_SUCC(halloc_init(), 初始化halloc);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for(index=1; index<1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-1KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<2048; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-2KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<4096; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-4KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<8192; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-8KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<16*1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-16KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<32*1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-32KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<64*1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-64KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<128*1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);hfree(memory);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc and hfree 0-128KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        mlayer_check_overflow();
        TEST_FUNC_RETURN_SUCC(halloc_destroy(), 摧毁halloc);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_malloc_performance)
        struct timespec start, end;
        void *memory[16*1024];
        int_t index;
        printf("\n\n\n\n\n\n\n");
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for(index=1; index<1024; index++) {memory[index] = malloc(index);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc 0-1KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<2048; index++) {memory[index] = malloc(index);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc 0-2KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<4096; index++) {memory[index] = malloc(index);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc 0-4KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<8192; index++) {memory[index] = malloc(index);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc 0-8KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<16*1024; index++) {memory[index] = malloc(index);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("malloc 0-16KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(index=1; index<16*1024; index++) {free(memory[index]);}
DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_halloc_performance)
        struct timespec start, end;
        void *memory;
        int_t index;
        TEST_FUNC_RETURN_SUCC(halloc_init(), 初始化halloc);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for(index=1; index<1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc 0-1KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<2048; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc 0-2KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<4096; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc 0-4KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<8192; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc 0-8KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        for(; index<16*1024; index++) {memory = halloc(index, MEM_DYNAMIC, MOD_SYSREM);}
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        printf("halloc 0-16KB wastes %.5lf seconds\n", (end.tv_sec-start.tv_sec) + (double)(end.tv_nsec-start.tv_nsec)/1000000000);
        mlayer_check_overflow();
        TEST_FUNC_RETURN_SUCC(halloc_destroy(), 摧毁halloc);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：内存泄漏越界测试函数使用函数
**********************************************************************************************/
void *test_thread(void *arg)
{
   void *memory;
    memory = halloc(100, MEM_DYNAMIC, MOD_SYSREM);
    printf("child thread memory:%p\n", memory);
    sleep(20);
}


DEFINE_TEST_CODE_START(test_memory_overflow)
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_memory_leak)
        thread_t id1, id2, id3;
        void *memory;
        id1 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
        //id2 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
        //id3 = thread_create(MOD_SYSREM, NULL, test_thread, NULL);
        TEST_FUNC_RETURN_IS_NOT_NULL(id1, 创建线程1);
        //TEST_FUNC_RETURN_IS_NOT_NULL(id2, 创建线程2);
        //TEST_FUNC_RETURN_IS_NOT_NULL(id3, 创建线程3);
        TEST_FUNC_RETURN_SUCC(halloc_init(), 初始化halloc);
        TEST_FUNC_RETURN_SUCC(gc_init(), 初始化gc);
        //检查内存泄漏检查
        memory = halloc(100, MEM_DYNAMIC, MOD_SYSREM);
        printf("main thread memory:%p\n", memory);
        checkinfo_t *headcheck = (checkinfo_t *)((char *)memory - sizeof(checkinfo_t));
        block_t *block = headcheck->block;
        block->lease = LEAK_CHECK;
        //检查申请时间过长检查
        memory = halloc(100, MEM_DYNAMIC, MOD_SYSREM);
        headcheck = (checkinfo_t *)((char *)memory - sizeof(checkinfo_t));
        block = headcheck->block;
        block->lease = LEAK_CHECK;
        printf("main thread memory:%p\n", memory);
        sleep(18);
        TEST_FUNC_RETURN_SUCC(thread_join(id1,NULL), 等待线程1退出);
        //TEST_FUNC_RETURN_SUCC(thread_join(id2,NULL), 等待线程2退出);
        //TEST_FUNC_RETURN_SUCC(thread_join(id3,NULL), 等待线程3退出);
        TEST_FUNC_RETURN_SUCC(halloc_destroy(), 摧毁halloc);
        TEST_FUNC_RETURN_SUCC(gc_destroy(), 摧毁gc);
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试主函数
**********************************************************************************************/
int main(void)
{
    _test_start_unit_test();
    TEST_SUITE_START(测试错误函数)
        TEST_CASE(test_halloc_false,          函数运行失败测试,       测试失败的操作);
    TEST_SUITE_END

    TEST_SUITE_START(系统层测试函数)
        //TEST_CASE(test_halloc_system_layer,        申请一个的内存页,       申请一个小于PAGE_TABLE_SIZE的内存页);
        //TEST_CASE(test_pab_stack_system_layer,      测试pab栈的使用,       测试pad栈对于block和page正常释放和申请);
        //TEST_CASE(test_page_table_system_layer,      测试页表的使用,       测试整个页表的正常使用);
        //TEST_CASE(test_cut_page_system_layer,      测试剪纸算法使用,       测试剪纸算法的正常使用);
        //TEST_CASE(test1_tatter_page_system_layer, 测试碎纸算法1使用,       测试碎纸算法1的正常使用);
        //TEST_CASE(test2_tatter_page_system_layer, 测试碎纸算法2使用,       测试碎纸算法2的正常使用);
    TEST_SUITE_END

    TEST_SUITE_START(资源管理层测试函数)
        //TEST_CASE(test_mf_threadstack_manager_layer,     申请释放一个的线程栈,         申请释放线程栈);
        //TEST_CASE(test_mf_staticmem_manager_layer,       申请释放一个的静态内存,       申请释放静态内存);
        //TEST_CASE(test_staticpage_work_manager_layer,    静态内存页的正常运转,         静态内存页的正常运转);
        //TEST_CASE(test_mf_huge_dynamicmem_manager_layer, 申请释放一个huge的动态内存块, 申请释放一个huge的动态内存块);
        //TEST_CASE(test_mf_smb_dynamicmem_manager_layer,  申请释放一个small-middle-big的动态内存块, 申请释放一个small-middle-big的动态内存块);
        //TEST_CASE(test_mf_smb_sort_manager_layer,        对于small-middle-big动态排序, 对于small-middle-big动态排序);
    TEST_SUITE_END

    TEST_SUITE_START(资源分配层测试函数)
        //TEST_CASE(test_static_stack_alloc_layer,     测试获取静态内存以及线程栈内存,     获取静态内存以及线程栈内存);
        //TEST_CASE(test_dynamic_alloc_layer,          测试获取动态内存,                  获取动态内存);
    TEST_SUITE_END

    TEST_SUITE_START(性能测试函数)
        //TEST_CASE(test_malloc_free_performance,  测试malloc性能,     malloc获取不同内存后释放下效率);
        //TEST_CASE(test_halloc_hfree_performance, 测试halloc性能,     halloc获取不同内存后释放下效率);
        //TEST_CASE(test_malloc_performance,       测试malloc性能,     malloc获取不同内存后不释放下效率);
        //TEST_CASE(test_halloc_performance,       测试halloc性能,     halloc获取不同内存后不释放下效率);
    TEST_SUITE_END

    TEST_SUITE_START(内存越界泄漏测试函数)
        //TEST_CASE(test_memory_overflow, 内存越界检查, 内存越界检查);
        TEST_CASE(test_memory_leak,     内存泄漏检查, 内存泄漏检查);
    TEST_SUITE_END

    return 0;
}
