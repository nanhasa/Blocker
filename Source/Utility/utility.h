#pragma once

namespace utility {

	/**
	 * \brief Used to get timestamp in milliseconds, thread safe
	 * \return Current time in milliseconds
	 */
	long timestampMs();

	/**
	 * \brief deltaTimeSec
	 * \param timestamp
	 * \return Elapsed seconds since the parameter value
	 */
	float deltaTimeSec(long timestamp);

	/**
	 * \brief deltaTimeMs
	 * \param timestamp
	 * \return Elapsed milliseconds since the parameter value
	 */
	int deltaTimeMs(long timestamp);
}
