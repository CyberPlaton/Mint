#if MINT_DISTR

#define INHERITS(class_name)

#define REFLECTED_CLASS(class_name)

#define REFLECTED_MEMBER(type, member_variable) type member_variable;

#else
#ifndef _MINT_REFLECTION_H_
#define _MINT_REFLECTION_H_


#include "Editor/ComponentEditor/Common/ApplicationComponentsDatabaseInterface.h"
#include "EntityMetaclassDatabase.h"


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


#define INHERITS(class_name) : public class_name

#define REFLECTED_CLASS(class_name) \
class_name()\
{\
	this->m_metaclass.set_metaclass_type(entt::type_id< class_name >().hash()); \
	mint::String component_name = this->m_metaclass.set_metaclass_name(typeid(class_name).name()); \
	auto db = mint::editor::IApplicationComponentsDatabase::get_component_database(); \
	db->register_component(component_name, &class_name::add_this_component_to_entity, &class_name::remove_this_component_from_entity); \
}; \
static void add_this_component_to_entity(entt::registry& registry, entt::entity entity) \
{ \
	if(!registry.all_of< class_name >(entity)) \
	{ \
		auto& component = registry.emplace< class_name >(entity); \
		component.m_metaclass.set_metaclass_entity(entity); \
		mint::reflection::CEntityMetaclassDatabase::Get().add_entity_metaclass(SCAST(mint::u64, entity), &component.m_metaclass); \
	}; \
} \
static void remove_this_component_from_entity(entt::registry& registry, entt::entity entity) \
{ \
	if(registry.all_of< class_name >(entity)) \
	{ \
		registry.erase< class_name >(entity); \
		mint::reflection::CEntityMetaclassDatabase::Get().remove_entity_metaclass< class_name >(SCAST(mint::u64, entity)); \
	}; \
}




#define REFLECTED_MEMBER(type, variant_type, member_variable) \
type member_variable; \
private: \
mint::reflection::SAutoAdder member_variable##_adder{this, #member_variable, {mint::reflection::VariantType::variant_type, #member_variable, &this->member_variable} }; \
public: \



#endif
#endif