#include "Property.h"


namespace maml
{


	SProperty::SProperty(const mint::String& name, mint::CAny& data) :
		m_name(name), m_data(data)
	{

	}


	SProperty::SProperty(const mint::String& name, mint::CAny&& data) :
		m_name(name), m_data(data)
	{

	}


	SProperty::SProperty(const mint::String& name, std::any& data) : 
		m_name(name), m_data({ data })
	{

	}


	SProperty::SProperty(const mint::String& name, std::any&& data) :
		m_name(name), m_data({ data })
	{

	}


	SProperty::~SProperty()
	{
		m_data.reset();
	}


	mint::String SProperty::get_property_name()
	{
		return m_name;
	}


}