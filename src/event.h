#pragma once

#include <boost/any.hpp>

using any = boost::any;

class Event
{
public:
    enum EventId
    {
        Initialize,
        TEST_DOWORK
    };
    Event(EventId id, any value)
    {
        m_id = id;
        m_data = value;
    }

    EventId m_id;
    any m_data;
};

