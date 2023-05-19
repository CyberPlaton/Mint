#ifndef _MINTFX_DEBUG_RENDERER_H_
#define _MINTFX_DEBUG_RENDERER_H_


#include "Common/Rectangle.h"


#include "../../SceneSystem/Common/IScene.h"
#include "../../Components/CUCA.h"
#include "../PrimitiveRenderer/PrimitiveRenderer.h"

namespace mint::fx
{

	class CDebugRenderer
	{
	public:
		static void on_render_destination_rectangle(const Vector< entt::entity >& entities);

		static void on_render_sprite_origin(const Vector< entt::entity >& entities);

	};

}


#endif