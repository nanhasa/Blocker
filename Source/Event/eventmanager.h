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
class EventManager {
public:
	EventManager() = delete;

	static bool addListener(const eventType evtType, const eventDelegate evtDelegate);
	static bool removeListener(const eventType evtType, const eventDelegate evtDelegate);
	static void triggerEvent(eventDataPtr pEvent);
	static bool queueEvent(eventDataPtr pEvent);
	static void onUpdate(float timeToProcess);

private:
	static std::map<const eventType, eventListenerVector> m_eventListenerMap;
	static std::queue<eventDataPtr> m_eventQueue;
	static std::mutex m_mapMtx;
	static std::mutex m_queueMtx;
};
