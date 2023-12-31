CXX = g++

SRCS = cache.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = headers.hpp cache.hpp

all: simulator

simulator: $(OBJS)
	$(CXX) $(OBJS) -o simulator

%.o: %.cpp $(DEPS)
	$(CXX) -c $< -o $@

clean:
	rm -f $(OBJS) simulator
