#if MINT_DISTR
#else
#include "LayerStack.h"


namespace mint::editor
{


	bool CLayerStack::try_push_layer(CLayer* layer)
	{
		if (layer->on_initialize())
		{
			m_layers.push_back(layer);

			return true;
		}

		delete layer; layer = nullptr;
		return false;
	}


	void CLayerStack::pop_layer()
	{
		auto top = m_layers[m_layers.size() - 1];

		m_layers.erase(m_layers.begin() + m_layers.size() - 1);

		top->on_terminate();

		delete top; top = nullptr;
	}


	void CLayerStack::clear_all_layers()
	{
		while (!m_layers.empty())
		{
			pop_layer();
		}
	}


	void CLayerStack::on_ui_frame()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			layer->on_ui_frame();
		}
	}


	void CLayerStack::on_before_update()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			layer->on_before_update();
		}
	}


	void CLayerStack::on_update(f32 dt)
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			layer->on_update(dt);
		}
	}


	void CLayerStack::on_after_update(f32 dt)
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			layer->on_after_update(dt);
		}
	}


	void CLayerStack::on_frame()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			layer->on_frame();
		}
	}


	void CLayerStack::print_registered_layers()
	{
		MINT_LOG_INFO("Dumping registered layers from top to bottom:");

		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			print_layer_recursive(layer);
		}
		MINT_LOG_SEPARATOR();
	}


	mint::Vector< CLayer* >& CLayerStack::get_all_layers()
	{
		return m_layers;
	}


	void CLayerStack::print_layer_recursive(ILayer* layer, mint::u32 depth /*= 0*/)
	{
		auto name = layer->get_layer_name();

		mint::String text;

		for (auto i = 0; i < depth; i++) text += "\t";

		text += "\"" + name + "\"";

		MINT_LOG_INFO("{}", text);

		if (layer->has_children_layers())
		{
			for (const auto& kid : layer->get_children_layers())
			{
				print_layer_recursive(kid, depth + 1);
			}
		}
	}


}
#endif