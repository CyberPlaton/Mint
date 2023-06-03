#include "CameraSystem.h"


namespace mint::fx
{

	bool CCameraManager::initialize()
	{
		return true;
	}

	void CCameraManager::terminate()
	{
		reset();
	}

	void CCameraManager::reset()
	{
		while (!m_cameraStack.empty())
		{
			pop_camera();
		}
	}


	bool CCameraManager::set_position(Vec2 position, bx::Easing::Enum easing_function, f32 speed /*= 2.0f*/, f32 duration /*= 1.0f*/)
	{
		m_targetTranslation = position;
		m_currentEasingFunction = bx::getEaseFunc(easing_function);
		m_translationSpeed = speed;
		m_translationAnimationDuration = duration;

		auto p = get_active_camera()->get_position();

		if (!mint::algorithm::is_value_in_between(position, m_targetTranslation + m_targetEpsilonVec, m_targetTranslation - m_targetEpsilonVec))
		{
			return false;
		}

		return true;
	}

	
	bool CCameraManager::set_rotation(f32 rotation, bx::Easing::Enum easing_function, f32 speed /*= 0.1f*/, f32 duration /*= 1.0f*/)
	{
		m_targetRotation = rotation;
		m_currentEasingFunction = bx::getEaseFunc(easing_function);
		m_rotationSpeed = speed;
		m_rotationAnimationDuration = duration;

		auto r = get_active_camera()->get_rotation();

		if (!mint::algorithm::is_value_in_between(rotation, m_targetRotation + m_targetEpsilonFloat, m_targetRotation - m_targetEpsilonFloat))
		{
			return false;
		}

		return true;
	}

	
	bool CCameraManager::set_zoom(f32 zoom, bx::Easing::Enum easing_function, f32 speed /*= 0.1f*/, f32 duration /*= 1.0f*/)
	{
		m_targetZoom = zoom;
		m_currentEasingFunction = bx::getEaseFunc(easing_function);
		m_zoomSpeed = speed;
		m_zoomAnimationDuration = duration;

		auto z = get_active_camera()->get_zoom();

		if (!mint::algorithm::is_value_in_between(z, m_targetZoom + m_targetEpsilonFloat, m_targetZoom - m_targetEpsilonFloat))
		{
			return false;
		}

		return true;
	}

	void CCameraManager::on_update(f32 dt)
	{
		auto camera = get_active_camera();

		auto position = camera->get_position();
		auto rotation = camera->get_rotation();
		auto zoom = camera->get_zoom();

		if (!mint::algorithm::is_value_in_between(position, m_targetTranslation + m_targetEpsilonVec, m_targetTranslation - m_targetEpsilonVec))
		{
			m_translationCursor += dt * m_translationSpeed;

			// Interpolate.
			auto v = glm::lerp(position, m_targetTranslation, m_currentEasingFunction((m_translationCursor) / m_translationAnimationDuration));

			camera->set_translation(v);
		}
		else
		{

		}
		
		if (!mint::algorithm::is_value_in_between(rotation, m_targetRotation + m_targetEpsilonFloat, m_targetRotation - m_targetEpsilonFloat))
		{
			m_rotationCursor += dt * m_rotationSpeed;

			// Interpolate.
			auto v = glm::lerp(rotation, m_targetRotation, m_currentEasingFunction((m_rotationCursor) / m_rotationAnimationDuration));

			camera->set_rotation(v);
		}
		else
		{

		}

		if (!mint::algorithm::is_value_in_between(zoom, m_targetZoom + m_targetEpsilonFloat, m_targetZoom - m_targetEpsilonFloat))
		{
			m_zoomCursor += dt * m_zoomSpeed;

			// Interpolate.
			auto v = glm::lerp(zoom, m_targetZoom, m_currentEasingFunction((m_zoomCursor) / m_zoomAnimationDuration));

			camera->set_zoom(v);
		}
		else
		{

		}

	}

	void CCameraManager::push_camera(CCamera2D* camera)
	{
		mint::algorithm::vector_push_back(m_cameraStack, camera);
	}

	void CCameraManager::pop_camera()
	{
		MINT_ASSERT(m_cameraStack.empty() == false, "Invalid operation. Popping an empty camera stack is not allowed!");

		auto camera = mint::algorithm::vector_get_last_element_as< CCamera2D* >(m_cameraStack);

		mint::algorithm::vector_erase_last(m_cameraStack);

		delete camera; camera = nullptr;
	}

	void CCameraManager::pop_to_default()
	{
		while (!m_cameraStack.size() == 1)
		{
			pop_camera();
		}
	}

	void CCameraManager::set_default_camera(CCamera2D* camera)
	{
		if (!m_cameraStack.empty())
		{
			auto camera_before = m_cameraStack[0];

			m_cameraStack[0] = camera;

			m_targetTranslation = camera->get_position();
			m_targetRotation = camera->get_rotation();
			m_targetZoom = camera->get_zoom();

			delete camera_before; camera_before = nullptr;
		}
		else
		{
			push_camera(camera);

			m_targetTranslation = camera->get_position();
			m_targetRotation = camera->get_rotation();
			m_targetZoom = camera->get_zoom();
		}
	}

	mint::fx::CCamera2D* CCameraManager::get_active_camera()
	{
		return mint::algorithm::vector_get_last_element_as< CCamera2D* >(m_cameraStack);
	}

	mint::fx::CCamera2D* CCameraManager::get_default_camera()
	{
		return mint::algorithm::vector_get_first_element_as< CCamera2D* >(m_cameraStack);
	}

	bool CCameraManager::teleport_to_position(Vec2 position)
	{
		auto camera = get_active_camera();

		camera->set_translation(position);

		m_targetTranslation = position;

		return true;
	}

	bool CCameraManager::teleport_to_rotation(f32 rotation)
	{
		auto camera = get_active_camera();

		camera->set_rotation(rotation);

		m_targetRotation = rotation;
	
		return true;
	}

	bool CCameraManager::teleport_to_zoom(f32 zoom)
	{
		auto camera = get_active_camera();

		camera->set_zoom(zoom);

		m_targetZoom = zoom;
	
		return true;
	}

}