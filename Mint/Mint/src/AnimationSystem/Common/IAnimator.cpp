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

	CAnimator::CAnimator(const String& animator_name, entt::entity entity) :
		m_name(animator_name), m_entity(entity)
	{
	}

	CAnimator::~CAnimator()
	{

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

	mint::Vec4 CAnimator::convert_source_rect_to_normalized_source_rect(const Vec4& source_rect, f32 texture_width, f32 texture_height)
	{
		return { source_rect.x / texture_width, source_rect.y / texture_height,
				 source_rect.z / texture_width, source_rect.w / texture_height };
	}

}