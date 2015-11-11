#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace as
{
namespace priv
{

template<typename T>
void CSFMLType<T>::Store(CSerializedValue *val, void *ptr)
{
    val->SetUserData(new T(*reinterpret_cast<T*>(ptr)));
}
template<typename T>
void CSFMLType<T>::Restore(CSerializedValue *val, void *ptr)
{
    auto buffer = reinterpret_cast<T*>(val->GetUserData());
    *reinterpret_cast<T*>(ptr) = *buffer;
}
template<typename T>
void CSFMLType<T>::CleanupUserData(CSerializedValue *val)
{
    auto buffer = reinterpret_cast<T*>(val->GetUserData());

    if (buffer)
    	delete buffer;

    val->SetUserData(nullptr);
}

template<typename T>
bool RegisterDrawable(asIScriptEngine* eng, const char* name)
{
	AS_ASSERT(eng->RegisterObjectMethod(name, "Rect get_LocalBounds() const", asMETHOD(T, getLocalBounds), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "Rect get_GlobalBounds() const", asMETHOD(T, getGlobalBounds), asCALL_THISCALL));
	return true;
}
template<typename T>
bool RegisterTransformable(asIScriptEngine* eng, const char* name)
{
	AS_ASSERT(eng->RegisterObjectMethod(name, "const Vec2& get_Origin() const", asMETHODPR(T, getOrigin, () const, const sf::Vector2f&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Origin(const Vec2&in)", asMETHODPR(T, setOrigin, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "const Vec2& get_Position() const", asMETHODPR(T, getPosition, () const, const sf::Vector2f&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Position(const Vec2&in)", asMETHODPR(T, setPosition, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_Rotation() const", asMETHODPR(T, getRotation, () const, float), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Rotation(float)", asMETHODPR(T, setRotation, (float), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "const Vec2& get_Scale() const", asMETHODPR(T, getScale, () const, const sf::Vector2f&), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Scale(const Vec2&in)", asMETHODPR(T, setScale, (const sf::Vector2f&), void), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod(name, "void Move(const Vec2&in)", asMETHODPR(T, move, (const sf::Vector2f&), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void Rotate(float)", asMETHODPR(T, rotate, (float), void), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void Scale(const Vec2&in)", asMETHODPR(T, scale, (const sf::Vector2f&), void), asCALL_THISCALL));
	return true;
}
template<typename T>
bool RegisterShape(asIScriptEngine* eng, const char* name)
{
	AS_ASSERT(eng->RegisterObjectMethod(name, "const Color& get_FillColor() const", asMETHOD(T, getFillColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_FillColor(const Color&in)", asMETHOD(T, setFillColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "const Color& get_OutlineColor() const", asMETHOD(T, getOutlineColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_OutlineColor(const Color&in)", asMETHOD(T, setOutlineColor), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_OutlineColor() const", asMETHOD(T, getOutlineThickness), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_OutlineColor(float)", asMETHOD(T, setOutlineThickness), asCALL_THISCALL));
	
	AS_ASSERT(eng->RegisterObjectMethod(name, "uint get_PointCount() const", asMETHOD(T, getPointCount), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "Vec2 get_opIndex(uint) const", asMETHOD(T, getPoint), asCALL_THISCALL));
	return true;
}
template<typename T>
bool RegisterSoundSource(asIScriptEngine* eng, const char* name)
{
	static bool SoundReg = false;
	if (!SoundReg)
	{
		SoundReg = true;

		AS_ASSERT(eng->RegisterEnum("SoundStatus"));
		AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Stopped", T::Stopped));
		AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Paused", T::Paused));
		AS_ASSERT(eng->RegisterEnumValue("SoundStatus", "Playing", T::Playing));
	}

	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_Pitch() const", asMETHOD(T, getPitch), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Pitch(float)", asMETHOD(T, setPitch), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_Pitch() const", asMETHOD(T, getVolume), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Pitch(float)", asMETHOD(T, setVolume), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod(name, "bool get_Loop() const", asMETHOD(T, getLoop), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Loop(bool)", asMETHOD(T, setLoop), asCALL_THISCALL));

	AS_ASSERT(eng->RegisterObjectMethod(name, "SoundStatus get_Status() const", asMETHOD(T, getStatus), asCALL_THISCALL));

	// 3D Sounds:
	AS_ASSERT(eng->RegisterObjectMethod(name, "bool get_RelativeToListener() const", asMETHOD(T, isRelativeToListener), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_RelativeToListener(bool)", asMETHOD(T, setRelativeToListener), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_Attenuation() const", asMETHOD(T, getAttenuation), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Attenuation(float)", asMETHOD(T, setAttenuation), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "float get_MinDistance() const", asMETHOD(T, getMinDistance), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_MinDistance(float)", asMETHOD(T, setMinDistance), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "Vec3 get_Position() const", asMETHOD(T, getPosition), asCALL_THISCALL));
	AS_ASSERT(eng->RegisterObjectMethod(name, "void set_Position(const Vec3&in)", asMETHODPR(T, setPosition, (const sf::Vector3f&), void), asCALL_THISCALL));
	return true;
}

}
}
