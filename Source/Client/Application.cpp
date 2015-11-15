#include "Application.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include <Core/AS_Addons/scriptarray/scriptarray.h>
#include <Core/AS_Addons/scriptmath/scriptmath.h>
#include <Core/AS_Addons/scriptstdstring/scriptstdstring.h>
#include <Core/AS_SFML/AS_SFML.hpp>

#include <atomic>

Application::Application()
{
	asPrepareMultithread();
}

Application::~Application()
{
	asUnprepareMultithread();
}

void Application::init()
{
	mMan.addExtension("Array", [](asIScriptEngine* eng) { RegisterScriptArray(eng, true); });
	mMan.addExtension("Math", RegisterScriptMath);
	mMan.addExtension("String", RegisterStdString);
	mMan.addExtension("String Utils", RegisterStdStringUtils);
	as::SFML::registerTypes(mMan);

	mMan.init();

	mWindow.create(sf::VideoMode(800, 600), "AngelscriptMP Client");
}

std::atomic_bool runs;

void Application::run()
{
	sf::Thread eventT(&Application::eventThread, this);
	sf::Thread drawT(&Application::drawThread, this);
	sf::Thread updateT(&Application::updateThread, this);

	runs = true;

	eventT.launch();
	drawT.launch();
	updateT.launch();

	updateT.wait();
	drawT.wait();
	eventT.wait();
}

sf::RenderTarget& Application::getRT()
{
	return mWindow;
}

void Application::eventThread()
{
	sf::Event ev;

	while (runs)
	{
		mWindow.waitEvent(ev);

		if (ev.type == sf::Event::Closed)
		{
			runs = false;
			mWindow.close();
		}
	}
}
void Application::drawThread()
{
	while (runs)
	{
		mWindow.clear();

		mWindow.display();

		sf::sleep(sf::milliseconds(1));
	}
}
void Application::updateThread()
{
	while (runs)
	{
		sf::sleep(sf::milliseconds(1));
	}
}
