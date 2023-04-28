#ifndef _MINTEDITOR_LAYER_STACK_H_
#define _MINTEDITOR_LAYER_STACK_H_


#include "Common/Layer.h"


namespace mint::editor
{

	class CLayerStack
	{
	public:
		bool try_push_layer(CLayer* layer);

		void pop_layer();

		void clear_all_layers();



		void on_ui_frame();

		void on_before_update();

		void on_update(f32 dt);

		void on_after_update(f32 dt);

		void on_frame();


	private:
		Vector< CLayer* > m_layers;

	};

}


#endif