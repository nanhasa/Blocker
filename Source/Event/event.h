#pragma once

#include <string>

#include "interfaces.h"

class Event : public IEvent {
public:
	virtual ~Event() {};

	virtual eventType vGetEventType() const = 0;

	/**
	 * \brief Used to access time of event creation
	 * \return Timestamp in milliseconds
	 */
	float vGetCreateTime() const override final;

	virtual std::string vGetEventName() const = 0;

protected:
	float m_createTimestamp; //!< Timestamp in milliseconds when event was created

	/**
	 * \brief Builder hidden so that this base class cannot be instantiated
	 */
	Event();
};
