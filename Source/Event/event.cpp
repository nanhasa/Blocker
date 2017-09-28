#include "Event/event.h"

#include "Utility/utility.h"

long Event::vGetCreateTime() const { return m_createTimestamp; }

Event::Event() : m_createTimestamp(utility::timestampMs()) {}
