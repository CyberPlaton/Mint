#ifndef _MAML_DOCUMENT_H_
#define _MAML_DOCUMENT_H_


#include "Common/Algorithm.h"
#include "../Logging/Logging.h"

#include "Node.h"
#include "Common/NodeAllocator.h"


#define MAML_DOCUMENT_SIZE_TINY 8 + 1
#define MAML_DOCUMENT_SIZE_SMALL 32 + 1
#define MAML_DOCUMENT_SIZE_DEFAULT 128 + 1
#define MAML_DOCUMENT_SIZE_BIG 1024 + 1
#define MAML_DOCUMENT_SIZE_HUGE 4096 + 1


namespace maml
{
	class CDocument
	{
	public:
		CDocument(mint::u64 expected_node_count = MAML_DOCUMENT_SIZE_TINY);

		

		bool is_empty();

		SNode* get_root();

		bool save_document(const mint::String& file_path);



		SNode* begin();

		SNode* advance(SNode* node);




		SNode* find_first_match_in_document(const mint::String& node_name);
		
		static SNode* find_first_match_in_node(SNode* node, const mint::String& node_name);



		static mint::Vector< SNode* > get_node_children(SNode* node);

		SNode* create_node(const mint::String& name, SNode* parent = nullptr);



		static mint::String get_node_name(SNode* node);

		static void add_property_to_node(SNode* node, SProperty& property);

		template < typename T >
		static void add_property_to_node(SNode* node, const mint::String& name, T& data);

		template < typename T >
		static void add_property_to_node(SNode* node, const mint::String& name, T&& data);


		template< typename T >
		static bool is_property_of_type(SNode* node, const mint::String& property_name);

		static mint::f32 get_float_property(SNode* node, const mint::String& property_name, mint::f32 default_value = 0.0f);

		static mint::u64 get_uint_property(SNode* node, const mint::String& property_name, mint::u64 default_value = 0);

		static mint::s64 get_sint_property(SNode* node, const mint::String& property_name, mint::u32 default_value = 0);

		static mint::String get_string_property(SNode* node, const mint::String& property_name, mint::String default_value = "");

		static bool get_bool_property(SNode* node, const mint::String& property_name, bool default_value = false);

		static mint::Vec2 get_vector2_property(SNode* node, const mint::String& property_name, mint::Vec2 default_value = {0.0f, 0.0f});

		static mint::Vec3 get_vector3_property(SNode* node, const mint::String& property_name, mint::Vec3 default_value = { 0.0f, 0.0f, 0.0f });

		static mint::Vec4 get_vector4_property(SNode* node, const mint::String& property_name, mint::Vec4 default_value = { 0.0f, 0.0f, 0.0f, 0.0f });

		static mint::Vector< mint::CAny > get_array_property(SNode* node, const mint::String& property_name, mint::Vector< mint::CAny > default_value = mint::Vector< mint::CAny >{});


	private:
		CNodeAllocator< SNode > m_nodes;

		SNode* m_root;


	private:

		template < typename T >
		static T _get_generic_property(SNode* node, const mint::String& property_name, T default_value);
	};


	template < typename T >
	T maml::CDocument::_get_generic_property(SNode* node, const mint::String& property_name, T default_value)
	{
		auto h = mint::algorithm::djb_hash(property_name);

		if (node->m_properties.lookup(h))
		{
			auto& data = node->m_properties.get(h);

			if (data.m_data.is< T >())
			{
				return data.m_data.cast< T >();
			}
		}

		return default_value;
	}


	template< typename T >
	bool maml::CDocument::is_property_of_type(SNode* node, const mint::String& property_name)
	{
		auto h = mint::algorithm::djb_hash(name);

		if(node->m_properties.lookup(h))
		{
			return node->m_properties.get(h).m_data.is< T >();
		}

		return false;
	}


	template < typename T >
	void maml::CDocument::add_property_to_node(SNode* node, const mint::String& name, T&& data)
	{
		auto h = mint::algorithm::djb_hash(name);

		node->m_properties.add(h, data);
	}


	template < typename T >
	void maml::CDocument::add_property_to_node(SNode* node, const mint::String& name, T& data)
	{
		auto h = mint::algorithm::djb_hash(name);

		node->m_properties.add(h, data);
	}


}


#endif