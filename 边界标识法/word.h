#pragma once
#define INIT 0
#define BUSY 1
typedef struct word
{
	int tag;
	int len;
	union {
	word* Llink;//左指针
    word* Uplink;//头边界指针
	};
	word* Rlink;//右指针
}word,*Pword;

void Init_word(Pword head);//初始化内存

static Pword Get_tail(Pword head);

Pword Change_word(Pword *head, Pword p, int len);

Pword Get_word(Pword *head,int size);

void del_word(Pword *head, Pword p);//摧毁

void   PrintfSpace(Pword head);

