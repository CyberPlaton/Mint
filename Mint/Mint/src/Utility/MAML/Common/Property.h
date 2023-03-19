#ifndef _MAML_PROPERTY_H_
#define _MAML_PROPERTY_H_


#include "Common/Common.h"
#include "Common/Any.h"


namespace maml
{
	struct SProperty
	{
		SProperty(const mint::String& name, mint::CAny& data);
		SProperty(const mint::String& name, mint::CAny&& data);
		SProperty() = default;
		~SProperty();

		mint::String m_name;

		mint::CAny m_data;
	};

}


#endif