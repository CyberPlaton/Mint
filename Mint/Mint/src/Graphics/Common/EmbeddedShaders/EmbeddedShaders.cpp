#include "EmbeddedShaders.h"


#if BX_PLATFORM_WINDOWS or BX_PLATFORM_XBOXONE
#include "Shaders/windows/sprite/ps_sprite.dx11.h"
#include "Shaders/windows/sprite/ps_sprite.dx12.h"
#include "Shaders/windows/sprite/vs_sprite.dx11.h"
#include "Shaders/windows/sprite/vs_sprite.dx12.h"
#include "Shaders/windows/backbuffer/vs_backbuffer.dx11.h"
#include "Shaders/windows/backbuffer/vs_backbuffer.dx12.h"
#include "Shaders/windows/backbuffer/ps_backbuffer.dx11.h"
#include "Shaders/windows/backbuffer/ps_backbuffer.dx12.h"
#elif BX_PLATFORM_LINUX
#include "Shaders/linux/sprite/ps_sprite.spv.h"
#include "Shaders/linux/sprite/vs_sprite.spv.h"
#include "Shaders/backbuffer/backbuffer/vs_backbuffer.spv.h"
#include "Shaders/backbuffer/backbuffer/ps_backbuffer.spv.h"
#elif BX_PLATFORM_ANDROID
#include "Shaders/android/sprite/ps_sprite.spv.h"
#include "Shaders/android/sprite/vs_sprite.spv.h"
#include "Shaders/backbuffer/sprite/ps_backbuffer.spv.h"
#include "Shaders/backbuffer/sprite/vs_backbuffer.spv.h"
#elif BX_PLATFORM_IOS
#include "Shaders/ios/sprite/ps_sprite.metal.h"
#include "Shaders/ios/sprite/vs_sprite.metal.h"
#include "Shaders/ios/backbuffer/ps_backbuffer.metal.h"
#include "Shaders/ios/backbuffer/vs_backbuffer.metal.h"
#elif BX_PLATFORM_OSX
#include "Shaders/osx/sprite/ps_sprite.metal.h"
#include "Shaders/osx/sprite/vs_sprite.metal.h"
#include "Shaders/osx/backbuffer/ps_backbuffer.metal.h"
#include "Shaders/osx/backbuffer/vs_backbuffer.metal.h"
#elif BX_PLATFORM_PS4 or BX_PLATFORM_PS5
#endif


namespace mint::fx
{

	mint::CMap< mint::ShaderProgramHandle > CEmbeddedShaders::m_shaders;


	bool CEmbeddedShaders::initialize()
	{
		bool result = true;

		auto renderer_type = bgfx::getRendererType();

#if BX_PLATFORM_WINDOWS or BX_PLATFORM_XBOXONE or BX_PLATFORM_WINRT
		switch (renderer_type)
		{
		case bgfx::RendererType::Direct3D9:
			MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Unsupported rendering API \"{}\"!", MINT_APP_TIME, "Direct3D9");
			return false;
		case bgfx::RendererType::Direct3D11:
		{
			if (result &= create_embedded_shader("Sprite", bgfx::RendererType::Direct3D11, &vs_sprite_dx11[0], BX_COUNTOF(vs_sprite_dx11),
												 &ps_sprite_dx11[0], BX_COUNTOF(ps_sprite_dx11)); 
												 result == false)
			{
				MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Direct3D11 embedded shader \"{}\"!", MINT_APP_TIME, "Sprite");
			}
			if (result &= create_embedded_shader("Backbuffer", bgfx::RendererType::Direct3D11, &vs_backbuffer_dx11[0], BX_COUNTOF(vs_backbuffer_dx11),
												 &ps_backbuffer_dx11[0], BX_COUNTOF(ps_backbuffer_dx11)); 
												 result == false)
			{
				MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Direct3D11 embedded shader \"{}\"!", MINT_APP_TIME, "Backbuffer");
			}
			break;
		}
		case bgfx::RendererType::Direct3D12:
		{
			if (result &= create_embedded_shader("Sprite", bgfx::RendererType::Direct3D12, &vs_sprite_dx12[0], BX_COUNTOF(vs_sprite_dx12),
				&ps_sprite_dx12[0], BX_COUNTOF(ps_sprite_dx12));
				result == false)
			{
				MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Direct3D12 embedded shader \"{}\"!", MINT_APP_TIME, "Sprite");
			}
			if (result &= create_embedded_shader("Backbuffer", bgfx::RendererType::Direct3D12, &vs_backbuffer_dx12[0], BX_COUNTOF(vs_backbuffer_dx12),
				&ps_backbuffer_dx12[0], BX_COUNTOF(ps_backbuffer_dx12));
				result == false)
			{
				MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Direct3D12 embedded shader \"{}\"!", MINT_APP_TIME, "Backbuffer");
			}
			break;
		}
		default:
			MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Microsoft platforms support Direct3D11 and Direct3D12 only!", MINT_APP_TIME);
			return false;
		}



