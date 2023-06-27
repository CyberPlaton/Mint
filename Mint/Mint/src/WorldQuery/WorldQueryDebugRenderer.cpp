#include "WorldQueryDebugRenderer.h"

namespace mint
{
	CWorldQueryDebugRender::CWorldQueryDebugRender() : CRenderingPass("CWorldQueryDebugRender")
	{
	}

	bool CWorldQueryDebugRender::initialize()
	{
		m_aabbColor = { 153, 0, 0, 30 };
		return true;
	}

	void CWorldQueryDebugRender::terminate()
	{

	}

	void CWorldQueryDebugRender::on_frame_begin(fx::ICamera* render_camera)
	{
		render_camera->use_camera();
	}

	void CWorldQueryDebugRender::on_frame(Vector< entt::entity >& entities)
	{
		if (m_renderAABBs)
		{
			auto proxy = world::CWorldQuery::Get().m_registeredProxies.begin();
			while (proxy)
			{
				fx::CPrimitiveRenderer::render_aabb_filled(proxy->m_aabb, m_aabbColor, m_renderFullInformation);

				proxy = world::CWorldQuery::Get().m_registeredProxies.advance(proxy);
			}
		}
	}

	void CWorldQueryDebugRender::on_frame_end()
	{
	}
}