CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++17

TARGETS = general brute-force greedy1 greedy2 dynamic-programming

all: $(TARGETS)

general: general.cpp
	$(CXX) $(CXXFLAGS) -o general general.cpp

brute-force: algorithms/brute-force.cpp
	$(CXX) $(CXXFLAGS) -o brute-force algorithms/brute-force.cpp

greedy1: algorithms/greedy1.cpp
	$(CXX) $(CXXFLAGS) -o greedy1 algorithms/greedy1.cpp

greedy2: algorithms/greedy2.cpp
	$(CXX) $(CXXFLAGS) -o greedy2 algorithms/greedy2.cpp

dynamic-programming: algorithms/dynamic-programming.cpp
	$(CXX) $(CXXFLAGS) -o dynamic-programming algorithms/dynamic-programming.cpp

clean:
	rm -f $(TARGETS)
