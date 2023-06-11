#ifndef _MINT_ANIMATOR_STACK_H_
#define _MINT_ANIMATOR_STACK_H_


#include "../Common/IAnimator.h"
#include "Utility/STL/Map2.h"


namespace mint::animation
{
	class CAnimatorStack
	{
	public:

		bool initialize();

		void terminate();

		void on_update(f32 dt);

		template < class T, typename... ARGS >
		bool try_push_entity_animator(const String& animator_name, ARGS&&... args);

		template < class T, typename... ARGS >
		bool try_push_entity_animator_at_index(const String& animator_name, u64 index, ARGS&&... args);

		void remove_entity_animator(const String& animator_name);

		template < class T >
		T* get_entity_animator_as(const String& animator_name);


	private:
		Vector< CAnimator* > m_animators;


	};

	template < class T >
	T* mint::animation::CAnimatorStack::get_entity_animator_as(const String& animator_name)
	{
		auto h = mint::algorithm::djb_hash(animator_name);

		for (auto& animator : m_animators)
		{
			if(animator->get_animator_handle() == h) return reinterpret_cast<T*>(animator);
		}

		return nullptr;
	}

	template < class T, typename... ARGS >
	bool mint::animation::CAnimatorStack::try_push_entity_animator(const String& animator_name, ARGS&&... args)
	{
		auto h = mint::algorithm::djb_hash(animator_name);

		mint::algorithm::vector_push_back< CAnimator* >(m_animators, new T(args...));

		T* animator = mint::algorithm::vector_get_last_element_as< T* >(m_animators);

		if (animator != nullptr) return true;
	}


	template < class T, typename... ARGS >
	bool mint::animation::CAnimatorStack::try_push_entity_animator_at_index(const String& animator_name, u64 index, ARGS&&... args)
	{
		auto h = mint::algorithm::djb_hash(animator_name);

		T* animator = nullptr;

		if (index >= 0 && index < m_animators.size())
		{
			reinterpret_cast< T* >( m_animators[index] )->~T();

			animator = new (m_animators[index]) T(args...);
		}
		else
		{
			mint::algorithm::vector_push_back(m_animators, new T(args...));
			
			animator = mint::algorithm::vector_get_last_element_as< T* >(m_animators);
		}

		if (animator != nullptr) return true;
	}

}


#endif