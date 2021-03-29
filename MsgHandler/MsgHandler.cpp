#include "MsgHandler.hpp"

MsgHandler::MsgHandler()
{
}

MsgHandler::~MsgHandler()
{
}

void MsgHandler::MsgPush(std::string strMsg)
{
    if (!strMsg.empty())
    {
        this->m_msgQueue.Push(strMsg);
    }
}

std::string MsgHandler::MsgPop()
{
    std::string strMsg;
    if (!m_msgQueue.Empty())
    {
        strMsg = this->m_msgQueue.Top();
        this->m_msgQueue.Pop();
    }

    return strMsg;
}