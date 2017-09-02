#pragma once
#include <string>

class Logger {
public:
	explicit Logger(const std::string& subsystem);
	~Logger();

	void debug(const std::string& message) const;
	void info(const std::string& message) const;
	void warn(const std::string& message) const;
	void error(const std::string& message) const;
	void fatal(const std::string& message) const;

private:
	enum LOGGING_LEVEL {NONE, INFO, WARN, ERROR, FATAL};
	LOGGING_LEVEL m_loggingLvl;
	std::string m_filename;
	const static std::string configFilename;
};