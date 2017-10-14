#include "Utility/config.h"

#include <algorithm>
#include <fstream>
#include <memory>

#include "locator.h"
#include "utility.h"

Config::Config() : m_log("Config"), m_initialized(false)
{
	loadFromFile();
}

Config::~Config() {}

int Config::get(std::string&& valueName, int defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	const auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) return defaultValue;

	// Try to convert value to int
	try {
		const auto output = std::stoi(it->second);
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
	const auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) { return defaultValue; }

	// Try to convert value to float
	try {
		const auto output = std::stof(it->second);
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
	const auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) { return defaultValue; }

	// Try to convert value to bool
	auto str = it->second;
	std::transform(str.begin(), str.end(), str.begin(),
	               [](char c) { return static_cast<char>(::tolower(c)); }); // Wrap ::tolower in lambda to avoid warning
	if (str == "1" || str == "true")
		return true;
	if (str == "0" || str == "false")
		return false;

	m_log.error("Could not create bool of: " + it->second);
	return defaultValue;
}

std::string Config::get(std::string&& valueName, std::string defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	const auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) { return defaultValue; }
	return it->second;
}

glm::vec3 Config::get(std::string&& valueName, glm::vec3 defaultValue)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	const auto it = m_valuemap.find(valueName);
	if (it == m_valuemap.end()) { return defaultValue; }

	// Try to convert values to float, then to glm::vec3
	try {
		std::string::size_type st; // Used to get the position after the number extracted
		auto str = it->second;
		const float x = std::stof(str, &st);
		if (st >= str.length() - 1)
			throw std::invalid_argument("Invalid data to create glm::vec3: " + it->second);
		str = str.substr(st + 1);
		const float y = std::stof(str, &st);
		if (st >= str.length() - 1)
			throw std::invalid_argument("Invalid data to create glm::vec3: " + it->second);
		str = str.substr(st + 1);
		const float z = std::stof(str);

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

bool Config::readFile(std::vector<std::string>& contents, const std::string& path) const
{
	std::ifstream stream(path);
	if (!stream.is_open()) {
		m_log.error("Could not open config file: " + path);
		return false;
	}

	contents.clear();
	std::string line;
	while (std::getline(stream, line)) {
		contents.emplace_back(std::move(line));
	}
	stream.close();
	return true;
}

void Config::loadFromFile()
{
	std::vector<std::string> file;
	const std::string filepath{ "../Data/Config/config.txt" };
	if (!readFile(file, filepath))
		return;

	m_valuemap.clear();
	for (unsigned int row = 1; row < file.size(); ++row) {
		if (file[row].empty())
			continue;

		// Erase comments
		auto separator = file[row].find('#');
		if (separator != std::string::npos)
			file[row] = file[row].substr(0, separator);
		if (file[row].empty())
			continue;

		separator = file[row].find('=');
		if (separator == std::string::npos) {
			// There were no separator between name and value
			// Create a copy of line and remove all white spaces of to see if 
			// line was erronous or just empty after comments were removed
			auto temp{ file[row] };
			temp.erase(std::remove_if(temp.begin(), temp.end(), isspace), temp.end());
			if (temp.empty())
				continue;
			m_log.error("Invalid row " + utility::toStr(row) + ": " + file[row]);
			continue;
		}

		// Use separator to extract value name and value to map
		const auto name = file[row].substr(0, separator);
		const auto value = file[row].substr(separator + 1);
		// TODO: trim name and value of whitespaces
		const auto it = m_valuemap.find(name);
		if (it != m_valuemap.end()) {
			m_log.warn(
				"Value pair \"" + it->first + ":" + it->second + "\" already exists. Overwriting the value with \"" + value + "\"");
		}
		m_valuemap[name] = value;
	}
	m_initialized = true;
}
