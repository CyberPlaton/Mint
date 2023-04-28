#ifndef _MINTEDITOR_LAYER_H_
#define _MINTEDITOR_LAYER_H_


#include "Common/Common.h"


namespace mint::editor
{

	class ILayer
	{
	public:
		virtual bool on_initialize() = 0;
		virtual void on_terminate() = 0;

		virtual void on_ui_frame() = 0;

		virtual void on_before_update() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_after_update(f32) = 0;

		virtual void on_frame() = 0;
	};



	class CLayer : public ILayer
	{
	public:
		virtual bool on_initialize() { MINT_ASSERT(false, "Invalid operation. Using interface Layer object!"); return false; };
		virtual void on_terminate() {};

		virtual void on_ui_frame() {};

		virtual void on_before_update() {};

		virtual void on_update(f32 dt) {};

		virtual void on_after_update(f32 dt) {};

		virtual void on_frame() {};



		virtual String get_layer_name() { return "None"; }
	};
}


#endif