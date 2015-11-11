#include "Shared.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace as::priv;

const SFMLType as::priv::Types[TypeCount] = {
	{ "Rect", RegRect, []() { return new CSFMLType<sf::FloatRect>(); } },
	{ "Vec2", RegVec2, []() { return new CSFMLType<sf::Vector2f>(); } },
	{ "Vec3", RegVec3, []() { return new CSFMLType<sf::Vector3f>(); } },
	{ "Texture", RegTexture },
	{ "Shapes::Circle", RegCircleShape, []() { return new CSFMLType<sf::CircleShape>(); } },
	{ "Shapes::Rectangle", RegRectangleShape, []() { return new CSFMLType<sf::RectangleShape>(); } },
	{ "Shapes::Convex", RegConvexShape, []() { return new CSFMLType<sf::ConvexShape>(); } },
	{ "Sprite", RegSprite, []() { return new CSFMLType<sf::Sprite>(); } },
	{ "Text", RegText, []() { return new CSFMLType<sf::Text>(); } },
	{ "Music", RegMusic },
	{ "Sound", RegSound },
	{ "Listener", RegListener }
};
