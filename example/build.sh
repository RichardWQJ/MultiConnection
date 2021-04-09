#!/bin/bash

g++ Test.cpp -I./include -L./lib -lMqttConnection -lpthread -std=c++11 -o TEST
export LD_LIBRARY_PATH="./lib:$LD_LIBRARY_PATH"