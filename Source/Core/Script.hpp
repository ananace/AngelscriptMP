#pragma once

#include <string>

class ScriptManager;

class Script
{
public:
	Script(ScriptManager& man, const std::string& file);
	~Script();

	bool reload();

private:
	std::string mFile;
	ScriptManager& mMan;
};
