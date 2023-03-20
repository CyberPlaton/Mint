#ifndef _MINT_COMMON_H_
#define _MINT_COMMON_H_
#pragma once

// Common Includes
#include <thread>
#include <vector>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3/SDL_syswm.h"

#include "bx/bx.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bimg/bimg.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/type_trait.hpp"

#include "box2d/box2d.h"
#include "entt/entt.hpp"


// Platform Detection
#if BX_PLATFORM_WINDOWS
#define MINT_PLATFORM_WINDOWS
#elif BX_PLATFORM_LINUX
#define MINT_PLATFORM_LINUX
#endif



// Common primitive data types
namespace mint
{
	// Microsoft Compiler
#ifdef _MSC_VER
	typedef unsigned __int8 u8;		// Unsigned 8-Bit number
	typedef unsigned __int16 u16;	// .. 16-Bit ..
	typedef unsigned __int32 u32;	// .. 32-Bit ..
	typedef unsigned __int64 u64;	// .. 64-Bit ..
	typedef __int8 s8;				// Signed 8-Bit number
	typedef __int16 s16;			// .. 16-Bit ..
	typedef __int32 s32;			// .. 32-Bit ..
	typedef __int64 s64;			// .. 64-Bit ..
	typedef float f32;
	typedef double f64;

	typedef char c8;
	typedef c8 Byte;
#else
	typedef unsigned char u8;		// Unsigned 8-Bit number
	typedef unsigned short u16;		// .. 16-Bit ..
	typedef unsigned int u32;		// .. 32-Bit ..
	typedef unsigned long long u64;	// .. 64-Bit ..
	typedef signed char s8;			// Signed 8-Bit number
	typedef signed short s16;		// .. 16-Bit ..
	typedef signed int s32;			// .. 32-Bit ..
	typedef signed long long s64;	// .. 64-Bit ..
	typedef float f32;
	typedef double f64;

	typedef char c8;
	typedef c8 Byte;
#endif
}



// Common globals

// Common defines
namespace mint
{
	using ShaderHandle = bgfx::ProgramHandle;

	using TextureHandle = bgfx::TextureHandle;

	using ScriptHandle = u64;

	using Window = SDL_Window;

	using String = std::string;


	template< typename T >
	using Vector = std::vector< T >;

	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4x4;
}


// Common macros
#define MINT_SAS_RENDERING_LAYERS_MAX 100
#define MINT_SAS_OUT_QUEUE_COUNT_MAX 3

#define MINT_INVALID_HANDLE SCAST(u64, -1)

#define STATIC_GET(CLASS, STATIC_MEMBER) \
static CLASS& Get() \
{ static CLASS STATIC_MEMBER; return STATIC_MEMBER;} \

#define STRING(s) #s

#define TYPESTRING(s) typeid(s).name()

#define MINT_SCOPED_LOCK(mutex) \
std::scoped_lock msl(mutex) \

#ifndef MINT_DISTR
// Assert that the expression is True, if it is not True,
// then something went terribly wrong, that the message is indicating.
#define MINT_ASSERT(expression, message) \
assert((message, expression))
#else
#define MINT_ASSERT()
#endif

	// Static cast is preferred over c-style cast as it is more safer and can throw compiler errors.
#define SCAST(type, value) static_cast< type >(value)

#ifdef MINT_PLATFORM_LINUX
#include <pthread.h>
#define MINT_CRITICAL_SECTION(pSection) pthread_mutex_t pSection
#define INITIALIZE_CRITICAL_SECTION(pSection) pSection = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#define DELETE_CRITICAL_SECTION(pSection) pthread_mutex_destroy(&pSection)
#define ENTER_CRITICAL_SECTION(pSection) pthread_mutex_lock(&pSection)
#define LEAVE_CRITICAL_SECTION(pSection) pthread_mutex_unlock(&pSection)

#elif defined(MINT_PLATFORM_WINDOWS)
#define MINT_CRITICAL_SECTION(pSection) CRITICAL_SECTION pSection
#define INITIALIZE_CRITICAL_SECTION(pSection) InitializeCriticalSection(&pSection)
#define DELETE_CRITICAL_SECTION(pSection) DeleteCriticalSection(&pSection)
#define ENTER_CRITICAL_SECTION(pSection) EnterCriticalSection(&pSection)
#define LEAVE_CRITICAL_SECTION(pSection) LeaveCriticalSection(&pSection)
#endif

#define MINT_BEGIN_CRITICAL_SECTION(critical_section, function) \
ENTER_CRITICAL_SECTION(critical_section); \
function \
LEAVE_CRITICAL_SECTION(critical_section);


#endif