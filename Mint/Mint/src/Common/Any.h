#ifndef _MINT_ANY_H_
#define _MINT_ANY_H_


#include "Common.h"

#include <any>

namespace mint
{
	class CAny
	{
	public:
		CAny(std::any& data);
		CAny(std::any&& data);
		CAny(const CAny& other);
		CAny& operator=(const CAny& other);
		CAny() = default;
		~CAny();


		u64 get_type();

		mint::String get_type_name();

		bool is_empty() const noexcept;

		template< typename T >
		bool is() const noexcept;

		void set(std::any& data);

		void set(std::any&& data);

		void reset();

		template< typename T >
		T cast();

	private:
		std::any m_data;
	};


	template< typename T >
	T mint::CAny::cast()
	{
		return std::any_cast<T>(m_data);
	}


	template< typename T >
	bool mint::CAny::is() const noexcept
	{
		return typeid(T).hash_code() == m_data.type().hash_code();
	}


}

#endif