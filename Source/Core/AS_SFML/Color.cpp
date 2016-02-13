#include "Shared.hpp"

#include <SFML/Graphics/Color.hpp>

namespace
{
	void createColor(void* mem)
	{
		new (mem) sf::Color();
	}
	void createColorData(void* mem, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
	{
		new (mem) sf::Color(r,g,b,a);
	}
	void destroyColor(sf::Color* mem)
	{
		mem->~Color();
	}
}

namespace sf
{
#if SFML_VERSION_MINOR < 2
	sf::Color operator-(const sf::Color& a, const sf::Color& b)
	{
		return{
			a.r - b.r,
			a.g - b.g,
			a.b - b.b,
			a.a - b.a
		};
	}
	sf::Color& operator-=(sf::Color& a, const sf::Color& b)
	{
		a.r -= b.r;
		a.g -= b.g;
		a.b -= b.b;
		a.a -= b.a;

		return a;
	}
#endif

	sf::Color operator*(const sf::Color& c, float i)
	{
		return{ uint8_t(c.r * i), uint8_t(c.g * i), uint8_t(c.b * i) };
	}
	sf::Color operator/(const sf::Color& c, const sf::Color& c2)
	{
		return{
			uint8_t((int)c.r / c2.r * 255),
			uint8_t((int)c.g / c2.g * 255),
			uint8_t((int)c.b / c2.b * 255),
			uint8_t((int)c.a / c2.a * 255),
		};
	}
	sf::Color operator/(const sf::Color& c, float i)
	{
		return{ uint8_t(c.r / i), uint8_t(c.g / i), uint8_t(c.b / i) };
	}
	sf::Color& operator*=(sf::Color& c, float i)
	{
		c.r = uint8_t(c.r * i);
		c.g = uint8_t(c.g * i);
		c.b = uint8_t(c.b * i);
		return c;
	}
	sf::Color& operator/=(sf::Color& c, float i)
	{
		c.r = uint8_t(c.r / i);
		c.g = uint8_t(c.g / i);
		c.b = uint8_t(c.b / i);
		return c;
	}

	namespace
	{
	void opAdd(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		sf::Color* b = (sf::Color*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a + *b);
	}
	void opSub(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		sf::Color* b = (sf::Color*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a - *b);
	}
	void opDiv(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		sf::Color* b = (sf::Color*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a / *b);
	}
	void opDivFloat(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		float b = gen->GetArgFloat(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a / b);
	}
	void opMul(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		sf::Color* b = (sf::Color*)gen->GetArgObject(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a * *b);
	}
	void opMulFloat(asIScriptGeneric* gen)
	{
		sf::Color* a = (sf::Color*)gen->GetObject();
		float b = gen->GetArgFloat(0);

		new (gen->GetAddressOfReturnLocation()) sf::Color(*a * b);
	}
	}
}

void as::priv::RegColor(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Color", sizeof(sf::Color), asOBJ_VALUE | asGetTypeTraits<sf::Color>()));

	AS_ASSERT(eng->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createColor), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f(uint8,uint8,uint8)", asFUNCTION(createColorData), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Color", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroyColor), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectProperty("Color", "uint8 R", asOFFSET(sf::Color, r)));
	AS_ASSERT(eng->RegisterObjectProperty("Color", "uint8 G", asOFFSET(sf::Color, g)));
	AS_ASSERT(eng->RegisterObjectProperty("Color", "uint8 B", asOFFSET(sf::Color, b)));
	AS_ASSERT(eng->RegisterObjectProperty("Color", "uint8 A", asOFFSET(sf::Color, a)));

	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opAssign(const Color&in)", asMETHODPR(sf::Color, operator=, (const sf::Color&), sf::Color&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "bool opEquals(const Color&in) const", asFUNCTIONPR(sf::operator==, (const sf::Color&, const sf::Color&), bool), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opAdd(const Color&in) const", asFUNCTION(sf::opAdd), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opSub(const Color&in) const", asFUNCTION(sf::opSub), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opMul(const Color&in) const", asFUNCTION(sf::opMul), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opDiv(const Color&in) const", asFUNCTION(sf::opDiv), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opMul(float) const", asFUNCTION(sf::opMulFloat), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color opDiv(float) const", asFUNCTION(sf::opDivFloat), asCALL_GENERIC));

	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opAddAssign(const Color&in)", asFUNCTIONPR(sf::operator+=, (sf::Color&, const sf::Color&), sf::Color&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opSubAssign(const Color&in)", asFUNCTIONPR(sf::operator-=, (sf::Color&, const sf::Color&), sf::Color&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opMulAssign(const Color&in)", asFUNCTIONPR(sf::operator*=, (sf::Color&, const sf::Color&), sf::Color&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opMulAssign(float)", asFUNCTIONPR(sf::operator*=, (sf::Color&, float), sf::Color&), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Color", "Color& opDivAssign(float)", asFUNCTIONPR(sf::operator/=, (sf::Color&, float), sf::Color&), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace("sf::Color"));

	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Red", const_cast<sf::Color*>(&sf::Color::Red)));
	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Green", const_cast<sf::Color*>(&sf::Color::Green)));
	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Blue", const_cast<sf::Color*>(&sf::Color::Blue)));

	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Cyan", const_cast<sf::Color*>(&sf::Color::Cyan)));
	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Magenta", const_cast<sf::Color*>(&sf::Color::Magenta)));
	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Yellow", const_cast<sf::Color*>(&sf::Color::Yellow)));

	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color White", const_cast<sf::Color*>(&sf::Color::White)));
	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Black", const_cast<sf::Color*>(&sf::Color::Black)));

	AS_ASSERT(eng->RegisterGlobalProperty("const sf::Color Transparent", const_cast<sf::Color*>(&sf::Color::Transparent)));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
