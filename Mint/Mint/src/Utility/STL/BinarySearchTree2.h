#ifndef _MINT_BINARY_SEARCH_TREE2_H_
#define _MINT_BINARY_SEARCH_TREE2_H_

#include "Common/ObjectAllocator2.h"
#include "Utility/Profiling/FunctionProfiler.h"

namespace mint
{

	/// @brief Generic, contiguous in memory and dynamically resizable datastructure.
	/// Properties: 
	/// add_node: amortized constant time.
	/// begin/advance: best case constant time, worst case linear time to the size of the memory block.
	/// destroy_node/remove_node: O(2 * logn) because of the std::unordered_map id lookup, thus logarithmic time.
	/// get/get_const/lookup: logarithmic time due to id lookup in std::unordered_map.
	/// reset: linear time to the capacity of objects, we potentially call the destructor of all objects.
	/// is_empty/size/capacity: constant time.
	/// @tparam Object 
	template < typename Object >
	class CBinarySearchTree2
	{
	public:
		CBinarySearchTree2() = default;
		~CBinarySearchTree2();

		/// @brief Allocate memory block required for storage.
		bool initialize(u64 expected_object_count, u64 alignment = 0);

		/// @brief Return the first initialized object in the datastructure. Guaranteed to be valid if the datastructure is not empty.
		Object* begin();

		/// @brief Return the next initialized object in the datastructure. Guaranteed to be valid if there are any initialized objects after the given one.
		Object* advance(Object* object);

		/// @brief Add a new node with given unique identifier. 
		Object* add_node(u64 identifier);

		/// @brief  
		Object* replace(u64 identifier, u64 index);

		/// @brief Remove node from datastructure. Does not call the destructor of Object.
		void remove_node(u64 identifier);

		/// @brief Remove node from datastructure. Does call the destructor of Object.
		void destroy_node(u64 identifier);

		/// @brief Get node with given unique identifier. 
		Object* get(u64 identifier);

		/// @brief Get node with given unique identifier, const version.
		const Object* get_const(u64 identifier) const;

		/// @brief Check whether node with given identifier exists. 
		bool lookup(u64 identifier) const;

		/// @brief Get the index of the node in the memory array. 
		u64 lookup_index(u64 identifier) const;

		/// @brief Reset the datastructure. Reusable after call.
		void reset();

		/// @brief  
		bool is_empty() const;

		/// @brief How many objects are currently stored. 
		u64 size() const;

		/// @brief How many objects can maximally be stored. 
		u64 capacity() const;


	private:
		CObjectAllocator2< Object > m_objects;
		
		std::unordered_map< u64, u64 > m_indices;

		
	private:
		CBinarySearchTree2(const CBinarySearchTree2& rh) = delete;
		CBinarySearchTree2& operator=(const CBinarySearchTree2& rh) = delete;

		void remove_or_destroy_node(u64 identifier, bool destroy);

	};

	template < typename Object >
	u64 mint::CBinarySearchTree2<Object>::lookup_index(u64 identifier) const
	{
		auto it = m_indices.find(identifier);

		if (it != m_indices.end())
		{
			return it->second;
		}

		return MINT_INVALID_HANDLE;
	}

	template < typename Object >
	Object* mint::CBinarySearchTree2<Object>::replace(u64 identifier, u64 index)
	{
		Object* object = nullptr;

		auto it = m_indices.find(identifier);

		if (it != m_indices.end())
		{
			object = m_objects.replace(index);

			if (object)
			{
				m_indices[identifier] = index;
			}
		}

		return nullptr;
	}

	template < typename Object >
	u64 mint::CBinarySearchTree2<Object>::capacity() const
	{
		return m_objects.capacity();
	}

	template < typename Object >
	void mint::CBinarySearchTree2<Object>::remove_or_destroy_node(u64 identifier, bool destroy)
	{
		auto it = m_indices.find(identifier);

		if (it != m_indices.end())
		{
			if(destroy) m_objects.destroy(it->second);
			else m_objects.remove(it->second);
			m_indices.erase(it);
		}
	}

	template < typename Object >
	void mint::CBinarySearchTree2<Object>::destroy_node(u64 identifier)
	{
		remove_or_destroy_node(identifier, true);
	}
	
	template < typename Object >
	const Object* mint::CBinarySearchTree2<Object>::get_const(u64 identifier) const
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree2::get_const");

		auto it = m_indices.find(identifier);

		if (it != m_indices.end())
		{
			return m_objects.get_const(it->second);
		}

		return nullptr;
	}


	template < typename Object >
	Object* mint::CBinarySearchTree2<Object>::add_node(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree2::add_node");

		Object* object = nullptr;
		u64 index = 0;

		object = m_objects.create(&index);

		if (object)
		{
			m_indices[identifier] = index;
		}

		return object;
	}

	template < typename Object >
	Object* mint::CBinarySearchTree2<Object>::advance(Object* object)
	{
		return m_objects.advance(object);
	}

	template < typename Object >
	Object* mint::CBinarySearchTree2<Object>::begin()
	{
		return m_objects.begin();
	}

	template < typename Object >
	u64 mint::CBinarySearchTree2<Object>::size() const
	{
		return m_objects.size();
	}

	template < typename Object >
	bool mint::CBinarySearchTree2<Object>::is_empty() const
	{
		return m_objects.size() == 0;
	}

	template < typename Object >
	void mint::CBinarySearchTree2<Object>::reset()
	{
		m_objects.reset();
		m_indices.clear();
	}

	template < typename Object >
	bool mint::CBinarySearchTree2<Object>::initialize(u64 expected_object_count, u64 alignment /*= 0*/)
	{
		return m_objects.initialize(expected_object_count, alignment);
	}

	template < typename Object >
	mint::CBinarySearchTree2<Object>::~CBinarySearchTree2()
	{
		reset();
	}

	template < typename Object >
	void mint::CBinarySearchTree2<Object>::remove_node(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree2::remove_node");

		remove_or_destroy_node(identifier, false);
	}

	template < typename Object >
	bool mint::CBinarySearchTree2<Object>::lookup(u64 identifier) const
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree2::lookup");

		return m_indices.find(identifier) != m_indices.end();
	}

	template < typename Object >
	Object* mint::CBinarySearchTree2<Object>::get(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree2::get");

		auto it = m_indices.find(identifier);

		if (it != m_indices.end())
		{
			return m_objects.get(it->second);
		}

		return nullptr;
	}

}


#endif