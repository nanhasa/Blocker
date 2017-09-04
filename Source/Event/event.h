#pragma once

#include <string>

#include "interfaces.h"

class Event : public IEvent {
public:
	virtual ~Event() {};

	EventType vGetEventType() const override = 0;

	/**
	 * \brief Used to access time of event creation
	 * \return Timestamp in milliseconds
	 */
	long vGetCreateTime() const override final;

	std::string vGetEventName() const override = 0;

protected:
	long m_createTimestamp; //!< Timestamp in milliseconds when event was created

	/**
	 * \brief Builder hidden so that this base class cannot be instantiated
	 */
	Event();
};
