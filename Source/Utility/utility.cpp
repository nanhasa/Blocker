#include <chrono>

#include "Utility/utility.h"

long utility::timestampMs()
{
	using namespace std::chrono;
	const auto now = time_point_cast<milliseconds>(system_clock::now());
	// Convert time_point to signed integral type
	return static_cast<long>(now.time_since_epoch().count());
}

float utility::deltaTimeSec(long timestamp)
{
	return static_cast<float>((timestampMs() - timestamp) / 1000);
}

int utility::deltaTimeMs(long timestamp)
{
	return static_cast<int>(timestampMs() - timestamp);
}
