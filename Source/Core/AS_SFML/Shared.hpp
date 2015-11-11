#pragma once

#include <angelscript.h>
#include <Core/AS_Addons/serializer/serializer.h>

#include <functional>

#define AS_ASSERT(f) if ((f) < 0) return false

namespace as
{
namespace priv
{

// Registering functions
//=======================
extern bool RegVec2(asIScriptEngine*);
extern bool RegVec3(asIScriptEngine*);
extern bool RegRect(asIScriptEngine*);
extern bool RegTexture(asIScriptEngine*);
extern bool RegCircleShape(asIScriptEngine*);
extern bool RegRectangleShape(asIScriptEngine*);
extern bool RegConvexShape(asIScriptEngine*);
extern bool RegSprite(asIScriptEngine*);
extern bool RegText(asIScriptEngine*);
extern bool RegMusic(asIScriptEngine*);
extern bool RegSound(asIScriptEngine*);
extern bool RegListener(asIScriptEngine*);

// Wrapper struct
//================
struct SFMLType
{
	const char* Name;
	std::function<bool(asIScriptEngine*)> Reg;
	std::function<CUserType*()> Ser;
};

// Type store
//============
static const size_t TypeCount = 12;
extern const SFMLType Types[TypeCount];

// Helper functions
//==================
template<typename T>
bool RegisterDrawable(asIScriptEngine* eng, const char* name);
template<typename T>
bool RegisterTransformable(asIScriptEngine* eng, const char* name);
template<typename T>
bool RegisterShape(asIScriptEngine* eng, const char* name);
template<typename T>
bool RegisterSoundSource(asIScriptEngine* eng, const char* name);

// Helper classes
//================
template<typename T>
struct CSFMLType : public CUserType
{
public:
	void Store(CSerializedValue *val, void *ptr);
	void Restore(CSerializedValue *val, void *ptr);
	void CleanupUserData(CSerializedValue *val);
};

}
}

#include "Shared.inl"
