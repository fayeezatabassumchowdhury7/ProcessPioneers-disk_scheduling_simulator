#include "types.h"
#include <iomanip>
#include <sstream>
using namespace std;

static bool anyArrivals(const vector<Result>& all)
{
    for (const auto& r : all)
        for (const auto& s : r.order)
            if (s.arrival != 0) return true;
    return false;
}

// Every number and name in the text comes from the simulation results.
string bestReport(const vector<Result>& all)
{
    if (all.empty()) return "No results to report.\n";

    // Requests arrive over time -> fairness = longest wait.
    // All arrive at time 0 -> every wait is just the finish time, so fairness = max skipped.
    bool dyn = anyArrivals(all);
    auto fair = [dyn](const Result& r) { return dyn ? r.maxWait : r.maxSkipped; };
    string meaning = dyn ? "the longest time any request waited (max wait)"
                         : "the largest number of later requests served before one request (max skipped)";

    const Result* eff = &all[0];     // lowest total seek
    const Result* fa = &all[0];      // fairest (tie: lower seek)
    const Result* worst = &all[0];   // least fair
    for (const auto& r : all) {
        if (r.totalSeek < eff->totalSeek) eff = &r;
        if (fair(r) < fair(*fa) || (fair(r) == fair(*fa) && r.totalSeek < fa->totalSeek)) fa = &r;
        if (fair(r) > fair(*worst)) worst = &r;
    }

    ostringstream o;
    o << fixed << setprecision(1);
    o << "\n===== Best-Algorithm Report =====\n";
    o << "Fairness is measured by " << meaning << ".\n";
    o << "Lowest total seek: " << eff->name << " (" << eff->totalSeek << ").\n";
    o << "Fairest: " << fa->name << " (" << fair(*fa) << ").\n";

    if (fair(*eff) <= fair(*fa)) {
        o << "Verdict: " << eff->name
          << " is best for both head movement and fairness on this workload.\n";
    } else {
        double extra = eff->totalSeek > 0
                           ? 100.0 * (fa->totalSeek - eff->totalSeek) / eff->totalSeek : 0.0;
        o << "Verdict: " << eff->name << " moves the head least (" << eff->totalSeek
          << "), but its fairness value is " << fair(*eff) << " compared with " << fair(*fa)
          << " for " << fa->name << ". " << fa->name << " is fairer but needs " << extra
          << "% more head movement (" << fa->totalSeek << " vs " << eff->totalSeek << "). "
          << "Choose " << eff->name << " for throughput and " << fa->name
          << " when no request should be left waiting.\n";
    }
    if (fair(*worst) > 0 && worst != fa)
        o << "Worst case: " << worst->name << " has the highest fairness value ("
          << fair(*worst) << ").\n";

    // Starvation signal: worst case far above the average means most requests
    // are served quickly while a few are left waiting.
    const Result* gap = nullptr;
    double gapRatio = 0, gapWorst = 0, gapAvg = 0;
    for (const auto& r : all) {
        double worstCase = dyn ? r.maxWait : r.maxSkipped;
        double avg = dyn ? r.avgWait : (double)r.totalSkipped / r.order.size();
        if (avg <= 0 || worstCase <= 0) continue;
        double ratio = worstCase / avg;
        if (ratio > gapRatio) { gapRatio = ratio; gap = &r; gapWorst = worstCase; gapAvg = avg; }
    }
    if (gap && gapRatio >= 3.0)
        o << "Starvation signal: " << gap->name << " has " << (dyn ? "average wait " : "average skipped ")
          << gapAvg << " but its worst request has " << gapWorst << " (" << gapRatio
          << "x the average). Most requests are served quickly while one is left behind.\n";
    return o.str();
}
