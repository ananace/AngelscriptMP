#include "Shared.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace
{
	void getTextureSize(asIScriptGeneric* gen)
	{
		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(reinterpret_cast<const sf::Texture*>(gen->GetObject())->getSize());
	}
}

void as::priv::RegTexture(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Texture", 0, asOBJ_REF | asOBJ_NOCOUNT));

	AS_ASSERT(eng->RegisterObjectMethod("Texture", "bool get_Repeated() const", asMETHOD(sf::Texture, isRepeated), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Texture", "void set_Repeated(bool)", asMETHOD(sf::Texture, setRepeated), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Texture", "bool get_Smooth() const", asMETHOD(sf::Texture, isSmooth), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Texture", "void set_Smooth(bool)", asMETHOD(sf::Texture, setSmooth), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("Texture", "Vec2 get_Size() const", asFUNCTION(getTextureSize), asCALL_GENERIC));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
