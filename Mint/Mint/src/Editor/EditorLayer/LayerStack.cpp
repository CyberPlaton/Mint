#if MINT_DISTR
#else
#include "LayerStack.h"


namespace mint::editor
{


	bool CLayerStack::try_push_layer(CLayer* layer, bool as_root_layer /*= false*/)
	{
		if (layer->on_initialize())
		{
			mint::algorithm::vector_push_back(m_layers, layer);

			if (as_root_layer) m_rootLayer = layer;

			return true;
		}

		delete layer; layer = nullptr;
		return false;
	}


	void CLayerStack::pop_layer()
	{
		MINT_ASSERT(m_layers.empty() == false, "Invalid operation. Popping an empty stack is not allowed!");

		auto top = m_layers[m_layers.size() - 1];

		m_layers.erase(m_layers.begin() + m_layers.size() - 1);

		top->on_terminate();

		delete top; top = nullptr;
	}


	void CLayerStack::clear_all_layers()
	{
		m_rootLayer->on_terminate();

		delete m_rootLayer; m_rootLayer = nullptr;
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

		print_layer_recursive(m_rootLayer);

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