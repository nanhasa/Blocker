#pragma once

#include <string>

#include "Utility/logger.h"

class StaticSafeLogger {
public:

	/**
	 * \brief StaticSafeLogger
	 * \param name
	 */
	explicit StaticSafeLogger(const std::string& name);

	/**
	 * \brief ~StaticSafeLogger
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
	 * \brief debug
	 * \param message
	 */
	void debug(const std::string& message);

	/**
	 * \brief info
	 * \param message
	 */
	void info(const std::string& message);

	/**
	 * \brief warn
	 * \param message
	 */
	void warn(const std::string& message);

	/**
	 * \brief error
	 * \param message
	 */
	void error(const std::string& message);

	/**
	 * \brief fatal
	 * \param message
	 */
	void fatal(const std::string& message);

private:
	Logger m_log;
	std::string m_name;
};
