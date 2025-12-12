TARGET = start

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRCS = main.cpp character.cpp menus.cpp crocodilo.cpp missile.cpp
OBJS = $(SRCS:.cpp=.o)

SFML_LIBS =-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)

fclean: clean
	rm -f $(TARGET)