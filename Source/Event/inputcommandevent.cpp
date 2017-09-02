#include "Event/inputcommandevent.h"

// 32bit GUID created with visual studio Tools->Create GUID->DEFINE GUID 
const EventType InputCommandEvent::eventType(0xf894bb78);
const std::string InputCommandEvent::m_eventName("Input Command");

InputCommandEvent::InputCommandEvent(std::string key) : m_pressedKey(key) {}

EventType InputCommandEvent::vGetEventType() const { return eventType; }

std::string InputCommandEvent::vGetEventName() const { return m_eventName; }
