#ifndef _MINTFX_CAMERA2D_H_
#define _MINTFX_CAMERA2D_H_


#include "Camera.h"


namespace mint::fx
{
	class CCamera2D : public ICamera
	{
	public:
		CCamera2D(const CColor& clear_color, u32 x = 0, u32 y = 0, u32 w = 0, u32 h = 0, f32 z = 1.0f, f32 r = 0.0f);

		CRect get_world_visible_area() override final;

		void use_camera() override final;

		void end_camera() override final;

		virtual void set_translation(Vec2 value);

		virtual void translate(Vec2 value);

		virtual void set_translation_offset(Vec2 value);

		virtual void translate_offset(Vec2 value);

		virtual void set_rotation(f32 value);

		virtual void rotate(f32 value);

		virtual void set_zoom(f32 value);

		virtual void zoom(f32 value);

		virtual bool on_update(f32 dt) { return true; }

		mint::Vec2 get_position();

		mint::Vec2 get_position_offset();

		mint::f32 get_rotation();

		mint::f32 get_zoom();


	protected:
		raylib::Camera2D m_camera;

	};

}


#endif