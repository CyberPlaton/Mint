#if MINT_DISTR
#else
#ifndef _MINTEDITOR_LAYER_STACK_H_
#define _MINTEDITOR_LAYER_STACK_H_


#include "Common/Layer.h"
#include "Utility/Logging/Logging.h"


namespace mint::editor
{

	class CLayerStack
	{
	public:
		bool try_push_layer(CLayer* layer, bool as_root_layer = false);

		void pop_layer();

		void clear_all_layers();

		void print_registered_layers();


		void on_ui_frame();

		void on_before_update();

		void on_update(f32 dt);

		void on_after_update(f32 dt);

		void on_frame();

		Vector< CLayer* >& get_all_layers();

		void print_layer_recursive(ILayer* layer, mint::u32 depth = 0);

		template < class T >
		T* get_layer_as(const String& name);

	private:
		Vector< CLayer* > m_layers;

		CLayer* m_rootLayer;


	private:
		template < class T >
		T* get_layer_recursive_as(const String& name, Vector< ILayer* >& layers);

	};


	template < class T >
	T* mint::editor::CLayerStack::get_layer_recursive_as(const String& name, Vector< ILayer* >& layers)
	{
		auto h = mint::algorithm::djb_hash(name);

		for (auto& layer : layers)
		{
			auto hh = mint::algorithm::djb_hash(layer->get_layer_name());

			if (hh == h) return reinterpret_cast<T*>(layer);

			if (layer->has_children_layers())
			{
				auto recursive_search = get_layer_recursive_as< T >(name, layer->get_children_layers());

				if (recursive_search != nullptr) return recursive_search;
			}
		}

		return nullptr;
	}


	template < class T >
	T* mint::editor::CLayerStack::get_layer_as(const String& name)
	{
		return get_layer_recursive_as< T >(name, m_rootLayer->get_children_layers());
	}


}



#endif
#endif