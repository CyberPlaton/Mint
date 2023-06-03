#include "EditorCamera.h"


namespace mint::fx
{

	CEditorCamera::CEditorCamera(const CColor& clear_color, u32 x /*= 0*/, u32 y /*= 0*/, u32 w /*= 0*/, u32 h /*= 0*/, f32 z /*= 1.0f*/, f32 r /*= 0.0f*/) :
		CCamera2D(clear_color, x, y, w, h, z, r)
	{
	}

}