#include "CommonLuaBinding.h"

namespace mint::luaglue
{


	namespace script
	{

	}

	namespace behavior
	{
		Vec2 Vector2DivideV(const Vec2& u, const Vec2& v)
		{
			return u / v;
		}


		Vec2 Vector2MultiplyV(const Vec2& u, const Vec2& v)
		{
			return u * v;
		}


		Vec2 Vector2AddV(const Vec2& u, const Vec2& v)
		{
			return u + v;
		}


		Vec2 Vector2SubtractV(const Vec2& u, const Vec2& v)
		{
			return u - v;
		}


		Vec2 Vector2DivideN(const Vec2& u, f32 v)
		{
			return u / v;
		}


		Vec2 Vector2MultiplyN(const Vec2& u, f32 v)
		{
			return u * v;
		}


		Vec2 Vector2AddN(const Vec2& u, f32 v)
		{
			return u + v;
		}


		Vec2 Vector2SubtractN(const Vec2& u, f32 v)
		{
			return u - v;
		}


		bool dependencies(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)

				.beginClass< Vec2 >("__Vector2__")
				.addConstructor< void(*)() >()
				.addData("x", &Vec2::x, true)
				.addData("y", &Vec2::y, true)
				.endClass()

				.beginClass< Color >("__Color__")
				.addConstructor<void(*)()>()
				.addData("r", &Color::r, true)
				.addData("g", &Color::g, true)
				.addData("b", &Color::b, true)
				.addData("a", &Color::a, true)
				.endClass()

				.beginClass< Vec4 >("__Vector4__")
				.addConstructor< void(*)() >()
				.addData("x", &Vec4::x, true)
				.addData("y", &Vec4::y, true)
				.addData("z", &Vec4::z, true)
				.addData("w", &Vec4::w, true)
				.endClass()

				.beginClass< Rectangle >("__Rectangle__")
				.addConstructor< void(*)() >()
				.addData("x", &Rectangle::x, true)
				.addData("y", &Rectangle::y, true)
				.addData("width", &Rectangle::width, true)
				.addData("height", &Rectangle::height, true)
				.endClass()

				.beginNamespace("Vec2")
				.addFunction("DivV", Vector2DivideV)
				.addFunction("MulV", Vector2MultiplyV)
				.addFunction("AddV", Vector2AddV)
				.addFunction("SubV", Vector2SubtractV)

				.addFunction("DivN", Vector2DivideN)
				.addFunction("MulN", Vector2MultiplyN)
				.addFunction("AddN", Vector2AddN)
				.addFunction("SubN", Vector2SubtractN)
				.endNamespace();


			return true;
		}

	}
}