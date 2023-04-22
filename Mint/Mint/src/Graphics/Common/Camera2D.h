#ifndef _MINTFX_CAMERA2D_H_
#define _MINTFX_CAMERA2D_H_


#include "Camera.h"


namespace mint::fx
{
	class CCamera2D : public ICamera
	{
	public:
		CCamera2D(const CColor& clear_color);

		CRect get_world_visible_area() override final;

		void use_camera() override final;

		void end_camera() override final;

		void set_translation(Vec2 value) override final;

		void set_translation_offset(Vec2 value) override final;

		void set_rotation(f32 value) override final;

		void set_zoom(f32 value) override final;


	private:
		raylib::Camera2D m_camera;

	};
}


#endif