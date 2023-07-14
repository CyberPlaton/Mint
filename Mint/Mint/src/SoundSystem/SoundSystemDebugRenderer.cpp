#include "SoundSystemDebugRenderer.h"

namespace mint
{
	CSoundSystemDebugRender::CSoundSystemDebugRender() : CRenderingPass("CSoundSystemDebugRender")
	{
	}

	bool CSoundSystemDebugRender::initialize()
	{
		m_color = { 173, 216, 230 , 30 };
		return true;
	}

	void CSoundSystemDebugRender::terminate()
	{

	}

	void CSoundSystemDebugRender::on_frame_begin(fx::ICamera* render_camera)
	{
		render_camera->use_camera();
	}

	void CSoundSystemDebugRender::on_frame(Vector< entt::entity >& entities)
	{
		if (m_renderPosition)
		{
			if (m_renderAll)
			{

			}
			else if (m_entityFilter != entt::null)
			{
				auto eposition = CUCA::transform_get_position(m_entityFilter);

				fx::CPrimitiveRenderer::render_circle_outlined_lines(eposition, 10.0f, m_color);
			}
		}

		if (m_renderCone)
		{

		}
	}

	void CSoundSystemDebugRender::on_frame_end()
	{
	}

	void CSoundSystemDebugRender::add_entity_to_filter(entt::entity entity)
	{
		m_entityFilter = entity;
	}

	void CSoundSystemDebugRender::clear_entity_filter()
	{
		m_entityFilter = entt::null;
	}

}