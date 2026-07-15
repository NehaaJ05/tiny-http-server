CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = \
src/main.cpp \
src/server.cpp \
src/request.cpp \
src/response.cpp

TARGET = server

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)