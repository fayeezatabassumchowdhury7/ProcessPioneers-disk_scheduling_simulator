#include "types.h"
#include "policies.h"
#include <algorithm>
#include <cstdlib>
using namespace std;

const char* algoName(Algo a)
{
    switch (a) {
    case Algo::FCFS:  return "FCFS";
    case Algo::SSTF:  return "SSTF";
    case Algo::SCAN:  return "SCAN";
    case Algo::CSCAN: return "C-SCAN";
    case Algo::LOOK:  return "LOOK";
    case Algo::CLOOK: return "C-LOOK";
    }
    return "?";
}

const vector<Algo>& allAlgos()
{
    static const vector<Algo> v = {Algo::FCFS, Algo::SSTF, Algo::SCAN,
                                   Algo::CSCAN, Algo::LOOK, Algo::CLOOK};
    return v;
}

// Calls the move function of the chosen algorithm (each lives in its own file).
static int moveHead(Algo a, const vector<Request>& p, int& pos, int& dir, const Config& c)
{
    switch (a) {
    case Algo::FCFS:  return moveFCFS(p, pos, dir, c);
    case Algo::SSTF:  return moveSSTF(p, pos, dir, c);
    case Algo::SCAN:  return moveSCAN(p, pos, dir, c);
    case Algo::CSCAN: return moveCSCAN(p, pos, dir, c);
    case Algo::LOOK:  return moveLOOK(p, pos, dir, c);
    case Algo::CLOOK: return moveCLOOK(p, pos, dir, c);
    }
    return 1;
}

static void addPath(vector<int>& path, int v)
{
    if (path.empty() || path.back() != v) path.push_back(v);
}

// Simulation clock: 1 cylinder of head movement = 1 time unit.
// "reqs" is passed by value, so the caller's list is never modified.
Result simulate(Algo a, vector<Request> reqs, const Config& cfg)
{
    stable_sort(reqs.begin(), reqs.end(),
                [](const Request& x, const Request& y) { return x.arrival < y.arrival; });

    Result res;
    res.name = algoName(a);
    res.disk = cfg.disk;
    res.head = cfg.head;

    vector<Request> pending;
    size_t next = 0, n = reqs.size();
    int t = 0, pos = cfg.head, dir = cfg.dir, moved = 0;
    res.path.push_back(pos);

    while (res.order.size() < n) {
        // 1. requests that have arrived by now join the queue
        while (next < n && reqs[next].arrival <= t) pending.push_back(reqs[next++]);

        // 2. serve every pending request at the current cylinder
        for (size_t i = 0; i < pending.size();) {
            if (pending[i].cylinder == pos) {
                const Request& r = pending[i];
                res.order.push_back({r.id, r.cylinder, r.arrival, t, t - r.arrival, moved, 0});
                addPath(res.path, pos);
                moved = 0;
                pending.erase(pending.begin() + i);
            } else {
                i++;
            }
        }
        if (res.order.size() == n) break;

        // 3. queue empty: head waits, clock jumps to the next arrival (idle gap)
        if (pending.empty()) { t = reqs[next].arrival; continue; }

        // 4. move the head
        int oldPos = pos, oldDir = dir;
        int cost = moveHead(a, pending, pos, dir, cfg);
        if (dir != oldDir) {                                // turned round: remember the turning point
            addPath(res.path, oldPos);
        } else if (abs(pos - oldPos) != 1) {                // jump: remember both ends
            addPath(res.path, oldPos);
            addPath(res.path, pos);
        }
        t += cost;
        moved += cost;
        res.totalSeek += cost;
    }
    res.endTime = t;
    computeMetrics(res);
    return res;
}
