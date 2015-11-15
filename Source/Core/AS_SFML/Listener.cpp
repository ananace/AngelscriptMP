#include "Shared.hpp"

#include <SFML/Audio/Listener.hpp>

namespace
{

}

void as::priv::RegListener(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf::Listener"));

	AS_ASSERT(eng->RegisterGlobalFunction("float get_GlobalVolume()", asFUNCTION(sf::Listener::getGlobalVolume), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_GlobalVolume(float)", asFUNCTION(sf::Listener::setGlobalVolume), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_Position()", asFUNCTION(sf::Listener::getPosition), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_Position(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setPosition, (const sf::Vector3f&), void), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_Direction()", asFUNCTION(sf::Listener::getDirection), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_Direction(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setDirection, (const sf::Vector3f&), void), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("sf::Vec3 get_UpVector()", asFUNCTION(sf::Listener::getUpVector), asCALL_CDECL));
	AS_ASSERT(eng->RegisterGlobalFunction("void set_UpVector(const sf::Vec3&in)", asFUNCTIONPR(sf::Listener::setUpVector, (const sf::Vector3f&), void), asCALL_CDECL));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
