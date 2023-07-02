#ifndef _MINT_APPLICATION_H_
#define _MINT_APPLICATION_H_


#include "Mint.h"


extern void register_scenes();


namespace mint
{

#if MINT_DISTR
	class CApplication : public mint::CMintEngine
	{
	public:

		bool initialize_application(const String& manifest_filepath);

		void terminate_application();

		void run();


	protected:

	};
#else

#define MINT_EDITOR_VERSION_MAJOR 2023
#define MINT_EDITOR_VERSION_MINOR 07
#define MINT_EDITOR_VERSION_PATCH 01
#define MINT_EDITOR_VERSION_STRING STRING(MINT_EDITOR_VERSION_MAJOR) "." STRING(MINT_EDITOR_VERSION_MINOR) "." STRING(MINT_EDITOR_VERSION_PATCH)

	class CEditor : public mint::CMintEngine
	{
	private:
			MINT_DEFINE_PERSISTENT_EVENT_LISTENER(CEditor::SSceneChangeEventDelegate, Event_SceneTransit,
			{
				auto editor = reinterpret_cast<CEditor*>(MINT_ENGINE());
				editor->m_sceneReloaded = true;
			});

	public:
		bool initialize_editor(const String& manifest_filepath);

		void terminate_editor();

		void run();

		bool is_in_editor_mode() override final;
	
		void print_engine_context() override final;

		void on_editor_ui_frame_begin();

		void on_editor_ui_frame_render();

		void on_editor_ui_frame_end();


		void on_editor_before_update();

		void on_editor_update(f32 dt);

		void on_editor_after_update(f32 dt);


		void on_editor_frame_begin();

		void on_editor_frame_render();

		void on_editor_frame_end();


	protected:
		editor::CLayerStack m_layerStack;

		CTimer m_timer;

		bool m_editingMode = false;

		bool m_sceneReloaded = false;


	protected:
		bool create_layer_stack();

		void register_engine_components_to_reflection_system();

		void toggle(bool& value);
	
		template < class T >
		T* get_layer_as(const String& name);


		void set_editing_mode();

		void unset_editing_mode();
	};


	template < class T >
	T* mint::CEditor::get_layer_as(const String& name)
	{
		return m_layerStack.get_layer_as< T >(name);
	}

#endif
}


#if MINT_DISTR
#define MINT_MAIN() \
int main(int argc, char* argv[]) \
{ \
	register_scenes(); \
	mint::CApplication app; \
	if (app.initialize_application("Manifest.manifest")) \
	{ \
		app.run(); \
	} \
	app.terminate_application(); \
	return 0; \
} 
#else
#define MINT_MAIN() \
int main(int argc, char* argv[]) \
{ \
	register_scenes(); \
	mint::CEditor editor; \
	if (editor.initialize_editor("Manifest.manifest")) \
	{ \
		editor.run(); \
	} \
	editor.terminate_editor(); \
	return 0; \
} 
#endif


#endif