CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS = -lgmpxx -lgmp

TARGETS = gen check

all: $(TARGETS)

gen: poly1305-gen.cpp poly1305-core.hpp poly1305-core.cpp
	$(CXX) $(CXXFLAGS) poly1305-gen.cpp poly1305-core.cpp $(LDFLAGS) -o ../bin/poly1305-gen

check: poly1305-check.cpp poly1305-core.hpp poly1305-core.cpp
	$(CXX) $(CXXFLAGS) poly1305-check.cpp poly1305-core.cpp $(LDFLAGS) -o ../bin/poly1305-check
