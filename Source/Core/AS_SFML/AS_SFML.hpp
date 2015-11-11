#pragma once

class asIScriptEngine;
class CSerializer;

namespace as
{
	class SFML
	{
	public:
		SFML(asIScriptEngine* engine);
		SFML(const SFML&) = delete;
		SFML(SFML&&);
		~SFML();

		SFML& operator=(const SFML&) = delete;

		bool isValid() const;
		void registerTypes(CSerializer&) const;

	private:
		void registerScript();

		unsigned char mFailures;
		asIScriptEngine* mEngine;
	};
}
