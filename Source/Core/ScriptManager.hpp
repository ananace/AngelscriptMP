#pragma once

#include <functional>
#include <list>
#include <string>
#include <tuple>

class asIScriptEngine;

class ScriptManager
{
public:
	typedef std::function<void(asIScriptEngine*)> ScriptExtensionFun;

	void addExtension(const std::string& name, const ScriptExtensionFun& function);

	void init();

	asIScriptEngine* getEngine();

private:
	std::list<std::tuple<std::string,ScriptExtensionFun>> mExtensions;
	asIScriptEngine* mEngine;
};
