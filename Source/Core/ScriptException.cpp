#ifndef NDEBUG
#include "ScriptManager.hpp"
#include <sstream>

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