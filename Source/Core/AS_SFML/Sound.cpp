#include "Shared.hpp"

#include <SFML/Audio/Sound.hpp>

namespace
{

}

void as::priv::RegSound(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Sound", 0, asOBJ_REF | asOBJ_NOCOUNT));

	RegisterSoundSource<sf::Sound>(eng, "Sound");

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
