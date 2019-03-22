#pragma once

class Msg
{
public:
    virtual ~Msg(){}
    enum MsgId
    {
        Initialize,
        TEST_DOWORK
    };
    Msg(MsgId id) : m_id(id)
    {

    }
    MsgId m_id;
};

template <typename T>
class MsgData : public Msg
{
public:
    MsgData(Msg::MsgId id, T value) : Msg(id), m_data(value) {}
    T m_data;
};

