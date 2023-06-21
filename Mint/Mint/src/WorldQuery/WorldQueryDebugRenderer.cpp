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
			for (auto& proxy : CWorldQuery::Get().m_registeredProxies)
			{
				CRect rect = mint::algorithm::compute_rect_from_aabb(proxy.second.m_aabb);

				fx::CPrimitiveRenderer::render_aabb_filled(proxy.second.m_aabb, m_aabbColor, m_renderFullInformation);

			}
		}
	}

	void CWorldQueryDebugRender::on_frame_end()
	{
	}
}