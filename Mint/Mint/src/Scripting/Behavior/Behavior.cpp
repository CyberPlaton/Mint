#include "Behavior.h"

#include "Components/CUCA.h"

namespace mint::scripting
{

	void CBehavior::on_update(f32 dt /*= 0.0f*/)
	{
		MINT_DEBUG_BREAK(m_ready == false);

		MINT_ASSERT(m_ready == true, "Invalid operation. Invoking a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_update");

		try
		{
			fn(SCAST(entt::entity, m_entity), dt);
		}
		catch(const luabridge::LuaException& e)
		{
			if(!m_error)
			{
				MINT_LOG_ERROR("[{:.4f}][CBehavior::on_update] Script execution error:", MINT_APP_TIME);
				MINT_LOG_ERROR("\tScript: Name: \"{}\", Path: \"{}\"", m_scriptName, m_scriptPath);
				MINT_LOG_ERROR("\tScript Entity: Name: \"{}\", ID: \"{}\"", CUCA::identifier_get_debug_name(SCAST(entt::entity, m_entity)), m_entity);
				MINT_LOG_ERROR("\tError Message: \"{}\"", e.what());


				MINT_DEBUG_BREAK(true);
				m_error = true;
			}
		}
	}


	void CBehavior::on_create()
	{
		MINT_ASSERT(m_ready == true, "Invalid operation. Invoking a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_create");

		try
		{
			fn(SCAST(entt::entity, m_entity));
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_ERROR("[{:.4f}][CBehavior::on_create] Script execution error:", MINT_APP_TIME);
				MINT_LOG_ERROR("\tScript: Name: \"{}\", Path: \"{}\"", m_scriptName, m_scriptPath);
				MINT_LOG_ERROR("\tScript Entity: Name: \"{}\", ID: \"{}\"", CUCA::identifier_get_debug_name(SCAST(entt::entity, m_entity)), m_entity);
				MINT_LOG_ERROR("\tError Message: \"{}\"", e.what());


				MINT_DEBUG_BREAK(true);
				m_error = true;
			}
		}
	}


	void CBehavior::on_destroy()
	{
		MINT_ASSERT(m_ready == true, "Invalid operation. Invoking a Behavior that is not ready!");

		auto fn = luabridge::getGlobal(m_state, "on_destroy");

		try
		{
			fn(SCAST(entt::entity, m_entity));
		}
		catch (const luabridge::LuaException& e)
		{
			if (!m_error)
			{
				MINT_LOG_ERROR("[{:.4f}][CBehavior::on_destroy] Script execution error:", MINT_APP_TIME);
				MINT_LOG_ERROR("\tScript: Name: \"{}\", Path: \"{}\"", m_scriptName, m_scriptPath);
				MINT_LOG_ERROR("\tScript Entity: Name: \"{}\", ID: \"{}\"", CUCA::identifier_get_debug_name(SCAST(entt::entity, m_entity)), m_entity);
				MINT_LOG_ERROR("\tError Message: \"{}\"", e.what());
				
				MINT_DEBUG_BREAK(true);
				m_error = true;
			}
		}
	}


	CBehavior::~CBehavior()
	{
	}


}