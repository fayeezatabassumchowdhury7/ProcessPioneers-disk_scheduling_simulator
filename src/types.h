#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

using namespace std;

// One disk request. "arrival" = time unit at which it enters the queue.
struct Request { int id; int cylinder; int arrival; };

// One request after it has been served.
struct Served {
    int id, cylinder, arrival;
    int time;     // clock value when the head reached this cylinder
    int wait;     // time - arrival
    int seek;     // cylinders moved since the previous service
    int skipped;  // requests served before this one that arrived after it
};

// disk = number of cylinders (0..disk-1), dir = +1 (R) or -1 (L),
// countJump = true: the C-SCAN / C-LOOK return jump counts as seek distance.
struct Config { int disk; int head; int dir; bool countJump; };

struct Workload { Config cfg; vector<Request> reqs; };

struct Result {
    string name;
    int disk = 0, head = 0;
    vector<Served> order;
    vector<int> path;          // turning points / service points, for the chart
    int totalSeek = 0;
    int endTime = 0;
    double avgSeek = 0, avgWait = 0, waitStdDev = 0;
    int maxWait = 0, totalSkipped = 0, maxSkipped = 0;
};

enum class Algo { FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK };

// simulator.cpp
const char* algoName(Algo a);
const vector<Algo>& allAlgos();
Result simulate(Algo a, vector<Request> reqs, const Config& cfg);

// metrics.cpp
void computeMetrics(Result& r);

// report.cpp
void printResult(const Result& r);
void printChart(const Result& r);
void printComparison(const vector<Result>& all);

// best_report.cpp
string bestReport(const vector<Result>& all);

// input.cpp
string validateWorkload(const Workload& w);          // "" = OK, otherwise error text
bool loadWorkload(const string& path, Workload& w, string& err);
bool saveWorkload(const string& path, const Workload& w, string& err);

// generator.cpp  kind: uniform | clustered | sequential | starvation
vector<Request> generateWorkload(const string& kind, int n, int disk,
                                      int head, unsigned seed, int spread);

// csv_export.cpp
bool exportCsv(const string& base, const vector<Result>& all, string& err);

#endif
