#include "Color.h"


namespace mint::fx
{


	CColor::CColor(u32 r, u32 g, u32 b, u32 a) :
		m_r(glm::clamp< u32 >(r, 0, 255)),
		m_g(glm::clamp< u32 >(g, 0, 255)),
		m_b(glm::clamp< u32 >(b, 0, 255)),
		m_a(glm::clamp< u32 >(a, 0, 255))
	{
		m_normalized.r = SCAST(f32, m_r) / 255.0f;
		m_normalized.g = SCAST(f32, m_g) / 255.0f;
		m_normalized.b = SCAST(f32, m_b) / 255.0f;
		m_normalized.a = SCAST(f32, m_a) / 255.0f;
	}


	CColor::CColor(mint::Vec4 vec, bool is_normalized /*= false*/)
	{
		if(is_normalized)
		{
			m_normalized = vec;

			m_r = SCAST(u32, m_normalized.r * 255.0f);
			m_g = SCAST(u32, m_normalized.g * 255.0f);
			m_b = SCAST(u32, m_normalized.b * 255.0f);
			m_a = SCAST(u32, m_normalized.a * 255.0f);
		}
		else
		{
			m_r = SCAST(u32, vec.r);
			m_g = SCAST(u32, vec.g);
			m_b = SCAST(u32, vec.b);
			m_a = SCAST(u32, vec.a);

			m_normalized.r = SCAST(f32, m_r) / 255.0f;
			m_normalized.g = SCAST(f32, m_g) / 255.0f;
			m_normalized.b = SCAST(f32, m_b) / 255.0f;
			m_normalized.a = SCAST(f32, m_a) / 255.0f;
		}
	}


	CColor::CColor(u32 rgba, bool is_argb /*= true*/)
	{
		if(is_argb)
		{
			m_a = (rgba >> 24) & 0xFF;
			m_r = (rgba >> 16) & 0xFF;
			m_g = (rgba >> 8) & 0xFF;
			m_b = rgba & 0xFF;
		}
		else
		{
			m_r = (rgba >> 24) & 0xFF;
			m_g = (rgba >> 16) & 0xFF;
			m_b = (rgba >> 8) & 0xFF;
			m_a = rgba & 0xFF;
		}

		
		m_r = glm::clamp< u32 >(m_r, 0, 255);
		m_g = glm::clamp< u32 >(m_g, 0, 255);
		m_b = glm::clamp< u32 >(m_b, 0, 255);
		m_a = glm::clamp< u32 >(m_a, 0, 255);


		m_normalized.r = SCAST(f32, m_r) / 255.0f;
		m_normalized.g = SCAST(f32, m_g) / 255.0f;
		m_normalized.b = SCAST(f32, m_b) / 255.0f;
		m_normalized.a = SCAST(f32, m_a) / 255.0f;
	}


	mint::fx::CColor CColor::to_normalized_color()
	{
		return { m_normalized };
	}


	u32 CColor::as_rgba()
	{
		return ((m_r << 24) | (m_g << 16) | (m_b << 8) | m_a);
	}


	u32 CColor::as_argb()
	{
		return ((m_a << 24) | (m_r << 16) | (m_g << 8) | m_b);
	}


	mint::Vec4 CColor::as_vec4()
	{
		return m_normalized;
	}


}