#include <algorithm>
#include <iostream>

#include "contract.h"
#include "Event/eventmanager.h"
#include "utility.h"

std::mutex EventManager::m_mapMtx;
std::mutex EventManager::m_queueMtx;
std::map<const eventType, eventListenerVector> EventManager::m_eventListenerMap;
std::queue<eventDataPtr> EventManager::m_eventQueue;

bool EventManager::addListener(const eventType evtType, const eventDelegate evtDelegate) {
	REQUIRE(evtDelegate);
	if (!evtDelegate) {
		std::cerr << "Attempted to add uncallable event delegate for " << evtType << std::endl;
		return false;
	}

	std::lock_guard<std::mutex> lock(EventManager::m_mapMtx);

	// Lambda function to compare delegate targets
	auto compare = [evtDelegate](eventDelegate del) {
		return evtDelegate.target_type() == del.target_type(); 
	};

	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		// New event type
		std::cout << "Adding new event type " << std::hex << evtType << " and a delegate for it" << std::endl;
		m_eventListenerMap.emplace(evtType, eventListenerVector({ evtDelegate }));
	}
	else {
		// Event type already exists, make sure that the delegate does not already exist
		if (std::any_of(it->second.begin(), it->second.end(), compare)) {
			std::cerr << "Attempted to add more than one delegate for the same object and event type " 
				<< std::hex << evtType << std::endl;
			return false;
		}
		std::cout << "Adding new delegate for event type " << std::hex << evtType << std::endl;
		it->second.emplace_back(std::move(evtDelegate));
		
	}

	ENSURE(m_eventListenerMap.find(evtType) != m_eventListenerMap.end());
	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 1);

	return true;
}

bool EventManager::removeListener(const eventType evtType, const eventDelegate evtDelegate) {
	REQUIRE(evtDelegate);
	if (!evtDelegate) {
		std::cerr << "Attempted to remove uncallable event delegate from event type " 
			<< std::hex << evtType << std::endl;
		return false;
	}

	std::lock_guard<std::mutex> lock(EventManager::m_mapMtx);

	// Lambda function to compare delegate targets
	auto compare = [evtDelegate](eventDelegate del) {
		return evtDelegate.target_type() == del.target_type();
	};

	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		std::cerr << "Attempted to remove non-existing event type " 
			<< std::hex << evtType << std::endl;
		return false;
	}

	// Event type found, remove the delegate
	auto del_it = std::find_if(it->second.begin(), it->second.end(), compare);
	if (del_it == it->second.end()) {
		std::cerr << "Attempted to delete non-existing delegate from " 
			<< std::hex << evtType << std::endl;
		return false;
	}
	std::cout << "Removing delegate from event " << std::hex << evtType << std::endl;
	it->second.erase(del_it);

	ENSURE(std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0);

	// Erase event type if no listeners are active for it
	if (it->second.empty()) {
		std::cout << "Removing event type " << std::hex << evtType 
			<< " because there are no delegates left after remove" << std::endl;
		m_eventListenerMap.erase(it);
	}

	ENSURE(std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); }));

	return true;
}

void EventManager::triggerEvent(eventDataPtr pEvent) {
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		std::cerr << "Attempting to trigger nullptr event" << std::endl;
		return;
	}

	std::lock_guard<std::mutex> lock(EventManager::m_mapMtx);

	eventType evtType = pEvent->vGetEventType();
	auto it = m_eventListenerMap.find(evtType);
	if (it == m_eventListenerMap.end()) {
		std::cerr << "Attempting to trigger event type " << std::hex << evtType << " with no delegates" << std::endl;
		return;
	}

	// Lambda to execute delegates, before calling make sure the delegate is still callable
	auto execute = [pEvent](auto func) {
		if (func) 
			func(pEvent);
	};

	// Execute all delegates for the event type
	std::cout << "Executing delegates for event type " << std::hex << evtType << std::endl;
	std::for_each(it->second.begin(), it->second.end(), execute);
}

bool EventManager::queueEvent(eventDataPtr pEvent) {
	REQUIRE(pEvent != nullptr);
	if (!pEvent) {
		std::cerr << "Attempting to queue nullptr event" << std::endl;
		return false;
	}

	std::lock_guard<std::mutex> lock(EventManager::m_queueMtx);

	std::cout << "Adding event for event type " 
		<< std::hex << pEvent->vGetEventType() << " to event queue" << std::endl;
	m_eventQueue.emplace(pEvent);

	ENSURE(m_eventQueue.back() == pEvent);
	
	return true;
}

void EventManager::onUpdate(float timeToProcess) {
	REQUIRE(timeToProcess >= 0.0f);
	if (timeToProcess < 0.0f) {
		std::cerr << "timeToProcess value invalid in EventManager::onUpdate" << std::endl;
		return;
	}

	std::lock_guard<std::mutex> lock(EventManager::m_queueMtx);

	std::cout << "Processing events from event queue" << std::endl;
	// Process events from queue until queue is empty or all the time given to process is used
	float elapsedTime = 0.0f;
	float startTime = utility::getTimestamp();
	while (!m_eventQueue.empty() && elapsedTime < timeToProcess) {
		// Get first event from queue and trigger it
		triggerEvent(m_eventQueue.front());
		m_eventQueue.pop();

		// Calculate the time used processing events
		elapsedTime = utility::getTimestamp() - startTime;
	}
}
