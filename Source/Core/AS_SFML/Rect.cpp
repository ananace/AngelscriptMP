#include "Shared.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace
{
	void createRect(void* mem)
	{
		new (mem) sf::FloatRect();
	}
	void createRect(void* mem, float l, float t, float w, float h)
	{
		new (mem) sf::FloatRect(l, t, w, h);
	}
	void createRect(void* mem, const sf::Vector2f& pos, const sf::Vector2f& size)
	{
		new (mem) sf::FloatRect(pos, size);
	}
	void createRect(void* mem, const sf::FloatRect& rect)
	{
		new (mem) sf::FloatRect(rect);
	}

	void destructRect(sf::FloatRect* mem)
	{
		mem->~Rect();
	}

	void setPos(sf::FloatRect* rect, const sf::Vector2f& pos)
	{
		rect->left = pos.x;
		rect->top = pos.y;
	}
	sf::Vector2f getPos(const sf::FloatRect* rect)
	{
		return{ rect->left, rect->top };
	}
	void setCenter(sf::FloatRect* rect, const sf::Vector2f& center)
	{
		rect->left = center.x - rect->width / 2.f;
		rect->top = center.y - rect->height / 2.f;
	}
	sf::Vector2f getCenter(const sf::FloatRect* rect)
	{
		return{ rect->left + rect->width / 2.f, rect->top + rect->height / 2.f };
	}
	void setSize(sf::FloatRect* rect, const sf::Vector2f& size)
	{
		rect->width = size.x;
		rect->height = size.y;
	}
	sf::Vector2f getSize(const sf::FloatRect* rect)
	{
		return{ rect->width, rect->height };
	}
}

void as::priv::RegRect(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));
		
	AS_ASSERT(eng->RegisterObjectType("Rect", sizeof(sf::FloatRect), asOBJ_VALUE | asGetTypeTraits<sf::FloatRect>()));
	AS_ASSERT(eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(createRect, (void*), void), asCALL_CDECL_OBJLAST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(const Rect&in)", asFUNCTIONPR(createRect, (void*, const sf::FloatRect&), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(const Vec2&in pos, const Vec2&in size)", asFUNCTIONPR(createRect, (void*, const sf::Vector2f&, const sf::Vector2f&), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(float left, float top, float width, float height)", asFUNCTIONPR(createRect, (void*, float,float,float,float), void), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectBehaviour("Rect", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructRect), asCALL_CDECL_OBJLAST));

	AS_ASSERT(eng->RegisterObjectProperty("Rect", "float Left", asOFFSET(sf::FloatRect, left)));
	AS_ASSERT(eng->RegisterObjectProperty("Rect", "float Top", asOFFSET(sf::FloatRect, top)));
	AS_ASSERT(eng->RegisterObjectProperty("Rect", "float Width", asOFFSET(sf::FloatRect, width)));
	AS_ASSERT(eng->RegisterObjectProperty("Rect", "float Height", asOFFSET(sf::FloatRect, height)));

	AS_ASSERT(eng->RegisterObjectMethod("Rect", "Rect& opAssign(const Rect&in)", asMETHODPR(sf::FloatRect, operator=, (const sf::FloatRect&), sf::FloatRect&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "bool opEquals(const Rect&in) const", asFUNCTIONPR(sf::operator==, (const sf::FloatRect&, const sf::FloatRect&), bool), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Rect", "void set_Center(const Vec2&in)", asFUNCTION(setCenter), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "Vec2 get_Center() const", asFUNCTION(getCenter), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "void set_Position(const Vec2&in)", asFUNCTION(setPos), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "Vec2 get_Position() const", asFUNCTION(getPos), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "void set_Size(const Vec2&in)", asFUNCTION(setSize), asCALL_CDECL_OBJFIRST));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "Vec2 get_Size() const", asFUNCTION(getSize), asCALL_CDECL_OBJFIRST));

	AS_ASSERT(eng->RegisterObjectMethod("Rect", "bool Contains(float x, float y) const", asMETHODPR(sf::FloatRect, contains, (float, float) const, bool), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "bool Contains(const Vec2&in) const", asMETHODPR(sf::FloatRect, contains, (const sf::Vector2f&) const, bool), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "bool Intersects(const Rect&in) const", asMETHODPR(sf::FloatRect, intersects, (const sf::FloatRect&) const, bool), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod("Rect", "bool Intersects(const Rect&in, Rect&out) const", asMETHODPR(sf::FloatRect, intersects, (const sf::FloatRect&, sf::FloatRect&) const, bool), asCALL_THISCALL));
	
	AS_ASSERT(eng->SetDefaultNamespace(""));
}
