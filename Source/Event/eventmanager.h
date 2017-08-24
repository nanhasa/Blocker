#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "interfaces.h"

using eventDataPtr = std::shared_ptr<IEvent>;
using eventDelegate = std::function<void(eventDataPtr)>;
using eventListenerVector = std::vector<std::function<void(eventDataPtr)>>;

// Because of this class is static and accessible from anywhere
// all functions are thread safe
//
// Allows objects to have only one delegate per event type
class EventManager {
public:
	EventManager() = delete;

	/**
	 * \brief Used to add listeners for certain events. Allows objects to have only one listener delegate per event type. Thread safe
	 * \param evtType Event GUID
	 * \param evtDelegate Function object called when event launches
	 * \pre evtDelegate
	 * \post m_eventListenerMap.find(evtType) != m_eventListenerMap.end()
	 * \post std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 1
	 * \return True if successful, otherwise false
	 */
	static bool addListener(const eventType evtType, const eventDelegate evtDelegate);

	/**
	 * \brief Used to remove object's listener delegate for certain event type. Thread safe
	 * \param evtType Event GUID
	 * \param evtDelegate Delegate being removed
	 * \pre evtDelegate
	 * \post std::count_if(m_eventListenerMap[evtType].begin(), m_eventListenerMap[evtType].end(), compare) == 0
	 * \post std::all_of(m_eventListenerMap.begin(), m_eventListenerMap.end(), [](auto pair) { return !pair.second.empty(); })
	 * \return True if successful, otherwise false
	 */
	static bool removeListener(const eventType evtType, const eventDelegate evtDelegate);

	/**
	 * \brief Triggers event listeners immediately by forwarding event to them. Thread safe
	 * \param pEvent Event being triggered
	 * \pre pEvent != nullptr
	 */
	static void triggerEvent(eventDataPtr pEvent);

	/**
	 * \brief Used to place event in the queue. If event is not valid, it is not placed in the queue. Thread safe
	 * \param pEvent Event being placed in the queue
	 * \pre pEvent != nullptr
	 * \post m_eventQueue.back() == pEvent
	 * \return True if event was valid to be placed in a queue, otherwise false
	 */
	static bool queueEvent(eventDataPtr pEvent);

	/**
	 * \brief Processes event queue until the queue is empty or the function has run out of time given to it. Thread safe
	 * \param timeToProcess Time in milliseconds to process queue
	 * \pre timeToProcess >= 0.0f
	 */
	static void onUpdate(float timeToProcess);

private:
	static std::map<const eventType, eventListenerVector> m_eventListenerMap;	//!< Map tracking which delegates listen to which events
	static std::queue<eventDataPtr> m_eventQueue;								//!< Queue for events
	static std::mutex m_mapMtx;													//!< Mutex used when accessing m_eventListenerMap
	static std::mutex m_queueMtx;												//!< Mutex used when accessing m_eventQueue
};
