#include "EventSystem.h"


namespace mint
{


	bool CEventSystem::initialize()
	{
		INITIALIZE_CRITICAL_SECTION(m_criticalSection);

		return true;
	}


	void CEventSystem::terminate()
	{
		reset();

		DELETE_CRITICAL_SECTION(m_criticalSection);
	}


	void CEventSystem::reset()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			while (!m_eventQueue.empty())
			{
				auto event = m_eventQueue.front();

				m_eventQueue.pop();

				delete event;
				event = nullptr;
			}


			for(auto& pair: m_listeners)
			{
				while(!pair.second.empty())
				{
					auto delegate = pair.second.end();

					pair.second.pop_back();

					delete *delegate;
					*delegate = nullptr;
				}
			}

			m_listeners.clear();

		);
	}


	void CEventSystem::update()
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			while (!m_eventQueue.empty())
			{
				auto event = m_eventQueue.front();

				auto event_type = event->get_event_type();

				for (auto& pair : m_listeners)
				{
					if (event_type == pair.first)
					{
						for (auto& delegate : pair.second)
						{
							delegate->execute(event);
						}
					}
				}

				m_eventQueue.pop();

				delete event; event = nullptr;
			}
		);
	}


	void CEventSystem::add_listener(const String& listened_event_type, SDelegate* delegate)
	{
		auto h = mint::algorithm::djb_hash(listened_event_type);

		bool listener_added = false;

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			for(auto& pair: m_listeners)
			{
				if(h == pair.first)
				{
					pair.second.push_back(delegate);

					listener_added = true;

					break;
				}
			}
		);


		if(!listener_added)
		{
			MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

				auto index = m_listeners.size();

				m_listeners.emplace_back(std::make_pair(h, Vector< SDelegate* >{}));

				m_listeners[index].second.push_back(delegate);

			);
		}
	}


	void CEventSystem::remove_listener(const String& listened_event_type, u64 delegate_identifier)
	{
		auto h = mint::algorithm::djb_hash(listened_event_type);

		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			for (auto& pair : m_listeners)
			{
				if (h == pair.first)
				{
					for(auto i = 0; i < pair.second.size(); i++)
					{
						auto& delegate = pair.second[i];

						if(delegate->get_unique_identifier() == delegate_identifier)
						{
							pair.second.erase(pair.second.begin() + i);

							delete delegate;
							delegate = nullptr;

							break;
						}
					}
				}
			}
		);
	}


	void CEventSystem::queue_event(SEvent* event)
	{
		MINT_BEGIN_CRITICAL_SECTION(m_criticalSection,

			m_eventQueue.push(event);

		);
	}

}