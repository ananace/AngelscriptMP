#include "Shared.hpp"

#include <SFML/Graphics/Text.hpp>

namespace
{
	void createText(void* mem)
	{
		new (mem) sf::Text();
	}
	template<typename... Args>
	void createTextData(sf::Text* mem, Args... args)
	{
		new (mem) sf::Text();
		mem->setString(args...);
	}
	void destroyText(sf::Text* mem)
	{
		mem->~Text();
	}
}

void as::priv::RegText(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Text", sizeof(sf::Text), asOBJ_VALUE | asGetTypeTraits<sf::Text>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createText), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_CONSTRUCT, "void f(const ::string&in)", asFunctionPtr(createTextData<const std::string&>), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroyText), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace("sf::Text"));
	AS_ASSERT(eng->RegisterEnum("Style"));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Regular", sf::Text::Regular));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Bold", sf::Text::Bold));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Italic", sf::Text::Italic));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Underlined", sf::Text::Underlined));
	AS_ASSERT(eng->RegisterEnumValue("Style", "StrikeThrough", sf::Text::StrikeThrough));
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	RegisterDrawable<sf::Text>(eng, "Text");
	RegisterTransformable<sf::Text>(eng, "Text");

	AS_ASSERT(eng->RegisterObjectMethod("Text", "uint get_CharacterSize() const", asMETHOD(sf::Text, getCharacterSize), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_CharacterSize(uint)", asMETHOD(sf::Text, setCharacterSize), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "const Color& get_Color() const", asMETHOD(sf::Text, getColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_Color(const Color&in)", asMETHOD(sf::Text, setColor), asCALL_THISCALL));
	//AS_ASSERT(eng->RegisterObjectMethod("Text", "const ::string& get_String() const", asMETHOD(sf::Text, getString), asCALL_THISCALL));
	//AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_String(const ::string&in)", asMETHOD(sf::Text, setString), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "uint get_Style() const", asMETHOD(sf::Text, getStyle), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_Style(uint)", asMETHOD(sf::Text, setStyle), asCALL_THISCALL));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
