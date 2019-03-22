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
    void subscribe(Msg::MsgId id, std::function<void(shared_ptr<Msg>)> func);

    template<typename T>
    void subscribe(Msg::MsgId id, std::function<void(T)> func)
    {
        auto cbWrapper = [func](shared_ptr<Msg> msg)
        {
            auto msgData = std::dynamic_pointer_cast<MsgData<T>>(msg);
            func(msgData->m_data);
        };
        subscribe(id, cbWrapper);
    }

    void unSubscribe(Msg::MsgId msg);
    void postMsg(Msg::MsgId msg);
    void postMsg(shared_ptr<Msg> msg);

    template <typename T>
    void postMsg(Msg::MsgId msgId, T value)
    {
        shared_ptr<MsgData<T>> msg = std::make_shared<MsgData<T>>(msgId, value);
        postMsg(msg);
    }

    static void clearAllSubscribe();
private:
    ManagerID m_mgrId{-1};
    static std::map<Msg::MsgId, std::map<ManagerID, std::function<void(shared_ptr<Msg>)>>> m_functionMaps;
};
