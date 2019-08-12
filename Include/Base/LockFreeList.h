#pragma  once
#include "Base/IntTypes.h"
#include "Base/Shared_ptr.h"
#include <atomic>

namespace Public{
namespace Base {


//�����б�
template<typename DATA>
class LockFreeList
{
public:
	LockFreeList() :m_pHead(NULL),m_pTail(NULL),m_Count(0)
	{
		m_pHead = new node();
	}
	~LockFreeList() 
	{
		clear();

		node* header = m_pHead.load();
		if (header) SAFE_DELETE(header);
		m_pHead = NULL;
		m_pTail = NULL;
		m_Count = 0;
	}

	void clear()
	{
		while (size() > 0) pop_front();
	}

	//���������Ѿ�����õĵ�
	void push_back(const DATA& data)
	{
		node* new_node = new node(data);
		node* next = NULL;
		node*  tail = m_pTail.load();

		/*
		*һ��ʼû������ʱ,m_pTailΪ��,��ʱֻ��Ҫ��m_pTailָָ�򱾽ڵ㼴��
		*����������ԭ�Ӳ������̻߳����m_pTail,����ͷָ���NEXT(����push��pop��ͬһ��Ԫ���ϵĻ���),
		*Ȼ�󷵻�,��������ԭ�Ӳ�����
		*�߳�ֻ�ᵥ���ĸ���tail���˳�if�ṹ.��������ִ��
		*/
		if (!m_pTail.load())
		{
			if (m_pTail.compare_exchange_weak(tail, new_node))
			{
				m_pHead.load()->next = new_node;
				m_Count.fetch_add(1);
				return;
			}
		}

		/*
		*����nextָ��,��tailָ���nextΪ��ʱ,���ʾ��ǰtailΪβԪ��
		*/
		do {
			tail = m_pTail.load();
			next = NULL;
		} while (!tail->next.compare_exchange_weak(next, new_node));

		/*
		* ��m_pTailָ��δ����ʱ,do while��ѭ���ǳ������е�.��Ϊtail->next != NULL
		1.�����һ���߳�T1������while�е�CAS����ɹ��Ļ�����ô�������е� ����̵߳�CAS����ʧ�ܣ�Ȼ��ͻ���ѭ����
		2.��ʱ�����T1 �̻߳�û�и���tailָ�룬�������̼߳���ʧ�ܣ���Ϊtail->next����NULL�ˡ�
		3.ֱ��T1�̸߳�����tailָ�룬�����������߳��е�ĳ���߳̾Ϳ��Եõ��µ�tailָ�룬�����������ˡ�
		*/
		m_pTail.compare_exchange_weak(tail, new_node);

		m_Count.fetch_add(1);
	}

	DATA& get(size_t offset)
	{
		node* dataptr = m_pHead.load();
		
		for(size_t i = 0;i < offset;i ++)
		{
			if (dataptr == NULL) return emtpyval;

			dataptr = dataptr->next.load();
		}

		if (dataptr == NULL) return emtpyval;

		node* nextptr = dataptr->next.load();
		if(nextptr == NULL) return emtpyval;

		return nextptr->data;
	}
	DATA& front()
	{
		return get(0);
	}

	void pop_front()
	{
		/*
		*�������Ϊ����ֱ�ӷ���
		*/
		if (!m_Count.load()) return;
		node* head = m_pHead.load();
		node* popData;

		do {
			/**ÿ��ѭ����Ҫ���»�ȡhead->next����ֵ*/
			popData = head->next;

			/*��popData == NULLʱ,
			*˵��Ԫ�ر������߳̽�Ԫ��ȡ����,
			*���߳̿���ֱ�ӷ���NULL,
			*��m_pTail����ȡ�����һ��Ԫ�ص��߳�����NULL*/
			if (!popData) return;

		} while (!head->next.compare_exchange_weak(popData, popData->next));

		/*����ѭ�����߳�,�����ŵ�����ʵ��Ԫ�ص��߳�,Ԫ�ؼ�һ*/
		m_Count.fetch_sub(1);

		/**�������һ��Ԫ�ص��߳����m_pTail��ΪNULL*/
		if (!head->next.load()) m_pTail.store(NULL);

		/**������ABA������.�������ڴ�ؿ��Ա���*/
		SAFE_DELETE(popData);
	}

	bool empty()
	{
		return m_Count.load() == 0;
	}

	size_t size()
	{
		return m_Count.load();
	}

private:
	DATA emtpyval;

	struct node
	{
		node() :next(NULL) {};
		node(const DATA& _data) :data(_data), next(NULL) {}

		DATA data;
		atomic<node*> next;
	};

	atomic<node*> m_pHead;
	atomic<node*> m_pTail;
	atomic<size_t> m_Count;
};


}
}

