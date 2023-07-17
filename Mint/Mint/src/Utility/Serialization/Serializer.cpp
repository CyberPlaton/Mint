#include "Serializer.h"


namespace mint
{


	maml::SNode* CSerializer::load_maml_document(const String& maml_document_filepath, maml::CDocument& document)
	{
		maml::CLexer lexer;
		maml::CParser parser(lexer.scan_from_file(maml_document_filepath));
		if(parser.parse(document))
		{
			return document.get_root();
		}
		return nullptr;
	}


	bool CSerializer::import_bool(bool* value, const String& name, maml::SNode* node, bool default_value /*= false*/)
	{
		*value = maml::CDocument::get_bool_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_float(f32* value, const String& name, maml::SNode* node, f32 default_value /*= 0.0f*/)
	{
		*value = maml::CDocument::get_float_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_uint(u64* value, const String& name, maml::SNode* node, u64 default_value /*= 0*/)
	{
		*value = maml::CDocument::get_uint_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_sint(s64* value, const String& name, maml::SNode* node, s64 default_value /*= 0*/)
	{
		*value = maml::CDocument::get_sint_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_string(String& value, const String& name, maml::SNode* node, const String& default_value /*= ""*/)
	{
		value = maml::CDocument::get_string_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_vec2(Vec2& value, const String& name, maml::SNode* node, Vec2 default_value /*= { 0.0f, 0.0f }*/)
	{
		value = maml::CDocument::get_vector2_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_vec3(Vec3& value, const String& name, maml::SNode* node, Vec3 default_value /*= { 0.0f, 0.0f, 0.0f }*/)
	{
		value = maml::CDocument::get_vector3_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_vec4(Vec4& value, const String& name, maml::SNode* node, Vec4 default_value /*= {0.0f, 0.0f, 0.0f, 0.0f}*/)
	{
		value = maml::CDocument::get_vector4_property(node, name, default_value);

		return true;
	}


	bool CSerializer::import_rect(CRect& value, const String& name, maml::SNode* node, CRect default_value /*= {0.0f, 0.0f, 0.0f, 0.0f}*/)
	{
		auto vec = maml::CDocument::get_vector4_property(node, name, default_value.get_as_vec4());

		value.set_rectangle(vec.x, vec.y, vec.z, vec.w);

		return true;
	}


	bool CSerializer::import_array(mint::Vector< mint::CAny >& value, const String& name, maml::SNode* node, mint::Vector< mint::CAny > default_value /*= Vector< CAny >{}*/)
	{
		value = maml::CDocument::get_array_property(node, name, default_value);

		return true;
	}


	bool CSerializer::export_bool(bool value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_float(f32 value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_uint(u64 value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_sint(s64 value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_string(const String& value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_vec2(const Vec2& value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_vec3(const Vec3& value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_vec4(const Vec4& value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


	bool CSerializer::export_rect(const CRect& value, const String& name, maml::SNode* node)
	{
		Vec4& _value = value.get_as_vec4();

		maml::CDocument::add_property_to_node(node, name, _value);

		return true;
	}


	bool CSerializer::export_array(const std::vector< CAny >& value, const String& name, maml::SNode* node)
	{
		maml::CDocument::add_property_to_node(node, name, value);

		return true;
	}


}