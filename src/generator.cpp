#include "types.h"
#include <algorithm>
using namespace std;

// Small linear congruential generator: same seed -> same workload on every machine.
static unsigned nextRand(unsigned& s)
{
    s = s * 1664525u + 1013904223u;
    return s >> 8;
}

static int clampInt(int v, int lo, int hi) { return max(lo, min(hi, v)); }

// uniform    : random cylinders, random arrival times in 0..spread
// clustered  : cylinders gather around 3 hot areas
// sequential : cylinders increase step by step, arrivals spread evenly
// starvation : one far request at the last cylinder, then a steady stream of
//              requests close to the head (shows SSTF starvation)
vector<Request> generateWorkload(const string& kind, int n, int disk, int head,
                                 unsigned seed, int spread)
{
    vector<Request> reqs;
    unsigned s = seed;
    int centers[3] = {disk / 5, disk / 2, (4 * disk) / 5};

    for (int i = 0; i < n; i++) {
        int cyl = 0, arr = 0;
        if (kind == "uniform") {
            cyl = (int)(nextRand(s) % (unsigned)disk);
            arr = (int)(nextRand(s) % (unsigned)(spread + 1));
        } else if (kind == "clustered") {
            int c = centers[nextRand(s) % 3];
            cyl = clampInt(c + (int)(nextRand(s) % 21) - 10, 0, disk - 1);
            arr = (int)(nextRand(s) % (unsigned)(spread + 1));
        } else if (kind == "sequential") {
            int start = (int)(seed % (unsigned)disk);
            cyl = (start + 2 * i) % disk;
            arr = (int)((long long)i * spread / max(1, n));
        } else {                                   // starvation
            if (i == 0) {
                cyl = disk - 1;
                arr = 0;
            } else {
                static const int offs[4] = {-2, 2, -1, 1};
                cyl = clampInt(head + offs[(i - 1) % 4], 0, disk - 1);
                arr = (i - 1) * 2;
            }
        }
        reqs.push_back({i + 1, cyl, arr});
    }
    return reqs;
}
