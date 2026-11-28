#include "types.h"
#include <algorithm>
#include <cmath>
using namespace std;

// Fills avgSeek, avgWait, maxWait, waitStdDev and the "skipped" counts.
// skipped(b) = number of requests served BEFORE b that arrived AFTER b
// (same arrival time: the one with the larger id counts as later).
void computeMetrics(Result& r)
{
    size_t n = r.order.size();
    if (n == 0) return;

    for (size_t i = 0; i < n; i++) {
        const Served& b = r.order[i];
        int count = 0;
        for (size_t j = 0; j < i; j++) {
            const Served& a = r.order[j];
            if (a.arrival > b.arrival || (a.arrival == b.arrival && a.id > b.id)) count++;
        }
        r.order[i].skipped = count;
    }

    double sumWait = 0;
    r.maxWait = 0;
    r.totalSkipped = 0;
    r.maxSkipped = 0;
    for (const auto& s : r.order) {
        sumWait += s.wait;
        r.maxWait = max(r.maxWait, s.wait);
        r.totalSkipped += s.skipped;
        r.maxSkipped = max(r.maxSkipped, s.skipped);
    }
    r.avgSeek = (double)r.totalSeek / n;
    r.avgWait = sumWait / n;

    double var = 0;
    for (const auto& s : r.order) var += (s.wait - r.avgWait) * (s.wait - r.avgWait);
    r.waitStdDev = sqrt(var / n);
}
