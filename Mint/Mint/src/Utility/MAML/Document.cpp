#include "Document.h"


namespace maml
{


	CDocument::CDocument(mint::u64 expected_node_count /*= MAML_DOCUMENT_SIZE_TINY*/) : 
		m_root(nullptr)
	{
		if(!m_nodes.initialize(expected_node_count, 0))
		{
			MINT_LOG_ERROR("[{:.4f}][CDocument::CDocument] Unable to allocate MAML Document!", MINT_APP_TIME);
		}

		m_root = create_node("Root");
	}


	bool CDocument::is_empty()
	{
		return m_nodes.advance(get_root()) != nullptr;
	}


	maml::SNode* CDocument::get_root()
	{
		return m_root;
	}


	bool CDocument::save_document(const mint::String& file_path)
	{
		return true;
	}


	maml::SNode* CDocument::begin()
	{
		return m_nodes.begin();
	}


	maml::SNode* CDocument::advance(SNode* node)
	{
		return m_nodes.advance(node);
	}


	maml::SNode* CDocument::find_first_match_in_document(const mint::String& node_name)
	{
		auto h = mint::algorithm::djb_hash(node_name);

		auto root = get_root();
		auto node = advance(root);

		while(node)
		{
			if (node->m_uuid == h) return node;

			node = advance(node);
		}

		return nullptr;
	}


	maml::SNode* CDocument::find_first_match_in_node(SNode* node, const mint::String& node_name)
	{
		auto h = mint::algorithm::djb_hash(node_name);

		if (node)
		{
			for (auto& kid : get_node_children(node))
			{
				if (kid->m_uuid == h) return kid;
			}
		}

		return nullptr;
	}


	mint::Vector< SNode* > CDocument::get_node_children(SNode* node)
	{
		return node->m_children;
	}


	maml::SNode* CDocument::create_node(const mint::String& name, SNode* parent /*= nullptr*/)
	{
		auto node = m_nodes.create();

		if(node)
		{
			node->m_document = this;
			node->m_name = name;
			node->m_parent = parent;
			node->m_uuid = mint::algorithm::djb_hash(name);
			if(parent)
			{
				parent->m_children.push_back(node);
			}

			return node;
		}

		return nullptr;
	}


	mint::String CDocument::get_node_name(SNode* node)
	{
		return node->m_name;
	}


	void CDocument::add_property_to_node(SNode* node, SProperty& property)
	{
		auto h = mint::algorithm::djb_hash(property.m_name);

		node->m_properties.add(h, property);
	}


	mint::f32 CDocument::get_float_property(SNode* node, const mint::String& property_name, mint::f32 default_value /*= 0.0f*/)
	{
		return _get_generic_property< mint::f32 >(node, property_name, default_value);
	}


	mint::u64 CDocument::get_uint_property(SNode* node, const mint::String& property_name, mint::u64 default_value /*= 0*/)
	{
		return _get_generic_property< mint::u64 >(node, property_name, default_value);
	}


	mint::s64 CDocument::get_sint_property(SNode* node, const mint::String& property_name, mint::u32 default_value /*= 0*/)
	{
		return _get_generic_property< mint::s64 >(node, property_name, default_value);
	}


	mint::String CDocument::get_string_property(SNode* node, const mint::String& property_name, mint::String default_value /*= ""*/)
	{
		return _get_generic_property< mint::String >(node, property_name, default_value);
	}


	bool CDocument::get_bool_property(SNode* node, const mint::String& property_name, bool default_value /*= false*/)
	{
		return _get_generic_property< bool >(node, property_name, default_value);
	}


	mint::Vec2 CDocument::get_vector2_property(SNode* node, const mint::String& property_name, mint::Vec2 default_value /*= {0.0f, 0.0f}*/)
	{
		return _get_generic_property< mint::Vec2 >(node, property_name, default_value);
	}


	mint::Vec3 CDocument::get_vector3_property(SNode* node, const mint::String& property_name, mint::Vec3 default_value /*= { 0.0f, 0.0f, 0.0f }*/)
	{
		return _get_generic_property< mint::Vec3 >(node, property_name, default_value);
	}


	mint::Vec4 CDocument::get_vector4_property(SNode* node, const mint::String& property_name, mint::Vec4 default_value /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/)
	{
		return _get_generic_property< mint::Vec4 >(node, property_name, default_value);
	}


	mint::Vector< mint::CAny > CDocument::get_array_property(SNode* node, const mint::String& property_name, mint::Vector< mint::CAny > default_value /*= Vector< mint::CAny >{}*/)
	{
		return _get_generic_property< mint::Vector< mint::CAny > >(node, property_name, default_value);
	}


}