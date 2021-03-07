CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
GMP = -lgmpxx -lgmp
OMP = -fopenmp

TARGETS = test-chacha20 aead

all: $(TARGETS)

test-chacha20: chacha20-core.cpp chacha20-core.hpp testchacha20.cpp testchacha20.hpp
	$(CXX) $(CXXFLAGS) chacha20-core.cpp chacha20-core.hpp testchacha20.cpp testchacha20.hpp $(OMP) -o ../bin/testchacha20

aead: chacha20-core.cpp chacha20-core.hpp poly1305-core.hpp poly1305-core.cpp aead_chacha20_poly1305.cpp
	$(CXX) $(CXXFLAGS) chacha20-core.cpp chacha20-core.hpp poly1305-core.hpp poly1305-core.cpp aead_chacha20_poly1305.cpp $(GMP) -o ../bin/aead_chacha20_poly1305
