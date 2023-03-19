#ifndef _MAML_NODE_ALLOCATOR_H_
#define _MAML_NODE_ALLOCATOR_H_


#include "Common/Common.h"


namespace maml
{

	template< class Object >
	class CNodeAllocator
	{
	public:
		CNodeAllocator();


		bool initialize(mint::u64 expected_object_count, mint::u64 alignment = 0);

		void terminate();

		Object* create();

		bool pop();

		Object* begin();

		Object* advance(Object* node);


	private:
		void* m_memoryBlockStart;

		void* m_memoryBlockEnd;

		void* m_stackTop;

		mint::u64 m_currentBlockSize;

		mint::u64 m_objectCount;

		mint::u64 m_usedAlignment;


	private:
		mint::u64 _get_next_power_2(mint::u64 value);


	};


	template< class Object >
	mint::u64 maml::CNodeAllocator<Object>::_get_next_power_2(mint::u64 value)
	{
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value++;

		return value;
	}


	template< class Object >
	Object* maml::CNodeAllocator<Object>::advance(Object* node)
	{
		mint::u64 osize = sizeof(Object);
		auto pobject = reinterpret_cast<void*>(node);

		auto pnext_location = reinterpret_cast<void*>(reinterpret_cast< mint::u64 >(pobject) + osize);

		if(m_memoryBlockEnd < pnext_location ||
			m_stackTop < pnext_location)
		{
			return nullptr;
		}

		return reinterpret_cast<Object*>(reinterpret_cast<void*>(pnext_location));
	}


	template< class Object >
	Object* maml::CNodeAllocator<Object>::begin()
	{
		if (m_memoryBlockStart == m_stackTop)
		{
			return nullptr;
		}

		return reinterpret_cast<Object*>(m_memoryBlockStart);
	}


	template< class Object >
	bool maml::CNodeAllocator<Object>::pop()
	{
		if(m_memoryBlockStart == m_stackTop)
		{
			return false;
		}

		mint::u64 new_top = reinterpret_cast<mint::u64>(m_stackTop) - sizeof(Object);

		reinterpret_cast<Object*>(m_stackTop)->~Object();

		std::memset(m_stackTop, NULL, sizeof(Object));

		m_stackTop = reinterpret_cast<void*>(new_top);

		m_objectCount--;

		return true;
	}


	template< class Object >
	Object* maml::CNodeAllocator<Object>::create()
	{
		mint::u64 new_top = reinterpret_cast<mint::u64>(m_stackTop) + sizeof(Object);

		if(new_top > reinterpret_cast< mint::u64>(m_memoryBlockEnd))
		{
			return nullptr;
		}

		auto allocation = new (m_stackTop) Object;

		m_stackTop = reinterpret_cast<void*>(new_top);

		m_objectCount++;

		return allocation;
	}


	template< class Object >
	void maml::CNodeAllocator<Object>::terminate()
	{
		auto object = begin();
		while(object)
		{
			object->~Object();

			std::memset(object, NULL, sizeof(Object));

			object = advance(object);
		}

		_aligned_free(m_memoryBlockStart);

		m_memoryBlockStart = nullptr;
		m_memoryBlockEnd = nullptr;
		m_stackTop = nullptr;
		m_currentBlockSize = 0;
		m_usedAlignment = 0;
		m_objectCount = 0;
	}


	template< class Object >
	bool maml::CNodeAllocator<Object>::initialize(mint::u64 expected_object_count, mint::u64 alignment /*= 0*/)
	{
		MINT_ASSERT(alignment <= sizeof(std::max_align_t), "Alignment must be smaller or equal to default max alignment!");
		MINT_ASSERT(expected_object_count > 0, "Must expect to allocate at least one object!");

		mint::u64 algnmnt = alignment;
		if(algnmnt == 0)
		{
			algnmnt = alignof(Object);

			algnmnt = _get_next_power_2(algnmnt);
		}

		m_memoryBlockStart = _aligned_malloc(sizeof(Object) * expected_object_count, algnmnt);

		m_stackTop = m_memoryBlockStart;

		mint::u64 start, end, size;
		start = reinterpret_cast<mint::u64>(m_memoryBlockStart);
		size = expected_object_count * SCAST(mint::u64, sizeof(Object));
		end = start + size;

		m_memoryBlockEnd = reinterpret_cast<void*>(end);

		m_currentBlockSize = size;

		m_usedAlignment = algnmnt;

		return true;
	}


	template< class Object >
	maml::CNodeAllocator<Object>::CNodeAllocator() : 
		m_memoryBlockEnd(nullptr), m_stackTop(nullptr), m_currentBlockSize(0), m_usedAlignment(0), m_objectCount(0)
	{

	}


}


#endif