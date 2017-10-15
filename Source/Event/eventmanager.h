#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include "interfaces.h"
#include "Utility/logger.h"

// This class is accessed through Service Locator pattern (Locator class), DO NOT CONSTRUCT EXPLICITLY
// All functions are thread safe
// Allows objects to have only one delegate per event type
class EventManager : public IEventManager {
public:

	/**
	 * \brief EventManager
	 */
	EventManager();

	/**
	 * \brief ~EventManager
	 */
	~EventManager();

	/**
	 * \brief Used to get listener ID when registering new listener. DO NOT EXPLICITLY CALL! EventListener class calls this in its builder. Thread safe
	 * \return Unique ListenerId (int)
	 */
	ListenerId registerListener() override;

	/**
	 * \brief Used to add listeners for certain events. Allows objects to have only one listener delegate per event type. Thread safe
	 * \param evtType Event GUID
	 * \param listener ListenerID assigned by registerListener(). Used to identify listener
	 * \param evtDelegate Function object called when event launches
	 * \pre evtDelegate
	 * \post m_eventListenerMap.find(evtType) != m_eventListenerMap.end()
	 * \post std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 1
	 * \return True if successful, otherwise false
	 */
	bool addListener(const EventType evtType, const ListenerId listener, const EventDelegate evtDelegate) override;

	/**
	 * \brief Used to remove object's listener delegate for certain event type. Thread safe
	 * \param evtType Event GUID
	 * \param listener ListenerID assigned by registerListener(). Used to identify listener
	 * \post std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0
	 * \post std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); })
	 * \return True if successful, otherwise false
	 */
	bool removeListener(const EventType evtType, const ListenerId listener) override;

	/**
	 * \brief Triggers event listeners immediately by forwarding event to them. Thread safe
	 * \param pEvent Event being triggered
	 * \pre pEvent != nullptr
	 */
	void triggerEvent(EventDataPtr pEvent) override;

	/**
	 * \brief Used to place event in the queue. If event is not valid, it is not placed in the queue. Thread safe
	 * \param pEvent Event being placed in the queue
	 * \pre pEvent != nullptr
	 * \post m_eventQueue.back() == pEvent
	 * \return True if event was valid to be placed in a queue, otherwise false
	 */
	bool queueEvent(EventDataPtr pEvent) override;

	/**
	 * \brief Processes event queue until the queue is empty or the function has run out of time given to it. Thread safe
	 * \param msToProcess Time in milliseconds to process queue
	 * \pre msToProcess >= 0
	 */
	void onUpdate(int msToProcess) override;

	/**
	 * \brief Used to get the count of events in event queue. Thread safe
	 * \return Count of events in event queue. 0 if empty.
	 */
	unsigned int getQueueLength() override;

protected: // Protected for testing purposes
	std::atomic<int> m_nextListenerID;									//!< ID given to next registering listener
	std::map<const EventType, EventListenerVector> m_eventListenerMap;	//!< Map tracking which delegates listen to which events
	std::queue<EventDataPtr> m_eventQueue;								//!< Queue for events
	std::mutex m_mapMtx;												//!< Mutex used when accessing m_eventListenerMap
	std::mutex m_queueMtx;												//!< Mutex used when accessing m_eventQueue
	Logger m_log;														//!< Used to write to log
};

// Null class for Service Locator
class NullEventManager : public IEventManager {
public:
	NullEventManager() {}
	~NullEventManager() {}
	ListenerId registerListener() override { return 0; };
	bool addListener(const EventType, const ListenerId, const EventDelegate) override { return false; }
	bool removeListener(const EventType, const ListenerId) override { return false; }
	void triggerEvent(EventDataPtr) override {}
	bool queueEvent(EventDataPtr) override { return false; }
	void onUpdate(int) override {}
	unsigned int getQueueLength() override { return 0; }
};
