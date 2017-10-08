#pragma once

#include <map>
#include <mutex>
#include <string>

#pragma warning (push, 2)  // Temporarily set warning level 2
#include <3rdParty/glm/glm.hpp>
#pragma warning (pop)      // Restore back

#include "Utility/logger.h"

class Config {
public:
	Config() = delete;

	/**
	 * \brief Used to load values from config file to memory. Clears old values after successfully opening file stream
	 * \return True if successful, otherwise false
	 */
	static bool loadFromFile();

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	static int get(std::string&& valueName, int defaultValue);

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	static float get(std::string&& valueName, float defaultValue);

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	static bool get(std::string&& valueName, bool defaultValue);

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	static std::string get(std::string&& valueName, std::string defaultValue);

	/**
	 * \brief Used to extract glm::vec3 value from config file
	 * \param valueName Value name in config file used to search for the value
	 * \param defaultValue Default value returned if error occured or value was not found
	 * \return Value from config file if successful, otherwise defaultValue
	 */
	static glm::vec3 get(std::string&& valueName, glm::vec3 defaultValue);

protected:

	/**
	* \brief Used to update config filepath, used for testing purposes
	* \param filepath New filepath to use
	*/
	static void updateFilepath(const std::string& filepath);

private:
	static Logger m_log;
	static std::string m_filepath; // Not const for testing purposes
	static std::map<const std::string, std::string> m_valuemap;
	static std::mutex m_mtx;
};
