#include "Application.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include <Core/AS_Addons/scriptarray/scriptarray.h>
#include <Core/AS_Addons/scriptmath/scriptmath.h>
#include <Core/AS_Addons/scriptstdstring/scriptstdstring.h>
#include <Core/AS_SFML/AS_SFML.hpp>


Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	mMan.addExtension("Array", [](asIScriptEngine* eng) { RegisterScriptArray(eng, true); });
	mMan.addExtension("Math", RegisterScriptMath);
	mMan.addExtension("String", RegisterStdString);
	mMan.addExtension("String Utils", RegisterStdStringUtils);
	as::SFML::registerTypes(mMan);

	mMan.init();

	mWindow.create({ 800, 600 }, "AngelscriptMP Client");
}

void Application::run()
{
	sf::Event ev;

	while (mWindow.isOpen())
	{
		if (mWindow.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				mWindow.close();
		}

		mWindow.clear();

		mWindow.display();
	}
}

sf::RenderTarget& Application::getRT()
{
	return mWindow;
}
