CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
GMP = -lgmpxx -lgmp
OMP = -fopenmp

TARGETS = test aead

all: $(TARGETS)

test: chacha20-core.cpp chacha20-core.hpp aead_chacha20_poly1305-core.cpp aead_chacha20_poly1305-core.hpp poly1305-core.cpp poly1305-core.hpp testchacha20.cpp
	$(CXX) $(CXXFLAGS) testchacha20.cpp chacha20-core.cpp aead_chacha20_poly1305-core.cpp poly1305-core.cpp $(GMP) -o ../bin/testchacha20

aead: chacha20-core.cpp chacha20-core.hpp poly1305-core.hpp poly1305-core.cpp aead_chacha20_poly1305-core.cpp aead_chacha20_poly1305-core.hpp aead-main.cpp
	$(CXX) $(CXXFLAGS) chacha20-core.cpp poly1305-core.cpp aead_chacha20_poly1305-core.cpp aead-main.cpp $(GMP) -o ../bin/aead_chacha20_poly1305
