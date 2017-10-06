#pragma once

#include "Event/event.h"
#include "Event/eventlistener.h"
#include "Event/eventmanager.h"
#include "interfaces.h"

// Extend EventManager class for testing purposes
class DerivedEventManager : public EventManager {
public:
	// Return listener count for given event type
	static unsigned int getEventListenerCount(const EventType evtType) {
		std::lock_guard<std::mutex> lock(EventManager::m_mapMtx);

		const auto it = m_eventListenerMap.find(evtType);
		if (it != m_eventListenerMap.end()) {
			return it->second.size();
		}
		return 0;
	}

	// Clear all listeners
	static void clearListeners() {
		std::lock_guard<std::mutex> lock(EventManager::m_mapMtx);
		m_eventListenerMap.clear();
	}

	static void flushQueue() {
		std::lock_guard<std::mutex> lock(EventManager::m_queueMtx);
		while (!m_eventQueue.empty()) {
			m_eventQueue.pop();
		}
	}
};


// Event for testing purposes
class TestEvent : public Event {
public:
	static const EventType eventType;

	TestEvent() {};
	~TestEvent() {};
	EventType vGetEventType() const override { return eventType; };
	std::string vGetEventName() const override { return m_eventName; };

private:
	static const std::string m_eventName;
};
const EventType TestEvent::eventType(0x7de5db2b);
const std::string TestEvent::m_eventName("TestEvent");

// Another test event
class TestEvent2 : public Event {
public:
	static const EventType eventType;

	TestEvent2() {};
	~TestEvent2() {};
	EventType vGetEventType() const override { return eventType; };
	std::string vGetEventName() const override { return m_eventName; };

private:
	static const std::string m_eventName;
};
const EventType TestEvent2::eventType(0x8545422b);
const std::string TestEvent2::m_eventName("TestEvent2");


// Used to count the callback count on event trigger
int g_callbackCounter = 0;




// Dummy classes that are registered for TestEvent
class TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(EventType evtType = TestEvent::eventType) { return m_listener.registerForEvent(evtType, std::bind(&TestClass1::callback, std::placeholders::_1)); }
	bool unregisterListener(EventType evtType = TestEvent::eventType) { return m_listener.unregisterForEvent(evtType); }

protected:
	EventListener m_listener;
};
class SubTestClass1 : public TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(EventType evtType = TestEvent::eventType) { return m_listener.registerForEvent(evtType, std::bind(&SubTestClass1::callback, std::placeholders::_1)); }
	bool unregisterListener(EventType evtType = TestEvent::eventType) { return m_listener.unregisterForEvent(evtType); }
};
class SubTestClass2 : public TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(EventType evtType = TestEvent::eventType) { return m_listener.registerForEvent(evtType, std::bind(&SubTestClass2::callback, std::placeholders::_1)); }
	bool unregisterListener(EventType evtType = TestEvent::eventType) { return m_listener.unregisterForEvent(evtType); }
};
class TestClass2 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(EventType evtType = TestEvent::eventType) { return m_listener.registerForEvent(evtType, std::bind(&TestClass2::callback, std::placeholders::_1)); }
	bool unregisterListener(EventType evtType = TestEvent::eventType) { return m_listener.unregisterForEvent(evtType); }
protected:
	EventListener m_listener;
};
