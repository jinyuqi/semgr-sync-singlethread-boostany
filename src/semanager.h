#pragma once

#include "msg.h"
#include <functional>
#include <map>
#include <vector>
#include <memory>

using std::shared_ptr;

typedef int ManagerID;

class SEManager
{
public:
    SEManager(int managerId = -1) : m_mgrId(managerId) {}
    void subscribe(MsgData::MsgId id, std::function<void(shared_ptr<MsgData>)> func);

    template <typename T>
    void subscribe(MsgData::MsgId id, std::function<void(T)> func)
    {
        auto cbWrapper = [func](shared_ptr<MsgData> data)
        {
            auto val = boost::any_cast<T>(data->m_data);
            func(val);
        };
        subscribe(id, cbWrapper);
    }

    void unSubscribe(MsgData::MsgId msg);
    void postMsg(MsgData::MsgId msg, boost::any any = boost::any());
    void postMsg(shared_ptr<MsgData> msg);

    template <typename T>
    void postMsg(MsgData::MsgId msgId, T value)
    {
        shared_ptr<MsgData> msg = std::make_shared<MsgData>(msgId, value);
        postMsg(msg);
    }

    static void clearAllSubscribe();
private:
    ManagerID m_mgrId{-1};
    static std::map<MsgData::MsgId, std::map<ManagerID, std::function<void(shared_ptr<MsgData>)>>> m_functionMaps;
};
