#include "Utility/config.h"

#include <algorithm>
#include <fstream>

Logger Config::m_log("Config");
std::string Config::m_filepath("../Data/Config/config.txt");
std::map<const std::string, std::string> Config::m_valuemap;
std::mutex Config::m_mtx;

bool Config::loadFromFile()
{
	std::ifstream stream(m_filepath);
	if (!stream.is_open()) {
		m_log.error("Could not open config file: " + m_filepath);
		return false;
	}

	std::lock_guard<std::mutex> lock(m_mtx);
	m_valuemap.clear();
	std::string line;
	for (int row = 1; std::getline(stream, line); ++row) {
		// Erase comments
		auto separator = line.find('#');
		if (separator != std::string::npos)
			line = line.substr(0, separator);
		if (line.empty())
			continue;

		separator = line.find(':');
		if (separator == std::string::npos) {
			// There were no separator between name and value
			// Create a copy of line and remove all white spaces of to see if 
			// line was erronous or just empty after comments were removed
			auto temp{ line };
			temp.erase(std::remove_if(temp.begin(), temp.end(), isspace), temp.end());
			if (temp.empty())
				continue;
			m_log.error("Invalid row " + toStr(row) + ": " + line);
			continue;
		}

		// Use separator to extract value name and value to map
		auto name = line.substr(0, separator);
		auto value = line.substr(separator + 1);
		// TODO: trim name and value of whitespaces
		auto it = m_valuemap.find(name);
		if (it != m_valuemap.end()) {
			m_log.warn("Value pair \"" + it->first + ":" + it->second + "\" already exists. Overwriting the value with \"" + value + "\"");
		}
		m_valuemap[name] = value;
	}
	stream.close();
	return true;
}

int Config::get(std::string&& valueName, int defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) {
		return defaultValue;
	}

	// Try to convert value to int
	try {
		auto output = std::stof(it->second);
		return output;
	}
	catch (std::invalid_argument& ia) {
		m_log.error("Could not create float of: " + it->second + ". Exception thrown: " + ia.what());
		return defaultValue;
	}
	catch (std::out_of_range& oor) {
		m_log.error("Could not create float of: " + it->second + ". Exception thrown: " + oor.what());
		return defaultValue;
	}
}

float Config::get(std::string&& valueName, float defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) {
		return defaultValue;
	}

	// Try to convert value to float
	try {
		auto output = std::stof(it->second);
		return output;
	}
	catch (std::invalid_argument& ia) {
		m_log.error("Could not create float of: " + it->second + ". Exception thrown: " + ia.what());
		return defaultValue;
	}
	catch (std::out_of_range& oor) {
		m_log.error("Could not create float of: " + it->second + ". Exception thrown: " + oor.what());
		return defaultValue;
	}
}

bool Config::get(std::string&& valueName, bool defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) {
		return defaultValue;
	}

	// Try to convert value to bool
	auto str = it->second; 
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str == "1" || str == "true") 
		return true;
	else if (str == "0" || str == "false")
		return false;
	else 
		m_log.error("Could not create bool of: " + it->second);
		
	return defaultValue;
}

std::string Config::get(std::string&& valueName, std::string defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) {
		return defaultValue;
	}
	return it->second;
}

glm::vec3 Config::get(std::string&& valueName, glm::vec3 defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) {
		return defaultValue;
	}

	// Try to convert values to float, then to glm::vec3
	try {
		std::string::size_type st; // Used to get the position after the number extracted
		auto str = it->second;
		float x = std::stof(str, &st);
		if (st == str.length() - 1)
			throw std::invalid_argument("Invalid data to create glm::vec3: " + it->second);
		str = str.substr(st);
		float y = std::stof(str, &st);
		if (st == str.length() - 1)
			throw std::invalid_argument("Invalid data to create glm::vec3: " + it->second);
		str = str.substr(st);
		float z = std::stof(str);

		return glm::vec3(x, y, z);
	}
	catch (std::invalid_argument& ia) {
		m_log.error("Could not create floats to create glm::vec3 of: " + it->second + ". Exception thrown: " + ia.what());
		return defaultValue;
	}
	catch (std::out_of_range& oor) {
		m_log.error("Could not create floats to create glm::vec3 of: " + it->second + ". Exception thrown: " + oor.what());
		return defaultValue;
	}
}

void Config::updateFilepath(const std::string & filepath)
{
	m_filepath = filepath;
}
