#include "Any.h"

namespace mint
{
	CAny::CAny(std::any& data)
	{
		set(data);
	}

	CAny::CAny(std::any&& data)
	{
		set(data);
	}

	CAny::CAny(const CAny& other)
	{
		auto data = other.m_data;
		set(data);
	}


	CAny::~CAny()
	{
		reset();
	}


	CAny& CAny::operator=(const CAny& other)
	{
		auto data = other.m_data;
		set(data);

		return *this;
	}


	mint::u64 CAny::get_type()
	{
		return m_data.type().hash_code();
	}


	std::string CAny::get_type_name()
	{
		return m_data.type().name();
	}


	bool CAny::is_empty()
	{
		return !m_data.has_value();
	}


	void CAny::set(std::any& data)
	{
		if (m_data.has_value()) m_data.reset();

		m_data = std::move(data);
	}


	void CAny::set(std::any&& data)
	{
		if (m_data.has_value()) m_data.reset();

		m_data = std::move(data);
	}


	void CAny::reset()
	{
		m_data.reset();
	}


}