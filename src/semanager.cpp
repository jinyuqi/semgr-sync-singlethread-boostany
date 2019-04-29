#include "semanager.h"
#include <algorithm>

std::map<Event::EventId, std::map<ManagerID, std::function<void(Event)>>> SEManager::m_functionMaps;

void SEManager::subscribe(Event::EventId msg, std::function<void(Event msg)> func)
{
    m_functionMaps[msg][m_mgrId] = func;
}

void SEManager::unSubscribe(Event::EventId msg)
{
    m_functionMaps[msg].erase(m_mgrId);
}

void SEManager::postMsg(Event::EventId msgId, boost::any any)
{
    postMsg(Event(msgId, any));
}

void SEManager::postMsg(Event msg)
{
    auto msgId = msg.m_id;
    if (m_functionMaps.find(msgId) != m_functionMaps.end())
    {
        auto functionMgrsList = m_functionMaps[msgId];

        for (auto it = functionMgrsList.begin(); it != functionMgrsList.end(); ++it)
        {
            it->second(msg);
        }
    }
}

void SEManager::clearAllSubscribe()
{
    m_functionMaps.clear();
}
