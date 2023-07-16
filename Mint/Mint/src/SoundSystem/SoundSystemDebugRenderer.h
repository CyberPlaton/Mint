#ifndef _MINTSOUND_SOUND_SYSTEM_DEBUG_RENDERER_H_
#define _MINTSOUND_SOUND_SYSTEM_DEBUG_RENDERER_H_


#include "Components/CUCA.h"
#include "Graphics/Common/IRenderingPass.h"
#include "Graphics/RenderingPasses/PrimitiveRenderer/PrimitiveRenderer.h"

namespace mint
{
	class CSoundSystemDebugRender : public mint::fx::CRenderingPass
	{
	public:
		CSoundSystemDebugRender();

		bool initialize() override final;
		void terminate() override final;

		void on_frame_begin(fx::ICamera* render_camera) override final;
		void on_frame(Vector< entt::entity >& entities)  override final;
		void on_frame_end()  override final;


		void set_render_position(bool value) { m_renderPosition = value; }
		void set_render_line_to_listener(bool value) { m_renderLineToListener = value; }
		void set_render_min_max_distance(bool value) { m_renderMinMaxDistance = value; }
		void set_render_sound_engine_3d_to_2d_morph_threshold(bool value) { m_renderSoundEngineMorphThreshold = value; }
		void set_sound_engine_3d_to_2d_morph_threshold(f32 value) { m_soundEngineMorphThreshold = value; }


		void set_listener_position(const Vec3& vec) { m_listenerPosition = vec; }
		void set_circle_color(const mint::fx::CColor& color) { m_circleColor = color; }
		void set_min_max_distance_color(const mint::fx::CColor& color) { m_minMaxColor = color; }
		void set_morph_threshold_color(const mint::fx::CColor& color) { m_morphThresholdColor = color; }

		void set_render_all_entities(bool value) { m_renderAll = value; }
		void add_entity_to_filter(entt::entity entity);
		void clear_entity_filter();

		
	private:
		bool m_renderAll = false;
		entt::entity m_entityFilter = entt::null;
		bool m_renderPosition = false;
		bool m_renderMinMaxDistance = false;
		bool m_renderLineToListener = false;
		bool m_renderSoundEngineMorphThreshold = false;
		f32 m_soundEngineMorphThreshold = 0.0f;
		Vec3 m_listenerPosition = { 0.0f, 0.0f, 0.0f };
		mint::fx::CColor m_circleColor;
		mint::fx::CColor m_minMaxColor;
		mint::fx::CColor m_morphThresholdColor;


	private:
		void _render_sound_source_min_max_distance(entt::entity entity, const Vec2& position, f32 min, f32 max);
		void _render_sound_source_line_to_listener(entt::entity entity, const Vec2& position, const Vec3& listener, f32 thikness);
	
	};

}


#endif