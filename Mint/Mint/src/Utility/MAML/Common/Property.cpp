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


	SProperty::~SProperty()
	{
		m_data.reset();
	}


}