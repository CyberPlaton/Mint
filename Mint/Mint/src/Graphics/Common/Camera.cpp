#include "Camera.h"


namespace mint::fx
{


	ICamera::ICamera(const SViewport& viewport, const CColor& clear_color)
	{
		m_viewportRect.set_rectangle(viewport.m_left, viewport.m_top, viewport.m_right, viewport.m_bottom);
		m_nearFarPlanes = { viewport.m_nearPlane, viewport.m_farPlane };

		m_view = Mat4(1.0f);
		m_projection = Mat4(1.0f);
		m_translation = Vec3(0.0f);
		m_rotation = Vec3(0.0f);
		m_scale = Vec3(0.0f);

		m_windowHandle = viewport.m_windowHandle;

		m_renderState = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
						BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
		
		m_viewClearState = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL;

		m_viewClearColor = clear_color;
	}


	ICamera::ICamera(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane, const CColor& clear_color)
	{
		m_viewportRect.set_rectangle(x, y, width, height);
		m_nearFarPlanes = { near_plane, far_plane };

		m_view = Mat4(1.0f);
		m_projection = Mat4(1.0f);
		m_translation = Vec3(0.0f);
		m_rotation = Vec3(0.0f);
		m_scale = Vec3(0.0f);
		m_windowHandle = nullptr;

		m_renderState = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
						BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
	
		m_viewClearState = BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL;

		m_viewClearColor = clear_color;
	}


	void ICamera::set_render_state(u64 state)
	{
		m_renderState = state;
	}


	void ICamera::set_view_clear_state(u64 state)
	{
		m_viewClearState = state;
	}


	void ICamera::set_view_clear_color(u8 r, u8 g, u8 b, u8 a)
	{
		m_viewClearColor.set_color(r, g, b, a);
	}


}