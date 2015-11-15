#include "Shared.hpp"

#include <SFML/Audio/Music.hpp>

namespace
{

}

void as::priv::RegMusic(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterEnum("SoundStatus"));
	AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Stopped", sf::SoundSource::Stopped));
	AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Paused", sf::SoundSource::Paused));
	AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Playing", sf::SoundSource::Playing));

	AS_ASSERT(eng->RegisterObjectType("Music", 0, asOBJ_REF | asOBJ_NOCOUNT));

	RegisterSoundSource<sf::Music>(eng, "Music");

	AS_ASSERT(eng->RegisterObjectMethod("Music", "uint get_ChannelCount() const", asMETHOD(sf::Music, getChannelCount), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Music", "uint get_SampleRate() const", asMETHOD(sf::Music, getSampleRate), asCALL_THISCALL));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
