#include "Event/event.h"
#include "utility.h"

float Event::vGetCreateTime() const { return m_createTimestamp; }

Event::Event() : m_createTimestamp(utility::getTimestamp()) {}
