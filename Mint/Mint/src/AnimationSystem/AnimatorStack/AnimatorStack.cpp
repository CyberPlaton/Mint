#include "AnimatorStack.h"

namespace mint::animation
{

	bool CAnimatorStack::initialize()
	{
		return true; 
	}

	void CAnimatorStack::terminate()
	{
	}

	void CAnimatorStack::on_update(f32 dt)
	{
		for (auto& animator : m_animators)
		{
			animator->on_animation_update(dt);
		}
	}

	void CAnimatorStack::remove_entity_animator(const String& animator_name)
	{

	}

}