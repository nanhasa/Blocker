#pragma once

#include <sstream>
#include <string>

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

	// Utility function to return hex format of a number
	template<typename T>
	std::string toHex(T&& num)
	{
		std::stringstream ss;
		ss << "0x" << std::hex << std::forward<T>(num);
		return ss.str();
	}

	// Utility function to return dec format of a number
	template<typename T>
	std::string toDec(T&& num)
	{
		std::stringstream ss;
		ss << std::dec << std::forward<T>(num);
		return ss.str();
	}

	// Utility function to turn number to string
	// Returns empty string if parameter is not integral or floating point
	template<typename T>
	std::string toStr(T&& num)
	{
#pragma warning(suppress: 4127)
		if (!std::is_integral<T>::value && !std::is_floating_point<T>::value)
			return std::string();
		return std::to_string(std::forward<T>(num));
	}

} // namespace utility