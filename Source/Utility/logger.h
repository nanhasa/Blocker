#pragma once

#include <sstream>
#include <string>
#include <vector>

class Logger {
public:

	/**
	 * \brief Constructor
	 * \param name Name of logger used when fetching initialization info from logconfig.json
	 */
	explicit Logger(const std::string& name);

	/**
	 * \brief Destructor
	 */
	~Logger();

	// Delete copy and move constructors
	Logger(Logger const&) = delete;
	Logger& operator=(Logger const&) = delete;

	/**
	 * \brief Used write log entry for debug level. Does not do anything if DEBUG level is not initialized.
	 * \param message Entry to be written to log
	 */
	void debug(const std::string& message) const;

	/**
	 * \brief Used write log entry for info level. Does not do anything if INFO level is not initialized.
	 * \param message Entry to be written to log
	 */
	void info(const std::string& message) const;

	/**
	 * \brief Used write log entry for warn level. Does not do anything if WARN level is not initialized.
	 * \param message Entry to be written to log
	 */
	void warn(const std::string& message) const;

	/**
	 * \brief Used write log entry for error level. Does not do anything if ERROR level is not initialized.
	 * \param message Entry to be written to log
	 */
	void error(const std::string& message) const;

	/**
	 * \brief Used write log entry for fatal level. Does not do anything if FATAL level is not initialized.
	 * \param message Entry to be written to log
	 */
	void fatal(const std::string& message) const;

private:
	enum LOGGING_LEVEL {DEBUG, INFO, WARN, ERR, FATAL};
	std::vector<LOGGING_LEVEL> m_enabledLogLevels;
	std::string m_logName;
	std::string m_filename;
	const std::string m_configFilename; // Cannot be static as it may not be initialized if logging is used as static object

	/**
	 * \brief Called to see if logging level is enabled for logger
	 * \param lvl Logging level to be tested
	 * \return True if level is enabled, otherwise False
	 */
	bool isEnabled(LOGGING_LEVEL lvl) const;

	/**
	 * \brief Used to write log file
	 * \param lvl Log level to be written in log entry
	 * \param message Entry to be written to log
	 * \post !ostream.is_open()
	 */
	void write(LOGGING_LEVEL lvl, const std::string& message) const;

	/**
	 * \brief Used to get timestamp in string
	 * \return Timestamp in yyyy-mm-dd 24hh:mm:ss
	 */
	static std::string datetime();

	/**
	 * \brief Used to transform logging level to string format
	 * \param lvl Logging level to be transformed
	 * \return String format of logging level
	 */
	static std::string levelToFixedString(LOGGING_LEVEL lvl);
};

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
