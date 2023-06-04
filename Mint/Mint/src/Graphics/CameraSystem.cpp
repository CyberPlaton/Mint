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


	void CCameraManager::on_update(f32 dt)
	{
		auto camera = get_active_camera();

		camera->on_update(dt);


// 		if (camera->on_update(dt))
// 		{
// 			auto position = camera->get_position();
// 			auto rotation = camera->get_rotation();
// 			auto zoom = camera->get_zoom();
// 
// 
// 
// 			if (!mint::algorithm::is_value_in_between(position, m_targetTranslation + m_targetEpsilonVec, m_targetTranslation - m_targetEpsilonVec))
// 			{
// 				m_translationCursor += dt * m_translationSpeed;
// 
// 				// Interpolate.
// 				auto v = glm::lerp(position, m_targetTranslation, m_currentEasingFunction((m_translationCursor) / m_translationAnimationDuration));
// 
// 				camera->set_translation(v);
// 			}
// 			else
// 			{
// 				m_targetTranslation = position;
// 			}
// 
// 			if (!mint::algorithm::is_value_in_between(rotation, m_targetRotation + m_targetEpsilonFloat, m_targetRotation - m_targetEpsilonFloat))
// 			{
// 				m_rotationCursor += dt * m_rotationSpeed;
// 
// 				// Interpolate.
// 				auto v = glm::lerp(rotation, m_targetRotation, m_currentEasingFunction((m_rotationCursor) / m_rotationAnimationDuration));
// 
// 				camera->set_rotation(v);
// 			}
// 			else
// 			{
// 				m_targetRotation = rotation;
// 			}
// 
// 			if (!mint::algorithm::is_value_in_between(zoom, m_targetZoom + m_targetEpsilonFloat, m_targetZoom - m_targetEpsilonFloat))
// 			{
// 				m_zoomCursor += dt * m_zoomSpeed;
// 
// 				// Interpolate.
// 				auto v = glm::lerp(zoom, m_targetZoom, m_currentEasingFunction((m_zoomCursor) / m_zoomAnimationDuration));
// 
// 				camera->set_zoom(v);
// 			}
// 			else
// 			{
// 				m_targetZoom = zoom;
//  			}
// 		}
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


	bool CCameraManager::is_camera_active(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		return m_activeCamera == h;
	}

}