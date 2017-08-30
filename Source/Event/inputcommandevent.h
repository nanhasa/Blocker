#pragma once

#include <string>

#include "Event/event.h"

class InputCommandEvent : public Event {
public:
	static const eventType m_eventType; //!< uint32_t representation of event GUID. Public so that registering for this event would be easier

	/**
	 * \brief Builder. Creates valid object
	 * \param key
	 */
	explicit InputCommandEvent(std::string key);

	~InputCommandEvent() {};

	/**
	 * \brief Destructor
	 * \return uint32_t value representing GUID
	 */
	eventType vGetEventType() const override final;

	/**
	 * \brief Returns event name in plain text
	 * \return Event name in plain text
	 */
	std::string vGetEventName() const override final;

private:
	static const std::string m_eventName;	//!< Event name in plain text
	const std::string m_pressedKey;			//!< Key pressed. Event payload
};
