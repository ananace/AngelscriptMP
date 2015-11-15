#include "Shared.hpp"
#include "../ScriptManager.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace as::priv;

const SFMLType as::priv::Types[TypeCount] = {
	{ "Vec2", RegVec2, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::Vector2f>(); } },
	{ "Vec3", RegVec3, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::Vector3f>(); } },
	{ "Rect", RegRect, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::FloatRect>(); } },
	{ "Color", RegColor, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::Color>(); } },
	{ "Texture", RegTexture, nullptr },
	{ "Shapes::Circle", RegCircleShape, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::CircleShape>(); } },
	{ "Shapes::Rectangle", RegRectangleShape, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::RectangleShape>(); } },
	{ "Shapes::Convex", RegConvexShape, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::ConvexShape>(); } },
	{ "Sprite", RegSprite, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::Sprite>(); } },
	{ "Text", RegText, []() -> CUserType* { return new ScriptManager::CSimpleType<sf::Text>(); } },
	{ "Music", RegMusic, nullptr },
	{ "Sound", RegSound, nullptr },
	{ "Listener", RegListener, nullptr }
};
