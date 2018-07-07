#pragma once
#define INIT 0
#define BUSY 1
typedef struct word
{
	int tag;
	int len;
	union {
	word* Llink;//��ָ��
    word* Uplink;//ͷ�߽�ָ��
	};
	word* Rlink;//��ָ��
}word,*Pword;

void Init_word(Pword head);//��ʼ���ڴ�

static Pword Get_tail(Pword head);

Pword Change_word(Pword *head, Pword p, int len);

Pword Get_word(Pword *head,int size);

void del_word(Pword *head, Pword p);//�ݻ�

void   PrintfSpace(Pword head);

