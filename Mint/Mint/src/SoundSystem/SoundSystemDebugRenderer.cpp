#include "SoundSystemDebugRenderer.h"

namespace mint
{
	CSoundSystemDebugRender::CSoundSystemDebugRender() : CRenderingPass("CSoundSystemDebugRender")
	{
	}

	bool CSoundSystemDebugRender::initialize()
	{
		m_circleColor = { 173, 216, 230, 30 };
		m_minMaxColor = { 10, 216, 230, 30 };
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
		// Always render listener position if desired.
		if (m_renderLineToListener)
		{
			fx::CPrimitiveRenderer::render_circle_outlined_lines({ m_listenerPosition.x, m_listenerPosition.y }, 10.0f, m_circleColor);
		}



		Vector< entt::entity > to_render_entities;

		if (!m_renderAll)
		{
			if (m_entityFilter == entt::null) return;

			to_render_entities.push_back(m_entityFilter);
		}
		else
		{
			// Note: We ignore entities provided, as sound sources without a SSprite component are discarded.
			auto& view = MINT_SCENE_REGISTRY()->get_registry_view< mint::component::SSoundSource >();

			for (auto& entity : view) to_render_entities.push_back(entity);
		}


		for (auto& entity : to_render_entities)
		{
			auto position = CUCA::transform_get_position(entity);

			if (m_renderPosition)
			{
				fx::CPrimitiveRenderer::render_circle_outlined_lines(position, 10.0f, m_circleColor);
			}
			if (m_renderMinMaxDistance)
			{
				auto min = CUCA::soundsource_get_sound_source_min_distance(entity);
				auto max = CUCA::soundsource_get_sound_source_max_distance(entity);
				_render_sound_source_min_max_distance(entity, position, min, max);
			}
			if (m_renderLineToListener)
			{
				_render_sound_source_line_to_listener(entity, position, m_listenerPosition, 1.3f);
			}
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

	void CSoundSystemDebugRender::_render_sound_source_min_max_distance(entt::entity entity, const Vec2& position, f32 min, f32 max)
	{
		DrawRing({ position.x, position.y }, min, max, 0.0f, 360.0f, 0, m_minMaxColor.as_cliteral());
		DrawRingLines({ position.x, position.y }, min, max, 0.0f, 360.0f, 0, MINT_BLACK().as_cliteral());
	}

	void CSoundSystemDebugRender::_render_sound_source_line_to_listener(entt::entity entity, const Vec2& position, const Vec3& listener, f32 thikness)
	{
		fx::CPrimitiveRenderer::render_line(position, { m_listenerPosition.x, m_listenerPosition.y }, MINT_BLACK(), thikness);
	}

}