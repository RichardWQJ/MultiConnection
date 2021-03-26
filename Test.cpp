#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>
#include <string.h>
#include "MqttConnection.hpp"

void PubThreadFun()
{
    std::shared_ptr<BaseConnection> client = std::make_shared<MqttConnection>(MQTT_HOST, MQTT_PORT, BDSTAR_MOSQUITTP_PUB_CONNECTION);
    client->Run();

    std::cout << "PUB Thread:[" << std::this_thread::get_id() << "]" << std::endl;

    std::string strMsgSend;
    while (1)
    {
        time_t now_time = time(NULL);
        tm *t_tm = localtime(&now_time);

        strMsgSend = asctime(t_tm);
        client->MsgSend(strMsgSend);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void SubThreadFun()
{
    std::shared_ptr<BaseConnection> client = std::make_shared<MqttConnection>(MQTT_HOST, MQTT_PORT, BDSTAR_MOSQUITTP_SUB_CONNECTION);
    client->Run();

    std::cout << "SUB Thread:[" << std::this_thread::get_id() << "]" << std::endl;
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

    std::cout << "MAIN Thread:[" << std::this_thread::get_id() << "]" << std::endl;
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}