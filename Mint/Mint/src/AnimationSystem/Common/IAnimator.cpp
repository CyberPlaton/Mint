#include "IAnimator.h"

namespace mint::animation
{

	entt::entity CAnimator::get_animator_entity()
	{
		return m_entity;
	}

	void CAnimator::set_animator_entity(entt::entity entity)
	{
		m_entity = entity;
	}

	mint::u64 CAnimator::get_animator_handle()
	{
		return mint::algorithm::djb_hash(m_name);
	}

	mint::String CAnimator::get_animator_name()
	{
		return m_name;
	}

	glm::u32 CAnimator::get_animation_duration()
	{
		return m_animationDuration;
	}

	void CAnimator::set_animation_duration(u32 seconds)
	{
		m_animationDuration = seconds;
	}

	glm::f32 CAnimator::get_animation_speed()
	{
		return m_animationSpeed;
	}

	void CAnimator::set_animation_speed(f32 speed)
	{
		m_animationSpeed = speed;
	}

	void CAnimator::set_animation_material(const String& material_name)
	{
		m_animationMaterial = material_name;
	}

	mint::String CAnimator::get_animation_material()
	{
		return m_animationMaterial;
	}

	void CAnimator::set_animation_easing_function(bx::Easing::Enum function)
	{
		m_easingFunction = function;
	}

	bx::Easing::Enum CAnimator::get_animation_easing_function()
	{
		return m_easingFunction;
	}

	bool CAnimator::is_entity_valid()
	{
		return is_handle_valid(entity_get_handle(m_entity)) == true;
	}


	void CAnimator::advance_animation_counter(f32 dt)
	{
		m_animationCounter += dt * m_animationSpeed;
	}

	glm::f32 CAnimator::get_animation_counter()
	{
		return m_animationCounter;
	}

	void CAnimator::set_animation_counter(f32 value)
	{
		m_animationCounter = glm::clamp(value, 0.0f, 1.0f);
	}

	void CAnimator::on_animation_update(f32 dt)
	{
		m_on_update_function(*this, dt, m_on_update_data);
	}

	void CAnimator::set_on_animation_update_function(Animator_on_animation_update function)
	{
		m_on_update_function = function;
	}

	void CAnimator::set_on_animator_initialize_function(Animator_on_animator_initialize function)
	{
		m_on_initialize_function = function;
	}

	void CAnimator::set_on_animator_terminate_function(Animator_on_animator_terminate function)
	{
		m_on_terminate_function = function;
	}

	void CAnimator::set_on_animation_enter_function(Animator_on_animation_enter function)
	{
		m_on_enter_function = function;
	}

	void CAnimator::set_on_animation_exit_function(Animator_on_animation_exit function)
	{
		m_on_exit_function = function;
	}

	void CAnimator::set_animator_animation_data(void* data)
	{
		m_on_update_data = data;
	}

	bool CAnimator::on_initialize()
	{
		return m_on_initialize_function(*this, m_on_update_data);
	}

	void CAnimator::on_terminate()
	{
		m_on_terminate_function(*this, m_on_update_data);
	}

	void CAnimator::on_animation_enter()
	{
		m_on_enter_function(*this, m_on_update_data);
	}

	void CAnimator::on_animation_exit()
	{
		m_on_exit_function(*this, m_on_update_data);
	}

}