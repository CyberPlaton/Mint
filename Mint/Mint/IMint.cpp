#include "IMint.h"


namespace mint
{
	mint::IMintEngine* IMintEngine::s_engine = nullptr;


	mint::IMintEngine* IMintEngine::get_engine()
	{
		return s_engine;
	}


	void IMintEngine::initialize_cuca_critical_sections()
	{
		INITIALIZE_CRITICAL_SECTION(CUCA::m_entityCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		INITIALIZE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	}


	void IMintEngine::delete_cuca_critical_sections()
	{
		DELETE_CRITICAL_SECTION(CUCA::m_entityCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_identifierCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_hierarchyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_transformCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_rigidBodyCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_spriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_animatedSpriteCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_scriptCriticalSection);
		DELETE_CRITICAL_SECTION(CUCA::m_dynamicGameobjectCriticalSection);
	}


	void IMintEngine::register_component_importer_functions()
	{
		IScene::register_component_importer< mint::component::SIdentifier >(&mint::component::SIdentifier::import_component);
		IScene::register_component_importer< mint::component::SSceneHierarchy >(&mint::component::SSceneHierarchy::import_component);
		IScene::register_component_importer< mint::component::SRigidBody >(&mint::component::SRigidBody::import_component);
		IScene::register_component_importer< mint::component::STransform >(&mint::component::STransform::import_component);
		IScene::register_component_importer< mint::component::SSprite >(&mint::component::SSprite::import_component);
		IScene::register_component_importer< mint::component::SAnimatedSprite >(&mint::component::SAnimatedSprite::import_component);
		IScene::register_component_importer< mint::component::SScript >(&mint::component::SScript::import_component);
	}


	void IMintEngine::register_component_exporter_functions()
	{
		IScene::register_component_exporter< mint::component::SIdentifier >(&mint::component::SIdentifier::export_component);
		IScene::register_component_exporter< mint::component::SSceneHierarchy >(&mint::component::SSceneHierarchy::export_component);
		IScene::register_component_exporter< mint::component::SRigidBody >(&mint::component::SRigidBody::export_component);
		IScene::register_component_exporter< mint::component::STransform >(&mint::component::STransform::export_component);
		IScene::register_component_exporter< mint::component::SSprite >(&mint::component::SSprite::export_component);
		IScene::register_component_exporter< mint::component::SAnimatedSprite >(&mint::component::SAnimatedSprite::export_component);
		IScene::register_component_exporter< mint::component::SScript >(&mint::component::SScript::export_component);
	}


}