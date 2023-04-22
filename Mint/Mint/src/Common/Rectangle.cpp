#include "Rectangle.h"


namespace mint
{

	CRect::CRect() :
		m_position(Vec2(0, 0)), m_dimension(Vec2(0, 0))
	{
	}


	CRect::CRect(f32 x, f32 y, f32 w, f32 h) :
		m_position(Vec2(x, y)), m_dimension(Vec2(w, h))
	{
	}


	void CRect::set_x(f32 value)
	{
		m_position.x = value;
	}


	void CRect::set_y(f32 value)
	{
		m_position.y = value;
	}


	void CRect::set_width(f32 value)
	{
		m_dimension.x = value;
	}


	void CRect::set_height(f32 value)
	{
		m_dimension.y = value;
	}


	void CRect::set_rectangle(f32 x, f32 y, f32 w, f32 h)
	{
		m_position.x = x;
		m_position.y = y;
		m_dimension.x = w;
		m_dimension.y = h;
	}


	glm::f32 CRect::get_x() const
	{
		return m_position.x;
	}


	glm::f32 CRect::get_y() const
	{
		return m_position.y;
	}


	glm::f32 CRect::get_width() const
	{
		return m_dimension.x;
	}


	glm::f32 CRect::get_height() const
	{
		return m_dimension.y;
	}


	mint::Vec2 CRect::get_top_left()
	{
		return { m_position.x, m_position.y };
	}


	mint::Vec2 CRect::get_top_right()
	{
		return { m_position.x + m_dimension.x, m_position.y };
	}


	mint::Vec2 CRect::get_bottom_left()
	{
		return { m_position.x, m_position.y + m_dimension.y };
	}


	mint::Vec2 CRect::get_bottom_right()
	{
		return { m_position.x + m_dimension.x, m_position.y + m_dimension.y };
	}


	mint::Vec4 CRect::get_as_vec4()
	{
		return { m_position.x, m_position.y, m_dimension.x, m_dimension.y };
	}


}