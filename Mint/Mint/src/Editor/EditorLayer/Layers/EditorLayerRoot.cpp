#if MINT_DISTR
#else
#include "EditorLayerRoot.h"

namespace mint::editor
{


	CRootLayer::CRootLayer()
	{

	}


	bool CRootLayer::on_initialize()
	{
		return true;
	}


	void CRootLayer::on_update(f32 dt)
	{

	}


	void CRootLayer::on_ui_frame()
	{

	}


	String CRootLayer::get_layer_name()
	{
		return "CRootLayer";
	}


}

#endif