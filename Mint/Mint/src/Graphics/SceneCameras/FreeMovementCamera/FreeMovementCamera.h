#ifndef _MINTFX_FRREE_MOVEMENT_CAMERA_H_
#define _MINTFX_FRREE_MOVEMENT_CAMERA_H_


#include "../../Common/Camera2D.h"
#include "UI/UI.h"
#include "Utility/Input/Input.h"


namespace mint::fx
{
	class CFreeMovementCamera : public CCamera2D
	{
	public:
		CFreeMovementCamera(const CColor& clear_color, u32 x = 0, u32 y = 0, u32 w = 0, u32 h = 0, f32 z = 1.0f, f32 r = 0.0f);

		void on_update(f32 dt) override final;
	};
}

#endif