#include "policies.h"
using namespace std;

// SCAN (elevator): move one cylinder in the current direction, go all the way to
// the disk end, then reverse. Requests met on the way are served by the simulator.
int moveSCAN(const vector<Request>&, int& pos, int& dir, const Config& c)
{
    if (pos + dir < 0 || pos + dir >= c.disk) dir = -dir;   // reached the end: turn round
    pos += dir;
    return 1;
}
