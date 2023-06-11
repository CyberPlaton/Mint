#ifndef _MINT_ANIMATOR_H_
#define _MINT_ANIMATOR_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Common/easing.h"
#include "Components/CUCA.h"


namespace mint::animation
{

	class IAnimator
	{
	public:

		virtual bool on_initialize() = 0;

		virtual void on_terminate() = 0;

		virtual void on_animation_enter() = 0;

		virtual void on_animation_update(f32) = 0;

		virtual void on_animation_exit() = 0;


		
		virtual void set_animation_easing_function(bx::Easing::Enum) = 0;

		virtual bx::Easing::Enum get_animation_easing_function() = 0;

		virtual u32 get_animation_duration() = 0;

		virtual void set_animation_duration(u32) = 0;

		virtual f32 get_animation_speed() = 0;

		virtual void set_animation_speed(f32) = 0;

		virtual void set_animation_material(const String&) = 0;

		virtual String get_animation_material() = 0;

		virtual entt::entity get_animator_entity() = 0;

		virtual void set_animator_entity(entt::entity) = 0;

		virtual u64 get_animator_handle() = 0;

		virtual mint::String get_animator_name() = 0;



		virtual Vec4 convert_source_rect_to_normalized_source_rect(const Vec4&, f32, f32) = 0;

	};




	class CAnimator : public IAnimator
	{
	public:
		CAnimator(const String& animator_name, entt::entity entity);
		virtual ~CAnimator();


		virtual bool on_initialize() { return false; }

		virtual void on_terminate() {}

		virtual void on_animation_enter() {}

		virtual void on_animation_update(f32 dt) {}

		virtual void on_animation_exit() {}

		void set_animation_easing_function(bx::Easing::Enum function) override final;

		bx::Easing::Enum get_animation_easing_function() override final;

		u32 get_animation_duration();

		void set_animation_duration(u32 seconds);

		f32 get_animation_speed();

		void set_animation_speed(f32 speed);

		void set_animation_material(const String& material_name) override final;

		String get_animation_material() override final;

		entt::entity get_animator_entity() override final;

		void set_animator_entity(entt::entity entity) override final;

		u64 get_animator_handle() override final;

		mint::String get_animator_name() override final;

		Vec4 convert_source_rect_to_normalized_source_rect(const Vec4& source_rect, f32 texture_width, f32 texture_height) override final;

	protected:
		mint::String m_name;
		entt::entity m_entity;

		String m_animationMaterial;

		u32 m_animationDuration;
		f32 m_animationSpeed;

		bx::Easing::Enum m_easingFunction;
	};
}

#endif