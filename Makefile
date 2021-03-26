TARGET = TEST

CROSS_CHAIN = #aarch64-linux-gnu-

CC = $(CROSS_CHAIN)gcc
CXX = $(CROSS_CHAIN)g++
AR = $(CROSS_CHAIN)ar
STRIP = $(CROSS_CHAIN)strip

SRC_PATH = \
	./Test.cpp \
	./Connection/Mqtt/MqttConnection.cpp \
	./Connection/Tcp/TcpConnection.cpp

INCLUDE_FLAG = \
	-I./Connection \
	-I./Connection/Mqtt \
	-I./Connection/Tcp \
	-I./third_party/include

LIB_FLAG = \
	-L./third_party/lib/x86_64
	#-L./third_party/lib/aarch64
LIB_FLAGS =
LIBXX_FLAGS = -lpthread -lmosquitto

C_FLAGS = 
CXX_FLAGS = -std=c++11

all:
	$(CXX) $(SRC_PATH) $(INCLUDE_FLAG) $(LIB_FLAG) $(LIBXX_FLAGS) $(CXX_FLAGS) -o $(TARGET)
