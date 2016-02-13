#pragma once

#include <angelscript.h>
#include <Core/AS_Addons/serializer/serializer.h>

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

namespace sf { class InputStream; }

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
		Type_Autodetect,
		Type_Text,
		Type_Bytecode
	};

	typedef std::function<void(asIScriptObject*)> ScriptObjectCallbackFun;
	typedef std::function<void(asIScriptEngine*)> ScriptExtensionFun;
	typedef std::function<bool(asIScriptModule*)> ScriptPreLoadCallbackFun;


	// Object functions

	ScriptManager();
	ScriptManager(const ScriptManager&) = delete;
	~ScriptManager();

	ScriptManager& operator=(const ScriptManager&) = delete;

	const asIScriptEngine* getEngine() const;
	asIScriptEngine* getEngine();


	// Script interface functions

	void addExtension(const std::string& name, const ScriptExtensionFun& function);
	template<typename T>
	void registerSerializedType(const std::string& name);
	void registerSerializedType(const std::string& name, const std::function<CUserType*()>& ser);

	void init();


	// Script management functions

	bool isLoaded(const std::string& name) const;

	bool loadFromFile(const std::string& file, ScriptType type = Type_Autodetect);
	bool loadFromMemory(const std::string& name, const void* data, size_t len, ScriptType type = Type_Autodetect);
	bool loadFromStream(const std::string& name, sf::InputStream& stream, ScriptType type = Type_Autodetect);

	void unload(const std::string& name);
	void unloadAll();

	bool isDefined(const std::string& define) const;
	void addDefine(const std::string& define);
	void removeDefine(const std::string& define);
	void clearDefines();


	// Utility functions

	void clearPreLoadCallbacks();
	void removePreLoadCallback(const std::string& id);
	void addPreLoadCallback(const std::string& id, const ScriptPreLoadCallbackFun& func);
	bool hasPreLoadCallback(const std::string& id) const;

	void addChangeNotice(asIScriptObject* obj, const ScriptObjectCallbackFun& callback);
	void removeChangeNotice(asIScriptObject* obj);


	// Script hook

	template<typename... Args>
	void runHook(const std::string& name, Args... args);
	template<typename T>
	static int setCTXArg(asIScriptContext*, uint32_t id, T arg);

	void registerHook(const std::string& name, const std::string& decl);

	bool addHook(const std::string& hook, asIScriptFunction* func, asIScriptObject* obj);
	bool removeHook(const std::string& hook, asIScriptFunction* func, asIScriptObject* obj);

private:
	struct Script
	{
		std::string Name;
		bool DirectLoad;
	};
	struct ChangeNotice
	{
		asILockableSharedBool* WeakRef;
		std::function<void(asIScriptObject*)> Callback;
	};
	struct ScriptHook
	{
		asIScriptFunction* Function;
		asIScriptObject* Object;
		asILockableSharedBool* WeakRef;
	};

	void addHookFromScript(const std::string& hook, const std::string& func);
	void removeHookFromScript(const std::string& hook, const std::string& func);
	
	std::list<std::string> mDefines;
	std::unordered_map<asIScriptObject*, ChangeNotice> mChangeNotice;
	std::list<std::pair<std::string, ScriptExtensionFun>> mExtensions;
	std::unordered_map<std::string, Script> mScripts;
	std::unordered_map<std::string, std::function<CUserType*()>> mSerializers;
	std::unordered_map<std::string, std::string> mRegisteredHooks;
	std::unordered_map<std::string, std::list<ScriptHook>> mScriptHooks;
	std::unordered_map<std::string, ScriptPreLoadCallbackFun> mPreLoadCallbacks;
	asIScriptEngine* mEngine;
};

#include "ScriptManager.inl"
