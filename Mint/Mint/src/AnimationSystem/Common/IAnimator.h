#ifndef _MINT_ANIMATOR_H_
#define _MINT_ANIMATOR_H_


#include "Common/Common.h"
#include "Common/Algorithm.h"
#include "Utility/Logging/Logging.h"
#include "Common/easing.h"
#include "Components/CUCA.h"
#include "Utility/Profiling/FunctionProfiler.h"


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


		virtual void advance_animation_counter(f32) = 0;

		virtual f32 get_animation_counter() = 0;

		virtual f32 get_current_easing_t_between_zero_and_one() = 0;

		virtual void set_animation_counter(f32) = 0;
		
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

		virtual bool is_entity_valid() = 0;
	};



	class CAnimator : public IAnimator
	{
	public:
		typedef bool (*Animator_on_animation_update)(CAnimator&, f32, void*);
		typedef void (*Animator_on_animation_enter)(CAnimator&, void*);
		typedef void (*Animator_on_animation_exit)(CAnimator&, void*);
		typedef bool (*Animator_on_animator_initialize)(CAnimator&, void*);
		typedef void (*Animator_on_animator_terminate)(CAnimator&, void*);



	public:
		CAnimator() = default;
		~CAnimator() = default;

		void set_entity(entt::entity entity) { m_entity = entity; }
		void set_name(const String& name) { m_name = name; }

		bool on_initialize() override final;

		void on_terminate()  override final;

		void on_animation_enter()  override final;

		void on_animation_update(f32 dt)  override final;

		void on_animation_exit()  override final;


		void set_on_animator_initialize_function(Animator_on_animator_initialize function);

		void set_on_animator_terminate_function(Animator_on_animator_terminate function);

		void set_on_animation_enter_function(Animator_on_animation_enter function);

		void set_on_animation_update_function(Animator_on_animation_update function);

		void set_on_animation_exit_function(Animator_on_animation_exit function);

		void set_animator_animation_data(void* data);



		void advance_animation_counter(f32 dt) override final;

		f32 get_animation_counter() override final;

		void set_animation_counter(f32 value) override final;

		f32 get_current_easing_t_between_zero_and_one() override final;


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

		bool is_entity_valid() override final;


	protected:
		mint::String m_name;
		entt::entity m_entity = entt::null;

		String m_animationMaterial;

		u32 m_animationDuration = 0;
		f32 m_animationSpeed = 0.0f;

		f32 m_animationCounter = 0.0f;

		bx::Easing::Enum m_easingFunction;


		Animator_on_animation_update m_on_update_function;
		Animator_on_animator_initialize m_on_initialize_function;
		Animator_on_animator_terminate m_on_terminate_function;
		Animator_on_animation_enter m_on_enter_function;
		Animator_on_animation_exit m_on_exit_function;
		void* m_on_update_data = nullptr;

	};


}

#endif