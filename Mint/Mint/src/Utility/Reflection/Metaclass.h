#if MINT_DISTR
#else
#ifndef _MINT_META_CLASS_H_
#define _MINT_META_CLASS_H_


#include "Common/Common.h"
#include "Common/Any.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"
#include "Variant.h"

#include <type_traits>


namespace mint::reflection
{
	class CMetaClass
	{
	public:
		CMetaClass();
		~CMetaClass() = default;

		void set_metaclass_entity(entt::entity entity);
		entt::entity get_metaclass_entity();

		void set_metaclass_type(u64 hash);
		u64 get_metaclass_type();

		void set_metaclass_name(const String& name);
		String get_metaclass_name();


		void add_member(const String& name, CVariant& variant);
		void add_member(const String& name, CVariant&& variant);


		template< typename T >
		bool is(const String& name);

		template< typename T >
		T* get_pointer(const String& name);

		Vector< CVariant >& get_all_members();


	private:
		CMap< CVariant > m_members;

		u64 m_type;

		String m_name;

		entt::entity m_entity;
	};


	template< typename T >
	bool mint::reflection::CMetaClass::is(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		return m_members.get_const(h).is< T >();
	}


	template< typename T >
	T* mint::reflection::CMetaClass::get_pointer(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		auto& variant = m_members.get_ref(h);

		if (std::is_pointer_v< T >)
		{
			if (!std::is_base_of_v< SBase, T >) MINT_ASSERT(false, "Invalid operation. Pointer type must be derived from SBase component class!");

			return variant.cast_safe< T >(VariantType_Pointer);
		}
		else if (std::is_same_v< u32, T >)
		{
			return variant.cast_safe< T >(VariantType_Uint32);
		}
		else if (std::is_same_v< u64, T >)
		{
			return variant.cast_safe< T >(VariantType_Uint64);
		}
		else if (std::is_same_v< s32, T >)
		{
			return variant.cast_safe< T >(VariantType_Sint32);
		}
		else if (std::is_same_v< s64, T >)
		{
			return variant.cast_safe< T >(VariantType_Sint64);
		}
		else if (std::is_same_v< f32, T >)
		{
			return variant.cast_safe< T >(VariantType_Float);
		}
		else if (std::is_same_v< f64, T >)
		{
			return variant.cast_safe< T >(VariantType_Double);
		}
		else if (std::is_same_v< String, T >)
		{
			return variant.cast_safe< T >(VariantType_String);
		}
		else if (std::is_same_v< bool, T >)
		{
			return variant.cast_safe< T >(VariantType_Boolean);
		}

		MINT_ASSERT(false, "Invalid operation. Unrecognized Variant pointer type!");
	}

}


#endif
#endif