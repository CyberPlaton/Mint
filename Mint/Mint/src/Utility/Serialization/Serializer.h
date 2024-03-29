#ifndef _MINT_SERIALIZER_H_
#define _MINT_SERIALIZER_H_


#include "Utility/MAML/MAML.h"
#include "Common/Rectangle.h"


namespace mint
{
	class CSerializer
	{
	public:

		static maml::SNode* load_maml_document(const String& maml_document_filepath, maml::CDocument& document);



		static bool import_bool(bool* value, const String& name, maml::SNode* node, bool default_value = false);

		static bool import_float(f32* value, const String& name, maml::SNode* node, f32 default_value = 0.0f);

		static bool import_uint(u64* value, const String& name, maml::SNode* node, u64 default_value = 0);

		static bool import_sint(s64* value, const String& name, maml::SNode* node, s64 default_value = 0);

		static bool import_string(String& value, const String& name, maml::SNode* node, const String& default_value = "");

		static bool import_vec2(Vec2& value, const String& name, maml::SNode* node, Vec2 default_value = { 0.0f, 0.0f });

		static bool import_vec3(Vec3& value, const String& name, maml::SNode* node, Vec3 default_value = { 0.0f, 0.0f, 0.0f });

		static bool import_vec4(Vec4& value, const String& name, maml::SNode* node, Vec4 default_value = {0.0f, 0.0f, 0.0f, 0.0f});

		static bool import_rect(CRect& value, const String& name, maml::SNode* node, CRect default_value = {0.0f, 0.0f, 0.0f, 0.0f});

		static bool import_array(Vector< CAny >& value, const String& name, maml::SNode* node, Vector< CAny > default_value = Vector< CAny >{});


		static bool export_bool(bool value, const String& name, maml::SNode* node);

		static bool export_float(f32 value, const String& name, maml::SNode* node);

		static bool export_uint(u64 value, const String& name, maml::SNode* node);

		static bool export_sint(s64 value, const String& name, maml::SNode* node);

		static bool export_string(const String& value, const String& name, maml::SNode* node);

		static bool export_vec2(const Vec2& value, const String& name, maml::SNode* node);

		static bool export_vec3(const Vec3& value, const String& name, maml::SNode* node);

		static bool export_vec4(const Vec4& value, const String& name, maml::SNode* node);

		static bool export_rect(const CRect& value, const String& name, maml::SNode* node);

		static bool export_array(const std::vector< CAny >& value, const String& name, maml::SNode* node);


	};
}


#endif