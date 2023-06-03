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
		auto& v = m_cameraStack.get_all();

		while (!v.empty())
		{
			auto camera = v[0];

			delete camera; camera = nullptr;

			v.erase(v.begin());
		}

		m_cameraStack.reset();
	}


	bool CCameraManager::set_position(Vec2 position, bx::Easing::Enum easing_function, f32 speed /*= 2.0f*/, f32 duration /*= 1.0f*/)
	{
		m_targetTranslation = position;
		m_currentEasingFunction = bx::getEaseFunc(easing_function);
		m_translationSpeed = speed;
		m_translationAnimationDuration = duration;

		auto p = get_active_camera()->get_position();

		if (!mint::algorithm::is_value_in_between(p, m_targetTranslation + m_targetEpsilonVec, m_targetTranslation - m_targetEpsilonVec))
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

		if (!mint::algorithm::is_value_in_between(r, m_targetRotation + m_targetEpsilonFloat, m_targetRotation - m_targetEpsilonFloat))
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

		if (camera->on_update(dt))
		{
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
				m_targetTranslation = position;
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
				m_targetRotation = rotation;
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
				m_targetZoom = zoom;
			}
		}
	}


	void CCameraManager::pop_camera(const String& name)
	{
		MINT_ASSERT(m_cameraStack.size() > 0, "Invalid operation. Camera Stack is empty!");

		auto h = mint::algorithm::djb_hash(name);

		if (m_cameraStack.lookup(h))
		{
			auto camera = m_cameraStack.get(h);

			delete camera; camera = nullptr;

			m_cameraStack.remove(h);
		}
	}



	mint::fx::CCamera2D* CCameraManager::get_active_camera()
	{
		return m_cameraStack.get(m_activeCamera);
	}

	mint::fx::CCamera2D* CCameraManager::get_default_camera()
	{
		return m_cameraStack.get(m_defaultCamera);
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

	void CCameraManager::set_camera_active(const String& name)
	{
		m_activeCamera = mint::algorithm::djb_hash(name);
	}

	mint::fx::CCamera2D* CCameraManager::find_camera(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		if (m_cameraStack.lookup(h))
		{
			return m_cameraStack.get(h);
		}

		return nullptr;
	}

	void CCameraManager::translate(Vec2 value)
	{
		m_cameraStack.get(m_activeCamera)->translate(value);

		m_targetTranslation = m_cameraStack.get(m_activeCamera)->get_position();
	}

	void CCameraManager::rotate(f32 value)
	{
		m_cameraStack.get(m_activeCamera)->rotate(value);
	
		m_targetRotation = m_cameraStack.get(m_activeCamera)->get_rotation();
	}

	void CCameraManager::zoom(f32 value)
	{
		m_cameraStack.get(m_activeCamera)->zoom(value);
	
		m_targetZoom = m_cameraStack.get(m_activeCamera)->get_zoom();
	}

	mint::Vec2 CCameraManager::get_position()
	{
		return m_cameraStack.get(m_activeCamera)->get_position();
	}

	mint::f32 CCameraManager::get_rotation()
	{
		return m_cameraStack.get(m_activeCamera)->get_rotation();
	}

	mint::f32 CCameraManager::get_zoom()
	{
		return m_cameraStack.get(m_activeCamera)->get_zoom();
	}

	bool CCameraManager::is_camera_active(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		return m_activeCamera == h;
	}

}