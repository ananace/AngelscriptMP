#include "Application.hpp"
#include "ResourceManager.hpp"

#include "States/IntroState.hpp"

#include <Core/Math.hpp>
#include <Core/Time.hpp>
#include <Core/FileWatcher.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include <Core/AS_Addons/scriptarray/scriptarray.h>
#include <Core/AS_Addons/scriptmath/scriptmath.h>
#include <Core/AS_Addons/scriptstdstring/scriptstdstring.h>
#include <Core/AS_SFML/AS_SFML.hpp>

#ifdef _WIN32
#include <codecvt>
#endif
#include <iostream>
#include <sstream>

namespace
{
	struct CArrayType : public CUserType
	{
		void Store(CSerializedValue *val, void *ptr)
		{
			CScriptArray *arr = (CScriptArray*)ptr;
			for (unsigned int i = 0; i < arr->GetSize(); i++)
				val->m_children.push_back(new CSerializedValue(val, "", "", arr->At(i), arr->GetElementTypeId()));
		}
		void Restore(CSerializedValue *val, void *ptr)
		{
			CScriptArray *arr = (CScriptArray*)ptr;
			arr->Resize(val->m_children.size());
			for (size_t i = 0; i < val->m_children.size(); ++i)
				val->m_children[i]->Restore(arr->At(i), arr->GetElementTypeId());
		}
	};

	/*
	struct CGridType : public CUserType
	{
		void Store(CSerializedValue *val, void *ptr)
		{
			CScriptGrid *grid = (CScriptGrid*)ptr;
			val->SetUserData(new unsigned int(grid->GetWidth()));

			for (unsigned int x = 0; x < grid->GetWidth(); ++x)
				for (unsigned int y = 0; y < grid->GetHeight(); ++y)
					val->m_children.push_back(new CSerializedValue(val, "", "", grid->At(x, y), grid->GetElementTypeId()));
		}
		void Restore(CSerializedValue *val, void *ptr)
		{
			CScriptGrid *grid = (CScriptGrid*)ptr;
			unsigned int width = *(unsigned int*)val->GetUserData();
			if (width == 0)
				return;

			grid->Resize(width, val->m_children.size() / width);
			for (size_t i = 0; i < val->m_children.size(); ++i)
				val->m_children[i]->Restore(grid->At(i / width, i % width), grid->GetElementTypeId());
		}
		void CleanupUserData(CSerializedValue *val)
		{
			unsigned int *buffer = (unsigned int*)val->GetUserData();
			delete buffer;
		}
	};
	*/

	template<typename T>
	void print(T in)
	{
		std::cout << in;
	}

#ifdef WIN32
	template<>
	void print(const std::string& str)
	{
		typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
		std::wcout << convert().from_bytes(str);
	}
#endif

	void printany(const void* ref, int type)
	{
		asIScriptContext* ctx = asGetActiveContext();
		asIObjectType* objType = ctx->GetEngine()->GetObjectTypeById(type);
		asIScriptFunction* func = objType->GetMethodByDecl("string ToString() const");
		std::string output;

		if (func)
		{
			asIScriptContext* callCtx = ctx->GetEngine()->RequestContext();
			callCtx->Prepare(func);
			callCtx->SetObject(const_cast<void*>(ref));
			callCtx->Execute();

			output = *reinterpret_cast<std::string*>(callCtx->GetReturnAddress());

			ctx->GetEngine()->ReturnContext(callCtx);
		}
		else
		{
			std::ostringstream oss;

			if (type & asTYPEID_OBJHANDLE)
				oss << "@";

			oss << objType->GetName();

			if (type & asTYPEID_SCRIPTOBJECT)
				oss << "[SO]";

			oss << "{" << ref << "}";

			output = oss.str();
		}

		std::cout << output;
	}

	template<typename T>
	void println(T in)
	{
		std::cout << in << std::endl;
	}

#ifdef WIN32
	template<>
	void println(const std::string& str)
	{
		typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
		std::wcout << convert().from_bytes(str) << std::endl;
	}
#endif

	void printlnany(void* ref, int type)
	{
		printany(ref, type);
		std::cout << std::endl;
	}

	void println()
	{
		std::cout << std::endl;
	}
}

