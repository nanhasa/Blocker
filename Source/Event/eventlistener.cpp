#include <algorithm>

#include "Event/eventlistener.h"
#include "Event/eventManager.h"

EventListener::EventListener() : m_listenerID(EventManager::registerListener()) {}

EventListener::~EventListener()
{
	for (auto evt : m_eventsListened) { EventManager::removeListener(evt, m_listenerID); }
}

bool EventListener::registerForEvent(const eventType evtType, std::function<void(std::shared_ptr<IEvent>)> evtDelegate)
{
	if (EventManager::addListener(evtType, m_listenerID, std::move(evtDelegate))) {
		m_eventsListened.emplace_back(evtType);
		return true;
	}
	return false;
}

bool EventListener::unregisterForEvent(const eventType evtType)
{
	if (EventManager::removeListener(evtType, m_listenerID)) {
		std::remove(m_eventsListened.begin(), m_eventsListened.end(), evtType);
		return true;
	}
	return false;
}
