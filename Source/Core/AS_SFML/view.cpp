#include "Shared.hpp"

#include <SFML/Graphics/View.hpp>

namespace
{
	void createView(void* mem)
	{
		new (mem) sf::View();
	}
	void createViewRect(void* mem, const sf::FloatRect& rect)
	{
		new (mem) sf::View(rect);
	}
	void createViewCS(void* mem, const sf::Vector2f& center, const sf::Vector2f& size)
	{
		new (mem) sf::View(center, size);
	}
	void destroyView(sf::View* mem)
	{
		mem->~View();
	}
}

void as::priv::RegView(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("View", sizeof(sf::View), asOBJ_VALUE | asGetTypeTraits<sf::View>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("View", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(createView), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("View", asBEHAVE_CONSTRUCT, "void f(const sf::Rect&in)", asFUNCTION(createViewRect), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("View", asBEHAVE_CONSTRUCT, "void f(const sf::Vec2&in, const sf::Vec2&in)", asFUNCTION(createViewCS), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("View", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destroyView), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("View", "View& opAssign(const View&in)", asMETHODPR(sf::View, operator=, (const sf::View&), sf::View&), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("View", "const Rect& get_Viewport() const", asMETHOD(sf::View, getViewport), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void set_Viewport(const Rect&in)", asMETHODPR(sf::View, setViewport, (const sf::FloatRect&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "const Vec2& get_Center() const", asMETHOD(sf::View, getCenter), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void set_Center(const Vec2&in)", asMETHODPR(sf::View, setCenter, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "const Vec2& get_Size() const", asMETHOD(sf::View, getSize), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void set_Size(const Vec2&in)", asMETHODPR(sf::View, setSize, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "float get_Rotation() const", asMETHOD(sf::View, getRotation), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void set_Rotation(float)", asMETHOD(sf::View, setRotation), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod("View", "void Move(const Vec2&in)", asMETHODPR(sf::View, move, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void Rotate(float)", asMETHOD(sf::View, rotate), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("View", "void Zoom(float)", asMETHOD(sf::View, zoom), asCALL_THISCALL));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
