#include "staticsafelogger.h"

StaticSafeLogger::StaticSafeLogger(const std::string& name) : m_log(), m_name(name) {}

StaticSafeLogger::~StaticSafeLogger() {}

StaticSafeLogger & StaticSafeLogger::operator=(StaticSafeLogger && rhs) noexcept
{
	m_log = std::move(rhs.m_log);
	m_name = std::move(rhs.m_name);
	return *this;
}

void StaticSafeLogger::debug(const std::string & message)
{
	m_log.initialize(m_name);
	m_log.debug(message);
}

void StaticSafeLogger::debug(const std::string & functionName, const std::string & message)
{
	m_log.initialize(m_name);
	m_log.debug(functionName, message);
}

void StaticSafeLogger::info(const std::string & message)
{
	m_log.initialize(m_name);
	m_log.info(message);
}

void StaticSafeLogger::info(const std::string & functionName, const std::string & message)
{
	m_log.initialize(m_name);
	m_log.info(functionName, message);
}

void StaticSafeLogger::warn(const std::string & message)
{
	m_log.initialize(m_name);
	m_log.warn(message);
}

void StaticSafeLogger::warn(const std::string & functionName, const std::string & message)
{
	m_log.initialize(m_name);
	m_log.warn(functionName, message);
}

void StaticSafeLogger::error(const std::string & message)
{
	m_log.initialize(m_name);
	m_log.error(message);
}

void StaticSafeLogger::error(const std::string & functionName, const std::string & message)
{
	m_log.initialize(m_name);
	m_log.error(functionName, message);
}

void StaticSafeLogger::fatal(const std::string & message)
{
	m_log.initialize(m_name);
	m_log.fatal(message);
}

void StaticSafeLogger::fatal(const std::string & functionName, const std::string & message)
{
	m_log.initialize(m_name);
	m_log.fatal(functionName, message);
}
