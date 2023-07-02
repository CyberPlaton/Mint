#ifndef _MINT_OBJECT_ALLOCATOR_H_
#define _MINT_OBJECT_ALLOCATOR_H_


#include "Common/Common.h"


namespace mint
{

	template< typename Object >
	class CObjectAllocator
	{
	public:
		CObjectAllocator();
		~CObjectAllocator();


		bool initialize(u64 expected_object_count, u64 alignment = 0);

		void terminate();

		void reset();

		Object* create();

		template< typename... ARGS >
		Object* create(const ARGS&... args)
		{
			if (!has_room_for_another_object()) return nullptr;

			u64 index = 0;
			for (auto i = 0; i < m_initializedBit.size(); i++)
			{
				if (!m_initializedBit[i]) { index = i; break; }
			}

			Object* object = get(index);

			object = new (object) Object(args...);

			m_initializedBit[index] = true;
			m_objectCount++;

			return object;
		}

		void remove(u64 index);

		Object* begin();

		Object* advance(Object* object);

		Object* reverse_begin();

		Object* reverse_advance(Object* object);


		Object* get(u64 index);

		Object* get_unsafe(u64 index);

		u64 size() const;

		bool has_room_for_another_object();

		bool can_remove_another_object();

		bool is_index_valid(u64 index);

		u64 get_index_for_memory_location(u64 memory_location);

		bool is_object_at_index_initialized(u64 index);

	private:
		Vector< bool > m_initializedBit;

		void* m_memoryBlockStart;

		void* m_memoryBlockEnd;

		u64 m_currentBlockSize;

		u64 m_objectCount;

		u64 m_maxObjectCount;

		u64 m_usedAlignment;
	};


	template< typename Object >
	Object* mint::CObjectAllocator<Object>::reverse_advance(Object* object)
	{
		auto start = reinterpret_cast<u64> (m_memoryBlockStart);

		if (reinterpret_cast<u64>(object) - sizeof(Object) < start)
		{
			return nullptr;
		}

		// Return the start of the memory location of the previous object relative to given one.
		return reinterpret_cast<Object*>(reinterpret_cast< u64 >(object) - sizeof(Object));
	}

	template< typename Object >
	Object* mint::CObjectAllocator<Object>::reverse_begin()
	{
		if (m_objectCount == 0)
		{
			return nullptr;
		}

		// Return the start of the memory location of the last object.
		return reinterpret_cast<Object*>(m_memoryBlockStart + m_objectCount * sizeof(Object) - 1);
	}


	template< typename Object >
	bool mint::CObjectAllocator<Object>::is_object_at_index_initialized(u64 index)
	{
		if (is_index_valid(index))
		{
			return m_initializedBit[index];
		}

		return false;
	}


	template< typename Object >
	u64 mint::CObjectAllocator<Object>::get_index_for_memory_location(u64 memory_location)
	{
		u64 object_size = sizeof(Object);

		return (memory_location - reinterpret_cast<u64>(m_memoryBlockStart)) / object_size;
	}

	template< typename Object >
	u64 mint::CObjectAllocator<Object>::size() const
	{
		return m_objectCount;
	}


	template< typename Object >
	bool mint::CObjectAllocator<Object>::can_remove_another_object()
	{
		return m_objectCount > 0;
	}


	template< typename Object >
	bool mint::CObjectAllocator<Object>::is_index_valid(u64 index)
	{
		return (index >= 0 &&
			index < m_maxObjectCount);
	}

	template< typename Object >
	bool mint::CObjectAllocator<Object>::has_room_for_another_object()
	{
		return m_objectCount < m_maxObjectCount;
	}


	template< typename Object >
	mint::CObjectAllocator<Object>::CObjectAllocator() :
		m_memoryBlockStart(nullptr), m_memoryBlockEnd(nullptr),
		m_currentBlockSize(0), m_objectCount(0), m_usedAlignment(0), m_maxObjectCount(0)
	{

	}


	template< typename Object >
	mint::CObjectAllocator<Object>::~CObjectAllocator()
	{
		terminate();
	}


	template< typename Object >
	void mint::CObjectAllocator<Object>::reset()
	{
		auto object = begin();
		while (object)
		{
			object->~Object();

			std::memset(object, NULL, sizeof(Object));

			object = advance(object);
		}



		m_initializedBit.insert(m_initializedBit.begin(), m_initializedBit.size(), false);
		m_objectCount = 0;
	}


	template< typename Object >
	Object* mint::CObjectAllocator<Object>::get(u64 index)
	{
		if (is_index_valid(index))
		{
			// Verify that the object at index is initialized and the index is accessing a valid object.
#if MINT_DISTR
#else
			if (!is_object_at_index_initialized(index)) return nullptr;
#endif

			// Compute the memory location of the Object.
			u64 object_size = sizeof(Object);

			auto location = reinterpret_cast<void*>(reinterpret_cast<u64>(m_memoryBlockStart) + object_size * index);

			// Assume location to be correct, as the index is validated.
			return reinterpret_cast<Object*>(location);

		}

		return nullptr;
	}



