#include "policies.h"
using namespace std;

// C-SCAN: always sweep in one direction. At the disk end, jump to the other end.
// The jump costs disk-1 if c.countJump is true, otherwise 0.
int moveCSCAN(const vector<Request>&, int& pos, int& dir, const Config& c)
{
    if (pos + dir < 0 || pos + dir >= c.disk) {
        pos = (dir > 0) ? 0 : c.disk - 1;
        return c.countJump ? c.disk - 1 : 0;
    }
    pos += dir;
    return 1;
}
