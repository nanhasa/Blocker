#include "Utility/logger.h"

Logger::Logger(const std::string & subsystem)
{
	(void)subsystem;
}

Logger::~Logger()
{
}

void Logger::debug(const std::string & message) const
{
	(void)message;
}

void Logger::info(const std::string & message) const
{
	(void)message;
}

void Logger::warn(const std::string & message) const
{
	(void)message;
}

void Logger::error(const std::string & message) const
{
	(void)message;
}

void Logger::fatal(const std::string & message) const
{
	(void)message;
}
