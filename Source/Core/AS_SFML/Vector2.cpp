#include "Shared.hpp"

#include <Core/Math.hpp>
#include <SFML/System/Vector2.hpp>

namespace
{
	void createVec(void* mem)
	{
		new (mem) sf::Vector2f();
	}
	void createVec(void* mem, float x, float y)
	{
		new (mem) sf::Vector2f(x, y);
	}
	void createVec(void* mem, const sf::Vector2f& pos)
	{
		new (mem) sf::Vector2f(pos);
	}
	void destructVec(sf::Vector2f* mem)
	{
		mem->~Vector2();
	}
}

bool as::priv::RegVec2(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Vec2", sizeof(sf::Vector2f), asGetTypeTraits<sf::Vector2f>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(createVec, (void*), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(float x, float y)", asFUNCTIONPR(createVec, (void*, float, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(const Vec2&in)", asFUNCTIONPR(createVec, (void*, const sf::Vector2f&), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructVec), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectProperty("Vec2", "float X", asOFFSET(sf::Vector2f, x)));
	AS_ASSERT(eng->RegisterObjectProperty("Vec2", "float Y", asOFFSET(sf::Vector2f, y)));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float get_Angle() const", asFUNCTIONPR(Math::PolarAngle, (const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "void set_Angle(float ang)", asFUNCTIONPR(Math::SetPolarAngle, (sf::Vector2f&, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float get_Length() const", asFUNCTIONPR(Math::Length, (const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "void set_Length(float len)", asFUNCTIONPR(Math::SetLength, (sf::Vector2f&, float), void), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float Dot(const Vec2&in) const", asFUNCTIONPR(Math::Dot, (const sf::Vector2f&, const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace(""));

	return true;
}
