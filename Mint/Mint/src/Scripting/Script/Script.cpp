#include "Script.h"


namespace mint::scripting
{


	void CScript::on_update(f32 dt /*= 0.0f*/)
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Script that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_update");

		try
		{
			fn(dt);
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CScript::on_update] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}


	void CScript::on_create()
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Script that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_create");

		try
		{
			fn();
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CScript::on_create] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}


	void CScript::on_destroy()
	{
		MINT_ASSERT(m_ready == true, "Trying to invoke a Script that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_destroy");

		try
		{
			fn();
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_WARN("[{:.4f}][CScript::on_destroy] Script: \"{}\". Message: \"{}\"", MINT_APP_TIME, m_scriptName, e.what());
				m_error = true;
			}
		}
	}


}
