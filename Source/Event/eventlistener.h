#pragma once

#include <memory>
#include <vector>

#include "interfaces.h"

// This class is used as a component in other classes to register event listeners
class EventListener {
public:

	/**
	 * \brief Builder. Creates valid event listener by getting listenerID from event manager.
	 */
	EventListener();

	/**
	 * \brief Destructor. Removes its delegates from event manager.
	 */
	~EventListener();

	/**
	 * \brief Registers delegate function for event
	 * \param evtType Event type to be listened
	 * \param evtDelegate Function to be called when event is triggered
	 * \return True if delegate was successfully registered, otherwise false.
	 */
	bool registerForEvent(const eventType evtType, std::function<void(std::shared_ptr<IEvent>)> evtDelegate);

	/**
	 * \brief Used to stop listening event
	 * \param evtType Event type to be stopped listening to
	 * \return True if successfully removed listener, otherwise false
	 */
	bool unregisterForEvent(const eventType evtType);

private:
	const int m_listenerID;						//!< Listener ID used to identify listener in event manager
	std::vector<eventType> m_eventsListened;	//!< Vector of event types being listened to
};
