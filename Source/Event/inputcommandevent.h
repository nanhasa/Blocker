#pragma once

#include <string>

#include "Event/event.h"

class InputCommandEvent : public Event {
public:
	static const eventType m_eventType;

	InputCommandEvent(std::string key);
	~InputCommandEvent() {};
	eventType vGetEventType() const override final;
	std::string vGetEventName() const override final;

private:
	static const std::string m_eventName;
	const std::string m_pressedKey;
};