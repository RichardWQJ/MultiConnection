#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>
#include <string.h>
#include "MqttConnection.hpp"
#include "MsgHandler.hpp"
#include "Util.h"

std::string strComponentName = "MQTT_TEST";

void PubThreadFun()
{
    std::shared_ptr<BaseConnection> client = std::make_shared<MqttConnection>(MQTT_HOST, MQTT_PORT, BDSTAR_MOSQUITTP_PUB_CONNECTION);
    client->Run();

    DebugPrint("PUB Thread:[%d]\r\n", std::this_thread::get_id());

    std::string strMsgSend;
    while (1)
    {
        time_t now_time = time(NULL);
        tm *t_tm = localtime(&now_time);

        strMsgSend = asctime(t_tm);
        strMsgSend = strMsgSend.substr(0, strMsgSend.length() - 1);
        client->MsgSend(strMsgSend);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void SubThreadFun()
{
    std::shared_ptr<BaseConnection> client = std::make_shared<MqttConnection>(MQTT_HOST, MQTT_PORT, BDSTAR_MOSQUITTP_SUB_CONNECTION);
    client->Run();

    DebugPrint("SUB Thread:[%d]\r\n", std::this_thread::get_id());

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

    DebugPrint("MAIN Thread:[%d]\r\n", std::this_thread::get_id());
    while (1)
    {
        std::string strWarnMsg = MsgHandler::GetInstance()->MsgPop();
        if (!strWarnMsg.empty())
        {
            InfoPrint("Warning Message:[%s] Comming\r\n", strWarnMsg.c_str());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}