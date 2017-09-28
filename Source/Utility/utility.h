#pragma once

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

namespace utility {

	/**
	 * \brief timeSinceEpoch
	 * \return 
	 */
	long timeSinceEpoch();

	/**
	 * \brief Used to get timestamp in milliseconds, thread safe
	 * \return Current time in milliseconds
	 */
	int timestampMs();

	/**
	 * \brief deltaTimeSec
	 * \param timestamp
	 * \return Elapsed seconds since the parameter value
	 */
	float deltaTimeSec(int timestamp);

	/**
	 * \brief deltaTimeMs
	 * \param timestamp
	 * \return Elapsed milliseconds since the parameter value
	 */
	int deltaTimeMs(int timestamp);

	/**
	 * \brief lerp
	 * \param start
	 * \param end
	 * \param percentage
	 * \return 
	 */
	glm::vec3 lerp(const glm::vec3& start, const glm::vec3& end, float percentage);
}
