#include "Shared.hpp"

#include <SFML/Window/Joystick.hpp>

namespace
{
	enum Stick
	{
		Left = 0,
		Right,
		Pov
	};

	void getStickPosition(asIScriptGeneric* gen)
	{
		uint32_t id = gen->GetArgDWord(0);
		Stick stick = Stick(gen->GetArgDWord(1));

		switch (stick)
		{
		case Left:
			new (gen->GetAddressOfReturnLocation()) sf::Vector2f(
				sf::Joystick::getAxisPosition(id, sf::Joystick::X),
				sf::Joystick::getAxisPosition(id, sf::Joystick::Y)
				); break;
		case Right:
			new (gen->GetAddressOfReturnLocation()) sf::Vector2f(
				sf::Joystick::getAxisPosition(id, sf::Joystick::R),
				sf::Joystick::getAxisPosition(id, sf::Joystick::U)
				); break;
		case Pov:
			new (gen->GetAddressOfReturnLocation()) sf::Vector2f(
				sf::Joystick::getAxisPosition(id, sf::Joystick::PovX),
				sf::Joystick::getAxisPosition(id, sf::Joystick::PovY)
				); break;
		}
	}
}

void as::priv::RegJoystick(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf::Joystick"));

	AS_ASSERT(eng->RegisterEnum("Axis"));
	AS_ASSERT(eng->RegisterEnumValue	("Axis", "X", sf::Joystick::X));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "Y", sf::Joystick::Y));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "Z", sf::Joystick::Z));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "RX", sf::Joystick::R));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "RY", sf::Joystick::U));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "RZ", sf::Joystick::V));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "PovX", sf::Joystick::PovX));
	AS_ASSERT(eng->RegisterEnumValue("Axis", "PovY", sf::Joystick::PovY));

	AS_ASSERT(eng->RegisterEnum("Stick"));
	AS_ASSERT(eng->RegisterEnumValue("Stick", "Left", 0));
	AS_ASSERT(eng->RegisterEnumValue("Stick", "Right", 1));
	AS_ASSERT(eng->RegisterEnumValue("Stick", "Pov", 2));

	AS_ASSERT(eng->RegisterGlobalFunction("bool IsConnected(uint)", asFUNCTION(sf::Joystick::isConnected), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("bool IsPressed(uint,uint)", asFUNCTION(sf::Joystick::isButtonPressed), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("float AxisPosition(uint,Axis)", asFUNCTION(sf::Joystick::getAxisPosition), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("Vec2 StickPosition(uint,Stick=Left)", asFUNCTION(getStickPosition), asCALL_GENERIC));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
