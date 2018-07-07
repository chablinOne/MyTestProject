#include"word.h"
#include<stdio.h>
#include<assert.h>
#define MAX_SIZE  6553598
extern Pword Phead;//�ڴ��ϱ߽�
extern Pword Ptail;//�ڴ��±߽�
//Pword userSpace[MAX_SIZE] = { NULL };//�û��ռ�����
//int usCount = 0;
static Pword Get_tail(Pword head)
{
	return head + head->len +1 ;
}

void Init_word(Pword head)//��ʼ���ڴ�
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
	{	//�ӿ���������ɾ�����ڴ��
		p->Llink->Rlink = p->Rlink;
		p->Rlink->Llink = p->Llink;

		p->tag = 1;
		Get_tail(p)->tag = 1;
		//�ж��Ƿ����һ���ڴ��
		if (*head == p)
		{
			*head = NULL;
		}
		//userSpace[usCount++] = p + 1;
		return p + 1;
	}
	else
	{
		//��ȡ���в��ֵ�ָ��
		Pword q = p + p->len - len;

		q->len = len;
		q->tag = 1;
		Get_tail(q)->Uplink = q;
		Get_tail(q)->tag = 1;

		//�޸�ʣ�ಿ�ֵ��ڴ�����Ϣ
		p->len = p->len - len - 2;
		Get_tail(p)->Uplink = p;
		//�����û��ɲ������ڴ��ַ
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
	int len = (size + 15) / 16;//��������Ҫ�ĸ�����
	
	Pword p=*head;
	for (;; p = p->Llink)//����һ�����ø�����
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
void del_word(Pword *head,Pword p)//�ڴ��ϲ�
{
	Pword ptr = p - 1;

	//�Ƿ�������Ϻϲ�
	if (ptr != Phead && INIT== (ptr - 1)->tag)
	{
		Pword foot= (ptr-1)->Llink;
		foot->len += ptr->len;
		foot->Uplink = ptr;
		foot->tag = 0;
	}
	else//���������Ϻϲ�  ���ϵ�����
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
	printf("�ռ��׵�ַ��%0x���ռ��С��%d��ǰ����ַ��%0x����̵�ַ��%0x\n", pre, pre->len, pre->Llink, pre->Rlink);
}
void   PrintfSpace(Pword head)
{
	if (head != NULL)
	{
		printf("--------�����ÿռ��---------\n");
		Pword p = head;
		Printf(p);
		for (p = p->Rlink; p != head; p = p->Rlink)
		{
			Printf(p);
		}

	}
	//printf("--------�û��ռ��---------\n");
	/*for (int i = 0; i < usCount; i++)
	{
		Pword us =userSpace[i];
		if (us)
		{
			printf("�ռ��׵�ַ��%0x,�ռ��С��%d\n", us, us->len);
		}
	}*/
}