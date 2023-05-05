#include "Variant.h"


namespace mint::reflection
{

	CVariant::CVariant(VariantType type, void* data_pointer) :
		m_dataPointer(data_pointer), m_type(type)
	{
	}


	CVariant::CVariant() :
		m_dataPointer(nullptr), m_type(VariantType_None)
	{
	}


	CVariant::~CVariant()
	{
		m_type = VariantType_None;
		m_dataPointer = nullptr;
	}


	void CVariant::set(VariantType type, void* data_pointer)
	{
		MINT_ASSERT(m_dataPointer == nullptr && m_type == VariantType_None, "Invalid operation. Reassigning Variant is not allowed!");

		m_dataPointer = data_pointer;
		m_type = type;
	}


	bool CVariant::is(VariantType type)
	{
		return type == m_type && m_type != VariantType_None;
	}


}