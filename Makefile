TARGET = TEST
TARGET_STATIC_LIB = libMqttConnection.a
TARGET_DYNAMIC_LIB = libMqttConnection.so

CROSS_CHAIN = aarch64-linux-gnu-

CC = $(CROSS_CHAIN)gcc
CXX = $(CROSS_CHAIN)g++
AR = $(CROSS_CHAIN)ar
STRIP = $(CROSS_CHAIN)strip

LIB_PATH = \
	./Connection/Mqtt/MqttConnection.o

LIB_SRC_PATH = \
	./Connection/Mqtt/MqttConnection.cpp \

TEST_SRC_PATH = \
	./Test.cpp

INCLUDE_FLAG = \
	-I./ \
	-I./Connection \
	-I./Connection/Mqtt \
	-I./Singleton \
	-I./third_party/include

LIB_FLAGS = \
	-L./ \
	-L./third_party/lib/aarch64
	# -L./third_party/lib/x86_64

LIBXX_FLAGS = -lpthread -lmosquitto
LIB_CUSTOM_FLAGS = -lMqttConnection

C_FLAGS = 
CXX_FLAGS = -std=c++11

GEN_DYNAMIC_FLAGS = -shared -fPIC

all:
	$(CXX) $(LIB_SRC_PATH) $(TEST_SRC_PATH) $(INCLUDE_FLAG) $(LIB_FLAGS) $(LIBXX_FLAGS) $(CXX_FLAGS) -o $(TARGET)
	$(STRIP) $(TARGET)

static:
	$(CXX) -c $(LIB_SRC_PATH) $(INCLUDE_FLAG) $(LIB_FLAGS) $(LIBXX_FLAGS) $(CXX_FLAGS) -o $(LIB_PATH)
	$(AR) -cr $(TARGET_STATIC_LIB) $(LIB_PATH)

dynamic:
	$(CXX) $(GEN_DYNAMIC_FLAGS) $(LIB_SRC_PATH) $(INCLUDE_FLAG) $(LIB_FLAGS) $(LIBXX_FLAGS) $(CXX_FLAGS) -o $(TARGET_DYNAMIC_LIB)
	$(STRIP) $(TARGET_DYNAMIC_LIB)

all_with_customlib:
	$(CXX) $(TEST_SRC_PATH) $(INCLUDE_FLAG) $(LIB_CUSTOM_FLAGS) $(LIB_FLAGS) $(LIBXX_FLAGS) $(CXX_FLAGS) -o $(TARGET)

clean:
	rm -rf $(LIB_PATH) $(TARGET) $(TARGET_DYNAMIC_LIB) $(TARGET_STATIC_LIB)