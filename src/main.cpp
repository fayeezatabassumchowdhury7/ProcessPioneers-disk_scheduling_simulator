#include "types.h"
#include <cstdlib>
#include <iostream>
using namespace std;

// ---------- small input helpers (read whole lines, never crash on bad input) ----------

static string ask(const string& prompt)
{
    cout << prompt;
    string s;
    if (!getline(cin, s)) { cout << "\n"; exit(0); }       // end of input: leave quietly
    size_t a = s.find_first_not_of(" \t\r");
    size_t b = s.find_last_not_of(" \t\r");
    return a == string::npos ? "" : s.substr(a, b - a + 1);
}

static bool toInt(const string& s, int& v)
{
    if (s.empty()) return false;
    char* end = nullptr;
    long x = strtol(s.c_str(), &end, 10);
    if (*end != '\0' || x < -1000000000L || x > 1000000000L) return false;
    v = (int)x;
    return true;
}

// Empty answer keeps the default value def.
static int askInt(const string& prompt, int lo, int hi, int def)
{
    while (true) {
        string s = ask(prompt + " [" + to_string(def) + "]: ");
        if (s.empty()) return def;
        int v;
        if (toInt(s, v) && v >= lo && v <= hi) return v;
        cout << "  Please enter a whole number from " << lo << " to " << hi << ".\n";
    }
}

static int askDir(int def)
{
    while (true) {
        string s = ask(string("Head direction R/L [") + (def > 0 ? "R" : "L") + "]: ");
        if (s.empty()) return def;
        if (s == "R" || s == "r") return 1;
        if (s == "L" || s == "l") return -1;
        cout << "  Please type R or L.\n";
    }
}

static void askConfig(Config& c)
{
    c.disk = askInt("Disk size (cylinders 0..N-1)", 1, 1000000, c.disk);
    if (c.head >= c.disk) c.head = 0;
    c.head = askInt("Initial head position", 0, c.disk - 1, c.head);
    c.dir = askDir(c.dir);
}

// ---------- menu actions ----------

static void showWorkload(const Workload& w, bool loaded)
{
    if (!loaded) { cout << "\nNo workload yet. Use option 1, 2 or 3 first.\n"; return; }
    cout << "\nDisk: 0.." << w.cfg.disk - 1 << "   Head: " << w.cfg.head
         << "   Direction: " << (w.cfg.dir > 0 ? "R" : "L")
         << "   Return jump counted: " << (w.cfg.countJump ? "yes" : "no") << "\n";
    cout << "Requests (" << w.reqs.size() << "):  id:cylinder@arrival\n";
    size_t shown = 0;
    for (const auto& r : w.reqs) {
        cout << "  " << r.id << ":" << r.cylinder << "@" << r.arrival;
        if (++shown % 8 == 0) cout << "\n";
        if (shown == 200) { cout << "\n  ... (" << w.reqs.size() - 200 << " more)"; break; }
    }
    cout << "\n";
}

static void manualEntry(Workload& w, bool& loaded)
{
    Workload t = w;
    if (!loaded) t.cfg = {200, 53, -1, true};
    askConfig(t.cfg);
    int n = askInt("Number of requests", 1, 100000, 8);
    t.reqs.clear();
    cout << "Enter each request as: cylinder [arrival]   (arrival defaults to 0)\n";
    for (int i = 1; i <= n; i++) {
        while (true) {
            string s = ask("  Request " + to_string(i) + ": ");
            size_t sp = s.find_first_of(" \t");
            string a = s.substr(0, sp);
            string b = (sp == string::npos) ? "" : s.substr(s.find_first_not_of(" \t", sp));
            int cyl, arr = 0;
            if (!toInt(a, cyl) || (!b.empty() && !toInt(b, arr))) {
                cout << "  Enter numbers only, like: 98 0\n";
                continue;
            }
            if (cyl < 0 || cyl >= t.cfg.disk) {
                cout << "  Cylinder must be between 0 and " << t.cfg.disk - 1 << ".\n";
                continue;
            }
            if (arr < 0) { cout << "  Arrival time cannot be negative.\n"; continue; }
            t.reqs.push_back({i, cyl, arr});
            break;
        }
    }
    string err = validateWorkload(t);
    if (!err.empty()) { cout << "Invalid data: " << err << "\n"; return; }
    w = t;
    loaded = true;
    cout << "Workload saved.\n";
}

