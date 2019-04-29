#pragma once

#include "event.h"
#include <functional>
#include <map>
#include <vector>
#include <memory>

typedef int ManagerID;

class SEManager
{
public:
    SEManager(int managerId = -1) : m_mgrId(managerId) {}
    void subscribe(Event::EventId id, std::function<void(Event)> func);

    template <typename T>
    void subscribe(Event::EventId id, std::function<void(T)> func)
    {
        auto cbWrapper = [func](Event data)
        {
            auto val = boost::any_cast<T>(data.m_data);
            func(val);
        };
        subscribe(id, cbWrapper);
    }

    void unSubscribe(Event::EventId msg);
    void postMsg(Event::EventId msg, boost::any any = boost::any());
    void postMsg(Event msg);

    template <typename T>
    void postMsg(Event::EventId msgId, T value)
    {
        Event msg{msgId, value};
        postMsg(msg);
    }

    static void clearAllSubscribe();
private:
    ManagerID m_mgrId{-1};
    static std::map<Event::EventId, std::map<ManagerID, std::function<void(Event)>>> m_functionMaps;
};
