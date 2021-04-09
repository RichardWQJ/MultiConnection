#include <functional>
#include "MqttConnection.hpp"

static void on_connect_callback(struct mosquitto *mosq, void *obj, int rc);
static void on_disconnect_callback(struct mosquitto *mosq, void *obj, int result);
static void on_publish_callback(struct mosquitto *mosq, void *obj, int mid);
static void on_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
static void on_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

MqttConnection::MqttConnection()
{
}

MqttConnection::~MqttConnection()
{
    if (IsConnected())
    {
        mosquitto_lib_cleanup();
        if (this->m_ptrMosq != NULL)
        {
            mosquitto_destroy(this->m_ptrMosq);
        }
    }
}

void MqttConnection::Init(string host, int port, ConnectType type)
{
    this->m_strHost = host;
    this->m_iPort = port;
    this->m_connectType = type;

    mosquitto_lib_init();

    if (this->m_ptrMosq == NULL)
    {
        this->m_ptrMosq = mosquitto_new(NULL, true, NULL);
    }

    mosquitto_connect_callback_set(this->m_ptrMosq, on_connect_callback);
    mosquitto_disconnect_callback_set(this->m_ptrMosq, on_disconnect_callback);

    mosquitto_publish_callback_set(this->m_ptrMosq, on_publish_callback);
    mosquitto_subscribe_callback_set(this->m_ptrMosq, on_subscribe_callback);
    mosquitto_message_callback_set(this->m_ptrMosq, on_message_callback);

    this->Connect();
}

void MqttConnection::Run()
{
    mosquitto_loop_start(this->m_ptrMosq);
}

void MqttConnection::Connect()
{
    if (mosquitto_connect(this->m_ptrMosq, this->m_strHost.c_str(), this->m_iPort, this->m_iKeepAlive) != MOSQ_ERR_SUCCESS)
    {
        ErrPrint("Connect Error\r\n");
        SetConnectedFlag(false);
        if (this->m_ptrMosq != NULL)
        {
            mosquitto_destroy(this->m_ptrMosq);
        }
        mosquitto_lib_cleanup();
        exit(-1);
    }
}

void MqttConnection::DisConnect()
{
}

void MqttConnection::MsgRecieve(std::string strMsg)
{
    if (this->mqttDataRecieveCallBackFunction != NULL)
    {
        this->mqttDataRecieveCallBackFunction(strMsg);
    }
    else
    {
        ErrPrint("mqttDataRecieveCallBackFunction is NULL!\r\n");
    }
}

void MqttConnection::MsgSend(std::string strTopic, std::string strMsg)
{
    InfoPrint("MsgSend--> Topic:[%s], Message:[%s]\r\n", strTopic.c_str(), strMsg.c_str());
    mosquitto_publish(this->m_ptrMosq, NULL, strTopic.c_str(), strMsg.length(), (void *)strMsg.c_str(), 1, false);
}

void MqttConnection::RegisterDataRecieveCallBackFunc(DataRecieveCallBack callBackFunc)
{
    this->mqttDataRecieveCallBackFunction = callBackFunc;
}

void MqttConnection::Subscribe(std::vector<std::string> &strTopicList)
{
    for (auto iter = strTopicList.begin(); iter != strTopicList.end(); iter++)
    {
        std::string strTopic = *iter;
        mosquitto_subscribe(this->m_ptrMosq, &this->m_mid, strTopic.c_str(), SUB_QOS);
    }
}

void MqttConnection::UnSubscribe(std::vector<std::string> &strTopicList)
{
    for (auto iter = strTopicList.begin(); iter != strTopicList.end(); iter++)
    {
        std::string strTopic = *iter;
        mosquitto_unsubscribe(this->m_ptrMosq, &this->m_mid, strTopic.c_str());
    }
}

bool MqttConnection::IsConnected()
{
    return this->m_isConnected;
}

void MqttConnection::SetConnectedFlag(bool flag)
{
    this->m_isConnected = flag;
}

static void on_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    DebugPrint("Mqtt Connect Success\r\n");
    MqttConnection::GetInstance()->SetConnectedFlag(true);
}

static void on_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    WarnPrint("Mqtt Disconnected\r\n");
    MqttConnection::GetInstance()->SetConnectedFlag(false);
}

static void on_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    DebugPrint("on_publish_callback\r\n");
}

static void on_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    DebugPrint("on_subscribe_callback\r\n");
}

static void on_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    InfoPrint("FROM topic:[%s], GOT message:[%s]", msg->topic, (const char *)msg->payload);

    std::string strWarnMsg = (const char *)msg->payload;
    if (!strWarnMsg.empty())
    {
        MqttConnection::GetInstance()->MsgRecieve(strWarnMsg);
    }
}