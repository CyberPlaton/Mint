#include "Asset.h"

namespace mint
{


	mint::String CAsset::get_asset_extension()
	{
		return read_string("extension");
	}


	void CAsset::set_asset_extension(const String& value)
	{
		write_string("extension", value);
	}


	mint::String CAsset::get_ressource_type()
	{
		return read_string("type");
	}


	void CAsset::set_ressource_type(const String& value)
	{
		write_string("type", value);
	}


	mint::String CAsset::get_asset_name()
	{
		return read_string("name");
	}


	void CAsset::set_asset_name(const String& value)
	{
		write_string("name", value);
	}


	mint::u64 CAsset::get_asset_id()
	{
		return read_uint("id");
	}


	void CAsset::set_asset_id(u64 value)
	{
		write_uint("id", value);
	}


	mint::String CAsset::get_asset_path()
	{
		return read_string("path");
	}


	void CAsset::set_asset_path(const String& value)
	{
		write_string("path", value);
	}


	mint::String CAsset::get_asset_source_path()
	{
		return read_string("source");
	}


	void CAsset::set_asset_source_path(const String& value)
	{
		write_string("source", value);
	}


	mint::String CAsset::get_asset_description()
	{
		return read_string("description");
	}


	void CAsset::set_asset_description(const String& value)
	{
		write_string("description", value);
	}


	mint::String CAsset::get_asset_author()
	{
		return read_string("author");
	}


	void CAsset::set_asset_author(const String& value)
	{
		write_string("author", value);
	}


	mint::u64 CAsset::get_asset_version()
	{
		return read_uint("version");
	}


	void CAsset::set_asset_version(u64 value)
	{
		write_uint("version", value);
	}


	void CAsset::write_string(const String& entry_name, const String& data)
	{
		_write_any< const String >(entry_name, data);
	}


	void CAsset::write_float(const String& entry_name, f32 data)
	{
		_write_any< f32 >(entry_name, data);
	}


	void CAsset::write_uint(const String& entry_name, u64 data)
	{
		_write_any< u64 >(entry_name, data);
	}


	void CAsset::write_sint(const String& entry_name, s64 data)
	{
		_write_any< s64 >(entry_name, data);
	}


	void CAsset::write_bool(const String& entry_name, bool data)
	{
		_write_any< bool >(entry_name, data);
	}


	void CAsset::write_vec2(const String& entry_name, Vec2 data)
	{
		_write_any< Vec2 >(entry_name, data);
	}


	void CAsset::write_vec3(const String& entry_name, Vec3 data)
	{
		_write_any< Vec3 >(entry_name, data);
	}


	void CAsset::write_vec4(const String& entry_name, Vec4 data)
	{
		_write_any< Vec4 >(entry_name, data);
	}


	void CAsset::write_array(const String& entry_name, Vector< CAny > data)
	{
		_write_any< Vector< CAny > >(entry_name, data);
	}


	mint::String CAsset::read_string(const String& entry_name)
	{
		return _read_any< String >(entry_name);
	}


	mint::f32 CAsset::read_float(const String& entry_name)
	{
		return _read_any< f32 >(entry_name);
	}


	mint::u64 CAsset::read_uint(const String& entry_name)
	{
		return _read_any< u64 >(entry_name);
	}


	mint::s64 CAsset::read_sint(const String& entry_name)
	{
		return _read_any< s64 >(entry_name);
	}


	bool CAsset::read_bool(const String& entry_name)
	{
		return _read_any< bool >(entry_name);
	}


	mint::Vec2 CAsset::read_vec2(const String& entry_name)
	{
		return _read_any< Vec2 >(entry_name);
	}


	mint::Vec3 CAsset::read_vec3(const String& entry_name)
	{
		return _read_any< Vec3 >(entry_name);
	}


	mint::Vec4 CAsset::read_vec4(const String& entry_name)
	{
		return _read_any< Vec4 >(entry_name);
	}


	mint::Vector< mint::CAny > CAsset::read_array(const String& entry_name)
	{
		return _read_any< Vector< CAny > >(entry_name);
	}


}