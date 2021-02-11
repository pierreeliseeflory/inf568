CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS = -lgmpxx -lgmp

TARGETS = ecm

all: $(TARGETS)

ecm: ecm.cpp ecm.hpp montgomery.cpp montgomery.hpp
	$(CXX) $(CXXFLAGS) ecm.cpp montgomery.cpp $(LDFLAGS) -o ecm
