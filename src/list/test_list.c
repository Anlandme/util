/******************************************************************************
        (c) COPYRIGHT 2011-  by D.C.P  Co.,Ltd
                          All rights reserved.
@file: test_signal_send.c
@brief: 
@attention gcc .././TEST/TEST.c .././errno/err.c list.c test_list.c -o test
@author   D.C.P 			                                                 
@version  0.0.0.1                                                               
@date     2017/03/14
******************************************************************************/
#include ".././util/util_inc.h"
#include ".././TEST/pub_TEST.h"

typedef struct {
        int value;
        list_node_t Node;
}example_t;


/**********************************************************************************************
*   Name：测试插入删除函数
**********************************************************************************************/
/**  @brief 测试当错误加入节点的情况 */
DEFINE_TEST_CODE_START(test_add_false_list)
    list_head_t list;
    list_node_t node;

    list_init(&list);//初始化
    list_node_init(&node);//节点初始化
    if( FUNC_FAIL == list_insert_node(INSERT_TO_HEAD, &list, NULL) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,加入NULL节点 函数返回不是预期FUNC_FAIL);

    if( FUNC_FAIL == list_insert_node(INSERT_TO_HEAD, NULL, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,加入时NULL链表时 函数返回不是预期FUNC_FAIL);

    if( FUNC_FAIL == list_insert_node(3, &list, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,使用非法的插入模式插入 函数返回不是预期FUNC_FAIL);
    //插入已存在的节点
    list_insert_node(INSERT_TO_HEAD, &list, &node);
    if( FUNC_FAIL == list_insert_node(INSERT_TO_HEAD, &list, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,重复插入一个节点 函数返回不是预期FUNC_FAIL);

DEFINE_TEST_CODE_END



/**  @brief 测试当错误删除节点的情况 */
DEFINE_TEST_CODE_START(test_del_false_list)
    list_head_t list;
    list_node_t node;

    list_init(&list);//初始化
    list_node_init(&node);//节点初始化
    list_insert_node(INSERT_TO_HEAD, &list, &node);//插入节点
    if( FUNC_FAIL == list_delete_node(&list, NULL) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,删除NULL节点 函数返回不是预期FUNC_FAIL);

    if( FUNC_FAIL == list_delete_node(NULL, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,删除时候NULL链表时 函数返回不是预期FUNC_FAIL);
    if( FUNC_FAIL == list_delete_node(NULL, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,删除时候NULL链表头节点也为空 函数返回不是预期FUNC_FAIL);
    //删除不存在的节点
    list_delete_node(&list, &node);
    if( FUNC_FAIL == list_delete_node(&list, &node) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,删除不存在的节点 函数返回不是预期FUNC_FAIL);
DEFINE_TEST_CODE_END



/**  @brief 测试当正常节点加入的情况 */
DEFINE_TEST_CODE_START(test_operate_ten_list)
    list_head_t list;
    list_node_t *node;
    list_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<10; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &list, node);//插入节点
    }
    
    for(i=0; i<10; i++){
        node = list_get_head_node(&list);
        list_delete_node(&list, node);
        free(node);
    }

    //从尾部加入 从尾部删除
    for(i=0; i<10; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_TAIL, &list, node);//插入节点
    }

    for(i=0; i<10; i++){
        node = list_get_tail_node(&list);
        list_delete_node(&list, node);
        free(node);
    }
DEFINE_TEST_CODE_END

/**  @brief 测试当正常节点嵌入的情况 */
DEFINE_TEST_CODE_START(test_embed_node_list)
    list_head_t list;
    list_node_t *node, *embed;
    list_init(&list);//初始化
    int i;
    //从头部嵌入 从头部删除
    node = (list_node_t *)malloc(sizeof(list_node_t));
    list_node_init(node);
    list_insert_node(INSERT_TO_HEAD, &list, node);//嵌入节点
    for(i=0; i<9; i++) {
        embed = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(embed);
        list_embed_node(i%2+1, &list, node, embed);//嵌入节点 随机嵌入
    }
    for(i=0; i<10; i++){
        node = list_get_head_node(&list);
        list_delete_node(&list, node);
        free(node);
    }

DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_operate_thousand_list)
    list_head_t list;
    list_node_t *node;
    list_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<1000; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &list, node);//插入节点
    }

    for(i=0; i<1000; i++){
        node = list_get_head_node(&list);
        list_delete_node(&list, node);
        free(node);
    }
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试其他函数接口
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_list_state)
    list_head_t list;
    list_node_t *node;
    list_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<10; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &list, node);//插入节点
    }
    test_printf("当前节点数： %d\n", list_get_current_member(&list));
    test_printf("所有进入的成员数： %d\n", list_get_sum_member(&list));
    test_printf("当前被删除成员数 %d\n", list_get_delete_member(&list));
    for(i=0; i<10; i++){
        node = list_get_head_node(&list);
        list_delete_node(&list, node);
        free(node);
    }
    test_printf("当前节点数： %d\n", list_get_current_member(&list));
    test_printf("所有进入的成员数： %d\n", list_get_sum_member(&list));
    test_printf("当前被删除成员数 %d\n", list_get_delete_member(&list));
DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_check_false_list)
    if( FUNC_FAIL == list_get_current_member(NULL) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,当list为NULL得到当前节点数 函数返回不是预期FUNC_FAIL);

    if( FUNC_FAIL == list_get_sum_member(NULL) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,当list为NULL得到当前所有进入的成员数 函数返回不是预期FUNC_FAIL);

    if( FUNC_FAIL == list_get_delete_member(NULL) )
        TEST_IS_TRUE(1,);
    else
        TEST_IS_TRUE(0,当list为NULL得到当前被删除成员数 函数返回不是预期FUNC_FAIL);

DEFINE_TEST_CODE_END

/**********************************************************************************************
*   Name：测试宏接口
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_list_macro)
    list_head_t list;
    example_t *node;
    list_node_t * tmp;
    list_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<10; i++) {
        node = (example_t *)malloc(sizeof(example_t));
        node->value = i;
        list_node_init(&(node->Node));
        list_insert_node(INSERT_TO_HEAD, &list, &(node->Node));//插入节点

    }

    //顺时针遍历节点
    tmp = list_get_head_node(&list); 
    do{
        node = list_takeout_struct(tmp, example_t, Node);
        test_printf("[%d]", node->value);
        tmp = list_get_next_node(tmp);
    }while(tmp != list_get_head_node(&list));
    test_printf("\n");
    //逆时针遍历节点
    tmp = list_get_tail_node(&list); 
    do{
        node = list_takeout_struct(tmp, example_t, Node);
        test_printf("[%d]", node->value);
        tmp = list_get_prev_node(tmp);
    }while(tmp != list_get_tail_node(&list));
    test_printf("\n");
    //删除节点
    for(i=0; i<10; i++){
        tmp = list_get_head_node(&list);
        list_delete_node(&list, tmp);
        node = list_takeout_struct(tmp, example_t, Node);
        free(node);
    }

DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试queue接口
**********************************************************************************************/
/**  @brief 测试当正常节点加入的情况 */
DEFINE_TEST_CODE_START(test_operate_ten_queue)
    queue_head_t list;
    queue_node_t *node;
    queue_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<10; i++) {
        node = (queue_node_t *)malloc(sizeof(queue_node_t));
        queue_node_init(node);
        queue_add_node( &list, node);//插入节点
    }

    for(i=0; i<10; i++){
        node = queue_remove_node(&list);
        free(node);
    }

DEFINE_TEST_CODE_END


DEFINE_TEST_CODE_START(test_operate_thousand_queue)
    queue_head_t list;
    queue_node_t *node;
    queue_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<1000; i++) {
        node = (queue_node_t *)malloc(sizeof(queue_node_t));
        queue_node_init(node);
        queue_add_node( &list, node);//插入节点
    }

    for(i=0; i<1000; i++){
        node = queue_remove_node(&list);
        free(node);
    }

DEFINE_TEST_CODE_END


/**********************************************************************************************
*   Name：测试queue接口
**********************************************************************************************/
/**  @brief 测试当正常节点加入的情况 */
DEFINE_TEST_CODE_START(test_operate_ten_stack)
    stack_head_t list;
    stack_node_t *node;
    stack_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<10; i++) {
        node = (stack_node_t *)malloc(sizeof(stack_node_t));
        stack_node_init(node);
        stack_push( &list, node);//插入节点
    }

    for(i=0; i<10; i++){
        node = stack_pop(&list);
        free(node);
    }

DEFINE_TEST_CODE_END

/**  @brief 测试当正常节点加入的情况 */
DEFINE_TEST_CODE_START(test_operate_thousand_stack)
    stack_head_t list;
    stack_node_t *node;
    stack_init(&list);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<1000; i++) {
        node = (stack_node_t *)malloc(sizeof(stack_node_t));
        stack_node_init(node);
        stack_push( &list, node);//插入节点
    }

    for(i=0; i<1000; i++){
        node = stack_pop(&list);
        free(node);
    }

DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试链表合并
**********************************************************************************************/
DEFINE_TEST_CODE_START(test_add_list_false)
    list_head_t main, children;
    TEST_FUNC_RETURN_FAIL(list_add_list(INSERT_TO_HEAD, &main, NULL, 1),  add返回错误操作);
    TEST_FUNC_RETURN_FAIL(list_add_list(INSERT_TO_HEAD, NULL, &children, 1),  add返回错误操作);
    TEST_FUNC_RETURN_FAIL(list_add_list(3, &main, &children, 1),  add返回错误操作);
    TEST_FUNC_RETURN_FAIL(list_add_list(INSERT_TO_HEAD, &main, &children, 0),  add返回错误操作);
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_add_head_list)
    list_head_t main, children;
    list_node_t *node;
    list_init(&main);//初始化
    list_init(&children);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<1; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(1, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&main), 当前main节点数);
    list_add_list(INSERT_TO_HEAD, &main, &children, 1);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(1, list_get_current_member(&main), 当前main节点数);
    //从头部加入 从头部删除
    for(i=0; i<4; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(4, list_get_current_member(&children), 当前chlidren节点数);
    list_add_list(INSERT_TO_HEAD, &main, &children, 4);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(5, list_get_current_member(&main), 当前main节点数);
    //从头部加入 从头部删除
    for(i=0; i<5; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(5, list_get_current_member(&children), 当前chlidren节点数);
    list_add_list(INSERT_TO_HEAD, &main, &children, 4);
    TEST_VALUE_IS_EQ(1, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(9, list_get_current_member(&main), 当前main节点数);
    //list_printf(&main);
    for(i=0; i<9; i++){
        node = list_get_head_node(&main);
        list_delete_node(&main, node);
        free(node);
    }
    //list_printf(&children);
    for(i=0; i<1; i++){
        node = list_get_head_node(&children);
        list_delete_node(&children, node);
        free(node);
    } 
DEFINE_TEST_CODE_END

DEFINE_TEST_CODE_START(test_add_tail_list)
    list_head_t main, children;
    list_node_t *node;
    list_init(&main);//初始化
    list_init(&children);//初始化
    int i;
    //从头部加入 从头部删除
    for(i=0; i<1; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(1, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&main), 当前main节点数);
    list_add_list(INSERT_TO_HEAD, &main, &children, 1);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(1, list_get_current_member(&main), 当前main节点数);
    //从头部加入 从头部删除
    for(i=0; i<4; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(4, list_get_current_member(&children), 当前chlidren节点数);
    list_add_list(INSERT_TO_TAIL, &main, &children, 4);
    TEST_VALUE_IS_EQ(0, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(5, list_get_current_member(&main), 当前main节点数);
    //从头部加入 从头部删除
    for(i=0; i<5; i++) {
        node = (list_node_t *)malloc(sizeof(list_node_t));
        list_node_init(node);
        list_insert_node(INSERT_TO_HEAD, &children, node);//插入节点
    }
    TEST_VALUE_IS_EQ(5, list_get_current_member(&children), 当前chlidren节点数);
    list_add_list(INSERT_TO_TAIL, &main, &children, 4);
    TEST_VALUE_IS_EQ(1, list_get_current_member(&children), 当前chlidren节点数);
    TEST_VALUE_IS_EQ(9, list_get_current_member(&main), 当前main节点数);
    //list_printf(&main);
    for(i=0; i<9; i++){
        node = list_get_head_node(&main);
        list_delete_node(&main, node);
        free(node);
    }
    //list_printf(&children);
    for(i=0; i<1; i++){
        node = list_get_head_node(&children);
        list_delete_node(&children, node);
        free(node);
    } 
DEFINE_TEST_CODE_END
/**********************************************************************************************
*   Name：测试主函数
**********************************************************************************************/
int main (void)
{

    TEST_SUITE_START(测试list加入节点接口非正常使用)
        TEST_CASE(test_add_false_list,          加入节点失败测试,       测试list_insert_node各种插入失败的操作);
        TEST_CASE(test_del_false_list,          删除节点失败测试,       测试list_delete_node各种插入失败的操作);
    TEST_SUITE_END

    TEST_SUITE_START(测试list节点增减正常使用)
        TEST_CASE(test_embed_node_list,         嵌入节点测试,         测试嵌入节点操作);
        TEST_CASE(test_operate_ten_list,        处理10个节点测试,      测试增减10个节点操作);
        TEST_CASE(test_operate_thousand_list,   处理1000个节点测试,    测试增减1000个节点操作 主要测试性能);
    TEST_SUITE_END

    TEST_SUITE_START(测试list其他接口使用)
        TEST_CASE(test_list_state,              接口正常使用,         打印链表的当前节点数 所有节点数 删除节点数);
        TEST_CASE(test_check_false_list,        接口非正常使用,       测试各种错误调用其他接口的操作);
    TEST_SUITE_END

    TEST_SUITE_START(测试list宏接口使用)
        TEST_CASE(test_list_macro,          宏接口使用,       测试各种宏接口的操作);
    TEST_SUITE_END


    TEST_SUITE_START(测试queue接口使用)
        TEST_CASE(test_operate_ten_queue,        处理10个节点测试,      测试增减10个节点操作);
        TEST_CASE(test_operate_thousand_queue,   处理1000个节点测试,    测试增减1000个节点操作 主要测试性能);
    TEST_SUITE_END

    TEST_SUITE_START(测试stack接口使用)
        TEST_CASE(test_operate_ten_stack,        处理10个节点测试,      测试增减10个节点操作);
        TEST_CASE(test_operate_thousand_stack,   处理1000个节点测试,    测试增减1000个节点操作 主要测试性能);
    TEST_SUITE_END

    TEST_SUITE_START(测试链表追加接口使用)
        TEST_CASE(test_add_list_false,   追加失败,         追加失败操作);
        TEST_CASE(test_add_head_list,    从首部追加链表,    从首部追加链表操作);
        TEST_CASE(test_add_tail_list,    从尾部追加链表,    从尾部追加链表操作);
    TEST_SUITE_END

    return 0;
}
