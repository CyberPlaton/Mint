#ifndef _MINT_UI_H_
#define _MINT_UI_H_


#include "Common/UICommon.h"


namespace mint
{

	class CUI
	{
	public:
		STATIC_GET(CUI, s_CUI);

		bool initialize(Window* window);

		void terminate();

		void begin();

		void end();


	private:
		static CUI* s_CUI;
	};

}

#endif