#ifndef _BASE_CONNECTION_
#define _BASE_CONNECTION_

#include <iostream>
#include <string>

using namespace std;

typedef enum
{
    BDSTAR_SOCKET_TCP_CONNECTION,
    BDSTAR_SOCKET_UDP_CONNECTION,
    BDSTAR_MOSQUITTP_PUB_CONNECTION,
    BDSTAR_MOSQUITTP_SUB_CONNECTION
} ConnectType;

class BaseConnection
{
public:
    ConnectType m_connectType;
    std::string m_strHost;
    int m_iPort;
    static bool m_isConnected;

public:
    BaseConnection() {}
    virtual ~BaseConnection() {};

    virtual void Init() {}
    virtual void Run() {}
    virtual void Connect() {}
    virtual void DisConnect() {}
    virtual void MsgRecieve() {}
    virtual void MsgSend(std::string &strMsg) {}
};

#endif //!_BASE_CONNECTION_