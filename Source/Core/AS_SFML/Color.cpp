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

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
