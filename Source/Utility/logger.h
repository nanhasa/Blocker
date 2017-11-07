#pragma once

#include <string>
#include <vector>

enum LOGGING_LEVEL { DEBUG, INFO, WARN, ERR, FATAL };

class Logger {
public:

	/**
	 * \brief Constructor. Creates an empty logger that does nothing until initialized.
	 */
	Logger();

	/**
	 * \brief Constructor
	 * \param name Name of logger used when fetching initialization info from logconfig.json
	 */
	explicit Logger(const std::string& name);

	/**
	 * \brief Destructor
	 */
	~Logger();

	// Delete copy and assignment, but define move assignment
	Logger(Logger const&) = delete;
	Logger& operator=(Logger const&) = delete;

	/**
	 * \brief operator=
	 * \param rhs
	 * \return
	 */
	Logger& operator=(Logger&& rhs) noexcept;

	/**
	 * \brief initialize
	 * \param name Name of logger used when fetching initialization info from logconfig.json
	 * \pre !name.empty()
	 */
	void initialize(const std::string& name);

	/**
	 * \brief Used to write debug level message to log. Does nothing if debug level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void debug(const std::string& message) const;

	/**
	 * \brief Used to write debug level message to log. Does nothing if debug level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void debug(const std::string& functionName, const std::string& message) const;

	/**
	 * \brief Used to write info level message to log. Does nothing if info level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void info(const std::string& message) const;

	/**
	 * \brief Used to write info level message to log. Does nothing if info level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void info(const std::string& functionName, const std::string& message) const;

	/**
	 * \brief Used to write warn level message to log. Does nothing if warn level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void warn(const std::string& message) const;

	/**
	 * \brief Used to write warn level message to log. Does nothing if warn level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void warn(const std::string& functionName, const std::string& message) const;

	/**
	 * \brief Used to write error level message to log. Does nothing if error level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void error(const std::string& message) const;

	/**
	 * \brief Used to write error level message to log. Does nothing if error level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void error(const std::string& functionName, const std::string& message) const;

	/**
	 * \brief Used to write fatal level message to log. Does nothing if fatal level is not active in logconfig.json
	 * \param message Entry to be written to log
	 */
	void fatal(const std::string& message) const;

	/**
	 * \brief Used to write fatal level message to log. Does nothing if fatal level is not active in logconfig.json
	 * \param functionName Function name written to file
	 * \param message Entry to be written to log
	 */
	void fatal(const std::string& functionName, const std::string& message) const;

private:
	std::vector<LOGGING_LEVEL> m_enabledLogLevels;	//!< Logging levels enabled for the logger
	std::string m_logName;							//!< Name of logger, used to fetch data from log config file
	std::string m_filename;							//!< Name of log output file
	std::string m_configFilename;					//!< Name of config file for logger

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
	 */
	void write(LOGGING_LEVEL lvl, const std::string& message) const;
};
