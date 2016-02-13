#include "Shared.hpp"

#include <SFML/Audio/Listener.hpp>

namespace
{
	void getPosition(asIScriptGeneric* gen)
	{
		new (gen->GetAddressOfReturnLocation()) sf::Vector3f(sf::Listener::getPosition());
	}
	void getDirection(asIScriptGeneric* gen)
	{
		new (gen->GetAddressOfReturnLocation()) sf::Vector3f(sf::Listener::getDirection());
	}
#if SFML_VERSION_MINOR > 1
	void getUpVector(asIScriptGeneric* gen)
	{
		new (gen->GetAddressOfReturnLocation()) sf::Vector3f(sf::Listener::getUpVector());
	}
#endif
}

void as::priv::RegListener(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf::Listener"));

	AS_ASSERT(eng->RegisterGlobalFunction("float get_GlobalVolume()", asFUNCTION(sf::Listener::getGlobalVolume), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_GlobalVolume(float)", asFUNCTION(sf::Listener::setGlobalVolume), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_Position()", asFUNCTION(getPosition), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_Position(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setPosition, (const sf::Vector3f&), void), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_Direction()", asFUNCTION(getDirection), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_Direction(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setDirection, (const sf::Vector3f&), void), asCALL_CDECL));
#if SFML_VERSION_MINOR > 1
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_UpVector()", asFUNCTION(getUpVector), asCALL_GENERIC));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_UpVector(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setUpVector, (const sf::Vector3f&), void), asCALL_CDECL));
#endif

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
