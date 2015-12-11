#include "Shared.hpp"
#include "../Time.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace
{
	void createSound(void* mem)
	{
		new (mem) sf::Sound();
	}
	void destroySound(sf::Sound* mem)
	{
		mem->~Sound();
	}

	void getLength(asIScriptGeneric* gen)
	{
		const sf::Sound* s = (const sf::Sound*)gen->GetObject();
		
		if (!s->getBuffer())
			new (gen->GetAddressOfReturnLocation()) Timespan(0);
		else
			new (gen->GetAddressOfReturnLocation()) Timespan(std::chrono::microseconds(s->getBuffer()->getDuration().asMicroseconds()));
	}
}

void as::priv::RegSound(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("SoundBuffer", 0, asOBJ_REF | asOBJ_NOCOUNT));

	AS_ASSERT(eng->RegisterObjectType("Sound", sizeof(sf::Sound), asOBJ_VALUE | asGetTypeTraits<sf::Sound>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sound", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createSound), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sound", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroySound), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Sound", "Sound& opAssign(const Sound&in)", asMETHODPR(sf::Sound, operator=, (const sf::Sound&), sf::Sound&), asCALL_THISCALL));

	RegisterSoundSource<sf::Sound>(eng, "Sound");

	AS_ASSERT(eng->RegisterObjectMethod("Sound", "const SoundBuffer@ get_Buffer() const", asMETHOD(sf::Sound, getBuffer), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Sound", "void set_Buffer(const SoundBuffer@)", asMETHOD(sf::Sound, setBuffer), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Sound", "Timespan get_Length() const", asFUNCTION(getLength), asCALL_GENERIC));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
