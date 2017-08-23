#include "Event/inputcommandevent.h"
#include "utility.h"

// 32bit GUID created with visual studio Tools->Create GUID->DEFINE GUID 
const eventType InputCommandEvent::m_eventType(0xf894bb78);
const std::string InputCommandEvent::m_eventName("Input Command");

InputCommandEvent::InputCommandEvent(std::string key) : m_pressedKey(key) {}

eventType InputCommandEvent::vGetEventType() const {
	return m_eventType;
}

std::string InputCommandEvent::vGetEventName() const {
	return m_eventName;
}
