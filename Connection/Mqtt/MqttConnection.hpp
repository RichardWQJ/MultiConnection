#ifndef _MQTT_CONNECTION_
#define _MQTT_CONNECTION_

#include "mosquitto.h"
#include "Util.h"
#include "BaseConnection.hpp"
#include "Singleton.hpp"

#define MQTT_HOST "127.0.0.1"
#define MQTT_PORT 1883

#define PUB_QOS 1
#define SUB_QOS 1

#define MQTT_TOPIC_WARN_VIEW "BDSTAR/VIEW"
#define MQTT_TOPIC_WARN_AUDIO "BDSTAR/AUDIO"

#define MQTT_TOPIC_WARN_MIDDLEWARE_TO_HMI "BDSTAR/MIDDLEWARE/HMI" //Msg: 中间件->HMI
#define MQTT_TOPIC_WARN_HMI_TO_MIDDLEWARE "BDSTAR/HMI/MIDDLEWARE" //Msg: HMI->中间件

class MqttConnection : public BaseConnection, public Singleton<MqttConnection>
{
public:
    MqttConnection();
    ~MqttConnection();

    void Init(string host, int port, ConnectType type);
    void Run();
    void Connect();
    void DisConnect();
    void MsgRecieve(std::string strMsg);
    void MsgSend(std::string strMsg) {}
    void MsgSend(std::string strTopic, std::string strMsg);
    void RegisterDataRecieveCallBackFunc(DataRecieveCallBack CallBackFunc);

    void Subscribe(std::vector<std::string> &strTopicList);
    void UnSubscribe(std::vector<string> &strTopicList);

    void SetConnectedFlag(bool flag);

private:
    bool IsConnected();

    DataRecieveCallBack mqttDataRecieveCallBackFunction = NULL;
    struct mosquitto *m_ptrMosq = NULL;
    int m_iKeepAlive = 60;
    int m_mid = 0;
};

#endif //!_MQTT_CONNECTION_