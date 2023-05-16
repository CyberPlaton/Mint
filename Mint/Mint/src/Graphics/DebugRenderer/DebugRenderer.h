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
		static void on_render(mint::IScene* scene);

		static void on_render(Vector< entt::entity >& entities);

	};

}


#endif