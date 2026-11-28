#include "policies.h"
#include <cstdlib>
using namespace std;

// SSTF: go to the pending request nearest to the head in one move.
// Tie-break: the request that arrived earlier (it comes first in p).
int moveSSTF(const vector<Request>& p, int& pos, int&, const Config&)
{
    int target = p[0].cylinder;
    for (const auto& r : p)
        if (abs(r.cylinder - pos) < abs(target - pos)) target = r.cylinder;
    int cost = abs(target - pos);
    pos = target;
    return cost;
}
