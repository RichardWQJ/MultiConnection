#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>
#include <string.h>
#include "MqttConnection.hpp"
#include "Util.h"

std::string strComponentName = "MQTT_TEST";

void DataRecieveCb(const std::string &dataRecieved)
{
    InfoPrint("Warning Message:[%s] Comming\r\n", dataRecieved.c_str());
}

void PubThreadFun()
{
    MqttConnection::GetInstance()->Init(MQTT_HOST, MQTT_PORT, BDSTAR_SOCKET_MQTT);
    MqttConnection::GetInstance()->Run();

    std::string strMsgSend;
    while (1)
    {
        time_t now_time = time(NULL);
        tm *t_tm = localtime(&now_time);

        strMsgSend = asctime(t_tm);
        strMsgSend = strMsgSend.substr(0, strMsgSend.length() - 1);
        MqttConnection::GetInstance()->MsgSend(MQTT_TOPIC_WARN_HMI, strMsgSend);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void SubThreadFun()
{
    std::vector<std::string> strTopicList;
    strTopicList.emplace_back(std::string(MQTT_TOPIC_WARN_HMI));

    MqttConnection::GetInstance()->Init(MQTT_HOST, MQTT_PORT, BDSTAR_SOCKET_MQTT);
    MqttConnection::GetInstance()->Subscribe(strTopicList);
    MqttConnection::GetInstance()->RegisterDataRecieveCallBackFunc(&DataRecieveCb);
    MqttConnection::GetInstance()->Run();

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(-1);
    }

    if (strcmp(argv[1], "pub") == 0)
    {
        std::thread mqttPubTh(PubThreadFun);
        mqttPubTh.detach();
    }
    else if (strcmp(argv[1], "sub") == 0)
    {
        std::thread mqttSubTh(SubThreadFun);
        mqttSubTh.detach();
    }
    else
    {
        exit(-1);
    }

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}