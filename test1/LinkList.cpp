#include "LinkList.h"

void LinkListInsert(LinkList &L, Data data) {
	LNode *p = L.firstNode.next, *pre = &L.firstNode;
	while (p) {
		if (!(p->data < data))
			break;
		pre = p;
		p = p->next;
	}
	LNode *temp = new LNode;
	temp->data = data;
	temp->next = pre->next;
	pre->next = temp;
}

void LinkListMerge(LinkList& LA, LinkList& LB)
{
	LNode *pa = LA.firstNode.next, *pb = LB.firstNode.next;
	LNode *prea = &LA.firstNode, *preb = &LB.firstNode;
	while (pb) {
		while (pa) {
			if (!(pa->data < pb->data))
				break;
			prea = pa;
			pa = pa->next;
		}
		LNode *temp = pb;
		preb->next = pb->next;
		pb = pb->next;

		temp->next = prea->next;
		prea->next = temp;
		pa = temp;
	}
}
