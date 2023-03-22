#ifndef _MINT_ASSET_H_
#define _MINT_ASSET_H_


#include "Common/Any.h"
#include "Common/Algorithm.h"
#include "Utility/STL/Map.h"


namespace mint
{
	class CAsset
	{
	public:

		String get_asset_extension();
		void set_asset_extension(const String& value);

		String get_ressource_type();
		void set_ressource_type(const String& value);

		String get_asset_name();
		void set_asset_name(const String& value);

		u64 get_asset_id();
		void set_asset_id(u64 value);

		String get_asset_path();
		void set_asset_path(const String& value);

		String get_asset_source_path();
		void set_asset_source_path(const String& value);

		String get_asset_description();
		void set_asset_description(const String& value);

		String get_asset_author();
		void set_asset_author(const String& value);

		u64 get_asset_version();
		void set_asset_version(u64 value);


		void write_string(const String& entry_name, const String& data);
		void write_float(const String& entry_name, f32 data);
		void write_uint(const String& entry_name, u64 data);
		void write_sint(const String& entry_name, s64 data);
		void write_bool(const String& entry_name, bool data);
		void write_vec2(const String& entry_name, Vec2 data);
		void write_vec3(const String& entry_name, Vec3 data);
		void write_vec4(const String& entry_name, Vec4 data);
		void write_array(const String& entry_name, Vector< CAny > data);


		String read_string(const String& entry_name);
		f32 read_float(const String& entry_name);
		u64 read_uint(const String& entry_name);
		s64 read_sint(const String& entry_name);
		bool read_bool(const String& entry_name);
		Vec2 read_vec2(const String& entry_name);
		Vec3 read_vec3(const String& entry_name);
		Vec4 read_vec4(const String& entry_name);
		Vector< CAny > read_array(const String& entry_name);


	private:
		CMap< CAny > m_entries;


	private:
		template< typename T >
		void _write_any(const String& entry_name, T& data);

		template< typename T >
		T _read_any(const String& entry_name);
	};


	template< typename T >
	T mint::CAsset::_read_any(const String& entry_name)
	{
		auto h = mint::algorithm::djb_hash(entry_name);

		return m_entries.get(h).cast< T >();
	}


	template< typename T >
	void mint::CAsset::_write_any(const String& entry_name, T& data)
	{
		auto h = mint::algorithm::djb_hash(entry_name);

		m_entries.add(h, CAny(data));
	}


}


#endif