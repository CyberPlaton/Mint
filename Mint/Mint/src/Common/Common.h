#ifndef _MINT_COMMON_H_
#define _MINT_COMMON_H_
#pragma once

#pragma warning(disable : 4005) // Disable "macro-redefinition" warning.
#pragma warning(disable : 4002) // Disable "too many args for function-like macro" warning.

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




// Forward Define Declaration
#ifndef MINT_PLATFORM_WINDOWS
#define MINT_PLATFORM_WINDOWS 0
#endif

#ifndef MINT_PLATFORM_LINUX
#define MINT_PLATFORM_LINUX 0
#endif

#ifndef MINTFX_USE_EMBEDDED_SHADERS
#define MINTFX_USE_EMBEDDED_SHADERS 0
#endif

#ifndef MINT_USE_WORLD_QUERY_DATABASE
#define MINT_USE_WORLD_QUERY_DATABASE 0
#endif

#ifndef MINTFX_OPENGL_330
#define MINTFX_OPENGL_330 0
#endif

#ifndef MINTFX_OPENGL_ES20
#define MINTFX_OPENGL_ES20 0
#endif

#define MINT_SAS_RENDERING_LAYERS_MAX 100
#define MINT_SAS_OUT_QUEUE_COUNT_MAX 3
#define MINTFX_MATERIAL_COUNT_MAX 256
#define MINTFX_PARTICLE_EMITTER_COUNT_MAX 256
#define MINTFX_PARTICLE_COUNT_PER_EMITTER_MAX 1024
#define MINT_ANIMATORS_COUNT_MAX 256
#define MINT_ANIMATOR_FRAME_NUMBER u8
#define MINT_QUERIES_COUNT_MAX 1024
#define MINT_QUERIES_FILTERS_COUNT_MAX 32
#define MINT_ENTITY_COUNT_MAX 65535
#define MINTSOUND_INCOMING_EVENT_COUNT_MAX 256


// Platform Detection
#include "platform.h"

#if BX_PLATFORM_WINDOWS
#define MINT_PLATFORM_WINDOWS BX_PLATFORM_WINDOWS
#define MINTFX_OPENGL_330 1
#elif BX_PLATFORM_LINUX
#define MINT_PLATFORM_LINUX 1
#define MINTFX_OPENGL_ES20 1
#endif


// Common Includes
#include <thread>
#include <vector>
#include <string>
#include <fstream>

#include "raylib/raylib.h"
#include "raylib/rlgl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/type_trait.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "entt/entt.hpp"
#include "Physics/Common/box2d/box2d.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"


// Common globals

// Common defines
namespace mint
{
	using SoundHandle = u64;
	using EntityHandle = u64;
	using ShaderHandle = u64;
	using TextureHandle = u64;
	using MaterialHandle = u64;
	using ScriptHandle = u64;
	using DelegateHandle = u64;
	using String = std::string;
	
	static inline bool is_handle_valid(u64 handle) { return handle != static_cast< u64 >(-1); }
	static inline EntityHandle entity_get_handle(entt::entity entity) { return static_cast<u64>(entity); }
	static inline u64 invalid_handle() { return static_cast<u64>(-1); }

	template< typename T >
	using Vector = std::vector< T >;

	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4x4;
	using Mat3 = glm::mat3x3;
}


// Common macros
#define BIT(n) 1 << n

#define SCAST(type, value) static_cast< type >(value)

#define MINT_INVALID_HANDLE SCAST(u64, -1)


#define STATIC_GET(CLASS, STATIC_MEMBER) \
static CLASS& Get() \
{ static CLASS STATIC_MEMBER; return STATIC_MEMBER;} \

#define STRINGIFY(s) #s

#define STRING(s) STRINGIFY(s)

#define CONCAT2(a, b)						STRING(a ## b)
#define CONCAT3(a, b, c)					STRING(CONCAT2(a, b) ## c)
#define CONCAT4(a, b, c, d)					STRING(CONCAT3(a, b, c) ## d)
#define CONCAT5(a, b, c, d, e)				STRING(CONCAT4(a, b, c, d) ## e)
#define CONCAT6(a, b, c, d, e, f)			STRING(CONCAT5(a, b, c, d, e) ## f)
#define CONCAT7(a, b, c, d, e, f, g)		STRING(CONCAT6(a, b, c, d, e, f) ## g)
#define CONCAT8(a, b, c, d, e, f, g, h)		STRING(CONCAT7(a, b, c, d, e, f, g) ## h)

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
#define MINT_VERIFY(expression, message) \
assert((message, expression))


#if MINT_DEBUG
#define MINT_DEBUG_BREAK(condition) \
if((condition) == true) DebugBreak()
#else
#define MINT_DEBUG_BREAK(condition)
#endif



#if MINT_PLATFORM_LINUX
#include <pthread.h>
#define MINT_CRITICAL_SECTION(pSection) pthread_mutex_t pSection
#define INITIALIZE_CRITICAL_SECTION(pSection) pSection = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#define DELETE_CRITICAL_SECTION(pSection) pthread_mutex_destroy(&pSection)
#define ENTER_CRITICAL_SECTION(pSection) pthread_mutex_lock(&pSection)
#define LEAVE_CRITICAL_SECTION(pSection) pthread_mutex_unlock(&pSection)

#elif MINT_PLATFORM_WINDOWS
#define NOGDI
#define NORASTEROPS
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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