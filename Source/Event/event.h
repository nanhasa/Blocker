#pragma once

#include <string>

#include "interfaces.h"

class Event : public IEvent {
public:
	virtual ~Event() {};

	virtual eventType vGetEventType() const = 0;
	float vGetCreateTime() const override final;
	virtual std::string vGetEventName() const = 0;

protected:
	float m_createTimestamp;
	Event();
};
