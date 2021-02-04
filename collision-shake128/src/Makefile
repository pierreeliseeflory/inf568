CXX = g++
MPICXX = mpic++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17 -g# -fsanitize=address
LDLIBS = fips202.a

SRC = collision-shake128.cpp
EXEC = ../bin/collision-shake128

SRCP = distinguished-collision-shake128.cpp
EXECP = ../bin/distinguished-collision-shake128

all: single-thread parallel

single-thread: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(EXEC)

parallel: $(SRCP)
	$(MPICXX) $(CXXFLAGS) $(SRCP) $(LDLIBS) -o $(EXECP)

clean:
	rm -rf $(EXEC)
	rm -rf $(EXECP)