	template< typename Object >
	Object* mint::CObjectAllocator<Object>::get_unsafe(u64 index)
	{
		if (is_index_valid(index))
		{
			// No prevention of access to uninitialized objects, as "get_unsafe" intended to be used that way,
			// i.e. creating new objects.

			// Compute the memory location of the Object.
			u64 object_size = sizeof(Object);

			auto location = reinterpret_cast<void*>(reinterpret_cast<u64>(m_memoryBlockStart) + object_size * index);

			// Assume location to be correct, as the index is validated.
			return reinterpret_cast<Object*>(location);

		}

		return nullptr;
	}



	template< typename Object >
	Object* mint::CObjectAllocator<Object>::advance(Object* object)
	{
		u64 object_size = sizeof(Object);

		auto next_memory_slot = reinterpret_cast<void*>(reinterpret_cast<u64>(object) + object_size);
		auto next_location = reinterpret_cast<u64>(next_memory_slot);

		u64 memory_end = reinterpret_cast<u64>(m_memoryBlockEnd) - object_size;
		u64 initialized_section_end = reinterpret_cast<u64>(m_memoryBlockStart) + size() * object_size;


		// Check whether we reached the end and cannot advance anymore
		// past the allocated memory block and the current initialized section!
		if (next_location < memory_end &&
			next_location < initialized_section_end)
		{
			return reinterpret_cast<Object*>(next_memory_slot);
		}

		return nullptr;
	}


	template< typename Object >
	Object* mint::CObjectAllocator<Object>::begin()
	{
		if (m_objectCount == 0)
		{
			return nullptr;
		}

		return reinterpret_cast<Object*>(m_memoryBlockStart);
	}


	template< typename Object >
	void mint::CObjectAllocator<Object>::remove(u64 index)
	{
		if (!is_index_valid(index) || !can_remove_another_object()) return;

		// Get the object at index.
		Object* object = get(index);

		// Call the destructor of the Object.
		object->~Object();

		// Set the memory to NULL.
		std::memset(object, NULL, sizeof(Object));

		// Set the m_initializedBit at index to false and reduce object count.
		m_initializedBit[index] = false;
		m_objectCount--;
	}


	template< typename Object >
	Object* mint::CObjectAllocator<Object>::create()
	{
		if (!has_room_for_another_object()) return nullptr;


		// Get index of the next uninitialized object using m_initializedBit.
		u64 index = 0;
		for (auto i = 0; i < m_initializedBit.size(); i++)
		{
			if (!m_initializedBit[i]) { index = i; break; }
		}


		// Create object at the index with emplacement new.
		Object* object = get_unsafe(index);

		object = new (object) Object;


		// Set the m_initializedBit at index to true and increase the object count.
		m_initializedBit[index] = true;
		m_objectCount++;

		return object;
	}


	template< typename Object >
	void mint::CObjectAllocator<Object>::terminate()
	{
		auto object = begin();
		while (object)
		{
			object->~Object();

			std::memset(object, NULL, sizeof(Object));

			object = advance(object);
		}

		_aligned_free(m_memoryBlockStart);

		m_initializedBit.clear();

		m_memoryBlockStart = nullptr;
		m_memoryBlockEnd = nullptr;
		m_currentBlockSize = 0;
		m_usedAlignment = 0;
		m_objectCount = 0;
	}


	template< typename Object >
	bool mint::CObjectAllocator<Object>::initialize(u64 expected_object_count, u64 alignment /*= 0*/)
	{
		MINT_ASSERT(alignment <= sizeof(std::max_align_t), "Alignment must be smaller or equal to default max alignment!");
		MINT_ASSERT(expected_object_count > 0, "Must expect to allocate at least one object!");

		u64 algnmnt = alignment;
		if (algnmnt == 0)
		{
			algnmnt = alignof(Object);

			algnmnt = mint::algorithm::get_next_power_of_2(algnmnt);
		}


		m_memoryBlockStart = _aligned_malloc(sizeof(Object) * expected_object_count, algnmnt);

		u64 start, end, size;
		start = reinterpret_cast<u64>(m_memoryBlockStart);
		size = expected_object_count * SCAST(u64, sizeof(Object));
		end = start + size;

		m_memoryBlockEnd = reinterpret_cast<void*>(end);

		m_currentBlockSize = size;

		m_usedAlignment = algnmnt;

		m_objectCount = 0;


		m_maxObjectCount = m_currentBlockSize / sizeof(Object);

		MINT_ASSERT(expected_object_count == m_maxObjectCount, "Check valid computation!");


		m_initializedBit.resize(expected_object_count);

		for (auto& v : m_initializedBit) v = false;

		return true;
	}

}

#endif