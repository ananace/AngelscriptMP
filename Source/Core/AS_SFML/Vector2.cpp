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

namespace sf
{
	namespace
	{
	void opAdd(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		sf::Vector2f* b = (sf::Vector2f*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a + *b);
	}
	void opSub(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		sf::Vector2f* b = (sf::Vector2f*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a - *b);
	}
	void opDiv(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		sf::Vector2f* b = (sf::Vector2f*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a / *b);
	}
	void opDivFloat(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		float b = gen->GetArgFloat(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a / b);
	}
	void opMul(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		sf::Vector2f* b = (sf::Vector2f*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a * *b);
	}
	void opMulFloat(asIScriptGeneric* gen)
	{
		sf::Vector2f* a = (sf::Vector2f*)gen->GetObject();
		float b = gen->GetArgFloat(0);

		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(*a * b);
	}
}
}

void as::priv::RegVec2(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Vec2", sizeof(sf::Vector2f), asOBJ_VALUE | asGetTypeTraits<sf::Vector2f>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(createVec, (void*), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(float x, float y)", asFUNCTIONPR(createVec, (void*, float, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_CONSTRUCT, "void f(const Vec2&in)", asFUNCTIONPR(createVec, (void*, const sf::Vector2f&), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructVec), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectProperty("Vec2", "float X", asOFFSET(sf::Vector2f, x)));
	AS_ASSERT(eng->RegisterObjectProperty("Vec2", "float Y", asOFFSET(sf::Vector2f, y)));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opAssign(const Vec2&in)", asMETHODPR(sf::Vector2f, operator=, (const sf::Vector2f&), sf::Vector2f&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "bool opEquals(const Vec2&in) const", asFUNCTIONPR(sf::operator==, (const sf::Vector2f&, const sf::Vector2f&), bool), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opAdd(const Vec2&in) const", asFUNCTION(sf::opAdd), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opSub(const Vec2&in) const", asFUNCTION(sf::opSub), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opDiv(const Vec2&in) const", asFUNCTION(sf::opDiv), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opDiv(float) const", asFUNCTION(sf::opDivFloat), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opMul(const Vec2&in) const", asFUNCTION(sf::opMul), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2 opMul(float) const", asFUNCTION(sf::opMulFloat), asCALL_GENERIC));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opAddAssign(const Vec2&in)", asFUNCTIONPR(sf::operator+=, (sf::Vector2f&, const sf::Vector2f&), sf::Vector2f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opSubAssign(const Vec2&in)", asFUNCTIONPR(sf::operator-=, (sf::Vector2f&, const sf::Vector2f&), sf::Vector2f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opDivAssign(const Vec2&in)", asFUNCTIONPR(sf::operator/=, (sf::Vector2f&, const sf::Vector2f&), sf::Vector2f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opDivAssign(float)", asFUNCTIONPR(sf::operator/=, (sf::Vector2f&, float), sf::Vector2f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opMulAssign(const Vec2&in)", asFUNCTIONPR(sf::operator*=, (sf::Vector2f&, const sf::Vector2f&), sf::Vector2f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "Vec2& opMulAssign(float)", asFUNCTIONPR(sf::operator*=, (sf::Vector2f&, float), sf::Vector2f&), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float get_Angle() const", asFUNCTIONPR(Math::PolarAngle, (const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "void set_Angle(float ang)", asFUNCTIONPR(Math::SetPolarAngle, (sf::Vector2f&, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float get_Length() const", asFUNCTIONPR(Math::Length, (const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "void set_Length(float len)", asFUNCTIONPR(Math::SetLength, (sf::Vector2f&, float), void), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float Dot(const Vec2&in) const", asFUNCTIONPR(Math::Dot, (const sf::Vector2f&, const sf::Vector2f&), float), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
