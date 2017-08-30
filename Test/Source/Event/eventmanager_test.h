#pragma once

#include "Event/event.h"
#include "Event/eventlistener.h"
#include "Event/eventmanager.h"
#include "interfaces.h"

// Extend EventManager class for testing purposes
class DerivedEventManager : public EventManager {
public:
	// Return listener count for given event type
	static unsigned int getEventListenerCount(const eventType evtType) {
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
		std::cout << "Clearing all listeners" << std::endl;
		m_eventListenerMap.clear();
	}

	static void flushQueue() {
		std::lock_guard<std::mutex> lock(EventManager::m_queueMtx);
		std::cout << "Flushing event queue" << std::endl;
		while (!m_eventQueue.empty()) {
			m_eventQueue.pop();
		}
	}
};


// Event for testing purposes
class TestEvent : public Event {
public:
	static const eventType m_eventType;

	TestEvent() {};
	~TestEvent() {};
	eventType vGetEventType() const override { return m_eventType; };
	std::string vGetEventName() const override { return m_eventName; };

private:
	static const std::string m_eventName;
};
const eventType TestEvent::m_eventType(0x7de5db2b);
const std::string TestEvent::m_eventName("TestEvent");

// Another test event
class TestEvent2 : public Event {
public:
	static const eventType m_eventType;

	TestEvent2() {};
	~TestEvent2() {};
	eventType vGetEventType() const override { return m_eventType; };
	std::string vGetEventName() const override { return m_eventName; };

private:
	static const std::string m_eventName;
};
const eventType TestEvent2::m_eventType(0x8545422b);
const std::string TestEvent2::m_eventName("TestEvent2");


// Used to count the callback count on event trigger
int g_callbackCounter = 0;




// Dummy classes that are registered for TestEvent
class TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(eventType evtType = TestEvent::m_eventType) { return m_listener.registerForEvent(evtType, std::bind(&TestClass1::callback, std::placeholders::_1)); }
	bool unregisterListener(eventType evtType = TestEvent::m_eventType) { return m_listener.unregisterForEvent(evtType); }

protected:
	EventListener m_listener;
};
class SubTestClass1 : public TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(eventType evtType = TestEvent::m_eventType) { return m_listener.registerForEvent(evtType, std::bind(&SubTestClass1::callback, std::placeholders::_1)); }
	bool unregisterListener(eventType evtType = TestEvent::m_eventType) { return m_listener.unregisterForEvent(evtType); }
};
class SubTestClass2 : public TestClass1 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(eventType evtType = TestEvent::m_eventType) { return m_listener.registerForEvent(evtType, std::bind(&SubTestClass2::callback, std::placeholders::_1)); }
	bool unregisterListener(eventType evtType = TestEvent::m_eventType) { return m_listener.unregisterForEvent(evtType); }
};
class TestClass2 {
public:
	static void callback(std::shared_ptr<IEvent> ptr) { ++g_callbackCounter; }
	bool registerListener(eventType evtType = TestEvent::m_eventType) { return m_listener.registerForEvent(evtType, std::bind(&TestClass2::callback, std::placeholders::_1)); }
	bool unregisterListener(eventType evtType = TestEvent::m_eventType) { return m_listener.unregisterForEvent(evtType); }
protected:
	EventListener m_listener;
};
