#ifndef _MINTFX_CAMERA_SYSTEM_H_
#define _MINTFX_CAMERA_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Camera2D.h"
#include "Common/easing.h"


namespace mint::fx
{
	class CCameraManager
	{
	public:
		STATIC_GET(CCameraManager, s_CCameraManager);

		bool initialize();

		void terminate();

		void reset();



		bool set_position(Vec2 position, bx::Easing::Enum easing_function, f32 speed = 2.0f, f32 duration = 1.0f);

		bool set_rotation(f32 rotation, bx::Easing::Enum easing_function, f32 speed = 0.1f, f32 duration = 1.0f);

		bool set_zoom(f32 zoom, bx::Easing::Enum easing_function, f32 speed = 0.1f, f32 duration = 1.0f);


		bool teleport_to_position(Vec2 position);

		bool teleport_to_rotation(f32 rotation);

		bool teleport_to_zoom(f32 zoom);



		void on_update(f32 dt);



		void push_camera(CCamera2D* camera);

		void pop_camera();

		void pop_to_default();

		void set_default_camera(CCamera2D* camera);

		mint::fx::CCamera2D* get_active_camera();

		mint::fx::CCamera2D* get_default_camera();

		template< class T >
		T* get_active_camera_as();

		template< class T >
		T* get_default_camera_as();


	private:
		Vector< CCamera2D* > m_cameraStack;

		bx::EaseFn m_currentEasingFunction;

		f32 m_translationAnimationDuration = 0.0f;
		f32 m_rotationAnimationDuration = 0.0f;
		f32 m_zoomAnimationDuration = 0.0f;


		f32 m_translationCursor = 0.0f;
		f32 m_rotationCursor = 0.0f;
		f32 m_zoomCursor = 0.0f;

		f32 m_translationSpeed = 0.0f;
		f32 m_rotationSpeed = 0.0f;
		f32 m_zoomSpeed = 0.0f;


		Vec2 m_targetEpsilonVec = {1.0f, 1.0f};
		f32 m_targetEpsilonFloat = 0.1f;


		Vec2 m_targetTranslation = { 0.0f, 0.0f };
		f32 m_targetRotation = 0.0f;
		f32 m_targetZoom = 0.0f;
	};


	template< class T >
	T* mint::fx::CCameraManager::get_default_camera_as()
	{
		return mint::algorithm::vector_get_first_element_as< T* >(m_cameraStack);
	}

	template< class T >
	T* mint::fx::CCameraManager::get_active_camera_as()
	{
		return mint::algorithm::vector_get_last_element_as< T* >(m_cameraStack);
	}

}

#endif