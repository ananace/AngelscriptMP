#include "Shared.hpp"

#include <SFML/Graphics/Font.hpp>

#if SFML_VERSION_MINOR > 1
namespace
{
	void getFamily(asIScriptGeneric* gen)
	{
		sf::Font* font = (sf::Font*)gen->GetObject();

		new (gen->GetAddressOfReturnLocation()) std::string(font->getInfo().family);
	}
}
#endif

void as::priv::RegFont(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Font", 0, asOBJ_REF | asOBJ_NOCOUNT));

#if SFML_VERSION_MINOR > 1
	AS_ASSERT(eng->RegisterObjectMethod("Font", "string get_Family() const", asFUNCTION(getFamily), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Font", "uint GetLineSpacing(uint) const", asMETHOD(sf::Font, getLineSpacing), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Font", "uint GetUnderlinePosition(uint) const", asMETHOD(sf::Font, getUnderlinePosition), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Font", "uint GetUnderlineThickness(uint) const", asMETHOD(sf::Font, getUnderlineThickness), asCALL_THISCALL));
#endif

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
