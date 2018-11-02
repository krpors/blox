CC = $(CXX)
CXXFLAGS = -std=c++11 -Wall -pedantic -MMD -MP
LDLIBS = $(shell pkg-config --libs sfml-all)

sources = $(wildcard *.cpp)

objects = $(patsubst %.cpp,%.o,$(sources))

all: main
all: CXXFLAGS += -Werror
main: $(objects)

debug: main
debug: CPPFLAGS = -UNDEBUG
debug: CXXFLAGS += -ggdb -O0
debug: LDFLAGS += -ggdb -O0

clean:
	$(RM) $(objects) $(objects:.o=.d) main

-include $(objects:.o=.d)

.PHONY: all clean
