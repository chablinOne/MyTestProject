#include<stdio.h>
#include"word.h"
#include<malloc.h>
#include<assert.h>
#define MAX_SIZE  6553598

Pword Phead;//�ڴ��ϱ߽�
Pword Ptail;//�ڴ��±߽�
int main()
{
	Pword head = (Pword)malloc(1024 * 1024 * 100);
	assert(NULL != head);
	
	Phead = head;
	Ptail = head + 6553600 - 1;

	//��ʼ���ڴ��
	Init_word(head);
	PrintfSpace(head);
	Get_word(&head, 10);
	Pword p = Get_word(&head,100000);
	del_word(&head,p);
	PrintfSpace(head);
	
	free(head);
	head =NULL;
	return 0;
}