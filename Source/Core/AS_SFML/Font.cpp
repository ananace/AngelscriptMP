#include "Shared.hpp"

#include <SFML/Graphics/Font.hpp>

namespace
{


}

void as::priv::RegFont(asIScriptEngine* eng)
{
	AS_ASSERT(eng->SetDefaultNamespace("sf"));

	AS_ASSERT(eng->RegisterObjectType("Font", 0, asOBJ_REF | asOBJ_NOCOUNT));

	AS_ASSERT(eng->SetDefaultNamespace(""));
}
