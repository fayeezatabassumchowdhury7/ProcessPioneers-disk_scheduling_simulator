#include "policies.h"
#include <cstdlib>
using namespace std;

// C-LOOK: sweep in one direction. When nothing is in front, jump straight to the
// farthest pending request behind the head and continue in the same direction.
int moveCLOOK(const vector<Request>& p, int& pos, int& dir, const Config& c)
{
    if (!anyAhead(p, pos, dir)) {
        int target = p[0].cylinder;
        for (const auto& r : p)
            if ((r.cylinder - target) * dir < 0) target = r.cylinder;   // farthest behind
        int cost = c.countJump ? abs(pos - target) : 0;
        pos = target;
        return cost;
    }
    pos += dir;
    return 1;
}
