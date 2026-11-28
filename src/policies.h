#ifndef POLICIES_H
#define POLICIES_H

#include "types.h"
#include <vector>

// One function per algorithm. Each one moves the head and returns the cost
// (time units = cylinders) of the move.
//   p   = requests that already arrived and are not served yet
//         (never empty, none is at the head position, oldest first)
//   pos = current head cylinder (changed by the function)
//   dir = current direction, +1 or -1 (changed when the head turns)
//   c   = disk size and return-jump setting
int moveFCFS (const vector<Request>& p, int& pos, int& dir, const Config& c);
int moveSSTF (const vector<Request>& p, int& pos, int& dir, const Config& c);
int moveSCAN (const vector<Request>& p, int& pos, int& dir, const Config& c);
int moveCSCAN(const vector<Request>& p, int& pos, int& dir, const Config& c);
int moveLOOK (const vector<Request>& p, int& pos, int& dir, const Config& c);
int moveCLOOK(const vector<Request>& p, int& pos, int& dir, const Config& c);

// true if at least one pending request lies in front of the head
inline bool anyAhead(const vector<Request>& p, int pos, int dir)
{
    for (const auto& r : p)
        if ((r.cylinder - pos) * dir > 0) return true;
    return false;
}

#endif
