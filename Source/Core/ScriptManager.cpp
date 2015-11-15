#include "ScriptManager.hpp"

#include <angelscript.h>

#include <iostream>

void error(const asSMessageInfo* msg)
{
	std::cerr << "Angelscript ";
	switch (msg->type)
	{
		case asMSGTYPE_ERROR: std::cerr << "error"; break;
		case asMSGTYPE_WARNING: std::cerr << "warning"; break;
		case asMSGTYPE_INFORMATION: std::cerr << "info"; break;
	}
	std::cerr << ";" << std::endl
		<< msg->message << std::endl;
}

void ScriptManager::addExtension(const std::string& name, const ScriptExtensionFun& function)
{
	mExtensions.emplace_back(name, function);
}

void ScriptManager::init()
{
	asIScriptEngine* eng = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	eng->SetMessageCallback(asFUNCTION(error), nullptr, asCALL_CDECL);

	uint32_t enumCount = 0, funcCount = 0, typeCount = 0;

	for (auto& ext : mExtensions)
	{
		std::cout << "Registering " << std::get<0>(ext) << std::endl;
		std::get<1>(ext)(eng);

		{
			auto count = eng->GetEnumCount();
			if (count != enumCount)
			std::cout << "> " << (count - enumCount) << " new enum(s)" << std::endl;
			enumCount = count;
		}
		{
			auto count = eng->GetGlobalFunctionCount();
			if (count != funcCount)
			std::cout << "> " << (count - funcCount) << " new global function(s)" << std::endl;
			funcCount = count;
		}
		{
			auto count = eng->GetObjectTypeCount();
			if (count != typeCount)
			std::cout << "> " << (count - typeCount) << " new object type(s)" << std::endl;
			typeCount = count;
		}
	}

	eng->ClearMessageCallback();
	mEngine = eng;
}

asIScriptEngine* ScriptManager::getEngine()
{
	return mEngine;
}
