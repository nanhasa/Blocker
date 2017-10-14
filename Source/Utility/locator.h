#pragma once

#include <memory>

#include "interfaces.h"

class Locator {
public:
	Locator() = delete;

	/**
	 * \brief ~Locator
	 */
	~Locator();

	/**
	 * \brief provideConfig
	 * \param config
	 */
	static void provideConfig(std::unique_ptr<IConfig> config);

	/**
	 * \brief getConfig
	 * \return
	 */
	static IConfig* getConfig();

	/**
	 * \brief provideEventManager
	 * \param evtMgr
	 */
	static void provideEventManager(std::unique_ptr<IEventManager> evtMgr);

	/**
	 * \brief getEventManager
	 * \return
	 */
	static IEventManager* getEventManager();

private:
	static std::unique_ptr<IConfig> m_config;
	static std::unique_ptr<IEventManager> m_eventManager;

	static bool m_configSet;
	static bool m_evtMgrSet;
};
