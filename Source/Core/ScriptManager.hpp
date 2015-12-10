#pragma once

#include <angelscript.h>
#include <Core/AS_Addons/serializer/serializer.h>
#include <Core/AS_Addons/scriptbuilder/scriptbuilder.h>

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#ifndef NDEBUG
#include <stdexcept>

#define AS_ASSERT(f) do { int __r = (f); if (__r < 0) throw ASException(#f, __r, __FILE__, __LINE__); } while (false)

namespace sf { class InputStream; }

class ASException : public std::runtime_error
{
public:
	ASException(const std::string& message, int errcode, const std::string& file, int line);
	~ASException() = default;

	const char* what() const noexcept;

	static const char* GetMessage(int code) noexcept;

private:
	std::string mMessage;
};
#else
#define AS_ASSERT(f) f
#endif

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

	enum ScriptType
	{
		Type_Text,
		Type_Bytecode
	};

	typedef std::function<void(asIScriptEngine*)> ScriptExtensionFun;

	void addExtension(const std::string& name, const ScriptExtensionFun& function);
	template<typename T>
	void registerSerializedType(const std::string& name);
	void registerSerializedType(const std::string& name, const std::function<CUserType*()>& ser);

	bool loadFromFile(const std::string& file, ScriptType type = Type_Text);
	bool loadFromMemory(const std::string& name, const void* data, size_t len, ScriptType type = Type_Text);
	bool loadFromStream(const std::string& name, sf::InputStream& stream, ScriptType type = Type_Text);

	void addDefine(const std::string& define);

	void init();

	asIScriptEngine* getEngine();

private:
	struct Script
	{
		std::string Name;
		bool DirectLoad;
	};

	std::list<asIScriptObject*> mObjects;
	std::list<std::pair<std::string, ScriptExtensionFun>> mExtensions;
	std::unordered_map<std::string, Script> mScripts;
	std::unordered_map<std::string, std::function<CUserType*()>> mSerializers;
	asIScriptEngine* mEngine;
	CScriptBuilder mBuilder;
};

#include "ScriptManager.inl"
