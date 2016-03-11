#include "Shared.hpp"

#include <SFML/Graphics/Text.hpp>

namespace
{
	void createText(asIScriptGeneric* gen)
	{
		sf::Font* font = nullptr;
		if ((font = (sf::Font*)gen->GetEngine()->GetUserData(0xF077)))
			new (gen->GetObject()) sf::Text("", *font);
		else
			new (gen->GetObject()) sf::Text();
	}
	void createTextData(asIScriptGeneric* gen)
	{
		const std::string* str = (const std::string*)gen->GetArgObject(0);

		sf::Font* font = nullptr;
		if ((font = (sf::Font*)gen->GetEngine()->GetUserData(0xF077)))
			new (gen->GetObject()) sf::Text(*str, *font);
		else
		{
			new (gen->GetObject()) sf::Text();
			((sf::Text*)gen->GetObject())->setString(*str);
		}
	}
	void destroyText(sf::Text* mem)
	{
		mem->~Text();
	}

	void getTextString(asIScriptGeneric* gen)
	{
		auto* t = (const sf::Text*)gen->GetObject();

		new (gen->GetAddressOfReturnLocation()) std::string(t->getString());
	}
	void setTextString(sf::Text& t, const std::string& s)
	{
		t.setString(s);
	}
}

void as::priv::RegText(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Text", sizeof(sf::Text), asOBJ_VALUE | asGetTypeTraits<sf::Text>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createText), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_CONSTRUCT, "void f(const ::string&in)", asFunctionPtr(createTextData), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectBehaviour("Text", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroyText), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->SetDefaultNamespace("sf::Text"));
	AS_ASSERT(eng->RegisterEnum("Style"));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Regular", sf::Text::Regular));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Bold", sf::Text::Bold));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Italic", sf::Text::Italic));
	AS_ASSERT(eng->RegisterEnumValue("Style", "Underlined", sf::Text::Underlined));
#if SFML_VERSION_MINOR > 1
	AS_ASSERT(eng->RegisterEnumValue("Style", "StrikeThrough", sf::Text::StrikeThrough));
#endif
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectMethod("Text", "Text& opAssign(const Text&in)", asMETHODPR(sf::Text, operator=, (const sf::Text&), sf::Text&), asCALL_THISCALL));

	RegisterDrawable<sf::Text>(eng, "Text");
	RegisterTransformable<sf::Text>(eng, "Text");

	AS_ASSERT(eng->RegisterObjectMethod("Text", "const Font@ get_Font() const", asMETHOD(sf::Text, getFont), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void SetFont(const Font@)", asMETHOD(sf::Text, setFont), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "uint get_CharacterSize() const", asMETHOD(sf::Text, getCharacterSize), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_CharacterSize(uint)", asMETHOD(sf::Text, setCharacterSize), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "const Color& get_FillColor() const", asMETHOD(sf::Text, getFillColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_FillColor(const Color&in)", asMETHOD(sf::Text, setFillColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "const Color& get_OutlineColor() const", asMETHOD(sf::Text, getOutlineColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_OutlineColor(const Color&in)", asMETHOD(sf::Text, setOutlineColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "float get_OutlineThickness() const", asMETHOD(sf::Text, getOutlineThickness), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_OutlineThickness(float)", asMETHOD(sf::Text, setOutlineThickness), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "const ::string& get_String() const", asFUNCTION(getTextString), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_String(const ::string&in)", asFUNCTION(setTextString), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "uint get_Style() const", asMETHOD(sf::Text, getStyle), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Text", "void set_Style(uint)", asMETHOD(sf::Text, setStyle), asCALL_THISCALL));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
