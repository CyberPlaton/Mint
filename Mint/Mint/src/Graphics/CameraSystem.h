#ifndef _MINTFX_CAMERA_SYSTEM_H_
#define _MINTFX_CAMERA_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Camera2D.h"
#include "Common/easing.h"
#include "Utility/STL/Map.h"


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


		void translate(Vec2 value);

		void rotate(f32 value);

		void zoom(f32 value);


		Vec2 get_position();

		f32 get_rotation();

		f32 get_zoom();



		void on_update(f32 dt);


		template< class T, typename... ARGS>
		void push_camera(const String& name, ARGS&&... args);

		void pop_camera(const String& name);


		template< class T, typename... ARGS>
		void set_default_camera(const String& name, ARGS&&... args);

		void set_camera_active(const String& name);

		bool is_camera_active(const String& name);

		mint::fx::CCamera2D* get_active_camera();

		mint::fx::CCamera2D* get_default_camera();

		mint::fx::CCamera2D* find_camera(const String& name);

		template< class T >
		T* find_camera_as(const String& name);

		template< class T >
		T* get_active_camera_as();

		template< class T >
		T* get_default_camera_as();


	private:
		CMap< CCamera2D* > m_cameraStack;
		u64 m_defaultCamera = 0;
		u64 m_activeCamera = 0;

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
	T* mint::fx::CCameraManager::find_camera_as(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		if (!m_cameraStack.lookup(h))
		{
			return reinterpret_cast< T* >( m_cameraStack.get(h) ) ;
		}
	}

	template< class T, typename... ARGS >
	void mint::fx::CCameraManager::push_camera(const String& name, ARGS&&... args)
	{
		auto h = mint::algorithm::djb_hash(name);

		if (!m_cameraStack.lookup(h))
		{
			m_cameraStack.add(h, new T(args...));
		}
	}

	template< class T, typename... ARGS>
	void mint::fx::CCameraManager::set_default_camera(const String& name, ARGS&&... args)
	{
		auto h = mint::algorithm::djb_hash(name);

		if (!m_cameraStack.lookup(h))
		{
			auto camera = new T(args...);

			m_cameraStack.add(h, camera);


			m_defaultCamera = h;
			if (m_activeCamera == 0) m_activeCamera = m_defaultCamera;


			m_targetTranslation = camera->get_position();
			m_targetRotation = camera->get_rotation();
			m_targetZoom = camera->get_zoom();
		}
	}

	template< class T >
	T* mint::fx::CCameraManager::get_default_camera_as()
	{
		if (m_cameraStack.lookup(m_defaultCamera))
		{
			return reinterpret_cast<T*>(m_cameraStack.get(m_defaultCamera));
		}

		return nullptr;
	}

	template< class T >
	T* mint::fx::CCameraManager::get_active_camera_as()
	{
		if (m_cameraStack.lookup(m_activeCamera))
		{
			return reinterpret_cast<T*>(m_cameraStack.get(m_activeCamera));
		}

		return nullptr;
	}

}

#endif