#ifndef _MINT_RECTANGLE_H_
#define _MINT_RECTANGLE_H_


#include "Common.h"


namespace mint
{

	class CRect
	{
	public:
		CRect();
		CRect(f32 x, f32 y, f32 w, f32 h);
		~CRect() = default;

		void set_x(f32 value);
		void set_y(f32 value);
		void set_width(f32 value);
		void set_height(f32 value);
		void set_rectangle(f32 x, f32 y, f32 w, f32 h);


		f32 get_x() const;
		f32 get_y() const;
		f32 get_width() const;
		f32 get_height() const;


		Vec2 get_top_left();
		Vec2 get_top_right();
		Vec2 get_bottom_left();
		Vec2 get_bottom_right();


		Vec4 get_as_vec4() const;


	private:
		Vec2 m_position;
		Vec2 m_dimension;

	};


}

#endif