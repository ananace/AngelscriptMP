#include "Application.hpp"

#include <Core/CPUID.hpp>

int main(int argc, char** argv)
{
#ifdef WIN32
	setlocale(LC_ALL, "");
#endif

	CPUFeatures::print();

	Application app;

	app.init();
	app.run();

	return 0;
}
