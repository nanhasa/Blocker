#include "Utility/utility.h"

#include <chrono>

static const auto gameStartTime = std::chrono::system_clock::now();

long utility::timeSinceEpoch()
{
	using namespace std::chrono;
	const auto now = time_point_cast<milliseconds>(system_clock::now());
	// Convert time_point to signed integral type
	return static_cast<long>(now.time_since_epoch().count());
}

int utility::timestampMs()
{
	using namespace std::chrono;
	auto now = system_clock::now();
	return static_cast<int>(duration_cast<milliseconds>(now - gameStartTime).count());
}

float utility::deltaTimeSec(int timestamp)
{
	return static_cast<float>((timestampMs() - timestamp) / 1000);
}

int utility::deltaTimeMs(int timestamp)
{
	return static_cast<int>(timestampMs() - timestamp);
}

glm::vec3 utility::lerp(const glm::vec3 & start, const glm::vec3 & end, float percentage)
{
	// Clamp percentage between 0..1
	if (percentage > 1.0f) percentage = 1.0f;
	if (percentage < 0.0f) percentage = 0.0f;

	// Start to end vector
	auto startToEnd = end - start;

	// Base vector to start and add percentage of startToEnd vector
	return start + startToEnd * percentage;
}
