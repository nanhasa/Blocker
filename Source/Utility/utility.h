#pragma once

#include <string>

namespace utility {

	/**
	 * \brief Used to get timestamp in milliseconds, thread safe
	 * \return Current time in milliseconds
	 */
	long timestampMs();

	float deltaTimeSec(long timestamp);

	int deltaTimeMs(long timestamp);

	std::string datetime();
}
