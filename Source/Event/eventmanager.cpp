#include "Event/eventmanager.h"

#include <algorithm>

#include "Utility/contract.h"
#include "Utility/utility.h"

EventManager::EventManager() : m_nextListenerID(0), m_log("EventManager") {}

EventManager::~EventManager() {}

ListenerId EventManager::registerListener() { return m_nextListenerID++; }

bool EventManager::addListener(const EventType evtType, const ListenerId listener, const EventDelegate evtDelegate)
{
	REQUIRE(evtDelegate);
	if (!evtDelegate) {
		m_log.error("addListener", "Attempted to add uncallable event delegate for " 
			+ utility::toHex(evtType));
		return false;
	}

	std::lock_guard<std::mutex> lock(m_mapMtx);

	// Lambda function to compare delegate targets
	const auto compare = [listener](auto rhs) { return listener == std::get<0>(rhs); };

	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		// New event type
		m_eventListenerMap.emplace(evtType, EventListenerVector({std::make_tuple(listener, evtDelegate)}));
	}
	else {
		// Event type already exists, make sure that the delegate does not already exist
		if (std::any_of(it->second.begin(), it->second.end(), compare)) {
			m_log.warn("addListener", "Attempted to add listener " + utility::toStr(listener)
				+ " twice for event type " + utility::toHex(evtType));
			return false;
		}
		// Existing event type but it doesnt have this listener
		it->second.emplace_back(listener, evtDelegate);
	}

	m_log.info("addListener", "Added new listener " + utility::toStr(listener)
		+ " for event type " + utility::toHex(evtType) + " and a delegate for it");

	ENSURE(m_eventListenerMap.find(evtType) != m_eventListenerMap.end());
	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 1);

	return true;
}

bool EventManager::removeListener(const EventType evtType, const ListenerId listener)
{
	std::lock_guard<std::mutex> lock(m_mapMtx);

	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		m_log.warn("removeListener", "Attempted to remove non-existing event type " + utility::toHex(evtType));
		return false;
	}

	// Lambda to compare items in vector
	const auto compare = [listener](auto rhs) { return listener == std::get<0>(rhs); };

	// Event type found, remove the delegate
	const auto del_it = std::find_if(it->second.begin(), it->second.end(), compare);
	if (del_it == it->second.end()) {
		m_log.warn("removeListener", "Attempted to delete non-existing delegate from " + utility::toHex(evtType));
		return false;
	}
	m_log.info("removeListener", "Removing listener " + utility::toStr(listener) + " from event " + utility::toHex(evtType));
	it->second.erase(del_it);

	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0);

	// Erase event type if no listeners are active for it
	if (it->second.empty()) {
		m_log.info("removeListener", "Removing event type " + utility::toHex(evtType)
			+ " because there are no delegates left after remove");
		m_eventListenerMap.erase(it);
	}

	ENSURE(std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); }));
	return true;
}

void EventManager::triggerEvent(EventDataPtr pEvent)
{
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		m_log.error("triggerEvent", "Attempting to trigger nullptr event in EventManager::triggerEvent");
		return;
	}

	std::lock_guard<std::mutex> lock(m_mapMtx);

	const EventType evtType = pEvent->vGetEventType();
	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		m_log.warn("triggerEvent", "Attempting to trigger event type " + utility::toHex(evtType) + " with no delegates");
		return;
	}

	// Lambda to execute delegates, before calling make sure the delegate is still callable
	const auto execute = [pEvent](auto tuple) {
		auto func = std::get<1>(tuple);
		if (func)
			func(pEvent);
	};

	// Execute all delegates for the event type
	m_log.debug("triggerEvent", "Executing delegates for event type " + utility::toHex(evtType));
	std::for_each(it->second.begin(), it->second.end(), execute);
}

bool EventManager::queueEvent(EventDataPtr pEvent)
{
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		m_log.error("queueEvent", "Attempting to queue nullptr event");
		return false;
	}

	std::lock_guard<std::mutex> lock(m_queueMtx);

	m_log.info("queueEvent", "Adding event " + utility::toHex(pEvent->vGetEventType()) + " to event queue");
	m_eventQueue.emplace(pEvent);

	ENSURE(m_eventQueue.back() == pEvent);

	return true;
}

void EventManager::onUpdate(int msToProcess)
{
	REQUIRE(msToProcess >= 0);
	if (msToProcess < 0) {
		m_log.error("onUpdate", "Invalid timeToProcess value: " + utility::toStr(msToProcess));
		return;
	}

	std::lock_guard<std::mutex> lock(m_queueMtx);

	m_log.debug("onUpdate", "Processing events from event queue");
	// Process events from queue until queue is empty or all the time given to process is used
	int elapsedMs = 0;
	const int startTime = utility::timestampMs();
	while (!m_eventQueue.empty() && elapsedMs < msToProcess) {
		// Get first event from queue and trigger it
		triggerEvent(m_eventQueue.front());
		m_eventQueue.pop();

		// Calculate the time used processing events
		elapsedMs = utility::deltaTimeMs(startTime);
	}
	m_log.debug("onUpdate", utility::toStr(m_eventQueue.size()) + " events left after processing events");
}

unsigned int EventManager::getQueueLength()
{
	std::lock_guard<std::mutex> lock(m_queueMtx);
	return m_eventQueue.size();
}
