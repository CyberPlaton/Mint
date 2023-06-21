#include "WorldQueryDebugRenderer.h"

namespace mint
{
	CWorldQueryDebugRender::CWorldQueryDebugRender() : CRenderingPass("CWorldQueryDebugRender")
	{
	}

	bool CWorldQueryDebugRender::initialize()
	{
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
		for (auto& proxy : CWorldQuery::Get().m_registeredProxies)
		{
			CRect rect = mint::algorithm::compute_rect_from_aabb(proxy.second.m_aabb);

			fx::CPrimitiveRenderer::render_rectangle_lines({ rect.get_x(), rect.get_y() }, { rect.get_width(), rect.get_height() }, MINT_RED_DARK(), 1.5f);

			fx::CPrimitiveRenderer::render_aabb_filled(proxy.second.m_aabb, { 0, 200, 125, 30 });

		}
	}

	void CWorldQueryDebugRender::on_frame_end()
	{
	}
}