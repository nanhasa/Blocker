#include "Event/eventlistener.h"

#include <algorithm>

#include "Event/eventManager.h"
#include "Utility/locator.h"

EventListener::EventListener() : m_listenerId(Locator::getEventManager()->registerListener()) {}

EventListener::~EventListener()
{
	for (auto evt : m_eventsListened) { Locator::getEventManager()->removeListener(evt, m_listenerId); }
}

bool EventListener::registerForEvent(const EventType evtType, std::function<void(std::shared_ptr<IEvent>)> evtDelegate)
{
	if (Locator::getEventManager()->addListener(evtType, m_listenerId, std::move(evtDelegate))) {
		m_eventsListened.emplace_back(evtType);
		return true;
	}
	return false;
}

bool EventListener::unregisterForEvent(const EventType evtType)
{
	if (Locator::getEventManager()->removeListener(evtType, m_listenerId)) {
		std::remove(m_eventsListened.begin(), m_eventsListened.end(), evtType);
		return true;
	}
	return false;
}
