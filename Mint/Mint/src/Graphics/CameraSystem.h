#ifndef _MINTFX_CAMERA_SYSTEM_H_
#define _MINTFX_CAMERA_SYSTEM_H_


#include "Common/Common.h"
#include "Common/Camera2D.h"
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