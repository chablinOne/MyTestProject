// 智能指针.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<memory>
#include<unordered_map>
#include <xmemory>
#include<Windows.h>
using namespace std;

//多线程下
class Test
{
public:
	Test() { cout << "Test()" << endl; }
	~Test() { cout << "~Test()" << endl; }
	void test() { cout << "call Test::test" << endl; }
};

//线程参数的结构体类型
struct ThreadData
{
	ThreadData(shared_ptr<Test> sp) :wp(sp) {}
	weak_ptr<Test> wp;
};

//子线程
DWORD WINAPI threadProc(void *lparg)
{
	//::Sleep(2000);

	ThreadData *p = (ThreadData*)lparg;
	//p->test();

	//通过weak智能指针的提升，来检测共享对象的存活状态
	shared_ptr<Test> sp = p->wp.lock();
	if (sp != NULL)
	{
		cout << "提升成功了!" << endl;
		sp->test();
	}
	else
	{
		cout << "提升失败了" << endl;
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
	//创建一个线程  类似pthread_create
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
1.强智能指针可以改变资源的引用计数
2.弱智能指针
专门用来统计资源的引用计数  void*  int

*/
class ReferenceCount
{
public:
	//给ptr指向的资源增加引用计数
	void incRef(void *ptr)
	{
		refMap[ptr]++;
	}
	//给ptr指向的资源减少引用计数
	int decRef(void *ptr)
	{
		return refMap[ptr]-=1;
	}
private:
	//map的key表示资源地址， value表示资源的引用计数
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
	//专门供智能指针，查看当前它所引用的资源的引用计数
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
	//资源引用的地方都采用弱智能指针
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

