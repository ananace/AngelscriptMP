#include "Script.hpp"
#include "ScriptManager.hpp"

Script::Script(ScriptManager& man, const std::string& file) :
	mFile(file), mMan(man)
{
}

Script::~Script()
{
}

bool Script::reload()
{
	// Load script from file using scriptbuilder retrieved from Script Manager
	// If already loaded, serialize existing data first, and restore script data afterwards

	return false;
}
