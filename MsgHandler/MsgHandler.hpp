#ifndef _MSG_HANDLER_H_
#define _MSG_HANDLER_H_

#include "Util.h"
#include "Singleton.hpp"
#include "PriorityQueue.hpp"

class MsgHandler : public Singleton<MsgHandler>
{
public:
    MsgHandler();
    ~MsgHandler();

    void MsgPush(std::string strMsg);
    std::string MsgPop();

private:
    PriorityQueue<string> m_msgQueue;
};

#endif // !_MSG_HANDLER_H_
