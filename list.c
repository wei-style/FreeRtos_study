#include "FreeRTOS.h"
#include <stdlib.h>
#include "list.h"
//结点初始化
void vListInitialiseTtem(ListItem_t* const pxItem)
{
	pxItem->pvContainer = NULL;//让这个结点不包含在任何一个链表中
}

//根结点初始化
void vListInitialise(List_t* const pxlist)
{
	pxlist->uxNumberOfItems = 0;
	pxlist->pxIndex =(ListItem_t *)& pxlist->xListEnd;//在FreeRtos中，pxIndex默认指向最后一个结点，左边是指向结点的指针，右边要将地址赋予指针
	pxlist->xListEnd.xItemValue = 0xffffffff;//将最后一个结点的排序辅助值设置为最大，那么就能确保他就是最后一个结点
	pxlist->xListEnd.pxNext = (ListItem_t*)&(pxlist->xListEnd);//让根节点中的mini结点中的指向下一个结点的指针指向自己，说明这个链表没有首元素，自己就是首元素
	pxlist->xListEnd.pxPrevious = (ListItem_t*)&(pxlist->xListEnd);//让根节点中的mini结点中的指向前一个结点的指针指向自己，说明这个链表没有新的元素了，自己就是最后一个结点
}

void vListInsertEnd(List_t* const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t* const pxIndex = pxList->pxIndex;//指向最后一个结点
	pxNewListItem->pxNext= pxIndex;//让新结点的下一个指向尾结点，从前往后的方向
	pxIndex->pxPrevious = pxNewListItem;//尾结点的上一个结点指向新结点，从后往前方向

	pxIndex->pxPrevious->pxNext = pxNewListItem;//让尾结点指向的上一个元素的下一个元素指向新节点，从前往后方向
	pxNewListItem->pxPrevious = pxIndex->pxPrevious;//让新结点的上一个结点变为尾结点的上一个结点，从后往前方向
	
	//记录该节点所在的链表
	pxNewListItem->pvContainer =(void*) pxList;

	(pxList->uxNumberOfItems)++;	
}

void vListInsert(List_t* const pxList, ListItem_t* const pxNewListItem)
{
	ListItem_t* p;
	TickType_t value = pxNewListItem->xItemValue;//获取节点的排序辅助值
	if (value == portMAX_DELAY)//如果这个值是这个平台的最大值，就将他放在有效结点的最后一个位置，也就是mini结点前面
	{
		p = pxList->xListEnd.pxPrevious;//xListEnd是结构体变量，不是地址，所以后面用.找到变量中的某个值
	}
	else
	{
		//p从链表的尾结点开始，如果头结点（尾结点的下一个结点是头节点）的排序辅助值小于等于当前要插入节点的排序辅助值，就让p指向前一个结点，依次遍历
		for (p = (ListItem_t*)&(pxList->xListEnd); p->pxNext->xItemValue <= value; p = p->pxNext);//找到结点要插入的位置
	}
	pxNewListItem->pxNext = p->pxNext;//当前结点的下一个结点更新为p结点的下一个结点
	p->pxNext->pxPrevious = pxNewListItem;//p结点的下一个结点的上一个结点更新为pxNewListItem
	pxNewListItem->pxPrevious = p;//当前结点的前一个结点变为p
	p->pxNext = pxNewListItem;//将要插入的结点放在p结点的后面,这一句代码要放在最后面，不然找不到p的下一个节点了

	pxNewListItem->pvContainer = (void*)pxList;

	(pxList->uxNumberOfItems)++;

}
UBaseType_t vListRemobe(ListItem_t* const pxItemToRemove)//将结点从链表中移除，不需要传入链表参数，因为结点内部包含了所在链表信息
{
	List_t* p = (List_t *)pxItemToRemove->pvContainer;
	
	ListItem_t* q;
	q = pxItemToRemove;
	q->pxNext->pxPrevious = q->pxPrevious;
	q->pxPrevious->pxNext = q->pxNext;
	if (p->pxIndex == pxItemToRemove)//如果要删除的结点是尾结点
	{
		p->pxIndex = pxItemToRemove->pxPrevious;//让被删除节点的前一个结点称为新的尾结点
	}

	q->pvContainer = NULL;//初始化这个结点所在的链表为空，这样它就不会出现在任何一个节点中
	//q->xItemValue = 0;
	//q->pxPrevious = NULL;
	//q->pxNext = NULL;
	//q->pvOwner = NULL;
	//q = NULL;
	return --(p->uxNumberOfItems);
}
