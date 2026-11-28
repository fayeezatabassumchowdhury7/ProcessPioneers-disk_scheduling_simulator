CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# one algorithm per file
ALGOS  = src/fcfs.cpp src/sstf.cpp src/scan.cpp src/cscan.cpp src/look.cpp src/clook.cpp

# clock, metrics, reports, input, generator, CSV + the algorithms
ENGINE = src/simulator.cpp src/metrics.cpp src/report.cpp src/best_report.cpp \
         src/input.cpp src/generator.cpp src/csv_export.cpp $(ALGOS)

HEADERS = src/types.h src/policies.h

# first version (fixed queue, no arrival times), kept in legacy/
LEGACY = legacy/MainMenu.cpp legacy/FCFS.cpp legacy/SSTF.cpp legacy/SCAN.cpp \
         legacy/CSCAN.cpp legacy/LOOK.cpp legacy/CLOOK.cpp

.PHONY: all test legacy clean

all: disksim

disksim: src/main.cpp $(ENGINE) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(ENGINE) -o disksim

sim_test: src/test_sim.cpp $(ENGINE) $(HEADERS)
	@test -s src/test_sim.cpp || (echo "ERROR: src/test_sim.cpp is empty. Upload the full file (about 6 KB)."; exit 1)
	$(CXX) $(CXXFLAGS) src/test_sim.cpp $(ENGINE) -o sim_test

test: sim_test
	./sim_test

legacy: $(LEGACY) legacy/algorithms.h
	$(CXX) $(CXXFLAGS) $(LEGACY) -o disksim_legacy

clean:
	rm -f disksim sim_test disksim_legacy
