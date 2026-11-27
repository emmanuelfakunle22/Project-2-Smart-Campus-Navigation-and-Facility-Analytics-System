CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -Iinclude
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

campus: $(OBJS)
	$(CXX) $(CXXFLAGS) -o campus $(OBJS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f campus src/*.o

.PHONY: clean
