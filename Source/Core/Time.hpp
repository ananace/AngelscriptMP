#pragma once

#include <chrono>
#include <iosfwd>

class ScriptManager;

#if !defined _MSC_VER || _MSC_VER >= 1900
typedef std::chrono::high_resolution_clock Clock;
#else
#pragma message("Compiling this project with Visual Studio 2015+ is recommended.")
#define NEEDS_HIGH_RESOLUTION_CLOCK
struct Clock
{
	typedef long long rep;
	typedef std::nano period;
	typedef std::chrono::nanoseconds duration;
	typedef std::chrono::time_point<duration> time_point;
	static const bool is_steady = true;

	static time_point now();
};
#endif
typedef Clock::time_point Timestamp;
typedef Clock::duration Timespan;

namespace Time
{
	extern Timespan getRunTime();
	extern Timespan getClockPrecision();
	extern void registerTimeTypes(ScriptManager&);
}

std::ostream& operator<<(std::ostream& os, const Timestamp&);
std::ostream& operator<<(std::ostream& os, const Timespan&);
