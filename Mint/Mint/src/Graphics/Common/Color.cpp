#include "Color.h"


namespace mint::fx
{


	CColor::CColor(u8 r, u8 g, u8 b, u8 a) :
		m_r(glm::clamp< u8 >(r, 0, 255)),
		m_g(glm::clamp< u8 >(g, 0, 255)),
		m_b(glm::clamp< u8 >(b, 0, 255)),
		m_a(glm::clamp< u8 >(a, 0, 255))
	{
	}


	CColor::CColor(mint::Vec4 vec, bool is_normalized /*= false*/)
	{
		if(is_normalized)
		{
			m_r = SCAST(u8, vec.r) * 255;
			m_g = SCAST(u8, vec.g) * 255;
			m_b = SCAST(u8, vec.b) * 255;
			m_a = SCAST(u8, vec.a) * 255;
		}
		else
		{
			m_r = SCAST(u8, vec.r);
			m_g = SCAST(u8, vec.g);
			m_b = SCAST(u8, vec.b);
			m_a = SCAST(u8, vec.a);
		}
	}


	CColor::CColor(u32 rgba, bool is_argb /*= true*/)
	{
		if(is_argb)
		{
			m_a = (SCAST(u32, rgba) >> 24) & 0xFF;
			m_r = (SCAST(u32, rgba) >> 16) & 0xFF;
			m_g = (SCAST(u32, rgba) >> 8) & 0xFF;
			m_b = SCAST(u32, rgba) & 0xFF;
		}
		else
		{
			m_r = (SCAST(u32, rgba) >> 24) & 0xFF;
			m_g = (SCAST(u32, rgba) >> 16) & 0xFF;
			m_b = (SCAST(u32, rgba) >> 8) & 0xFF;
			m_a = SCAST(u32, rgba) & 0xFF;
		}

		
		m_r = glm::clamp< f32 >(m_r, 0.0f, 255.0f);
		m_g = glm::clamp< f32 >(m_g, 0.0f, 255.0f);
		m_b = glm::clamp< f32 >(m_b, 0.0f, 255.0f);
		m_a = glm::clamp< f32 >(m_a, 0.0f, 255.0f);
	}


	mint::Vec4 CColor::to_normalized_color_vec4() const
	{
		return { SCAST(f32, m_r) / 255.0f,
				 SCAST(f32, m_g) / 255.0f,
				 SCAST(f32, m_b) / 255.0f,
				 SCAST(f32, m_a) / 255.0f };
	}


	u32 CColor::as_rgba()
	{
		return ((SCAST(u32, m_r) << 24) | (SCAST(u32, m_g) << 16) | (SCAST(u32, m_b) << 8) | SCAST(u32, m_a));
	}


	u32 CColor::as_argb()
	{
		return ((SCAST(u32, m_a) << 24) | (SCAST(u32, m_r) << 16) | (SCAST(u32, m_g) << 8) | SCAST(u32, m_b));
	}


	mint::Vec4 CColor::as_vec4() const
	{
		return { SCAST(f32, m_r),
				 SCAST(f32, m_g),
				 SCAST(f32, m_b),
				 SCAST(f32, m_a) };
	}


	mint::u32 CColor::as_abgr()
	{
		return ((SCAST(u32, m_a) << 24) | (SCAST(u32, m_b) << 16) | (SCAST(u32, m_g) << 8) | SCAST(u32, m_r));
	}


	void CColor::set_color(u8 r, u8 g, u8 b, u8 a)
	{
		m_r = glm::clamp< u8 >(r, 0, 255);
		m_g = glm::clamp< u8 >(g, 0, 255);
		m_b = glm::clamp< u8 >(b, 0, 255);
		m_a = glm::clamp< u8 >(a, 0, 255);
	}


	Color CColor::as_cliteral()
	{
		return { m_r, m_g, m_b, m_a };
	}


	Color CColor::as_cliteral() const
	{
		return { m_r, m_g, m_b, m_a };
	}


}