#ifndef _MINT_MAP_2_H_
#define _MINT_MAP_2_H_


#include "Common/ObjectAllocator.h"


namespace mint
{

	template< typename Object >
	class CMap2
	{
	public:
		CMap2();
		~CMap2();

		bool initialize(u64 expected_object_count, u64 alignment = 0);


		Object* begin();

		Object* advance(Object* object);


		template< typename... ARGS >
		Object* emplace(u64 identifier, const ARGS&... args)
		{
			u64 index = m_data.size();
			Object* result = m_data.create(args...);

			if (result != nullptr)
			{
				m_indices.emplace_back(std::make_pair(identifier, index));
			}

			return result;
		}

		template< typename... ARGS >
		Object* replace(u64 identifier, u64 index, const ARGS&... args)
		{
			if (m_data.is_index_valid(index) && index < m_data.size())
			{
				// Destroy old object and create new one in his stead.
				Object* object = m_data.get(index);

				object->~Object();

				object = new (object) Object(args...);

				// Replace the old identifier with the new one.
				for (auto i = 0; i < m_indices.size(); i++)
				{
					if (m_indices[i].second == index)
					{
						m_indices[i].first = identifier; break;
					}
				}


				return object;
			}

			return nullptr;
		}


		Object* replace(u64 identifier, u64 index);

		Object* emplace(u64 identifier);

		Object* get(u64 identifier);

		bool lookup(u64 identifier) const;

		u64 lookup_index(u64 identifier) const;

		void remove(u64 identifier);

		void reset();

		bool is_empty() const;

		u64 size() const;


	private:
		Vector< std::pair< u64, u64 > > m_indices;

		CObjectAllocator< Object > m_data;



	private:
		CMap2(const CMap2& rh) = delete;
		CMap2& operator=(const CMap2& rh) = delete;
	};


	template< typename Object >
	mint::CMap2<Object>::CMap2()
	{

	}


	template< typename Object >
	mint::CMap2<Object>::~CMap2()
	{
		m_data.terminate();
	}


	template< typename Object >
	bool mint::CMap2<Object>::initialize(u64 expected_object_count, u64 alignment /*= 0*/)
	{
		return m_data.initialize(expected_object_count, alignment);
	}


	template< typename Object >
	Object* mint::CMap2<Object>::begin()
	{
		return m_data.begin();
	}


	template< typename Object >
	Object* mint::CMap2<Object>::advance(Object* object)
	{
		return m_data.advance(object);
	}


	template< typename Object >
	Object* mint::CMap2<Object>::get(u64 identifier)
	{
		for (const auto& id : m_indices)
		{
			if (id.first == identifier) return m_data.get(id.second);
		}

		return nullptr;
	}



	template< typename Object >
	bool mint::CMap2<Object>::lookup(u64 identifier) const
	{
		for (const auto& id : m_indices)
		{
			if (id.first == identifier) return true;
		}

		return false;
	}

	template< typename Object >
	u64 mint::CMap2<Object>::lookup_index(u64 identifier) const
	{
		for (const auto& id : m_indices)
		{
			if (id.first == identifier) return id.second;
		}

		return (-1);
	}

	template< typename Object >
	void mint::CMap2<Object>::remove(u64 identifier)
	{
		for (const auto& id : m_indices)
		{
			if (id.first == identifier) return m_data.remove(id.second);
		}
	}


	template< typename Object >
	void mint::CMap2<Object>::reset()
	{
		m_data.reset();
		m_indices.clear();
	}


	template< typename Object >
	bool mint::CMap2<Object>::is_empty() const
	{
		return m_data.size() == 0;
	}


	template< typename Object >
	u64 mint::CMap2<Object>::size() const
	{
		return m_data.size();
	}


	template< typename Object >
	Object* mint::CMap2<Object>::replace(u64 identifier, u64 index)
	{
		if (m_data.is_index_valid(index) && index < m_data.size())
		{
			// Destroy old object and create new one in his stead.
			Object* object = m_data.get(index);

			object->~Object();

			object = new (object) Object;


			// Replace the old identifier with the new one.
			for (auto i = 0; i < m_indices.size(); i++)
			{
				if (m_indices[i].second == index)
				{
					m_indices[i].first = identifier; break;
				}
			}


			return object;
		}

		return nullptr;
	}


	template< typename Object >
	Object* mint::CMap2<Object>::emplace(u64 identifier)
	{
		u64 index = m_data.size();
		Object* result = m_data.create();

		if (result != nullptr)
		{
			m_indices.emplace_back(std::make_pair(identifier, index));
		}

		return result;
	}

}


#endif