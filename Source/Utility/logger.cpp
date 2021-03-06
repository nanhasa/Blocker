#include "Utility/logger.h"

#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <3rdParty/rapidjson/document.h>
#include <3rdParty/rapidjson/istreamwrapper.h>

#include "interfaces.h"
#include "Utility/config.h"
#include "Utility/contract.h"
#include "Utility/locator.h"

namespace {

	/**
	* \brief Used to get timestamp in string
	* \return Timestamp in yyyy-mm-dd 24hh:mm:ss
	*/
	std::string datetime()
	{
		using namespace std::chrono;
		const auto now = system_clock::now();
		const std::time_t now_t = system_clock::to_time_t(now);
		tm timeinfo;
		localtime_s(&timeinfo, &now_t);
		std::stringstream ss;
		ss << std::put_time(&timeinfo, "%F %X");
		return ss.str();
	}

	/**
	* \brief Used to transform logging level to string format
	* \param lvl Logging level to be transformed
	* \return String format of logging level
	*/
	std::string levelToFixedString(LOGGING_LEVEL lvl)
	{
		static const std::vector<std::string> lookup{ "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
		auto output = lookup[lvl];
		while (output.length() < 5) {
			output += " ";
		}
		return output;
	}

} // Anonymous namespace

Logger::Logger() : m_enabledLogLevels(), m_logName(""), m_filename(""), m_configFilename("logconfig.json") {}

Logger::Logger(const std::string & name)
	: m_enabledLogLevels(), m_logName(""),  m_filename(""), m_configFilename("logconfig.json")
{
	initialize(name);
}

Logger::~Logger() {}

Logger & Logger::operator=(Logger && rhs) noexcept
{
	m_enabledLogLevels = std::move(rhs.m_enabledLogLevels);
	m_logName = std::move(rhs.m_logName);
	m_filename = std::move(m_filename);
	m_configFilename = std::move(m_configFilename);
	return *this;
}

void Logger::initialize(const std::string & name)
{
	REQUIRE(!name.empty());
	if (!m_logName.empty() || name.empty())
		return;

	m_logName = name;

	std::ifstream stream;
	// Builder should not emit exceptions so wrap the 3rd party code in try catch
	try {
		// Open config file
		stream.open(Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Log/" + m_configFilename);
		if (!stream.is_open()) {
			std::cerr << "Could not open file " << m_configFilename << std::endl;
			return;
		}

		// Give ifstream to rapidjson stream wrapper
		rapidjson::IStreamWrapper isw(stream);
		rapidjson::Document doc;
		doc.ParseStream(isw);
		stream.close(); // rapidjson has parsed file so close stream
						// Check for parse errors
		if (doc.HasParseError()) {
			std::cerr << "Parse error in: " << m_configFilename << std::endl;
			return;
		}
		// Validate root object
		if (!doc.IsObject()) {
			std::cerr << "Root object not valid in: " << m_configFilename << std::endl;
			return;
		}
		// Check if current log object exists
		if (!doc.HasMember(m_logName.c_str())) {
			std::cerr << "Object " << m_logName << " not found in: " << m_configFilename << std::endl;
			return;
		}

		const rapidjson::Value& obj = doc[m_logName.c_str()];
		// Check that current log is object
		if (!obj.IsObject()) {
			std::cerr << m_logName << " is not an object in: " << m_configFilename << std::endl;
			return;
		}
		// Check current log object's properties
		if (!obj.HasMember("file") && !obj["file"].IsString()) {
			std::cerr << m_logName << " does not have valid file property in: " << m_configFilename << std::endl;
			return;
		}
		if (!obj.HasMember("detail") && !obj["detail"].IsArray()) {
			std::cerr << m_logName << " does not have valid detail property in: " << m_configFilename << std::endl;
			return;
		}

		// Initialize members
		m_filename = obj["file"].GetString();
		auto a = obj["detail"].GetArray();
		for (rapidjson::SizeType i = 0; i < a.Size(); ++i) { // Uses SizeType instead of size_t
			if (!a[i].IsString()) continue;

			const std::string str = a[i].GetString();
			if (str == "DEBUG") m_enabledLogLevels.emplace_back(DEBUG);
			else if (str == "INFO") m_enabledLogLevels.emplace_back(INFO);
			else if (str == "WARN") m_enabledLogLevels.emplace_back(WARN);
			else if (str == "ERROR") m_enabledLogLevels.emplace_back(ERR);
			else if (str == "FATAL") m_enabledLogLevels.emplace_back(FATAL);
		}

		// Create or truncate log file
		std::ofstream ofs(
			Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Log/" + m_filename, 
			std::ofstream::out | std::ofstream::trunc);

		if (!ofs.is_open()) 
			std::cerr << "Error opening file " << m_filename << " when truncating" << std::endl;
	}
	catch (...) {
		std::cerr << "Something went terribly wrong in Logger builder" << std::endl;
	}
}

void Logger::debug(const std::string & message) const
{
	if (isEnabled(DEBUG))
		write(DEBUG, message);
}

void Logger::debug(const std::string & functionName, const std::string & message) const
{
	if (isEnabled(DEBUG))
		write(DEBUG, functionName + "(): " + message);
}

void Logger::info(const std::string & message) const
{
	if (isEnabled(INFO))
		write(INFO, message);
}

void Logger::info(const std::string & functionName, const std::string & message) const
{
	if (isEnabled(INFO))
		write(INFO, functionName + "(): " + message);
}

void Logger::warn(const std::string & message) const
{
	if (isEnabled(WARN))
		write(WARN, message);
}

void Logger::warn(const std::string & functionName, const std::string & message) const
{
	if (isEnabled(WARN))
		write(WARN, functionName + "(): " + message);
}

void Logger::error(const std::string & message) const
{
	if (isEnabled(ERR))
		write(ERR, message);
}

void Logger::error(const std::string & functionName, const std::string & message) const
{
	if (isEnabled(ERR))
		write(ERR, functionName + "(): " + message);
}

void Logger::fatal(const std::string & message) const
{
	if (isEnabled(FATAL))
		write(FATAL, message);
}

void Logger::fatal(const std::string & functionName, const std::string & message) const
{
	if (isEnabled(FATAL))
		write(FATAL, functionName + "(): " + message);
}

bool Logger::isEnabled(LOGGING_LEVEL lvl) const
{
	if (m_enabledLogLevels.empty()) 
		return false;
	
	return std::find(m_enabledLogLevels.begin(), m_enabledLogLevels.end(), lvl) != m_enabledLogLevels.end();
}

void Logger::write(LOGGING_LEVEL lvl, const std::string& message) const
{
	if (m_filename.empty()) return;
	
	std::ofstream ostream(Locator::getConfig()->get("DataPath", std::string("../Data/")) + "Log/" + m_filename, 
		std::ofstream::out | std::ofstream::app);

	if (!ostream.is_open()) {
		return;
	}
	ostream << datetime() << " " << levelToFixedString(lvl) << " - " << message << std::endl;
}
