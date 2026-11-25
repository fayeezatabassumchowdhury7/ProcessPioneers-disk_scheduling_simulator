CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

.PHONY: all test clean legacy

ALGOS    = src/fcfs.cpp src/sstf.cpp src/scan.cpp src/cscan.cpp src/look.cpp src/clook.cpp
ENGINE   = src/simulator.cpp src/metrics.cpp src/report.cpp src/best_report.cpp \
           src/input.cpp src/generator.cpp src/csv_export.cpp $(ALGOS)

all: disksim sim_test

disksim: src/main.cpp $(ENGINE) src/types.h src/policies.h
	$(CXX) $(CXXFLAGS) src/main.cpp $(ENGINE) -o disksim

sim_test: src/test_sim.cpp $(ENGINE) src/types.h src/policies.h
	$(CXX) $(CXXFLAGS) src/test_sim.cpp $(ENGINE) -o sim_test

test: sim_test
	./sim_test

clean:
	rm -f disksim sim_test disksim_legacy

# Old static-queue version (6 separate algorithm files), kept for reference
LEGACY = legacy/MainMenu.cpp legacy/FCFS.cpp legacy/SSTF.cpp legacy/SCAN.cpp \
         legacy/CSCAN.cpp legacy/LOOK.cpp legacy/CLOOK.cpp

legacy: $(LEGACY) legacy/algorithms.h
	$(CXX) $(CXXFLAGS) $(LEGACY) -o disksim_legacy
