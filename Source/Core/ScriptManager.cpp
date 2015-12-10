#include "ScriptManager.hpp"

#include <SFML/System/InputStream.hpp>

#include <angelscript.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#ifndef NDEBUG
ASException::ASException(const std::string& message, int errcode, const std::string& file, int line) : std::runtime_error("")
{
	std::ostringstream oss;

	if (file.empty())
		oss << message << " (" << errcode << " - " << std::string(GetMessage(errcode)) << ")";
	else
		oss << file << ":" << line << ": Call to AngelScript failed with error " << (-errcode) << " - " << std::string(GetMessage(errcode)) << std::endl << ">  The call was: '" << message << "'";

	mMessage = oss.str();
}

const char* ASException::what() const noexcept
{
	return mMessage.c_str();
}

inline const char* ASException::GetMessage(int code) noexcept
{
	switch (code)
	{
	case asSUCCESS:
		return "Success";
	case asERROR:
		return "Error";
	case asCONTEXT_ACTIVE:
		return "Context is active";
	case asCONTEXT_NOT_FINISHED:
		return "Context is not finished";
	case asCONTEXT_NOT_PREPARED:
		return "Context is not prepared";
	case asINVALID_ARG:
		return "Invalid argument";
	case asNO_FUNCTION:
		return "No function";
	case asNOT_SUPPORTED:
		return "Not supported";
	case asINVALID_NAME:
		return "Invalid name";
	case asNAME_TAKEN:
		return "Name is taken";
	case asINVALID_DECLARATION:
		return "Invalid declaration";
	case asINVALID_OBJECT:
		return "Invalid object";
	case asINVALID_TYPE:
		return "Invalid type";
	case asALREADY_REGISTERED:
		return "Already registered";
	case asMULTIPLE_FUNCTIONS:
		return "Multiple functions";
	case asNO_MODULE:
		return "No module";
	case asNO_GLOBAL_VAR:
		return "No global variable";
	case asINVALID_CONFIGURATION:
		return "Invalid configuration";
	case asINVALID_INTERFACE:
		return "Invalid interface";
	case asCANT_BIND_ALL_FUNCTIONS:
		return "Can't bind all functions";
	case asLOWER_ARRAY_DIMENSION_NOT_REGISTERED:
		return "Lower array dimension not registered";
	case asWRONG_CONFIG_GROUP:
		return "Wrong config group";
	case asCONFIG_GROUP_IS_IN_USE:
		return "Config group is in use";
	case asILLEGAL_BEHAVIOUR_FOR_TYPE:
		return "Illegal behaviour for type";
	case asWRONG_CALLING_CONV:
		return "Wrong calling convention";
	case asBUILD_IN_PROGRESS:
		return "Build in progress";
	case asINIT_GLOBAL_VARS_FAILED:
		return "Initializing global variables failed";
	case asOUT_OF_MEMORY:
		return "Out of memory";
	case asMODULE_IS_IN_USE:
		return "Module is in use";

	default:
		return "";
	}
}
#endif

namespace
{

	void error(const asSMessageInfo* msg)
	{
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

	class BytecodeStore : public asIBinaryStream
	{
	public:
		BytecodeStore() : mTellg(0) { }
		BytecodeStore(const char* data, size_t len) :
			mTellg(0)
		{
			mStore.assign(data, data + len);
		}

		void Read(void *ptr, asUINT size)
		{
			char* data = (char*)ptr;

			for (uint32_t i = 0; i < size; ++i)
			{
				data[i] = mStore[mTellg + i];
			}

			mTellg += size;
		}
		void Write(const void *ptr, asUINT size)
		{
			const char* data = (const char*)ptr;

			for (uint32_t i = 0; i < size; ++i)
				mStore.push_back(data[i]);
		}

	private:
		std::vector<char> mStore;
		size_t mTellg;
	};
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

	//eng->ClearMessageCallback();
	mEngine = eng;
}

bool ScriptManager::loadFromFile(const std::string& file, ScriptType type)
{
	std::ifstream ifs(file.c_str());
	if (!ifs)
		return false;

	ifs.seekg(0, std::ios::end);
	size_t len = size_t(ifs.tellg());
	ifs.seekg(0, std::ios::beg);

	std::vector<char> data(len);
	ifs.read(&data[0], len);

	// Remove extraneous nullbytes
	auto it = std::remove(data.begin(), data.end(), '\0');
	if (it != data.end())
		data.erase(it, data.end());

	return loadFromMemory(file, &data[0], data.size(), type);
}
bool ScriptManager::loadFromMemory(const std::string& name, const void* data, size_t len, ScriptType type)
{
	bool reload = mScripts.count(name) > 0;

	asIScriptModule* module = mEngine->GetModule(name.c_str(), asGM_ONLY_IF_EXISTS);
	CSerializer serial;

	if (reload && module)
	{
		for (auto& reg : mSerializers)
			serial.AddUserType(reg.second(), reg.first);

		serial.Store(module);
	}

	BytecodeStore bcode;
	if (type == Type_Text)
	{
		static const char* scratchName = "!!ScratchSpace!!";

		mEngine->DiscardModule(scratchName);
		mBuilder.StartNewModule(mEngine, scratchName);

		mBuilder.AddSectionFromMemory(name.c_str(), (const char*)data, len);

		int r = mBuilder.BuildModule();
		if (r < 0)
		{
			puts(ASException::GetMessage(r));
			return false;
		}

#ifdef NDEBUG
		mBuilder.GetModule()->SaveByteCode(&bcode, true);
#else
		mBuilder.GetModule()->SaveByteCode(&bcode, false);
#endif
	}
	else
	{
		bcode = BytecodeStore((const char*)data, len);
	}

	if (module)
		module->Discard();
	module = mEngine->GetModule(name.c_str(), asGM_ALWAYS_CREATE);
	int r = module->LoadByteCode(&bcode);
	if (r < 0)
		return false;

	if (reload)
	{
		serial.Restore(module);

		mEngine->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE);

		auto* fun = module->GetFunctionByName("OnReload");
		if (fun)
		{
			auto* ctx = mEngine->RequestContext();
			ctx->Prepare(fun);

			ctx->Execute();

			ctx->Unprepare();
			mEngine->ReturnContext(ctx);
		}
	}
	else
	{
		auto* fun = module->GetFunctionByName("OnLoad");
		if (fun)
		{
			auto* ctx = mEngine->RequestContext();
			ctx->Prepare(fun);

			ctx->Execute();

			ctx->Unprepare();
			mEngine->ReturnContext(ctx);
		}
	}


	return true;
}
bool ScriptManager::loadFromStream(const std::string& name, sf::InputStream& stream, ScriptType type)
{
	auto len = stream.getSize();
	std::vector<char> data(len);
	stream.read(&data[0], len);

	return loadFromMemory(name, &data[0], len, type);
}


asIScriptEngine* ScriptManager::getEngine()
{
	return mEngine;
}
