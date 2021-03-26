#include <functional>
#include "MqttConnection.hpp"

bool BaseConnection::m_isConnected = false;

MqttConnection::MqttConnection(string host, int port, ConnectType type)
// : m_strHost(host), m_iPort(port), m_connectType(type)
{
    m_strHost = host;
    m_iPort = port;
    m_connectType = type;
    Init();
}

MqttConnection::~MqttConnection()
{
    if (MqttConnection::m_isConnected)
    {
        mosquitto_lib_cleanup();
        if (this->m_ptrMosq != NULL)
        {
            mosquitto_destroy(this->m_ptrMosq);
        }
    }
}

void MqttConnection::Init()
{
    mosquitto_lib_init();

    if (this->m_ptrMosq == NULL)
    {
        this->m_ptrMosq = mosquitto_new(NULL, true, NULL);
    }

    std::cout << "DEBUG" << std::endl;

    // auto ConnectCallback = std::bind(&MqttConnection::on_connect_callback,
    //                                  this,
    //                                  std::placeholders::_1,
    //                                  std::placeholders::_2,
    //                                  std::placeholders::_3);

    mosquitto_connect_callback_set(this->m_ptrMosq, MqttConnection::on_connect_callback);
    mosquitto_disconnect_callback_set(this->m_ptrMosq, MqttConnection::on_disconnect_callback);

    if (this->m_connectType == BDSTAR_MOSQUITTP_PUB_CONNECTION)
    {
        mosquitto_publish_callback_set(this->m_ptrMosq, MqttConnection::on_publish_callback);
    }
    else
    {
        mosquitto_subscribe_callback_set(this->m_ptrMosq, MqttConnection::on_subscribe_callback);
        mosquitto_message_callback_set(this->m_ptrMosq, MqttConnection::on_message_callback);
    }

    this->Connect();

    if (this->m_connectType == BDSTAR_MOSQUITTP_SUB_CONNECTION)
    {
        mosquitto_subscribe(this->m_ptrMosq, &this->m_mid, MQTT_TOPIC_WARN_VIEW, SUB_QOS);
    }
}

void MqttConnection::Run()
{
    mosquitto_loop_start(this->m_ptrMosq);
}

void MqttConnection::Connect()
{
    if (mosquitto_connect(this->m_ptrMosq, this->m_strHost.c_str(), this->m_iPort, this->m_iKeepAlive) != MOSQ_ERR_SUCCESS)
    {
        std::cout << "Connect Error" << std::endl;
        MqttConnection::m_isConnected = false;
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

void MqttConnection::MsgRecieve()
{
}

void MqttConnection::MsgSend(std::string &strMsg)
{
    std::cout << "MsgSend:[" << strMsg.substr(0, strMsg.length() - 1) << "]" << std::endl;
    mosquitto_publish(this->m_ptrMosq, NULL, MQTT_TOPIC_WARN_VIEW, strMsg.length(), (void *)strMsg.c_str(), 1, false);
}

void MqttConnection::on_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    std::cout << "Mqtt Connect Success" << std::endl;
    MqttConnection::m_isConnected = true;
}

void MqttConnection::on_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
    std::cout << "Mqtt Disconnected" << std::endl;
    MqttConnection::m_isConnected = false;
}

void MqttConnection::on_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    std::cout << "on_publish_callback" << std::endl;
}

void MqttConnection::on_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    std::cout << "on_subscribe_callback" << std::endl;
}

void MqttConnection::on_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    std::cout << "FROM topic: " << msg->topic << std::endl;
    std::string strWarnMsg = (const char *)msg->payload;
    std::cout << "GOT message:" << strWarnMsg << std::endl;
}