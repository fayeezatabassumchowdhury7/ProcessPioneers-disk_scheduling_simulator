#include "policies.h"
#include <cstdlib>
using namespace std;

// FCFS: go to the oldest pending request in one move.
int moveFCFS(const vector<Request>& p, int& pos, int&, const Config&)
{
    int target = p[0].cylinder;            // p is sorted by arrival, so p[0] is the oldest
    int cost = abs(target - pos);
    pos = target;
    return cost;
}
