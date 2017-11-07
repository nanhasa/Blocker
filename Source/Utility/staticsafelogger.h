#pragma once

#include <string>

#include "Utility/logger.h"

class StaticSafeLogger {
public:

	/**
	 * \brief Constructor. Creates empty Logger object which is initialized when logger is actually called
	 * \param name Name of logger used when fetching initialization info from logconfig.json
	 */
	explicit StaticSafeLogger(const std::string& name);

	/**
	 * \brief Destructor
	 */
	~StaticSafeLogger();

	// Delete copy and assignment, but define move assignment
	StaticSafeLogger(StaticSafeLogger const&) = delete;
	StaticSafeLogger& operator=(StaticSafeLogger const&) = delete;

	/**
	 * \brief operator=
	 * \param rhs
	 * \return
	 */
	StaticSafeLogger& operator=(StaticSafeLogger&& rhs) noexcept;

	/**
	 * \brief Used to write debug level message to log. Does nothing if debug level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void debug(const std::string& message);

	/**
	 * \brief Used to write debug level message to log. Does nothing if debug level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void debug(const std::string& functionName, const std::string& message);

	/**
	 * \brief Used to write info level message to log. Does nothing if info level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void info(const std::string& message);

	/**
	 * \brief Used to write info level message to log. Does nothing if info level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void info(const std::string& functionName, const std::string& message);

	/**
	 * \brief Used to write warn level message to log. Does nothing if warn level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void warn(const std::string& message);

	/**
	 * \brief Used to write warn level message to log. Does nothing if warn level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void warn(const std::string& functionName, const std::string& message);

	/**
	 * \brief Used to write error level message to log. Does nothing if error level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void error(const std::string& message);

	/**
	 * \brief Used to write error level message to log. Does nothing if error level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void error(const std::string& functionName, const std::string& message);

	/**
	 * \brief Used to write fatal level message to log. Does nothing if fatal level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void fatal(const std::string& message);

	/**
	 * \brief Used to write fatal level message to log. Does nothing if fatal level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void fatal(const std::string& functionName, const std::string& message);

private:
	Logger m_log;		//!< Actual logger that is guarded against static usage
	std::string m_name;	//!< Name of logger, used to fetch data from log config file
};
