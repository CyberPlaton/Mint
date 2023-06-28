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
			if (m_renderAll)
			{
				auto proxy = world::CWorldQuery::Get().m_registeredProxies.begin();
				while (proxy)
				{
					fx::CPrimitiveRenderer::render_aabb_filled(proxy->m_aabb, m_aabbColor, m_renderFullInformation);

					proxy = world::CWorldQuery::Get().m_registeredProxies.advance(proxy);
				}
			}
			else
			{
				auto proxy = world::CWorldQuery::Get().get_entity_proxy(m_entityFilter);

				fx::CPrimitiveRenderer::render_aabb_filled(proxy->m_aabb, m_aabbColor, m_renderFullInformation);
			}
		}
	}

	void CWorldQueryDebugRender::on_frame_end()
	{
	}

	void CWorldQueryDebugRender::set_render_all_entities(bool value)
	{
		m_renderAll = value;
	}

	void CWorldQueryDebugRender::add_entity_to_filter(entt::entity entity)
	{
		m_entityFilter = entity;
	}

	void CWorldQueryDebugRender::clear_entity_filter()
	{
		m_entityFilter = entt::null;
	}

}