#pragma once

#include <string>
#include <vector>

enum LOGGING_LEVEL { DEBUG, INFO, WARN, ERR, FATAL };

class Logger {
public:

	/**
	 * \brief Logger
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
	 * \param name
	 * \pre !name.empty()
	 */
	void initialize(const std::string& name);

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
	std::vector<LOGGING_LEVEL> m_enabledLogLevels;
	std::string m_logName;
	std::string m_filename;
	std::string m_configFilename; // Cannot be static as it may not be initialized if logging is used as static object

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
