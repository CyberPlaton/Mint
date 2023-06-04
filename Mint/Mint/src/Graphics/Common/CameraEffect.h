#ifndef _MINTFX_CAMERA_EFFECT_H_
#define _MINTFX_CAMERA_EFFECT_H_


#include "Common/easing.h"


namespace mint::fx
{
	class CCamera2D;

	class ICameraEffect
	{
	public:
		virtual void on_update(CCamera2D*, f32) = 0;
	};

}


#endif