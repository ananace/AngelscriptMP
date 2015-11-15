#pragma once

class ScriptManager;
class CSerializer;

namespace as
{
	class SFML
	{
	public:
		static void registerTypes(ScriptManager&);
		static void registerTypes(CSerializer&);
	};
}
