#pragma once

#include <boost/any.hpp>

using any = boost::any;

class MsgData
{

public:
    enum MsgId
    {
        Initialize,
        TEST_DOWORK
    };
    MsgData(MsgId id, any value)
    {
        m_id = id;
        m_data = value;
    }

    MsgId m_id;
    any m_data;
};

