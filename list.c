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
