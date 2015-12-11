#pragma once

#include "../ScriptManager.hpp"

#include <functional>

namespace as
{
namespace priv
{

// Registering functions
//========================
extern void RegVec2(asIScriptEngine*);
extern void RegVec3(asIScriptEngine*);
extern void RegRect(asIScriptEngine*);
extern void RegRenderer(asIScriptEngine*);
extern void RegColor(asIScriptEngine*);
extern void RegTexture(asIScriptEngine*);
extern void RegCircleShape(asIScriptEngine*);
extern void RegRectangleShape(asIScriptEngine*);
extern void RegConvexShape(asIScriptEngine*);
extern void RegSprite(asIScriptEngine*);
extern void RegText(asIScriptEngine*);
extern void RegMusic(asIScriptEngine*);
extern void RegSound(asIScriptEngine*);
extern void RegListener(asIScriptEngine*);
extern void RegKeyboard(asIScriptEngine*);
extern void RegJoystick(asIScriptEngine*);
extern void RegMouse(asIScriptEngine*);

// Wrapper struct
//==================
struct SFMLType
{
	const char* Name;
	std::function<void(asIScriptEngine*)> Reg;
	std::function<CUserType*()> Ser;
};

// Type store
//============
static const size_t TypeCount = 17;
extern const SFMLType Types[TypeCount];

// Helper functions
//==================
template<typename T>
void RegisterDrawable(asIScriptEngine* eng, const char* name);
template<typename T>
void RegisterTransformable(asIScriptEngine* eng, const char* name);
template<typename T>
void RegisterShape(asIScriptEngine* eng, const char* name);
template<typename T>
void RegisterSoundSource(asIScriptEngine* eng, const char* name);

}
}

#include "Shared.inl"
