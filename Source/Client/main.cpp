#include "Application.hpp"

int main(int argc, char** argv)
{
#ifdef WIN32
	setlocale(LC_ALL, "");
#endif

	Application app;

	app.init();
	app.run();

	return 0;
}
