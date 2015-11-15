#include "Shared.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace as::priv;

const SFMLType as::priv::Types[TypeCount] = {
	{ "Vec2", RegVec2, []() { return new CSFMLType<sf::Vector2f>(); } },
	{ "Vec3", RegVec3, []() { return new CSFMLType<sf::Vector3f>(); } },
	{ "Rect", RegRect, []() { return new CSFMLType<sf::FloatRect>(); } },
	{ "Color", RegColor, []() { return new CSFMLType<sf::Color>(); } },
	{ "Texture", RegTexture, nullptr },
	{ "Shapes::Circle", RegCircleShape, []() { return new CSFMLType<sf::CircleShape>(); } },
	{ "Shapes::Rectangle", RegRectangleShape, []() { return new CSFMLType<sf::RectangleShape>(); } },
	{ "Shapes::Convex", RegConvexShape, []() { return new CSFMLType<sf::ConvexShape>(); } },
	{ "Sprite", RegSprite, []() { return new CSFMLType<sf::Sprite>(); } },
	{ "Text", RegText, []() { return new CSFMLType<sf::Text>(); } },
	{ "Music", RegMusic, nullptr },
	{ "Sound", RegSound, nullptr },
	{ "Listener", RegListener, nullptr }
};
