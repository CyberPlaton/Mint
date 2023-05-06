#if MINT_DISTR
#else
#ifndef _MINTEDITOR_COMPONENT_EDITOR_H_
#define _MINTEDITOR_COMPONENT_EDITOR_H_


#include "../../Common/EditorCommon.h"
#include "Components/Components.h"
#include "../../IMint.h"


namespace mint::editor
{

	class IComponentEditor
	{
	public:
		virtual void on_terminate() = 0;

		virtual void on_update(f32) = 0;

		virtual void on_ui_frame() = 0;
		
		virtual bool is_ready() = 0;
		
		virtual void set_ready(bool) = 0;

		virtual bool is_active() = 0;

		virtual void set_active(bool) = 0;

		virtual String get_component_name() = 0;
		
		virtual void set_component_name(const String&) = 0;

		virtual mint::f32 get_window_width() = 0;

		virtual mint::f32 get_window_height() = 0;

		virtual ImGuiWindowFlags get_flags() = 0;

		virtual void set_flags(ImGuiWindowFlags) = 0;
	};


	class CComponentEditor : public IComponentEditor
	{
	public:
		CComponentEditor(reflection::CMetaClass* metaclass);
		virtual ~CComponentEditor();

		virtual void on_terminate() {}

		virtual void on_update(f32 dt) {}

		virtual void on_ui_frame() {}


		bool is_ready() override final;
		void set_ready(bool value) override final;

		bool is_active()  override final;
		void set_active(bool value) override final;

		String get_component_name()  override final;
		void set_component_name(const String& value)  override final;

		mint::f32 get_window_width()  override final;

		mint::f32 get_window_height()  override final;

		ImGuiWindowFlags get_flags()  override final;
		void set_flags(ImGuiWindowFlags value)  override final;


	protected:
		reflection::CMetaClass* m_metaclass;

		ImGuiWindowFlags m_windowFlags;

		bool m_active;

		bool m_ready;

		f32 m_windowWidth;

		f32 m_windowHeight;

		String m_componentName;
	};

}

#endif
#endif