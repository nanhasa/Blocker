#include "Utility/locator.h"

#include "Utility/config.h"
#include "Event/eventmanager.h"

std::unique_ptr<IConfig> Locator::m_config = std::make_unique<NullConfig>();
std::unique_ptr<IEventManager> Locator::m_eventManager = std::make_unique<NullEventManager>();
bool Locator::m_configSet = false;
bool Locator::m_evtMgrSet = false;

Locator::~Locator() {}

void Locator::provideConfig(std::unique_ptr<IConfig> config)
{
	if (config && !m_configSet) {
		m_config = std::move(config);
		m_configSet = true;
	}
}

IConfig* Locator::getConfig()
{
	return m_config.get();
}

void Locator::provideEventManager(std::unique_ptr<IEventManager> evtMgr)
{
	if (evtMgr && !m_evtMgrSet) {
		m_eventManager = std::move(evtMgr);
		m_evtMgrSet = true;
	}
}

IEventManager* Locator::getEventManager()
{
	return m_eventManager.get();
}
