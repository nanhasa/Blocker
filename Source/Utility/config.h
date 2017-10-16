#pragma once

#include <map>
#include <mutex>
#include <string>
#include <vector>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "interfaces.h"
#include "Utility/logger.h"


// This class is accessed through Service Locator pattern (Locator class)
// DO NOT BUILD EXPLICITLY
class Config : public IConfig {
public:

	/**
	 * \brief Constructor. Only to be used when providing this class to Locator class
	 */
	Config();

	/**
	 * \brief Destructor
	 */
	~Config();

	/**
	 * \brief Used to loadTexture values from config file to memory. Clears old values after successfully opening file stream
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return True if successful, otherwise false
	 */
	int get(std::string&& valueName, int defaultValue) override;

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	float get(std::string&& valueName, float defaultValue) override;

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	bool get(std::string&& valueName, bool defaultValue) override;

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	std::string get(std::string&& valueName, std::string defaultValue) override;

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	glm::vec3 get(std::string&& valueName, glm::vec3 defaultValue) override;

private:
	Logger m_log;
	std::map<const std::string, std::string> m_valuemap;
	std::mutex m_mtx;
	bool m_initialized;

	/**
	 * \brief Used to initialize m_valuemap with contents of config file
	 */
	void loadFromFile();

	/**
	 * \brief Used to read files contents to content vector
	 * \param contents Output parameter that will hold file contents
	 * \param path Filepath to config file
	 * \return True if load was successful, otherwise false
	 */
	bool readFile(std::vector<std::string>& contents, const std::string& path) const;
};


// Null class for uninitialized Service Locator
class NullConfig : public IConfig {
public:
	NullConfig() {}
	~NullConfig() {}

	int get(std::string&&, int defaultValue) override { return defaultValue; }
	float get(std::string&&, float defaultValue)  override { return defaultValue; }
	bool get(std::string&&, bool defaultValue)  override { return defaultValue; }
	std::string get(std::string&&, std::string defaultValue)  override { return defaultValue; }
	glm::vec3 get(std::string&&, glm::vec3 defaultValue)  override { return defaultValue; }
};
