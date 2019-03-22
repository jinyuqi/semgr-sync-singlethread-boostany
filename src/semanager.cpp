#include "semanager.h"
#include <algorithm>

std::map<Msg::MsgId, std::map<ManagerID, std::function<void(shared_ptr<Msg>)>>> SEManager::m_functionMaps;

void SEManager::subscribe(Msg::MsgId msg, std::function<void(shared_ptr<Msg> msg)> func)
{
    m_functionMaps[msg][m_mgrId] = func;
}

void SEManager::unSubscribe(Msg::MsgId msg)
{
    m_functionMaps[msg].erase(m_mgrId);
}

void SEManager::postMsg(Msg::MsgId msgId)
{
    postMsg(std::make_shared<Msg>(msgId));
}

void SEManager::postMsg(shared_ptr<Msg> msg)
{
    auto msgId = msg->m_id;
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
