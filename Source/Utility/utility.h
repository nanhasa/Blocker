#pragma once

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

}
