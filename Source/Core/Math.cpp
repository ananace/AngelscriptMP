#include "Math.hpp"
#include "ScriptManager.hpp"

sf::Vector2f sf::operator*(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return{ a.x * b.x, a.y * b.y };
}
sf::Vector2f sf::operator/(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return{ a.x / b.x, a.y / b.y };
}
sf::Vector2f& sf::operator*=(sf::Vector2f& a, const sf::Vector2f& b)
{
	a.x *= b.x;
	a.y *= b.y;

	return a;
}
sf::Vector2f& sf::operator/=(sf::Vector2f& a, const sf::Vector2f& b)
{
	a.x /= b.x;
	a.y /= b.y;

	return a;
}

sf::Vector3f sf::operator*(const sf::Vector3f& a, const sf::Vector3f& b)
{
	return{ a.x * b.x, a.y * b.y, a.z * b.z };
}
sf::Vector3f sf::operator/(const sf::Vector3f& a, const sf::Vector3f& b)
{
	return{ a.x / b.x, a.y / b.y, a.z / b.z };
}
sf::Vector3f& sf::operator*=(sf::Vector3f& a, const sf::Vector3f& b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;

	return a;
}
sf::Vector3f& sf::operator/=(sf::Vector3f& a, const sf::Vector3f& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;

	return a;
}

void Math::registerScriptData(ScriptManager& man)
{
	man.addExtension("Math types", [](asIScriptEngine* eng) {
		AS_ASSERT(eng->RegisterGlobalProperty("const float PI", (void*)&Math::PI));
		AS_ASSERT(eng->RegisterGlobalProperty("const float PI2", (void*)&Math::PI2));
		AS_ASSERT(eng->RegisterGlobalProperty("const float RAD2DEG", (void*)&Math::RAD2DEG));
		AS_ASSERT(eng->RegisterGlobalProperty("const float DEG2RAD", (void*)&Math::DEG2RAD));
	});
}