#elif BX_PLATFORM_LINUX or BX_PLATFORM_ANDROID
		if (result &= create_embedded_shader("Sprite", bgfx::RendererType::Vulkan, &vs_sprite_spv[0], BX_COUNTOF(vs_sprite_spv),
																				   &ps_sprite_spv[0], BX_COUNTOF(ps_sprite_spv)); result == false)
		{
			MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Vulkan embedded shader \"{}\"!", MINT_APP_TIME, "Sprite");
		}

#elif BX_PLATFORM_IOS or BX_PLATFORM_OSX
		if (result &= create_embedded_shader("Sprite", bgfx::RendererType::Metal, &vs_sprite_metal[0], BX_COUNTOF(vs_sprite_metal),
																				  &ps_sprite_metal[0], BX_COUNTOF(ps_sprite_metal)); result == false)
		{
			MINT_LOG_ERROR("[{:.4f}][CEmbeddedShaders::initialize] Failed creating Metal embedded shader \"{}\"!", MINT_APP_TIME, "Sprite");
		}


#elif BX_PLATFORM_PS4 or BX_PLATFORM_PS5
#elif BX_PLATFORM_NX
#endif

		return result;
	}


	void CEmbeddedShaders::terminate()
	{
	}


	bool CEmbeddedShaders::create_embedded_shader(const mint::String& name, bgfx::RendererType::Enum renderer, const u8* vs_array_pointer, u32 vs_array_size, const u8* ps_array_pointer, u32 ps_array_size)
	{
		mint::String vs_name("vs_"); vs_name += name;
		mint::String ps_name("ps_"); ps_name += name;


		bgfx::EmbeddedShader vs_shader;
		vs_shader.name = vs_name.c_str();

		bgfx::EmbeddedShader ps_shader;
		ps_shader.name = ps_name.c_str();


		vs_shader.data[renderer].type = renderer;
		vs_shader.data[renderer].size = vs_array_size;
		vs_shader.data[renderer].data = vs_array_pointer;

		ps_shader.data[renderer].type = renderer;
		ps_shader.data[renderer].size = ps_array_size;
		ps_shader.data[renderer].data = ps_array_pointer;


		auto vs_handle = bgfx::createEmbeddedShader(&vs_shader, renderer, vs_name.c_str());
		auto ps_handle = bgfx::createEmbeddedShader(&ps_shader, renderer, ps_name.c_str());


		auto program = bgfx::createProgram(vs_handle, ps_handle, true);


		if(bgfx::isValid(program))
		{
			m_shaders.add(mint::algorithm::djb_hash(name), program);

			return true;
		}

		return false;
	}


	mint::ShaderProgramHandle CEmbeddedShaders::get_embedded_shader(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);
		if(m_shaders.lookup(h))
		{
			return m_shaders.get(h);
		}

		return BGFX_INVALID_HANDLE;
	}


	bool CEmbeddedShaders::lookup_embedded_shader(const String& name)
	{
		auto h = mint::algorithm::djb_hash(name);

		return m_shaders.lookup(h);
	}


}