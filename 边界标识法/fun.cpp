#include"word.h"
#include<stdio.h>
#include<assert.h>
#define MAX_SIZE  6553598
extern Pword Phead;//内存上边界
extern Pword Ptail;//内存下边界
//Pword userSpace[MAX_SIZE] = { NULL };//用户空间数组
//int usCount = 0;
static Pword Get_tail(Pword head)
{
	return head + head->len +1 ;
}

void Init_word(Pword head)//初始化内存
{
	head[0].len = 6553598;
	head[0].Llink = head;
	head[0].Rlink = head;
	head[0].tag = INIT;

	Pword tail = Get_tail(head);
    tail->Uplink = head;
	tail->tag = INIT;
}

Pword Change_word(Pword *head,Pword p, int len)
{
	if (p->len - len - 2 < 1)
	{	//从空闲链表中删除该内存块
		p->Llink->Rlink = p->Rlink;
		p->Rlink->Llink = p->Llink;

		p->tag = 1;
		Get_tail(p)->tag = 1;
		//判断是否最后一个内存块
		if (*head == p)
		{
			*head = NULL;
		}
		//userSpace[usCount++] = p + 1;
		return p + 1;
	}
	else
	{
		//获取剪切部分的指针
		Pword q = p + p->len - len;

		q->len = len;
		q->tag = 1;
		Get_tail(q)->Uplink = q;
		Get_tail(q)->tag = 1;

		//修改剩余部分的内存块的信息
		p->len = p->len - len - 2;
		Get_tail(p)->Uplink = p;
		//返回用户可操作的内存地址
		//userSpace[usCount++] = q+ 1;
		return q + 1;
	}
}
Pword Get_word(Pword*head, int size)
{
	if (size <= 0 || NULL == head)
	{
		return NULL;
	}
	int len = (size + 15) / 16;//计算所需要的格子数
	
	Pword p=*head;
	for (;; p = p->Llink)//检测第一个可用格子数
	{
		if (p->len >= len || p == (*head)->Rlink)
		{
			break;
		}
	}
	if (p->len >= len)
	{
		return Change_word(head, p, len);
	}
	else
	{
		return NULL;
	}
}
void del_word(Pword *head,Pword p)//内存块合并
{
	Pword ptr = p - 1;

	//是否可以向上合并
	if (ptr != Phead && INIT== (ptr - 1)->tag)
	{
		Pword foot= (ptr-1)->Llink;
		foot->len += ptr->len;
		foot->Uplink = ptr;
		foot->tag = 0;
	}
	else//不可以向上合并  整合到链表
	{
		if (NULL == *head)
		{
			*head = ptr;
			ptr->Rlink = ptr;
			p->Llink = ptr;
		}
		else
		{
			(*head)->Llink->Rlink = ptr;
			ptr->Llink = (*head)->Llink;
			(*head)->Llink = ptr;
			ptr->Rlink = (*head);
		}
	}
	if ((Get_tail(ptr) + 1) != Ptail && INIT == (Get_tail(ptr) + 1)->tag)
	{
		Pword foot = (Get_tail(ptr)+1)->Uplink;
		foot->len += ptr->len;
		ptr->tag = INIT;
	}
	/*for (int i = 0; i < usCount; i++)
	{
		if (ptr == userSpace[i])
		{
			userSpace[i] = NULL;
		}
	}*/
}
void  Printf(Pword pre)
{
	printf("空间首地址：%0x，空间大小：%d，前驱地址：%0x，后继地址：%0x\n", pre, pre->len, pre->Llink, pre->Rlink);
}
void   PrintfSpace(Pword head)
{
	if (head != NULL)
	{
		printf("--------可利用空间表---------\n");
		Pword p = head;
		Printf(p);
		for (p = p->Rlink; p != head; p = p->Rlink)
		{
			Printf(p);
		}

	}
	//printf("--------用户空间表---------\n");
	/*for (int i = 0; i < usCount; i++)
	{
		Pword us =userSpace[i];
		if (us)
		{
			printf("空间首地址：%0x,空间大小：%d\n", us, us->len);
		}
	}*/
}