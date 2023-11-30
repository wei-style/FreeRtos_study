#ifndef LIST_H
#define LIST_H

#include "FreeRTOS.h"

struct xLIST_ITEM//结点结构体
{
	TickType_t xItemValue;             /* 辅助值，用于帮助节点做顺序排列 */
	struct xLIST_ITEM* pxNext;       /* 指向链表下一个节点 */
	struct xLIST_ITEM* pxPrevious;   /* 指向链表前一个节点 */
	void* pvOwner;					   /* 指向拥有该节点的内核对象，通常是TCB */
	void* pvContainer;		       /* 指向该节点所在的链表 */
};
typedef struct xLIST_ITEM ListItem_t;  /* 节点数据类型重定义 */

//结点初始化
void vListInitialiseTtem(ListItem_t* const pxItem);//结点的结构体指针

//定义mini结点结构体，它用来连接链表中最后插入的元素和首元素（双向链表首就是尾，尾就是首，用mini节点分），这个时候可以将它看作是链表的最后一个结点，mini结点没有定义拥有者和所在链表，因为它包含在根节点中。
struct xMINI_LIST_ITEM
{
	TickType_t xItemValue;                      /* 辅助值，用于帮助节点做升序排列 */
	struct xLIST_ITEM* pxNext;                /* 指向链表下一个节点 */
	struct xLIST_ITEM* pxPrevious;            /* 指向链表前一个节点 */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;  /* 最小节点数据类型重定义 */

//定义链表根结点结构体，它相当于包裹在mini结点周围，它的前一项也是最后插入的元素，他的后一项是首元素，同时它的内部还有整个链表的有效节点个数，它是链表元素插入和删除的参考点位。
typedef struct xLIST
{
	UBaseType_t uxNumberOfItems;    /* 链表节点计数器 */  //用于表示链表中有效成员的个数
	ListItem_t* pxIndex;			/* 链表节点索引指针 */	//用于找到链表中的成员，FreeRtos默认指向最后一个元素
	MiniListItem_t xListEnd;		/* 链表最后一个节点 */	//指向链表的最后一个结点
} List_t;

//根结点初始化
void vListInitialise(List_t * const pxlist);
