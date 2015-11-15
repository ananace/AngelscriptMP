#pragma once

#include <angelscript.h>
#include <Core/AS_Addons/serializer/serializer.h>

#include <functional>
#include <list>
#include <string>
#include <tuple>
#include <unordered_map>

class ScriptManager
{
public:
	template<typename T>
	class CSimpleType : public CUserType
	{
	public:
		void Store(CSerializedValue *val, void *ptr);
		void Restore(CSerializedValue *val, void *ptr);
		void CleanupUserData(CSerializedValue *val);
	};

	typedef std::function<void(asIScriptEngine*)> ScriptExtensionFun;

	void addExtension(const std::string& name, const ScriptExtensionFun& function);
	template<typename T>
	void registerSerializedType(const std::string& name);
	void registerSerializedType(const std::string& name, const std::function<CUserType*()>& ser);

	void init();

	asIScriptEngine* getEngine();

private:
	std::list<std::tuple<std::string,ScriptExtensionFun>> mExtensions;
	std::unordered_map<std::string,std::function<CUserType*()>> mSerializers;
	asIScriptEngine* mEngine;
};

#include "ScriptManager.inl"
