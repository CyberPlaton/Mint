#if MINT_DISTR
#else
#include "Metaclass.h"


namespace mint::reflection
{


	CMetaClass::CMetaClass() :
		m_type(0)
	{
	}


	void CMetaClass::set_metaclass_type(u64 hash)
	{
		m_type = hash;
	}


	mint::u64 CMetaClass::get_metaclass_type()
	{
		return m_type;
	}


	void CMetaClass::set_metaclass_name(const String& name)
	{
		String __name = name;
		u64 index;

		if (index = name.find("class"); index != std::string::npos)
		{
			__name = name.substr(index + 6);
		}
		else if (index = name.find("struct"); index != std::string::npos)
		{
			__name = name.substr(index + 7);
		}
		if (index = name.find_last_of("::"); index != std::string::npos)
		{
			__name = name.substr(index + 1);
		}

		MINT_ASSERT(!__name.empty() == true, "Invalid operation. Empty Metaclass name provided.");

		m_name = __name;
	}


	mint::String CMetaClass::get_metaclass_name()
	{
		return m_name;
	}


	void CMetaClass::add_member(const String& name, CVariant& variant)
	{
		auto h = mint::algorithm::djb_hash(name);

		m_members.add(h, variant);
	}


	void CMetaClass::add_member(const String& name, CVariant&& variant)
	{
		auto h = mint::algorithm::djb_hash(name);

		m_members.add(h, variant);
	}


	mint::Vector< CVariant >& CMetaClass::get_all_members()
	{
		return m_members.get_all();
	}


}
#endif