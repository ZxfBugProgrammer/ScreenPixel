#pragma once

struct Data
{
	int y, x, deltax, deltay, cur;

	Data(int ty = 0, int tx = 0, int tdeltay = 0,int tdeltax = 0) {
		y = ty, x = tx, deltax = tdeltax, deltay = tdeltay;
		cur = 0;
	}
	
	//×¢ÒâÒªĞ±ÂÊÅÅĞò
	bool operator < (const Data &t) const {
		if (x == t.x)
			return 1.0*deltax/deltay < 1.0*t.deltax/t.deltay;
		return x < t.x;
	}
};

struct LNode
{
	Data data;
	LNode *next;
	LNode() {
		next = nullptr;
	}
};

struct LinkList
{
	int val;
	LNode firstNode;

	LinkList() {
		firstNode.next = nullptr;
	}
};

void LinkListInsert(LinkList &L, Data data);
void LinkListMerge(LinkList &LA, LinkList& LB);