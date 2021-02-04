CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS = -lgmpxx -lgmp

TARGETS = montgomery x25519

all: $(TARGETS)

montgomery: montgomery.cpp montgomery.hpp
	$(CXX) $(CXXFLAGS) montgomery.cpp $(LDFLAGS) -o ../bin/montgomery

x25519: x25519.cpp montgomery.hpp montgomery.cpp
	$(CXX) $(CXXFLAGS) x25519.cpp montgomery.cpp $(LDFLAGS) -o ../bin/x25519
