#include "Shared.hpp"

#include <SFML/Window/Mouse.hpp>

namespace
{
	void getPosition(asIScriptGeneric* gen)
	{
		new (gen->GetAddressOfReturnLocation()) sf::Vector2f(sf::Mouse::getPosition(*(sf::Window*)gen->GetEngine()->GetUserData(0x6AE1)));
	}
}

void as::priv::RegMouse(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf::Mouse"));

	AS_ASSERT(eng->RegisterEnum("Button"));
	AS_ASSERT(eng->RegisterEnumValue("Button", "Left", sf::Mouse::Left));
	AS_ASSERT(eng->RegisterEnumValue("Button", "Right", sf::Mouse::Right));
	AS_ASSERT(eng->RegisterEnumValue("Button", "Middle", sf::Mouse::Middle));
	AS_ASSERT(eng->RegisterEnumValue("Button", "XButton1", sf::Mouse::XButton1));
	AS_ASSERT(eng->RegisterEnumValue("Button", "XButton2", sf::Mouse::XButton2));

	AS_ASSERT(eng->RegisterGlobalFunction("bool IsPressed(Button)", asFUNCTION(sf::Mouse::isButtonPressed), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("Vec2 get_Position()", asFUNCTION(getPosition), asCALL_GENERIC));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}