CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address

SRC = keccak.cpp permutations.cpp
EXEC = shake128

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -o bin/$(EXEC)

clean:
	rm -rf $(EXEC)