#include "Shared.hpp"

#include <SFML/Audio/Music.hpp>

namespace
{

}

bool as::priv::RegMusic(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Music", 0, asOBJ_REF | asOBJ_NOCOUNT));

	if (!RegisterSoundSource<sf::Music>(eng, "Music")) return false;

	AS_ASSERT(eng->RegisterObjectMethod("Music", "uint get_ChannelCount() const", asMETHOD(sf::Music, getChannelCount), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Music", "uint get_SampleRate() const", asMETHOD(sf::Music, getSampleRate), asCALL_THISCALL));

	AS_ASSERT(eng->SetDefaultNamespace(""));

	return true;
}