Application::Application() :
	mState(mEngine)
{
	auto dur = Time::getClockPrecision();

	std::cout << Clock::now() << std::endl;
	std::cout << "Clock precision seems to be ~" << dur << std::endl;

	if (dur > std::chrono::milliseconds(2))
		std::cout << "This might lead to problems running the application on your platform..." << std::endl;

	mEngine.add<ScriptManager>();
	mEngine.add<sf::RenderWindow>();
	mEngine.add<FileWatcher>();
	mEngine.add<ResourceManager>();
	mEngine.add<sf::Font>();

	mEngine.set<Application>(this);
}

Application::~Application()
{
}

void Application::init()
{
	auto beg = Clock::now();
	mEngine.init();

	auto& font = mEngine.get<sf::Font>();
	font.loadFromFile("FiraSans-Regular.otf");


	auto& man = mEngine.get<ScriptManager>();

	man.addExtension("Array", [](asIScriptEngine* eng) { RegisterScriptArray(eng, true); });
	man.registerSerializedType("array", []() {
		return new CArrayType();
	});
	man.addExtension("Math", RegisterScriptMath);
	man.addExtension("String", RegisterStdString);
	man.registerSerializedType<std::string>("string");
	man.addExtension("String Utils", RegisterStdStringUtils);
	man.addExtension("Printing", [](asIScriptEngine* eng) {
		AS_ASSERT(eng->RegisterGlobalFunction("void print(int)", asFUNCTION(print<int>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(float)", asFUNCTION(print<float>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(string&in)", asFUNCTION(print<const std::string&>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(const ?&in)", asFUNCTION(printany), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println()", asFUNCTIONPR(println, (), void), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(int)", asFUNCTION(println<int>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(float)", asFUNCTION(println<float>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(string&in)", asFUNCTION(println<const std::string&>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(const ?&in)", asFUNCTION(printlnany), asCALL_CDECL));
	});
	Math::registerScriptData(man);
	Time::registerTimeTypes(man);
	as::SFML::registerTypes(man);
	mEngine.get<ResourceManager>().registerScript(man);

	man.init();

	man.getEngine()->SetUserData(&mEngine.get<sf::RenderWindow>(), 0x6AE1);
	man.getEngine()->SetUserData(&font, 0xF077);

	man.registerHook("Tick",   "void f(const Timespan&in)");
	man.registerHook("Update", "void f(const Timespan&in)");
	man.registerHook("Draw",   "void f(sf::Renderer@)");
	man.registerHook("DrawUI", "void f(sf::Renderer@)");

	man.registerHook("Keyboard.Key", "void f(sf::Keyboard::Key, bool)");
	man.registerHook("Joystick.Button", "void f(uint, uint, bool)");
	man.registerHook("Joystick.Moved", "void f(uint, sf::Joystick::Axis, float)");
	man.registerHook("Mouse.Button", "void f(const sf::Vec2&in, sf::Mouse::Button,bool)");
	man.registerHook("Mouse.Moved", "void f(const sf::Vec2&in)");
	man.registerHook("Text.Entered", "void f(uint)");

	auto& watch = mEngine.get<FileWatcher>();

	watch.addSource(".", true);

	auto end = Clock::now();
	std::cout << "Init took " << (end - beg) << std::endl;
}

void Application::run()
{
	std::cout << "Application started in " << Time::getRunTime() << std::endl;

	sf::Event ev;
	std::string modified;
	auto& window = mEngine.get<sf::RenderWindow>();
	auto& watch = mEngine.get<FileWatcher>();
	auto& man = mEngine.get<ScriptManager>();

	mState.primeRT(&window);

	window.create({ 800, 600 }, "Geometry Shaders are Go!", 7U, sf::ContextSettings(24, 8, 1, 4, 0));
	auto ctx = window.getSettings();

	sf::View uiView = window.getDefaultView(), gameView({}, { 0, 2500 });
	{
		sf::Vector2f size = (sf::Vector2f)window.getSize();
		uiView.setSize(size);
		uiView.setCenter(size / 2.f);

		gameView.setSize(gameView.getSize().y * (size.x / size.y), gameView.getSize().y);
	}
	window.setView(gameView);

	gameView = window.getView();

	mState.changeState<IntroState>();

	const Timespan tickLength = std::chrono::milliseconds(15);
	Timespan tickTime(0);
	Timestamp now = Clock::now(), nextGC = now + std::chrono::seconds(2);

	auto oldframe = now;

	while (window.isOpen())
	{
		now = Clock::now();
		Timespan dt = now - oldframe;
		oldframe = now;

		tickTime += dt;

		if (watch.pollChange(modified))
		{
			std::cout << modified << " was modified!" << std::endl;
			if (man.isLoaded(modified))
			{
				man.loadFromFile(modified);
			}
		}


		// -------------
		// Handle Events

		if (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::Resized)
			{
				sf::Vector2f size = (sf::Vector2f)window.getSize();
				uiView.setSize(size);
				uiView.setCenter(size / 2.f);

				gameView.setSize(gameView.getSize().y * (size.x / size.y), gameView.getSize().y);
			}
			else
			{
				mState.event(ev);
				/*
				if (ev.type == sf::Event::KeyPressed ||
					ev.type == sf::Event::KeyReleased)
				{
					bool pressed = ev.type == sf::Event::KeyPressed;

					man.runHook<int, bool>("Keyboard.Key", ev.key.code, pressed);
				}
				else if(ev.type == sf::Event::JoystickButtonPressed ||
					ev.type == sf::Event::JoystickButtonReleased)
				{
					bool pressed = ev.type == sf::Event::JoystickButtonPressed;

					man.runHook<uint32_t, uint32_t, bool>("Joystick.Button", ev.joystickButton.joystickId, ev.joystickButton.button, pressed);
				}
				else if (ev.type == sf::Event::JoystickMoved)
				{
					man.runHook<uint32_t, uint32_t, float>("Joystick.Moved", ev.joystickMove.joystickId, ev.joystickMove.axis, ev.joystickMove.position);
				}
				else if (ev.type == sf::Event::MouseButtonPressed ||
					ev.type == sf::Event::MouseButtonReleased)
				{
					bool pressed = ev.type == sf::Event::MouseButtonPressed;

					sf::Vector2f pos{ float(ev.mouseButton.x), float(ev.mouseButton.y) };
					man.runHook<const sf::Vector2f*, uint32_t, bool>("Mouse.Button", &pos, ev.mouseButton.button, pressed);
				}
				else if (ev.type == sf::Event::MouseMoved)
				{
					sf::Vector2f pos(sf::Mouse::getPosition(window));
					man.runHook<const sf::Vector2f*>("Mouse.Moved", &pos);
				}
				else if (ev.type == sf::Event::TextEntered)
				{
					man.runHook<uint32_t>("Text.Entered", ev.text.unicode);
				}
			*/}
		}


		// -----------
		// Run updates

		while (tickTime >= tickLength)
		{
			// Run fixed updates
			mState.tick(tickLength);
			man.runHook<const Timespan*>("Tick", &tickLength);

			tickTime -= tickLength;
		}

		// Run per-frame updates
		mState.update(dt);
		man.runHook<const Timespan*>("Update", &dt);


		// ----------------
		// Draw the results

		window.clear();

		// Draw things
		//window.setView(gameView);
		mState.draw(window);
		man.runHook<sf::RenderTarget*>("Draw", &window);
		gameView = window.getView();

		// Draw UI things
		window.setView(uiView);
		mState.drawUI(window);
		man.runHook<sf::RenderTarget*>("DrawUI", &window);

		window.setView(gameView);

		window.display();

		if (now > nextGC)
		{
			man.getEngine()->GarbageCollect(asGC_ONE_STEP);

			nextGC = now + std::chrono::seconds(2);
		}
	}
}

sf::RenderTarget& Application::getRT()
{
	return mEngine.get<sf::RenderWindow>();
}

template<>
int ScriptManager::setCTXArg<const std::string*>(asIScriptContext* ctx, uint32_t id, const std::string* arg)
{
	return ctx->SetArgObject(id, (std::string*)arg);
}
template<>
int ScriptManager::setCTXArg<const Timespan*>(asIScriptContext* ctx, uint32_t id, const Timespan* arg)
{
	return ctx->SetArgObject(id, (Timespan*)arg);
}
template<>
int ScriptManager::setCTXArg<sf::RenderTarget*>(asIScriptContext* ctx, uint32_t id, sf::RenderTarget* arg)
{
	return ctx->SetArgObject(id, arg);
}
