#if MINT_DISTR
#else
#ifndef _MINT_VARIANT_H_
#define _MINT_VARIANT_H_


#include "Common/Common.h"


namespace mint::reflection
{
	enum VariantType
	{
		VariantType_None = 0,
		VariantType_Uint32,
		VariantType_Uint64,
		VariantType_Sint32,
		VariantType_Sint64,
		VariantType_Float,
		VariantType_Double,
		VariantType_String,
		VariantType_Boolean,
		VariantType_Vec2,
		VariantType_Vec3,
		VariantType_Vec4,
		VariantType_Rect,
		VariantType_Color,
		VariantType_Pointer
	};


	class CVariant
	{
	public:
		CVariant();
		CVariant(VariantType type, String name, void* data_pointer);
		~CVariant();

		VariantType get_type();

		String get_name();

		void set(VariantType type, void* data_pointer);

		bool is(VariantType type);

		template < typename T >
		T* cast_safe(VariantType type);



	private:
		VariantType m_type;
		String m_name;
		void* m_dataPointer;
	};


	template < typename T >
	T* mint::reflection::CVariant::cast_safe(VariantType type)
	{
		if (is(type))
		{
			return reinterpret_cast<T*>(m_dataPointer);
		}

		return nullptr;
	}


}

#endif
#endif