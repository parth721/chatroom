CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra
TARGET_SERVER = server
TARGET_CLIENT = client
SRC_SERVER = server.cpp
SRC_CLIENT = client.cpp 

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) -o $(TARGET_SERVER) $(SRC_SERVER)

$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CXX) $(CXXFLAGS) -o $(TARGET_CLIENT) $(SRC_CLIENT)

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

.PHONY: all clean
