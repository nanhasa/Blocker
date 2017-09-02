#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <tuple>
#include <vector>

#include "interfaces.h"

using ListenerId = int;
using EventDataPtr = std::shared_ptr<IEvent>;
using EventDelegate = std::function<void(EventDataPtr)>;
using EventListenerVector = std::vector<std::tuple<ListenerId, std::function<void(EventDataPtr)>>>;

// Because of this class is static and accessible from anywhere
// all functions are thread safe
//
// Allows objects to have only one delegate per event type
class EventManager {
public:
	EventManager() = delete;

	/**
	 * \brief Used to get listener ID when registering new listener. DO NOT EXPLICITLY CALL! EventListener class calls this in its builder. Thread safe
	 * \return Unique ListenerId (int)
	 */
	static ListenerId registerListener();

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
	static bool addListener(const EventType evtType, const ListenerId listener, const EventDelegate evtDelegate);

	/**
	 * \brief Used to remove object's listener delegate for certain event type. Thread safe
	 * \param evtType Event GUID
	 * \param listener ListenerID assigned by registerListener(). Used to identify listener
	 * \post std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0
	 * \post std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); })
	 * \return True if successful, otherwise false
	 */
	static bool removeListener(const EventType evtType, const ListenerId listener);

	/**
	 * \brief Triggers event listeners immediately by forwarding event to them. Thread safe
	 * \param pEvent Event being triggered
	 * \pre pEvent != nullptr
	 */
	static void triggerEvent(EventDataPtr pEvent);

	/**
	 * \brief Used to place event in the queue. If event is not valid, it is not placed in the queue. Thread safe
	 * \param pEvent Event being placed in the queue
	 * \pre pEvent != nullptr
	 * \post m_eventQueue.back() == pEvent
	 * \return True if event was valid to be placed in a queue, otherwise false
	 */
	static bool queueEvent(EventDataPtr pEvent);

	/**
	 * \brief Processes event queue until the queue is empty or the function has run out of time given to it. Thread safe
	 * \param timeToProcess Time in milliseconds to process queue
	 * \pre timeToProcess >= 0.0f
	 */
	static void onUpdate(float timeToProcess);

	/**
	 * \brief Used to get the count of events in event queue. Thread safe
	 * \return Count of events in event queue. 0 if empty.
	 */
	static unsigned int getQueueLength();

protected: // Protected for testing purposes
	static std::atomic<int> m_nextListenerID;									//!< ID given to next registering listener
	static std::map<const EventType, EventListenerVector> m_eventListenerMap;	//!< Map tracking which delegates listen to which events
	static std::queue<EventDataPtr> m_eventQueue;								//!< Queue for events
	static std::mutex m_mapMtx;													//!< Mutex used when accessing m_eventListenerMap
	static std::mutex m_queueMtx;												//!< Mutex used when accessing m_eventQueue
};
