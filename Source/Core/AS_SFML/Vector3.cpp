#include "Shared.hpp"

#include <Core/Math.hpp>
#include <SFML/System/Vector3.hpp>

namespace
{
	void createVec(void* mem)
	{
		new (mem) sf::Vector3f();
	}
	void createVec(void* mem, float x, float y, float z)
	{
		new (mem) sf::Vector3f(x, y, z);
	}
	void createVec(void* mem, const sf::Vector3f& pos)
	{
		new (mem) sf::Vector3f(pos);
	}
	void destructVec(sf::Vector3f* mem)
	{
		mem->~Vector3();
	}
}

namespace sf
{
	sf::Vector3f operator*(const sf::Vector3f& a, const sf::Vector3f& b)
	{
		return{ a.x * b.x, a.y * b.y, a.z * b.z };
	}
	sf::Vector3f operator/(const sf::Vector3f& a, const sf::Vector3f& b)
	{
		return{ a.x / b.x, a.y / b.y, a.z / b.z };
	}
	sf::Vector3f& operator*=(sf::Vector3f& a, const sf::Vector3f& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		a.z *= b.z;

		return a;
	}
	sf::Vector3f& operator/=(sf::Vector3f& a, const sf::Vector3f& b)
	{
		a.x /= b.x;
		a.y /= b.y;
		a.z /= b.z;

		return a;
	}
}

void as::priv::RegVec3(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Vec3", sizeof(sf::Vector3f), asOBJ_VALUE | asGetTypeTraits<sf::Vector3f>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(createVec, (void*), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(float x, float y, float z)", asFUNCTIONPR(createVec, (void*, float, float, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec3", asBEHAVE_CONSTRUCT, "void f(const Vec3&in)", asFUNCTIONPR(createVec, (void*, const sf::Vector3f&), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Vec3", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructVec), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectProperty("Vec3", "float X", asOFFSET(sf::Vector3f, x)));
	AS_ASSERT(eng->RegisterObjectProperty("Vec3", "float Y", asOFFSET(sf::Vector3f, y)));
	AS_ASSERT(eng->RegisterObjectProperty("Vec3", "float Z", asOFFSET(sf::Vector3f, z)));

	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opAssign(const Vec3&in)", asMETHODPR(sf::Vector3f, operator=, (const sf::Vector3f&), sf::Vector3f&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "bool opEquals(const Vec3&in) const", asFUNCTIONPR(sf::operator==, (const sf::Vector3f&, const sf::Vector3f&), bool), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opAdd(const Vec3&in) const", asFUNCTIONPR(sf::operator+, (const sf::Vector3f&, const sf::Vector3f&), sf::Vector3f), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opSub(const Vec3&in) const", asFUNCTIONPR(sf::operator-, (const sf::Vector3f&, const sf::Vector3f&), sf::Vector3f), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opDiv(const Vec3&in) const", asFUNCTIONPR(sf::operator/, (const sf::Vector3f&, const sf::Vector3f&), sf::Vector3f), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opDiv(float) const", asFUNCTIONPR(sf::operator/, (const sf::Vector3f&, float), sf::Vector3f), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opMul(const Vec3&in) const", asFUNCTIONPR(sf::operator*, (const sf::Vector3f&, const sf::Vector3f&), sf::Vector3f), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec2 opMul(float) const", asFUNCTIONPR(sf::operator*, (const sf::Vector3f&, float), sf::Vector3f), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opAddAssign(const Vec3&in)", asFUNCTIONPR(sf::operator+=, (sf::Vector3f&, const sf::Vector3f&), sf::Vector3f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opSubAssign(const Vec3&in)", asFUNCTIONPR(sf::operator-=, (sf::Vector3f&, const sf::Vector3f&), sf::Vector3f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opDivAssign(const Vec3&in)", asFUNCTIONPR(sf::operator/=, (sf::Vector3f&, const sf::Vector3f&), sf::Vector3f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opDivAssign(float)", asFUNCTIONPR(sf::operator/=, (sf::Vector3f&, float), sf::Vector3f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opMulAssign(const Vec3&in)", asFUNCTIONPR(sf::operator*=, (sf::Vector3f&, const sf::Vector3f&), sf::Vector3f&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "Vec3& opMulAssign(float)", asFUNCTIONPR(sf::operator*=, (sf::Vector3f&, float), sf::Vector3f&), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "float get_Angle() const", asFUNCTIONPR(Math::PolarAngle, (const sf::Vector3f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "void set_Angle(float ang)", asFUNCTIONPR(Math::SetPolarAngle, (sf::Vector3f&, float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "float get_Length() const", asFUNCTIONPR(Math::Length, (const sf::Vector3f&), float), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Vec3", "void set_Length(float len)", asFUNCTIONPR(Math::SetLength, (sf::Vector3f&, float), void), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Vec2", "float Dot(const Vec3&in) const", asFUNCTIONPR(Math::Dot, (const sf::Vector3f&, const sf::Vector3f&), float), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
