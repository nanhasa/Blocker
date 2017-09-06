#pragma once

#include <sstream>
#include <string>
#include <vector>

class Logger {
public:
	explicit Logger(const std::string& name);
	~Logger();

	// Delete copy and move constructors
	Logger(Logger const&) = delete;
	Logger& operator=(Logger const&) = delete;

	void debug(const std::string& message) const;
	void info(const std::string& message) const;
	void warn(const std::string& message) const;
	void error(const std::string& message) const;
	void fatal(const std::string& message) const;

private:
	enum LOGGING_LEVEL {DEBUG, INFO, WARN, ERROR, FATAL};
	std::vector<LOGGING_LEVEL> m_enabledLogLevels;
	std::string m_logName;
	std::string m_filename;
	const std::string m_configFilename; // Cannot be static as it may not be initialized if logging is used as static object

	bool isEnabled(LOGGING_LEVEL lvl) const;
	void write(LOGGING_LEVEL lvl, const std::string& message) const;
	std::string datetime() const;
	std::string levelToFixedString(LOGGING_LEVEL lvl) const;
};


template<typename T>
std::string toHex(T&& num)
{
	std::stringstream ss;
	ss << "0x" << std::hex << std::forward<T>(num);
	return ss.str();
}

template<typename T>
std::string toDec(T&& num)
{
	std::stringstream ss;
	ss << std::dec << std::forward<T>(num);
	return ss.str();
}

template<typename T>
std::string toStr(T&& num)
{
#pragma warning(suppress: 4127)
	if (!std::is_integral<T>::value && !std::is_floating_point<T>::value)
		return std::string();
	return std::to_string(std::forward<T>(num));
}