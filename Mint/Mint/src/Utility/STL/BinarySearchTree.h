#ifndef _MINT_BINARY_SEARCH_TREE_H_
#define _MINT_BINARY_SEARCH_TREE_H_


#include "Common/ObjectAllocator.h"
#include "Utility/Profiling/FunctionProfiler.h"


namespace mint
{

	template < typename Object >
	class CBinarySearchTree
	{
	public:
		CBinarySearchTree() = default;
		~CBinarySearchTree();

		bool initialize(u64 expected_object_count, u64 alignment = 0);

		Object* begin();

		Object* advance(Object* object);

		Object* add_node(u64 identifier);

		void remove_node(u64 identifier);

		Object* replace_node(u64 identifier);

		Object* get(u64 identifier);

		const Object* get_const(u64 identifier) const;

		bool lookup(u64 identifier) const;

		void reset();

		bool is_empty() const;

		u64 size() const;

	private:
		CObjectAllocator< Object > m_objects;

		std::unordered_map< u64, u64 > m_indices;

	private:
		CBinarySearchTree(const CBinarySearchTree& rh) = delete;
		CBinarySearchTree& operator=(const CBinarySearchTree& rh) = delete;

	};

	template < typename Object >
	const Object* mint::CBinarySearchTree<Object>::get_const(u64 identifier) const
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::get_const");

		MINT_ASSERT(lookup(identifier) == true, "Invalid operation. Accessing not existing node is not allowed!");

		const auto id = m_indices.at(identifier);

		return m_objects.get_const(id);
	}

	template < typename Object >
	Object* mint::CBinarySearchTree<Object>::replace_node(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::replace_node");

		MINT_ASSERT(lookup(identifier) == true, "Invalid operation. Replacing not existing node is not allowed!");

		Object* object = m_objects.get(m_indices[identifier]);

		object->~Object();

		object = new (object) Object;

		return object;
	}

	template < typename Object >
	Object* mint::CBinarySearchTree<Object>::add_node(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::add_node");

		MINT_ASSERT(lookup(identifier) == false, "Invalid operation. Adding nodes with duplicate identifiers is not allowed!");

		Object* object = nullptr;

		m_indices[identifier] = m_objects.size();
		object = m_objects.create();

		return object;
	}

	template < typename Object >
	Object* mint::CBinarySearchTree<Object>::advance(Object* object)
	{
		return m_objects.advance(object);
	}

	template < typename Object >
	Object* mint::CBinarySearchTree<Object>::begin()
	{
		return m_objects.begin();
	}

	template < typename Object >
	u64 mint::CBinarySearchTree<Object>::size() const
	{
		return m_objects.size();
	}

	template < typename Object >
	bool mint::CBinarySearchTree<Object>::is_empty() const
	{
		return m_objects.size() == 0;
	}

	template < typename Object >
	void mint::CBinarySearchTree<Object>::reset()
	{
		m_objects.reset();
	}

	template < typename Object >
	bool mint::CBinarySearchTree<Object>::initialize(u64 expected_object_count, u64 alignment /*= 0*/)
	{
		return m_objects.initialize(expected_object_count, alignment);
	}

	template < typename Object >
	mint::CBinarySearchTree<Object>::~CBinarySearchTree()
	{
	}

	template < typename Object >
	void mint::CBinarySearchTree<Object>::remove_node(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::remove_node");

		MINT_ASSERT(lookup(identifier) == true, "Invalid operation. Removing not existing node is not allowed!");

		m_objects.remove(m_indices[identifier]); // Object destructor called inside.
		m_indices.erase(m_indices.find(identifier));
	}

	template < typename Object >
	bool mint::CBinarySearchTree<Object>::lookup(u64 identifier) const
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::lookup");

		return m_indices.find(identifier) != m_indices.end();
	}

	template < typename Object >
	Object* mint::CBinarySearchTree<Object>::get(u64 identifier)
	{
		MINT_PROFILE_SCOPE("Engine::STL", "CBinarySearchTree::get");

		MINT_ASSERT(lookup(identifier) == true, "Invalid operation. Accessing not existing node is not allowed!");

		return m_objects.get(m_indices[identifier]);
	}

}


#endif