#ifndef _MINT_OBJECT_ALLOCATOR2_H_
#define _MINT_OBJECT_ALLOCATOR2_H_

#include "Common/Common.h"

namespace mint
{
	template< typename Object >
	class CObjectAllocator2
	{
	public:
		CObjectAllocator2();
		~CObjectAllocator2();


		bool initialize(u64 expected_object_count, u64 alignment = 0);

		void terminate();

		void reset();


		Object* create(u64* created_at_index);

		Object* replace(u64 index);

		void destroy(u64 index);

		void remove(u64 index);


		Object* begin();

		Object* advance(Object* object);


		Object* get(u64 index);

		const Object* get_const(u64 index) const;


		u64 size() const;

		u64 capacity() const;


	private:
		Vector< bool > m_initializedBit;

		std::stack< u64 > m_memoryFragmentationIndices;

		void* m_memoryBlockStart;

		void* m_memoryBlockEnd;

		u64 m_currentMemoryBlockTop;

		u64 m_currentBlockSize;

		u64 m_objectCount;

		u64 m_maxObjectCount;

		u64 m_usedAlignment;


	private:
		void resize(u64 expected_object_count);

		u64 get_next_power_of_2(u64 value);

		void destroy_or_remove_object(u64 index, bool destroy = false);

		bool has_room_for_another_object() const;

		bool can_remove_another_object() const;

		u64 get_index_for_memory_location(u64 memory_location) const;

		bool is_object_at_index_initialized(u64 index) const;

		bool is_object_initialized(const Object* object) const;

		bool is_object_at_location_initialized(u64 memory_location) const;
	};


	template< typename Object >
	u64 mint::CObjectAllocator2<Object>::capacity() const
	{
		return m_maxObjectCount;
	}

	template< typename Object >
	Object* mint::CObjectAllocator2<Object>::replace(u64 index)
	{
		// Get the object at index.
		Object* object = get(index);

		// Call the destructor of the Object.
		object->~Object();

		// Set the memory to NULL.
		std::memset(object, NULL, sizeof(Object));

		// Create a new object in his stead and return it.
		object = new (object) Object;

		return object;
	}

	template< typename Object >
	void mint::CObjectAllocator2<Object>::resize(u64 expected_object_count)
	{
		// Store previous data required for moving.
		auto prev_start = m_memoryBlockStart;
		auto prev_object_count = m_objectCount;

		// Reallocate memory block and initialize data as required.
		m_memoryBlockStart = _aligned_malloc(sizeof(Object) * expected_object_count, m_usedAlignment);

		u64 start = 0, end = 0, size = 0;
		start = reinterpret_cast<u64>(m_memoryBlockStart);
		size = expected_object_count * static_cast<u64>(sizeof(Object));
		end = start + size;

		m_memoryBlockEnd = reinterpret_cast<void*>(end);
		m_currentBlockSize = size;
		m_maxObjectCount = m_currentBlockSize / sizeof(Object);

		m_initializedBit.resize(expected_object_count, false);


		// Move memory to new location.
		std::memmove(m_memoryBlockStart, prev_start, prev_object_count * sizeof(Object));

		// Free old and unrequired memory.
		_aligned_free(prev_start);
	}

	template< typename Object >
	void mint::CObjectAllocator2<Object>::destroy_or_remove_object(u64 index, bool destroy /*= false*/)
	{
		if (!can_remove_another_object()) return;

		// Check whether we made memory fragmented.
		const auto fragmented = index < m_objectCount - 1;

		// Get the object at index.
		Object* object = get(index);

		// Call the destructor of the Object.
		if(destroy) object->~Object();

		// Set the memory to NULL.
		std::memset(object, NULL, sizeof(Object));

		// Set the m_initializedBit at index to false and reduce object count.
		m_initializedBit[index] = false;
		m_objectCount--;

		if (fragmented) m_memoryFragmentationIndices.push(index);
		else m_currentMemoryBlockTop--;
	}

	template< typename Object >
	bool mint::CObjectAllocator2<Object>::is_object_at_location_initialized(u64 memory_location) const
	{
		return is_object_at_index_initialized(get_index_for_memory_location(memory_location));
	}

	template< typename Object >
	bool mint::CObjectAllocator2<Object>::is_object_initialized(const Object* object) const
	{
		u64 index = get_index_for_memory_location(reinterpret_cast< u64 >(object));

		return is_object_at_index_initialized(index);
	}

	template< typename Object >
	u64 mint::CObjectAllocator2<Object>::get_next_power_of_2(u64 value)
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

	template< typename Object >
	bool mint::CObjectAllocator2<Object>::is_object_at_index_initialized(u64 index) const
	{
		return  index < m_maxObjectCount && m_initializedBit[index];
	}


	template< typename Object >
	u64 mint::CObjectAllocator2<Object>::get_index_for_memory_location(u64 memory_location) const
	{
		u64 object_size = sizeof(Object);

		return (memory_location - reinterpret_cast<u64>(m_memoryBlockStart)) / object_size;
	}

	template< typename Object >
	u64 mint::CObjectAllocator2<Object>::size() const
	{
		return m_objectCount;
	}


	template< typename Object >
	bool mint::CObjectAllocator2<Object>::can_remove_another_object() const
	{
		return m_objectCount > 0;
	}

	template< typename Object >
	bool mint::CObjectAllocator2<Object>::has_room_for_another_object() const
	{
		return m_objectCount < m_maxObjectCount;
	}