static void loadFromFile(Workload& w, bool& loaded)
{
    string path = ask("File path (for example examples/textbook.txt): ");
    Workload t;
    string err;
    if (!loadWorkload(path, t, err)) { cout << "Error: " << err << "\n"; return; }
    w = t;
    loaded = true;
    cout << "Loaded " << w.reqs.size() << " requests.\n";
}

static void generate(Workload& w, bool& loaded)
{
    Workload t = w;
    if (!loaded) t.cfg = {200, 53, -1, true};
    cout << "\n1) uniform   2) clustered   3) sequential   4) starvation demo (for SSTF)\n";
    int k = askInt("Workload type", 1, 4, 1);
    static const char* kinds[] = {"", "uniform", "clustered", "sequential", "starvation"};
    askConfig(t.cfg);
    int n = askInt("Number of requests", 2, 100000, k == 4 ? 100 : 20);
    int seed = askInt("Random seed (same seed = same workload)", 0, 1000000000, 1);
    int spread = askInt("Arrival times spread over 0..", 0, 1000000, 50);
    t.reqs = generateWorkload(kinds[k], n, t.cfg.disk, t.cfg.head, (unsigned)seed, spread);
    string err = validateWorkload(t);
    if (!err.empty()) { cout << "Invalid data: " << err << "\n"; return; }
    w = t;
    loaded = true;
    cout << "Generated " << kinds[k] << " workload with " << n << " requests.\n";
}

static vector<Result> runAll(const Workload& w)
{
    vector<Result> all;
    for (Algo a : allAlgos()) all.push_back(simulate(a, w.reqs, w.cfg));
    return all;
}

int main()
{
    Workload w;
    w.cfg = {200, 53, -1, true};
    bool loaded = false;

    cout << "===== Disk Scheduling Simulator (Process Pioneers) =====\n";
    while (true) {
        cout << "\n 1) Load workload from file\n"
             << " 2) Enter workload manually\n"
             << " 3) Generate random workload\n"
             << " 4) Show current workload\n"
             << " 5) Run one algorithm (table + head path chart)\n"
             << " 6) Compare all algorithms\n"
             << " 7) Best-algorithm report\n"
             << " 8) Export results to CSV\n"
             << " 9) Toggle C-SCAN/C-LOOK return jump counting (now: "
             << (w.cfg.countJump ? "counted" : "not counted") << ")\n"
             << "10) Save current workload to file\n"
             << " 0) Exit\n";
        int choice;
        if (!toInt(ask("Choice: "), choice)) { cout << "Please enter a number from the menu.\n"; continue; }

        if (choice == 0) break;
        if (choice == 1) { loadFromFile(w, loaded); continue; }
        if (choice == 2) { manualEntry(w, loaded); continue; }
        if (choice == 3) { generate(w, loaded); continue; }
        if (choice == 4) { showWorkload(w, loaded); continue; }
        if (choice == 9) {
            w.cfg.countJump = !w.cfg.countJump;
            cout << "Return jump is now " << (w.cfg.countJump ? "counted" : "not counted") << ".\n";
            continue;
        }
        if (choice < 1 || choice > 10) { cout << "Invalid choice.\n"; continue; }
        if (!loaded) { cout << "No workload yet. Use option 1, 2 or 3 first.\n"; continue; }

        if (choice == 5) {
            cout << "\n";
            for (size_t i = 0; i < allAlgos().size(); i++)
                cout << " " << i + 1 << ") " << algoName(allAlgos()[i]) << "\n";
            int k = askInt("Algorithm", 1, (int)allAlgos().size(), 1);
            Result r = simulate(allAlgos()[k - 1], w.reqs, w.cfg);
            printResult(r);
            printChart(r);
        } else if (choice == 6) {
            printComparison(runAll(w));
        } else if (choice == 7) {
            vector<Result> all = runAll(w);
            printComparison(all);
            cout << bestReport(all);
        } else if (choice == 8) {
            string base = ask("File name prefix [results]: ");
            if (base.empty()) base = "results";
            string err;
            if (exportCsv(base, runAll(w), err))
                cout << "Wrote " << base << "_summary.csv and " << base << "_requests.csv\n";
            else
                cout << "Error: " << err << "\n";
        } else if (choice == 10) {
            string path = ask("Save as (for example examples/my_workload.txt): ");
            string err;
            if (saveWorkload(path, w, err)) cout << "Saved " << path << "\n";
            else cout << "Error: " << err << "\n";
        }
    }
    cout << "Goodbye.\n";
    return 0;
}
