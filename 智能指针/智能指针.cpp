// ����ָ��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include<memory>
#include<unordered_map>
#include <xmemory>
#include<Windows.h>
using namespace std;

//���߳���
class Test
{
public:
	Test() { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
	void test() { cout << "call Test::test" << endl; }
};

//�̲߳����Ľṹ������
struct ThreadData
{
	ThreadData(shared_ptr<Test> sp) :wp(sp) {}
	weak_ptr<Test> wp;
};

//���߳�
DWORD WINAPI threadProc(void *lparg)
{
	//::Sleep(2000);

	ThreadData *p = (ThreadData*)lparg;
	//p->test();

	//ͨ��weak����ָ�������������⹲�����Ĵ��״̬
	shared_ptr<Test> sp = p->wp.lock();
	if (sp != NULL)
	{
		cout << "�����ɹ���!" << endl;
		sp->test();
	}
	else
	{
		cout << "����ʧ����" << endl;
	}

	delete p;

	return 0;
}

void func()
{
	//Test *p = new Test();
	shared_ptr<Test> sp(new Test());
	cout << sp.use_count() << endl;

	ThreadData *data = new ThreadData(sp);
	//����һ���߳�  ����pthread_create
	::CreateThread(NULL, 0, threadProc, data, 0, NULL);

	::Sleep(2000);

	//delete p;
	//::Sleep(5000);
}
int main()
{
	func();

	::Sleep(5000);

	return 0;
}

/*
1.ǿ����ָ����Ըı���Դ�����ü���
2.������ָ��
ר������ͳ����Դ�����ü���  void*  int

*/
class ReferenceCount
{
public:
	//��ptrָ�����Դ�������ü���
	void incRef(void *ptr)
	{
		refMap[ptr]++;
	}
	//��ptrָ�����Դ�������ü���
	int decRef(void *ptr)
	{
		return refMap[ptr]-=1;
	}
private:
	//map��key��ʾ��Դ��ַ�� value��ʾ��Դ�����ü���
	unordered_map<void*, int> refMap;
};

template<typename T>
class CSmartPtr
{
public:
	CSmartPtr(T *ptr = NULL)
		:mptr(ptr)
	{
		if (mptr != NULL)
		{
			incRef();
		}
	}
	CSmartPtr(const CSmartPtr<T> &src)
		:mptr(src.mptr)
	{
		if (mptr != NULL)
		{
			incRef();
		}
	}
	CSmartPtr<T>& operator=(const CSmartPtr<T> &src)
	{
		if (this == &src)
			return *this;

		if (decRef() == 0)
		{
			delete mptr;
			mptr = NULL;
		}

		mptr = src.mptr;
		if (mptr != NULL)
		{
			incRef();
		}
		return *this;
	}
	~CSmartPtr()
	{
		if (decRef() == 0)
		{
			delete mptr;
			mptr = NULL;
		}
	}

	T& operator*()
	{
		return *mptr;
	}
	const T& operator*()const
	{
		return *mptr;
	}
	T* operator->()
	{
		return mptr;
	}
	T* operator->()const
	{
		return mptr;
	}

private:
	T *mptr;
	//ר�Ź�����ָ�룬�鿴��ǰ�������õ���Դ�����ü���
	static ReferenceCount refCnt;

	void incRef() { refCnt.incRef(mptr); }
	int decRef() { return refCnt.decRef(mptr); }
};
template<typename T>
ReferenceCount CSmartPtr<T>::refCnt;
class B;
class A
{
public:
	
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
	void func() { cout << "call A::func" << endl; }
	//��Դ���õĵط�������������ָ��
	weak_ptr<B> _ptrb;
};
class B
{
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
	weak_ptr<A> _ptra;
};
int main6()
{
	shared_ptr<A>ptra(new A());
	shared_ptr<B>ptrb(new B());

	ptra->_ptrb = ptrb;
	ptrb->_ptra = ptra;
	return 0;
}
#if 0
int main(int argc, char* argv[])
{
	CSmartPtr<int> ptr1(new int);
	*ptr1 = 30;
	cout << *ptr1 << endl;

	CSmartPtr<int> ptr2(ptr1);
	return 0;
}

int main()
{
	CSmartPtr<int> ptr1(new int);
	*ptr1 = 30;
	cout << *ptr1 << endl;
	const CSmartPtr<int> ptr2(new int(40));
	cout << *ptr2 << endl;
	CSmartPtr<A> ptr3(new A());
	ptr3->func();
	const CSmartPtr<A> ptr4(new A());
	ptr4->func();

    return 0;
}
#endif

