#include "Behavior.h"


namespace mint::scripting
{

	void CBehavior::on_update(f32 dt /*= 0.0f*/)
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_update");

		try
		{
			fn(m_entity, dt);
		}
		catch(const luabridge::LuaException& e)
		{
			if(!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CBehavior::on_update] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}


	void CBehavior::on_create()
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_create");

		try
		{
			fn(m_entity);
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CBehavior::on_create] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}


	void CBehavior::on_destroy()
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_destroy");

		try
		{
			fn(m_entity);
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CBehavior::on_destroy] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}

}