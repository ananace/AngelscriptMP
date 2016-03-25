#include "Shared.hpp"

#include <SFML/Graphics/Shader.hpp>

namespace
{

}

void as::priv::RegShader(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Shader", 0, asOBJ_REF | asOBJ_NOCOUNT));

	AS_ASSERT(eng->SetDefaultNamespace("sf::Shader"));
	
	AS_ASSERT(eng->RegisterGlobalFunction("bool get_Available()", asFUNCTION(sf::Shader::isAvailable), asCALL_CDECL));

	AS_ASSERT(eng->RegisterObjectType("CurrentTexture_t", 0, asOBJ_REF | asOBJ_NOCOUNT));
	AS_ASSERT(eng->RegisterGlobalProperty("const CurrentTexture_t@ CurrentTexture", &sf::Shader::CurrentTexture));

	AS_ASSERT(eng->RegisterEnum("Type"));
	AS_ASSERT(eng->RegisterEnumValue("Type", "Vertex", sf::Shader::Vertex));
	AS_ASSERT(eng->RegisterEnumValue("Type", "Fragment", sf::Shader::Fragment));

	AS_ASSERT(eng->SetDefaultNamespace("sf"));


#if SFML_VERSION_PATCH > 2
	AS_ASSERT(eng->RegisterGlobalFunction("bool get_GeometryAvailable()", asFUNCTION(sf::Shader::isGeometryAvailable), asCALL_CDECL));
	AS_ASSERT(eng->RegisterEnumValue("Type", "Geometry", sf::Shader::Geometry));

	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const sf::Shader::CurrentTexture_t@)", asMETHODPR(sf::Shader, setUniform, (const std::string&, sf::Shader::CurrentTextureType), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Texture@)", asMETHODPR(sf::Shader, setUniform, (const std::string&, const sf::Texture&), void), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, float)", asMETHODPR(sf::Shader, setUniform, (const std::string&, float), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, int)", asMETHODPR(sf::Shader, setUniform, (const std::string&, int), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, bool)", asMETHODPR(sf::Shader, setUniform, (const std::string&, bool), void), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Vec2&in)", asMETHODPR(sf::Shader, setUniform, (const std::string&, const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Vec3&in)", asMETHODPR(sf::Shader, setUniform, (const std::string&, const sf::Vector3f&), void), asCALL_THISCALL));
#else
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const sf::Shader::CurrentTexture_t@)", asMETHODPR(sf::Shader, setParameter, (const std::string&, sf::Shader::CurrentTextureType), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Texture@)", asMETHODPR(sf::Shader, setParameter, (const std::string&, const sf::Texture&), void), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, float)", asMETHODPR(sf::Shader, setParameter, (const std::string&, float), void), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Vec2&in)", asMETHODPR(sf::Shader, setParameter, (const std::string&, const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Shader", "void set_opIndex(const string&in, const Vec3&in)", asMETHODPR(sf::Shader, setParameter, (const std::string&, const sf::Vector3f&), void), asCALL_THISCALL));
#endif

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