	template< typename Object >
	mint::CObjectAllocator2<Object>::CObjectAllocator2() :
		m_memoryBlockStart(nullptr), m_memoryBlockEnd(nullptr),
		m_currentBlockSize(0), m_objectCount(0), m_usedAlignment(0), m_maxObjectCount(0), m_currentMemoryBlockTop(0)
	{

	}


	template< typename Object >
	mint::CObjectAllocator2<Object>::~CObjectAllocator2()
	{
		terminate();
	}


	template< typename Object >
	void mint::CObjectAllocator2<Object>::reset()
	{
		auto count = 0;
		auto object = begin();
		while (count < m_objectCount)
		{
			object->~Object();

			object = advance(object);

			count++;
		}

		std::memset(m_memoryBlockStart, NULL, m_currentBlockSize);

		while (!m_memoryFragmentationIndices.empty()) m_memoryFragmentationIndices.pop();
		
		std::fill(m_initializedBit.begin(), m_initializedBit.end(), false);

		m_objectCount = 0;
		m_currentMemoryBlockTop = 0;
	}


	template< typename Object >
	Object* mint::CObjectAllocator2<Object>::get(u64 index)
	{
		// Compute the memory location of the Object.
		u64 object_size = sizeof(Object);

		auto location = reinterpret_cast<void*>(reinterpret_cast<u64>(m_memoryBlockStart) + object_size * index);

		// Assume location to be correct, as the index is validated.
		return reinterpret_cast<Object*>(location);
	}

	template< typename Object >
	const Object* mint::CObjectAllocator2<Object>::get_const(u64 index) const
	{
		// Compute the memory location of the Object.
		u64 object_size = sizeof(Object);

		auto location = reinterpret_cast<void*>(reinterpret_cast<u64>(m_memoryBlockStart) + object_size * index);

		// Assume location to be correct, as the index is validated.
		return reinterpret_cast<Object*>(location);
	}


	template< typename Object >
	Object* mint::CObjectAllocator2<Object>::advance(Object* object)
	{
		u64 object_size = sizeof(Object);

		auto next_memory_slot = reinterpret_cast<void*>(reinterpret_cast<u64>(object) + object_size);
		auto next_location = reinterpret_cast<u64>(next_memory_slot);
		auto memory_end = reinterpret_cast<u64>(m_memoryBlockEnd);
		
		while (next_location <= memory_end && is_object_at_location_initialized(next_location) == false)
		{
			next_location += object_size;
		}

		// Check what we got, either reached end of memory block, then nothing left to iterate,
		// or we found an initialized object.
		if (next_location >= memory_end)
		{
			return nullptr;
		}
		
		return reinterpret_cast<Object*>(reinterpret_cast<void*>(next_location));
	}


	template< typename Object >
	Object* mint::CObjectAllocator2<Object>::begin()
	{
		if (m_objectCount == 0)
		{
			return nullptr;
		}
		else if (is_object_at_index_initialized(0) == false)
		{
			return advance(reinterpret_cast<Object*>(m_memoryBlockStart));
		}

		return reinterpret_cast<Object*>(m_memoryBlockStart);
	}


	template< typename Object >
	void mint::CObjectAllocator2<Object>::remove(u64 index)
	{
		destroy_or_remove_object(index);
	}

	template< typename Object >
	void mint::CObjectAllocator2<Object>::destroy(u64 index)
	{
		destroy_or_remove_object(index, true);
	}

	template< typename Object >
	Object* mint::CObjectAllocator2<Object>::create(u64* created_at_index)
	{
		if (!has_room_for_another_object()) resize(m_maxObjectCount * 2);

		u64 index = -1;
		// First check whether we have fragmented memory and should fill it.
		if (!m_memoryFragmentationIndices.empty())
		{
			index = m_memoryFragmentationIndices.top();
			
			m_memoryFragmentationIndices.pop();
		}
		// Get index of the next uninitialized object.
		else
		{
			index = m_currentMemoryBlockTop++;
		}

		// Create object at the index with emplacement new.
		Object* object = get(index);

		object = new (object) Object;


		// Set the m_initializedBit at index to true and increase the object count.
		m_initializedBit[index] = true;
		m_objectCount++;

		// Report to user at which index we create the object.
		*created_at_index = index;

		return object;
	}

	template< typename Object >
	void mint::CObjectAllocator2<Object>::terminate()
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
		m_currentMemoryBlockTop = 0;
	}


	template< typename Object >
	bool mint::CObjectAllocator2<Object>::initialize(u64 expected_object_count, u64 alignment /*= 0*/)
	{
		u64 algnmnt = alignment;
		if (algnmnt == 0)
		{
			algnmnt = alignof(Object);

			algnmnt = get_next_power_of_2(algnmnt);
		}


		m_memoryBlockStart = _aligned_malloc(sizeof(Object) * expected_object_count, algnmnt);

		u64 start = 0, end = 0, size = 0;
		start = reinterpret_cast<u64>(m_memoryBlockStart);
		size = expected_object_count * static_cast<u64>(sizeof(Object));
		end = start + size;

		m_memoryBlockEnd = reinterpret_cast<void*>(end);
		m_currentBlockSize = size;
		m_usedAlignment = algnmnt;
		m_maxObjectCount = m_currentBlockSize / sizeof(Object);

		m_initializedBit.resize(expected_object_count, false);

		return true;
	}

}

#endif