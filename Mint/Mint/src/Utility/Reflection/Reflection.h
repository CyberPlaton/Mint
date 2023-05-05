#ifndef _MINT_REFLECTION_H_
#define _MINT_REFLECTION_H_


#include "Metaclass.h"


namespace mint::reflection
{
	struct SBase;

	struct SAutoAdder
	{
		SAutoAdder(SBase* base, const String& name, CVariant variant);
	};

	struct SBase
	{
		friend class SAutoAdder;

		static CMetaClass* get_metaclass(SBase* component);


	protected:
		CMetaClass m_metaclass;
	};

}


#if MINT_DISTR
#define INHERITS(class_name)

#define REFLECTED_CLASS(class_name)

#define REFLECTED_MEMBER(type, member_variable) type member_variable;

#else
#define INHERITS(class_name) : public class_name

#define REFLECTED_CLASS(class_name) \
class_name()\
{\
	this->m_metaclass.set_metaclass_type(entt::type_id< class_name >().hash()); \
	this->m_metaclass.set_metaclass_name(typeid(class_name).name()); \
}


#define REFLECTED_MEMBER(type, variant_type, member_variable) \
type member_variable; \
private: \
mint::reflection::SAutoAdder member_variable##_adder{this, #member_variable, {mint::reflection::VariantType::variant_type, &this->member_variable} }; \
public: \

#endif



#endif