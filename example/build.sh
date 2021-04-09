#!/bin/bash

if [ $1 == x86_64 ];then
    g++ Test.cpp -I./include -L./lib/x86_64 -lMqttConnection -lpthread -std=c++11 -o TEST
    export LD_LIBRARY_PATH="./lib/x86_64:$LD_LIBRARY_PATH"
elif [ $1 == aarch64 ];then
    aarch64-linux-gnu-g++ Test.cpp -I./include -L./lib/aarch64 -lMqttConnection -lpthread -std=c++11 -o TEST
    export LD_LIBRARY_PATH="./lib/aarch64:$LD_LIBRARY_PATH"
else
    echo "Please usage: 
            \"./build.sh x86_64\" for building x86_64 application.
            \"./build.sh aarch64\" for building ARM application."
fi