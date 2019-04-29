#include "semanager.h"
#include <algorithm>

std::map<MsgData::MsgId, std::map<ManagerID, std::function<void(shared_ptr<MsgData>)>>> SEManager::m_functionMaps;

void SEManager::subscribe(MsgData::MsgId msg, std::function<void(shared_ptr<MsgData> msg)> func)
{
    m_functionMaps[msg][m_mgrId] = func;
}

void SEManager::unSubscribe(MsgData::MsgId msg)
{
    m_functionMaps[msg].erase(m_mgrId);
}

void SEManager::postMsg(MsgData::MsgId msgId, boost::any any)
{
    postMsg(std::make_shared<MsgData>(msgId, any));
}

void SEManager::postMsg(shared_ptr<MsgData> msg)
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
