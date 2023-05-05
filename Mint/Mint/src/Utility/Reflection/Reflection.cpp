#include "Reflection.h"


namespace mint::reflection
{

 	SAutoAdder::SAutoAdder(SBase* base, const String& name, CVariant variant)
 	{
 		base->m_metaclass.add_member(name, variant);
 	}


    mint::reflection::CMetaClass* SBase::get_metaclass(SBase* component)
	{
        return &component->m_metaclass;
	}


}