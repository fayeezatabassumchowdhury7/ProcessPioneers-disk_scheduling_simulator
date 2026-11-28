#include "types.h"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

void printResult(const Result& r)
{
    cout << "\n===== " << r.name << " =====\n";
    cout << left << setw(6) << "Req" << setw(8) << "Cyl" << setw(10) << "Arrival"
         << setw(10) << "Served@" << setw(8) << "Wait" << setw(8) << "Seek" << "Skipped\n";
    for (const auto& s : r.order)
        cout << left << setw(6) << s.id << setw(8) << s.cylinder << setw(10) << s.arrival
             << setw(10) << s.time << setw(8) << s.wait << setw(8) << s.seek << s.skipped << "\n";

    cout << "Service order: " << r.head;
    for (const auto& s : r.order) cout << " -> " << s.cylinder << " (" << s.seek << ")";
    cout << "\n";
    cout << fixed << setprecision(2)
         << "Total seek: " << r.totalSeek << "   Avg seek: " << r.avgSeek << "\n"
         << "Avg wait: " << r.avgWait << "   Max wait: " << r.maxWait
         << "   Wait std-dev: " << r.waitStdDev << "\n"
         << "Skipped total: " << r.totalSkipped << "   Max skipped: " << r.maxSkipped << "\n"
         << "Finished at time: " << r.endTime << "\n";
}

// Text chart: one row per point of the head path; '*' position = cylinder.
void printChart(const Result& r)
{
    const int W = 50;
    int span = max(1, r.disk - 1);
    cout << "\nHead path - " << r.name << "  (cylinder 0 on the left, " << r.disk - 1
         << " on the right)\n";
    for (size_t i = 0; i < r.path.size(); i++) {
        int col = (int)((long long)r.path[i] * W / span);
        cout << setw(6) << r.path[i] << " |" << string(col, ' ') << "*";
        if (i > 0) {
            int d = abs(r.path[i] - r.path[i - 1]);
            cout << "  (" << (r.path[i] > r.path[i - 1] ? "+" : "-") << d << ")";
        }
        cout << "\n";
    }
}

static string cell(double v, double best, int prec)
{
    ostringstream o;
    o << fixed << setprecision(prec) << v << (v == best ? "*" : " ");
    return o.str();
}

template <typename F>
static double minOf(const vector<Result>& all, F f)
{
    double m = f(all[0]);
    for (const auto& r : all) m = min(m, (double)f(r));
    return m;
}

void printComparison(const vector<Result>& all)
{
    if (all.empty()) return;
    double bSeek = minOf(all, [](const Result& r) { return (double)r.totalSeek; });
    double bAvgS = minOf(all, [](const Result& r) { return r.avgSeek; });
    double bAvgW = minOf(all, [](const Result& r) { return r.avgWait; });
    double bMaxW = minOf(all, [](const Result& r) { return (double)r.maxWait; });
    double bSd = minOf(all, [](const Result& r) { return r.waitStdDev; });
    double bMaxSk = minOf(all, [](const Result& r) { return (double)r.maxSkipped; });
    double bTotSk = minOf(all, [](const Result& r) { return (double)r.totalSkipped; });

    cout << "\n===== Comparison (* = best in column) =====\n";
    cout << left << setw(9) << "Algo" << right << setw(11) << "TotalSeek" << setw(11) << "AvgSeek"
         << setw(11) << "AvgWait" << setw(11) << "MaxWait" << setw(11) << "WaitSD"
         << setw(11) << "MaxSkip" << setw(11) << "TotSkip" << "\n";
    for (const auto& r : all)
        cout << left << setw(9) << r.name << right
             << setw(11) << cell(r.totalSeek, bSeek, 0)
             << setw(11) << cell(r.avgSeek, bAvgS, 2)
             << setw(11) << cell(r.avgWait, bAvgW, 2)
             << setw(11) << cell(r.maxWait, bMaxW, 0)
             << setw(11) << cell(r.waitStdDev, bSd, 2)
             << setw(11) << cell(r.maxSkipped, bMaxSk, 0)
             << setw(11) << cell(r.totalSkipped, bTotSk, 0) << "\n";
}
