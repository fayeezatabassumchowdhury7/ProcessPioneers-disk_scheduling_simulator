#include "policies.h"
using namespace std;

// LOOK: like SCAN, but turn round as soon as no pending request is in front,
// without going to the disk end.
int moveLOOK(const vector<Request>& p, int& pos, int& dir, const Config&)
{
    if (!anyAhead(p, pos, dir)) dir = -dir;
    pos += dir;
    return 1;
}
