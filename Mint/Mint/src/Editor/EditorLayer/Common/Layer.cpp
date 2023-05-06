#if MINT_DISTR
#else
#include "Layer.h"

namespace mint::editor
{
	mint::f32 CLayer::get_window_width()
	{
		return MINT_ENGINE()->get_main_window_const().get_w();
	}


	mint::f32 CLayer::get_window_height()
	{
		return MINT_ENGINE()->get_main_window_const().get_h();
	}


	mint::f32 CLayer::percent(mint::f32 value, mint::f32 p)
	{
		if (p < 1) p = 1;
		if (p > 100) p = 100;

		return value * (p / 100.0f);
	}


	void CLayer::add_child_layer(ILayer* layer)
	{
		mint::algorithm::vector_push_back(m_childrenLayers, layer);

		layer->set_parent_layer(this);
	}


	bool CLayer::has_children_layers() const
	{
		return !m_childrenLayers.empty();
	}


	bool CLayer::has_parent_layer() const
	{
		return m_parentLayer != nullptr;
	}


	void CLayer::on_ui_frame()
	{
		for (const auto& layer : get_children_layers())
		{
			layer->on_ui_frame();
		}
	}


	void CLayer::on_before_update()
	{
		for (const auto& layer : get_children_layers())
		{
			layer->on_before_update();
		}
	}


	void CLayer::on_update(f32 dt)
	{
		for (const auto& layer : get_children_layers())
		{
			layer->on_update(dt);
		}
	}


	void CLayer::on_after_update(f32 dt)
	{
		for (const auto& layer : get_children_layers())
		{
			layer->on_after_update(dt);
		}
	}


	void CLayer::on_frame()
	{
		for (const auto& layer : get_children_layers())
		{
			layer->on_frame();
		}
	}


	void CLayer::on_terminate()
	{
		auto& children = get_children_layers();

		while (!children.empty())
		{
			auto kid = children[0];

			children.erase(children.begin());

			delete kid; kid = nullptr;
		}
	}


	void CLayer::set_parent_layer(ILayer* layer)
	{
		m_parentLayer = layer;
	}


}
#endif