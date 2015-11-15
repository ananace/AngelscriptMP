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
}


void as::priv::RegSprite(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Sprite", sizeof(sf::Sprite), asOBJ_VALUE | asGetTypeTraits<sf::Sprite>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sprite", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createSprite<sf::Sprite>), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Sprite", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroySprite<sf::Sprite>), asCALL_CDECL_OBJFIRST));

	RegisterDrawable<sf::Sprite>(eng, "Sprite");
	//RegisterShape<sf::Sprite>(eng, "Sprite");
	RegisterTransformable<sf::Sprite>(eng, "Sprite");

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
