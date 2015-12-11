#include "Shared.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace
{
	template<typename T>
	void createSprite(void* mem)
	{
		new (mem) T();
	}
	template<typename T, typename... Args>
	void createShapeData(void* mem, Args... args)
	{
		new (mem) T(args...);
	}
	template<typename T>
	void destroySprite(T* mem)
	{
		mem->~T();
	}


	void setSpriteTexture(sf::Sprite& s, const sf::Texture& t)
	{
		s.setTexture(t);
	}
	void getSpriteTextureRect(asIScriptGeneric* gen)
	{
		const sf::Sprite* s = (const sf::Sprite*)gen->GetObject();
		new (gen->GetAddressOfReturnLocation()) sf::FloatRect(s->getTextureRect());
	}
	void setSpriteTextureRect(sf::Sprite& s, const sf::FloatRect& r)
	{
		s.setTextureRect(sf::IntRect(r));
	}
}


void as::priv::RegSprite(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Sprite", sizeof(sf::Sprite), asOBJ_VALUE | asGetTypeTraits<sf::Sprite>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sprite", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createSprite<sf::Sprite>), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sprite", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroySprite<sf::Sprite>), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "Sprite& opAssign(const Sprite&in)", asMETHODPR(sf::Sprite, operator=, (const sf::Sprite&), sf::Sprite&), asCALL_THISCALL));

	RegisterDrawable<sf::Sprite>(eng, "Sprite");
	RegisterTransformable<sf::Sprite>(eng, "Sprite");

	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "void set_Color(const Color&in)", asMETHOD(sf::Sprite, setColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "const Color& get_Color() const", asMETHOD(sf::Sprite, getColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "void SetTexture(const Texture@)", asFUNCTION(setSpriteTexture), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "const Texture@ get_Texture() const", asMETHOD(sf::Sprite, getTexture), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "void set_TextureRect(const Rect&in)", asFUNCTION(setSpriteTextureRect), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Sprite", "Rect get_TextureRect() const", asFUNCTION(getSpriteTextureRect), asCALL_GENERIC));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
