#include "CommonLuaBinding.h"

namespace mint::luaglue
{


	namespace ubound
	{

	}

	namespace bound
	{
		////////////////////////////////////////////////////////////////////////////////////
		// Common Helpers
		////////////////////////////////////////////////////////////////////////////////////
		raylib::Vector2 Vector2DivideV(const raylib::Vector2& u, const raylib::Vector2& v)
		{
			return u.Divide(v);
		}


		raylib::Vector2 Vector2MultiplyV(const raylib::Vector2& u, const raylib::Vector2& v)
		{
			return u.Multiply(v);
		}


		raylib::Vector2 Vector2AddV(const raylib::Vector2& u, const raylib::Vector2& v)
		{
			return u.Add(v);
		}


		raylib::Vector2 Vector2SubtractV(const raylib::Vector2& u, const raylib::Vector2& v)
		{
			return u.Subtract(v);
		}


		raylib::Vector2 Vector2DivideN(const raylib::Vector2& u, f32 v)
		{
			return u.Divide(v);
		}


		raylib::Vector2 Vector2MultiplyN(const raylib::Vector2& u, f32 v)
		{
			return u.Multiply({v, v});
		}


		raylib::Vector2 Vector2AddN(const raylib::Vector2& u, f32 v)
		{
			return u.Add({v, v});
		}


		raylib::Vector2 Vector2SubtractN(const raylib::Vector2& u, f32 v)
		{
			return u.Subtract({v, v});
		}


		////////////////////////////////////////////////////////////////////////////////////
		// Registration
		////////////////////////////////////////////////////////////////////////////////////

		bool CommonDependencies(lua_State* state)
		{
			luabridge::getGlobalNamespace(state)

				////////////////////////////////////////////////////////////////////////////
				// Defining underlying raylib types
				////////////////////////////////////////////////////////////////////////////

				.beginClass< Vector2 >("__Vector2__")
				.addConstructor< void(*)() >()
				.addData("x", &Vector2::x, true)
				.addData("y", &Vector2::y, true)
				.endClass()

				.beginClass< Color >("__Color__")
				.addConstructor<void(*)()>()
				.addData("r", &Color::r, true)
				.addData("g", &Color::g, true)
				.addData("b", &Color::b, true)
				.addData("a", &Color::a, true)
				.endClass()

				.beginClass< Vector4 >("__Vector4__")
				.addConstructor< void(*)() >()
				.addData("x", &Vector4::x, true)
				.addData("y", &Vector4::y, true)
				.addData("z", &Vector4::z, true)
				.addData("w", &Vector4::w, true)
				.endClass()

				.beginClass< Rectangle >("__Rectangle__")
				.addConstructor< void(*)() >()
				.addData("x", &Rectangle::x, true)
				.addData("y", &Rectangle::y, true)
				.addData("width", &Rectangle::width, true)
				.addData("height", &Rectangle::height, true)
				.endClass()

				////////////////////////////////////////////////////////////////////////////
				// Defining derived raylib cpp types used in the engine
				////////////////////////////////////////////////////////////////////////////

				.deriveClass< raylib::Vector2, Vector2 >("Vec2")
				.addConstructor< void(*)() >()
				.addConstructor< void(*)(f32, f32) >()
				.addProperty("X", &raylib::Vector2::GetX, &raylib::Vector2::SetX)
				.addProperty("Y", &raylib::Vector2::GetY, &raylib::Vector2::SetY)
				.endClass()

				.deriveClass< raylib::Color, Color >("Color")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(u32, u32, u32, u32)>()
				.addProperty("R", &raylib::Color::GetR, &raylib::Color::SetR)
				.addProperty("G", &raylib::Color::GetG, &raylib::Color::SetG)
				.addProperty("B", &raylib::Color::GetB, &raylib::Color::SetB)
				.addProperty("A", &raylib::Color::GetA, &raylib::Color::SetA)
				.endClass()

				.deriveClass< raylib::Vector4, Vector4 >("Vec4")
				.addConstructor< void(*)() >()
				.addConstructor< void(*)(f32, f32, f32, f32) >()
				.addConstructor< void(*)(f32, f32, f32) >()
				.addConstructor< void(*)(Color) >()
				.addProperty("X", &raylib::Vector4::GetX, &raylib::Vector4::SetX)
				.addProperty("Y", &raylib::Vector4::GetY, &raylib::Vector4::SetY)
				.addProperty("Z", &raylib::Vector4::GetZ, &raylib::Vector4::SetZ)
				.addProperty("W", &raylib::Vector4::GetW, &raylib::Vector4::SetW)
				.endClass()

				.deriveClass< raylib::Rectangle, Rectangle >("Rect")
				.addConstructor< void(*)() >()
				.addConstructor< void(*)(f32, f32, f32, f32) >()
				.addProperty("X", &raylib::Rectangle::GetX, &raylib::Rectangle::SetX)
				.addProperty("Y", &raylib::Rectangle::GetY, &raylib::Rectangle::SetY)
				.addProperty("Width", &raylib::Rectangle::GetWidth, &raylib::Rectangle::SetWidth)
				.addProperty("Height", &raylib::Rectangle::GetHeight, &raylib::Rectangle::SetHeight)
				.endClass()




				////////////////////////////////////////////////////////////////////////////
				// Registering helper functions
				////////////////////////////////////////////////////////////////////////////

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