#include "LayerStack.h"


namespace mint::editor
{


	bool CLayerStack::try_push_layer(CLayer* layer)
	{
		if(layer->on_initialize())
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
		while(!m_layers.empty())
		{
			pop_layer();
		}
	}


	void CLayerStack::on_ui_frame()
	{
		for(auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
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
		mint::u32 i = m_layers.size();

		MINT_LOG_INFO("[{:.4f}][CLayerStack::print_registered_layers] Dumping registered layers from top to bottom:", MINT_APP_TIME);

		for (auto it = m_layers.rbegin(); it != m_layers.rend(); it++)
		{
			auto layer = *it;

			auto name = layer->get_layer_name();

			MINT_LOG_INFO("\t#{} = \"{}\".", i, name);

			i--;
		}
	}


	mint::Vector< CLayer* >& CLayerStack::get_all_layers()
	{
		return m_layers;
	}


}