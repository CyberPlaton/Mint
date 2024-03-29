#ifndef _MINT_MAP_H_
#define _MINT_MAP_H_


#include "Common/Common.h"


namespace mint
{


	template < typename T >
	class CMap
	{
	public:
		CMap() = default;
		CMap(const CMap& other) = default;
		CMap& operator=(const CMap& other) = default;
		~CMap();

		
		void add(u64 identifier, T& data);

		void add(u64 identifier, T&& data);

		template < typename... ARGS >
		T& emplace_back(u64 identifier, const ARGS&... args)
		{
			MINT_ASSERT(lookup(identifier) == false, "Invalid operation. Overriding indices does not work for emplace_back!");

			u64 index = m_data.size();

			m_indices.emplace_back(std::make_pair(identifier, index));

			return m_data.emplace_back(args...);
		}

		T* get_ptr(u64 identifier);

		T get(u64 identifier);

		T& get_ref(u64 identifier);

		const T& get_const(u64 identifier) const;

		T get_by_index(u64 index);

		bool lookup(u64 identifier) const;

		u64 lookup_index(u64 identifier) const;

		Vector< T >& get_all();

		const Vector< T >& get_all_const() const;

		void remove(u64 identifier);

		void reset();

		bool is_empty();

		u64 size();

		Vector < std::pair< u64, u64 > > get_indices();


	private:
		Vector< std::pair< u64, u64 > > m_indices;

		Vector< T > m_data;


	private:
		void _override_data(u64 identifier, T& data);

		void _restore_integrity_on_remove(u64 removed_at_index);

	};


	template < typename T >
	T* mint::CMap<T>::get_ptr(u64 identifier)
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return &m_data[m_indices[i].second];
		}
	}


	template < typename T >
	T& mint::CMap<T>::get_ref(u64 identifier)
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return m_data[m_indices[i].second];
		}
	}


	template < typename T >
	const mint::Vector< T >& mint::CMap<T>::get_all_const() const
	{
		return m_data;
	}


	template < typename T >
	const T& mint::CMap<T>::get_const(u64 identifier) const
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return m_data[m_indices[i].second];
		}
	}


	template < typename T >
	void mint::CMap<T>::_restore_integrity_on_remove(u64 removed_at_index)
	{
		u64 starting_index = removed_at_index;

		for (auto i = starting_index; i < m_indices.size(); i++)
		{
			m_indices[i].second -= 1;
		}
	}


	template < typename T >
	void mint::CMap<T>::_override_data(u64 identifier, T& data)
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first)
			{
				m_data[m_indices[i].second] = std::move(data); return;
			}
		}
	}


	template < typename T >
	Vector < std::pair< mint::u64, mint::u64 > > mint::CMap<T>::get_indices()
	{
		return m_indices;
	}


	template < typename T >
	u64 mint::CMap<T>::size()
	{
		return m_data.size();
	}


	template < typename T >
	bool mint::CMap<T>::is_empty()
	{
		return size() != 0;
	}


	template < typename T >
	void mint::CMap<T>::reset()
	{
		m_indices.clear();
		m_data.clear();
	}


	template < typename T >
	void mint::CMap<T>::remove(u64 identifier)
	{
		u64 index = 0;
		for (index; index < m_indices.size(); index++)
		{
			if(identifier == m_indices[index].first)
			{
				m_data.erase(m_data.begin() + m_indices[index].second);
				m_indices.erase(m_indices.begin() + index);
				break;
			}
		}


		_restore_integrity_on_remove(index);
	}


	template < typename T >
	Vector< T >& mint::CMap<T>::get_all()
	{
		return m_data;
	}


	template < typename T >
	bool mint::CMap<T>::lookup(u64 identifier) const
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return true;
		}

		return false;
	}

	template < typename T >
	u64 mint::CMap<T>::lookup_index(u64 identifier) const
	{
		for (auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return m_indices[i].second;
		}

		return (-1);
	}

	template < typename T >
	T mint::CMap<T>::get_by_index(u64 index)
	{
		MINT_ASSERT(index < m_data.size(), "Index out of bound!");

		return reinterpret_cast< T >( m_data[index] );
	}


	template < typename T >
	T mint::CMap<T>::get(u64 identifier)
	{
		for(auto i = 0; i < m_indices.size(); i++)
		{
			if (identifier == m_indices[i].first) return m_data[m_indices[i].second];
		}
	}


	template < typename T >
	void mint::CMap<T>::add(u64 identifier, T&& data)
	{
		T& _data = data;

		add(identifier, _data);
	}


	template < typename T >
	void mint::CMap<T>::add(u64 identifier, T& data)
	{
		 if(lookup(identifier))
		 {
			 _override_data(identifier, data); return;
		 }

		 u64 index = m_data.size();

		 m_indices.emplace_back(std::make_pair(identifier, index));

		 m_data.push_back(std::move(data));
	}


	template < typename T >
	mint::CMap<T>::~CMap()
	{
		reset();
	}
}



#endif