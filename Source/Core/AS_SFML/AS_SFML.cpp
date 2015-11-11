#include "AS_SFML.hpp"
#include "Shared.hpp"

using namespace as;

SFML::SFML(asIScriptEngine* engine) :
	mFailures(0), mEngine(engine)
{
	registerScript();
}
SFML::SFML(SFML&& rhs) :
	mFailures(std::move(rhs.mFailures)), mEngine(std::move(rhs.mEngine))
{
	rhs.mEngine = nullptr;
}
SFML::~SFML()
{
	if (!mEngine)
		return;

	// TODO: Anything to do here?
}

bool SFML::isValid() const
{
	return mFailures == 0;
}

void SFML::registerTypes(CSerializer& ser) const
{
	for (auto& i : as::priv::Types)
	{
		if (!i.Ser)
			continue;

		ser.AddUserType(i.Ser(), i.Name);
	}
}

void SFML::registerScript()
{
	for (auto& i : as::priv::Types)
	{
		if (!i.Reg)
			continue;

		if (!i.Reg(mEngine))
			++mFailures;
	}

	mEngine->SetDefaultNamespace("");
}
