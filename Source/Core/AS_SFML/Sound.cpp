#include "Shared.hpp"

#include <SFML/Audio/Sound.hpp>

namespace
{

}

bool as::priv::RegSound(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Sound", 0, asOBJ_REF | asOBJ_NOCOUNT));

	if (!RegisterSoundSource<sf::Sound>(eng, "Sound")) return false;

	AS_ASSERT(eng->SetDefaultNamespace(""));

	return true;
}
