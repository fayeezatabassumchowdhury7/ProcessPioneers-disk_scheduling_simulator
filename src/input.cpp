#include "types.h"
#include <fstream>
#include <sstream>
using namespace std;

string validateWorkload(const Workload& w)
{
    const Config& c = w.cfg;
    if (c.disk <= 0 || c.disk > 1000000) return "Disk size must be between 1 and 1000000.";
    if (c.head < 0 || c.head >= c.disk) return "Head position must be between 0 and disk size - 1.";
    if (c.dir != 1 && c.dir != -1) return "Direction must be R or L.";
    if (w.reqs.empty()) return "The request queue is empty.";
    if (w.reqs.size() > 100000) return "Too many requests (maximum is 100000).";
    for (const auto& r : w.reqs) {
        if (r.cylinder < 0 || r.cylinder >= c.disk)
            return "Request " + to_string(r.id) + ": cylinder " + to_string(r.cylinder) +
                   " is outside the disk range 0.." + to_string(c.disk - 1) + ".";
        if (r.arrival < 0)
            return "Request " + to_string(r.id) + ": arrival time cannot be negative.";
    }
    return "";
}

// File format ('#' starts a comment line, blank lines are ignored):
//   disk head direction(R/L) countJump(1/0)
//   cylinder arrival          (one line per request; arrival is optional, default 0)
bool loadWorkload(const string& path, Workload& w, string& err)
{
    ifstream in(path);
    if (!in) { err = "Cannot open file: " + path; return false; }

    Workload tmp;
    tmp.cfg = {0, 0, 1, true};
    string line;
    int lineNo = 0;
    bool haveHeader = false;

    while (getline(in, line)) {
        lineNo++;
        size_t first = line.find_first_not_of(" \t\r");
        if (first == string::npos || line[first] == '#') continue;

        istringstream ss(line);
        string extra;
        if (!haveHeader) {
            char d = 0;
            int jump = 0;
            if (!(ss >> tmp.cfg.disk >> tmp.cfg.head >> d >> jump) || (ss >> extra)) {
                err = "Line " + to_string(lineNo) + ": header must be: disk head R|L 1|0";
                return false;
            }
            if (d == 'R' || d == 'r') tmp.cfg.dir = 1;
            else if (d == 'L' || d == 'l') tmp.cfg.dir = -1;
            else { err = "Line " + to_string(lineNo) + ": direction must be R or L"; return false; }
            tmp.cfg.countJump = (jump != 0);
            haveHeader = true;
        } else {
            int cyl = 0, arr = 0;
            if (!(ss >> cyl)) {
                err = "Line " + to_string(lineNo) + ": expected 'cylinder [arrival]'";
                return false;
            }
            if (!(ss >> arr)) {
                if (!ss.eof()) { err = "Line " + to_string(lineNo) + ": arrival must be a number"; return false; }
                arr = 0;
            } else if (ss >> extra) {
                err = "Line " + to_string(lineNo) + ": too many values";
                return false;
            }
            tmp.reqs.push_back({(int)tmp.reqs.size() + 1, cyl, arr});
        }
    }
    if (!haveHeader) { err = "File is empty (no header line)."; return false; }

    err = validateWorkload(tmp);
    if (!err.empty()) return false;
    w = tmp;
    return true;
}

bool saveWorkload(const string& path, const Workload& w, string& err)
{
    ofstream out(path);
    if (!out) { err = "Cannot write file: " + path; return false; }
    out << "# disk head direction countJump\n";
    out << w.cfg.disk << " " << w.cfg.head << " " << (w.cfg.dir > 0 ? 'R' : 'L') << " "
        << (w.cfg.countJump ? 1 : 0) << "\n";
    out << "# cylinder arrival\n";
    for (const auto& r : w.reqs) out << r.cylinder << " " << r.arrival << "\n";
    return true;
}
