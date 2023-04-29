#ifndef _MINTEDITOR_LAYER_H_
#define _MINTEDITOR_LAYER_H_


#include "Common/Common.h"
#include "../../Common/EditorCommon.h"
#include "../../IMint.h"


namespace mint::editor
{

	class ILayer
	{
	public:
		virtual bool on_initialize() = 0;
		virtual void on_terminate() = 0;

		virtual bool is_enabled() = 0;
		virtual void set_is_enabled(bool) = 0;


		virtual void on_ui_frame() = 0;

		virtual void on_before_update() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_after_update(f32) = 0;

		virtual void on_frame() = 0;

		virtual mint::f32 get_window_width() = 0;

		virtual mint::f32 get_window_height() = 0;

		virtual ImGuiWindowFlags get_flags() = 0;
	};



	class CLayer : public ILayer
	{
	public:
		virtual bool on_initialize() { MINT_ASSERT(false, "Invalid operation. Using interface Layer object!"); return false; };
		virtual void on_terminate() {};

		bool is_enabled() { return m_enabled; };
		void set_is_enabled(bool enabled) { m_enabled = enabled; };


		virtual void on_ui_frame() {};

		virtual void on_before_update() {};

		virtual void on_update(f32 dt) {};

		virtual void on_after_update(f32 dt) {};

		virtual void on_frame() {};

		virtual ImGuiWindowFlags get_flags() { return ImGuiWindowFlags_None; }

		mint::f32 get_window_width();

		mint::f32 get_window_height();

		mint::f32 percent(mint::f32 value, mint::u32 p);

		virtual String get_layer_name() { return "None"; }

	protected:
		bool m_enabled = true;
	};
}


#endif