#include "types.h"
#include <fstream>
using namespace std;

// Writes <base>_summary.csv (one row per algorithm) and
// <base>_requests.csv (one row per served request).
bool exportCsv(const string& base, const vector<Result>& all, string& err)
{
    ofstream sum(base + "_summary.csv");
    ofstream req(base + "_requests.csv");
    if (!sum || !req) { err = "Cannot write files starting with: " + base; return false; }

    sum << "algorithm,total_seek,avg_seek,avg_wait,max_wait,wait_stddev,"
           "total_skipped,max_skipped,end_time\n";
    req << "algorithm,request_id,cylinder,arrival,served_at,wait,seek,skipped\n";
    for (const auto& r : all) {
        sum << r.name << "," << r.totalSeek << "," << r.avgSeek << "," << r.avgWait << ","
            << r.maxWait << "," << r.waitStdDev << "," << r.totalSkipped << ","
            << r.maxSkipped << "," << r.endTime << "\n";
        for (const auto& s : r.order)
            req << r.name << "," << s.id << "," << s.cylinder << "," << s.arrival << ","
                << s.time << "," << s.wait << "," << s.seek << "," << s.skipped << "\n";
    }
    return true;
}
