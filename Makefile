CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC      = $(wildcard src/*.cpp)
TARGET   = disksim

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	bash tests/run_tests.sh

clean:
	rm -f $(TARGET)
