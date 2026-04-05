CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = bin/solver
SRCS = Solver.cc

$(TARGET): $(SRCS)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -rf bin