#include <algorithm>
#include <iostream>

#include "Event/eventmanager.h"
#include "Utility/contract.h"
#include "Utility/utility.h"

std::atomic<ListenerId> EventManager::m_nextListenerID(0);
std::mutex EventManager::m_mapMtx;
std::mutex EventManager::m_queueMtx;
std::map<const EventType, EventListenerVector> EventManager::m_eventListenerMap;
std::queue<EventDataPtr> EventManager::m_eventQueue;
Logger EventManager::m_log("EventManager");

ListenerId EventManager::registerListener() { return m_nextListenerID++; }

bool EventManager::addListener(const EventType evtType, const ListenerId listener, const EventDelegate evtDelegate)
{
	REQUIRE(evtDelegate);
	if (!evtDelegate) {
		m_log.error("EventManager::addListener - Attempted to add uncallable event delegate for " + Logger::toHex(evtType));
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
			m_log.warn("Attempted to add listener " + Logger::toStr(listener) + " twice for event type " + m_log.toHex(evtType));
			return false;
		}
		// Existing event type but it doesnt have this listener
		it->second.emplace_back(listener, evtDelegate);
	}

	m_log.info("Added new listener " + Logger::toStr(listener) + " for event type " + Logger::toHex(evtType) + " and a delegate for it");

	ENSURE(m_eventListenerMap.find(evtType) != m_eventListenerMap.end());
	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 1);

	return true;
}

bool EventManager::removeListener(const EventType evtType, const ListenerId listener)
{
	std::lock_guard<std::mutex> lock(m_mapMtx);

	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		m_log.warn("Attempted to remove non-existing event type " + Logger::toHex(evtType));
		return false;
	}

	// Lambda to compare items in vector
	const auto compare = [listener](auto rhs) { return listener == std::get<0>(rhs); };

	// Event type found, remove the delegate
	const auto del_it = std::find_if(it->second.begin(), it->second.end(), compare);
	if (del_it == it->second.end()) {
		m_log.warn("Attempted to delete non-existing delegate from " + Logger::toHex(evtType));
		return false;
	}
	m_log.info("Removing listener " + Logger::toStr(listener) + " from event " + Logger::toHex(evtType));
	it->second.erase(del_it);

	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0);

	// Erase event type if no listeners are active for it
	if (it->second.empty()) {
		m_log.info("Removing event type " + Logger::toHex(evtType) + " because there are no delegates left after remove");
		m_eventListenerMap.erase(it);
	}

	ENSURE(std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); }
	));

	return true;
}

void EventManager::triggerEvent(EventDataPtr pEvent)
{
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		m_log.error("EventManager::triggerEvent - Attempting to trigger nullptr event in EventManager::triggerEvent");
		return;
	}

	std::lock_guard<std::mutex> lock(m_mapMtx);

	const EventType evtType = pEvent->vGetEventType();
	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		m_log.warn("Attempting to trigger event type " + Logger::toHex(evtType) + " with no delegates");
		return;
	}

	// Lambda to execute delegates, before calling make sure the delegate is still callable
	const auto execute = [pEvent](auto tuple) {
		auto func = std::get<1>(tuple);
		if (func)
			func(pEvent);
	};

	// Execute all delegates for the event type
	m_log.debug("Executing delegates for event type " + Logger::toHex(evtType));
	std::for_each(it->second.begin(), it->second.end(), execute);
}

bool EventManager::queueEvent(EventDataPtr pEvent)
{
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		m_log.error("EventManager::queueEvent - Attempting to queue nullptr event");
		return false;
	}

	std::lock_guard<std::mutex> lock(m_queueMtx);

	m_log.info("Adding event " + Logger::toHex(pEvent->vGetEventType()) + " to event queue");
	m_eventQueue.emplace(pEvent);

	ENSURE(m_eventQueue.back() == pEvent);

	return true;
}

void EventManager::onUpdate(int msToProcess)
{
	REQUIRE(msToProcess >= 0);
	if (msToProcess < 0) {
		m_log.error("EventManager::onUpdate - Invalid timeToProcess value: " + Logger::toStr(msToProcess));
		return;
	}

	std::lock_guard<std::mutex> lock(m_queueMtx);

	m_log.debug("Processing events from event queue");
	// Process events from queue until queue is empty or all the time given to process is used
	int elapsedMs = 0;
	const long startTime = utility::timestampMs();
	while (!m_eventQueue.empty() && elapsedMs < msToProcess) {
		// Get first event from queue and trigger it
		triggerEvent(m_eventQueue.front());
		m_eventQueue.pop();

		// Calculate the time used processing events
		elapsedMs = utility::deltaTimeMs(startTime);
	}
	m_log.debug(Logger::toStr(m_eventQueue.size()) + " events left after processing events");
}

unsigned int EventManager::getQueueLength()
{
	std::lock_guard<std::mutex> lock(m_queueMtx);
	return m_eventQueue.size();
}
