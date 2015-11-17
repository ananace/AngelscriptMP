#include "ScriptManager.hpp"

#include <angelscript.h>

#include <iostream>

void error(const asSMessageInfo* msg)
{
	if (msg->type == asMSGTYPE_ERROR)
		return;

	std::cerr << "Angelscript ";
	switch (msg->type)
	{
		case asMSGTYPE_ERROR: std::cerr << "error"; break;
		case asMSGTYPE_WARNING: std::cerr << "warning"; break;
		case asMSGTYPE_INFORMATION: std::cerr << "info"; break;
	}
	if (msg->section && msg->section[0] != 0)
		std::cerr << " (" << msg->section << ":" << msg->row << ":" << msg->col << ")";
	std::cerr << ";" << std::endl
		<< ">  " << msg->message << std::endl;
}

void ScriptManager::addExtension(const std::string& name, const ScriptExtensionFun& function)
{
	mExtensions.emplace_back(name, function);
}
void ScriptManager::registerSerializedType(const std::string& name, const std::function<CUserType*()>& ser)
{
	mSerializers[name] = ser;
}

void ScriptManager::init()
{
	asIScriptEngine* eng = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	eng->SetMessageCallback(asFUNCTION(error), nullptr, asCALL_CDECL);

#ifndef NDEBUG
	std::unordered_map<std::string, uint32_t> counts;
#define CHECK_COUNT(Name, Check) do { \
	auto cnt = eng->Get ## Check ## Count(); \
	if (counts.count(Name) > 0 && cnt != counts.at(Name)) \
	{ \
		std::cout << ">  " << (cnt - counts.at(Name)) << " new " Name "(s)" << std::endl; \
	} \
	counts[Name] = cnt; \
} while (false);
#else
#define CHECK_COUNT(A, B)
#endif
	for (auto& ext : mExtensions)
	{
		std::cout << "Registering " << std::get<0>(ext) << "..." << std::endl;
		try
		{
			std::get<1>(ext)(eng);

			CHECK_COUNT("enum", Enum);
			CHECK_COUNT("funcdef", Funcdef);
			CHECK_COUNT("global function", GlobalFunction);
			CHECK_COUNT("global property", GlobalProperty);
			CHECK_COUNT("object type", ObjectType);
			CHECK_COUNT("typedef", Typedef);
		}
		catch (const std::runtime_error& ex)
		{
			std::cerr << std::string(ex.what()) << std::endl;
			std::cout << "Failed to register " << std::get<0>(ext) << std::endl;
		}
	}
#undef CHECK_COUNT

	eng->ClearMessageCallback();
	mEngine = eng;
}

asIScriptEngine* ScriptManager::getEngine()
{
	return mEngine;
}
