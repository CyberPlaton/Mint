#ifndef _MINT_APPLICATION_H_
#define _MINT_APPLICATION_H_


#include "Mint.h"


extern void register_scenes();


namespace mint
{

	class CApplication : public mint::CMintEngine
	{
	public:

		bool initialize_application(const String& manifest_filepath);

		void terminate_application();

		void run();


	protected:

	};


	class CEditor : public mint::CMintEngine
	{
	public:

		bool initialize_editor(const String& manifest_filepath);

		void terminate_editor();

		void run();

	
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

	};
}


#ifdef MINT_DISTR
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