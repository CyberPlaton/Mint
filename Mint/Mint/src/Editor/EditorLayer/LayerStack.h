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
		bool try_push_layer(CLayer* layer);

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

	private:
		Vector< CLayer* > m_layers;

	};

}


#endif
#endif