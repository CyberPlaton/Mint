#ifndef _MAML_PROPERTY_H_
#define _MAML_PROPERTY_H_


#include "Common/Common.h"
#include "Common/Any.h"


namespace maml
{
	struct SProperty
	{
		SProperty(const mint::String& name, std::any& data);
		SProperty(const mint::String& name, std::any&& data);
		SProperty(const mint::String& name, mint::CAny& data);
		SProperty(const mint::String& name, mint::CAny&& data);
		SProperty() = default;
		~SProperty();

		template< typename T >
		bool is() const noexcept;

		template< typename T >
		T cast();

		mint::String get_property_name();

		mint::String m_name;

		mint::CAny m_data;
	};


	template< typename T >
	T maml::SProperty::cast()
	{
		return m_data.cast< T >();
	}


	template< typename T >
	bool maml::SProperty::is() const noexcept
	{
		return m_data.is< T >();
	}


}


#endif