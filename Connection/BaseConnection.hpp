#ifndef _BASE_CONNECTION_
#define _BASE_CONNECTION_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef enum
{
    BDSTAR_SOCKET_TCP,
    BDSTAR_SOCKET_UDP,
    BDSTAR_SOCKET_MQTT,
} ConnectType;

typedef void (*DataRecieveCallBack)(const std::string &rData);

class MqttConnection;
class TCPConnection;
class BaseConnection
{
    friend class MqttConnection;
    friend class TCPConnection;

public:
    BaseConnection() {}
    virtual ~BaseConnection() {}

    virtual void Init() {}
    virtual void Run() {}
    virtual void Connect() {}
    virtual void DisConnect() {}
    virtual void MsgRecieve(std::string strMsg) {}
    virtual void MsgSend(std::string strMsg) {}
    virtual void MsgSend(std::string strTopic, std::string strMsg) {}
    virtual void RegisterDataRecieveCallBackFunc(DataRecieveCallBack CallBackFunc) {}

    virtual void Subscribe(std::vector<std::string> &strTopicList) {}

private:
    ConnectType m_connectType;
    std::string m_strHost;
    int m_iPort;
    bool m_isConnected;
};

#endif //!_BASE_CONNECTION_