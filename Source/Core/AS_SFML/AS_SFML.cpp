#include "AS_SFML.hpp"
#include "Shared.hpp"

#include "../ScriptManager.hpp"

using namespace as;

void SFML::registerTypes(CSerializer& ser)
{
	for (auto& i : as::priv::Types)
	{
		if (!i.Ser)
			continue;

		ser.AddUserType(i.Ser(), i.Name);
	}
}

void SFML::registerTypes(ScriptManager& man)
{
	for (auto& i : as::priv::Types)
	{
		if (!i.Reg)
			continue;

		man.addExtension(i.Name, i.Reg);
	}
}
