// Automatic tests. Expected values were calculated by hand (see tests/TEST_REPORT.md).
#include "types.h"
#include <iostream>
using namespace std;

static int failed = 0, total = 0;

static void check(const string& name, long got, long expected)
{
    total++;
    bool ok = (got == expected);
    if (!ok) failed++;
    cout << (ok ? "PASS  " : "FAIL  ") << name << "   got " << got << ", expected " << expected << "\n";
}

static vector<Request> book()                       // textbook queue, all arrive at time 0
{
    vector<int> c = {98, 183, 37, 122, 14, 124, 65, 67};
    vector<Request> r;
    for (size_t i = 0; i < c.size(); i++) r.push_back({(int)i + 1, c[i], 0});
    return r;
}

int main()
{
    Config L{200, 53, -1, true}, R{200, 53, +1, true};
    Config Rno{200, 53, +1, false};

    // T1-T6: textbook example, head 53, disk 0..199
    check("T1  FCFS",                    simulate(Algo::FCFS,  book(), L).totalSeek, 640);
    check("T2  SSTF",                    simulate(Algo::SSTF,  book(), L).totalSeek, 236);
    check("T3  SCAN toward 0",           simulate(Algo::SCAN,  book(), L).totalSeek, 236);
    check("T4  LOOK toward 0",           simulate(Algo::LOOK,  book(), L).totalSeek, 208);
    check("T5  C-SCAN up, jump counted", simulate(Algo::CSCAN, book(), R).totalSeek, 382);
    check("T6  C-LOOK up, jump counted", simulate(Algo::CLOOK, book(), R).totalSeek, 322);

    // T7-T8: same, return jump not counted
    check("T7  C-SCAN jump not counted", simulate(Algo::CSCAN, book(), Rno).totalSeek, 183);
    check("T8  C-LOOK jump not counted", simulate(Algo::CLOOK, book(), Rno).totalSeek, 153);

    // T9-T10: sweeping upward first
    check("T9  SCAN toward 199",         simulate(Algo::SCAN,  book(), R).totalSeek, 331);
    check("T10 LOOK toward 199",         simulate(Algo::LOOK,  book(), R).totalSeek, 299);

    // T11: single request exactly at the head -> no movement, no waiting
    {
        Result r = simulate(Algo::SSTF, {{1, 53, 0}}, L);
        check("T11 request at head: seek", r.totalSeek, 0);
        check("T11 request at head: wait", r.maxWait, 0);
    }
    // T12: request at the head + one more, direction L: head request is served first at cost 0
    {
        Result r = simulate(Algo::SCAN, {{1, 20, 0}, {2, 53, 0}}, L);
        check("T12 head request served first", r.order[0].cylinder, 53);
        check("T12 total seek",                 r.totalSeek, 33);
    }
    // T13: idle gap: 60 at t=0, 20 at t=100 (head 53, direction R)
    {
        vector<Request> q = {{1, 60, 0}, {2, 20, 100}};
        check("T13 FCFS idle gap seek",  simulate(Algo::FCFS, q, R).totalSeek, 47);
        check("T13 SCAN idle gap seek",  simulate(Algo::SCAN, q, R).totalSeek, 325);
        check("T13 C-SCAN idle gap seek",simulate(Algo::CSCAN,q, R).totalSeek, 365);
        check("T13 LOOK finish time",    simulate(Algo::LOOK, q, R).endTime, 140);
    }
    // T14: arrival during a seek must wait (SSTF is already moving to 60)
    {
        Config c{200, 50, +1, true};
        vector<Request> q = {{1, 60, 0}, {2, 52, 5}};
        Result r = simulate(Algo::SSTF, q, c);
        check("T14 SSTF finishes started seek first", r.order[0].cylinder, 60);
        check("T14 SSTF total seek",                  r.totalSeek, 18);
    }
    // T15: request that appears in front of a moving LOOK head is served on the way
    {
        Config c{200, 50, +1, true};
        vector<Request> q = {{1, 60, 0}, {2, 55, 5}};
        Result r = simulate(Algo::LOOK, q, c);
        check("T15 LOOK serves 55 on the way", r.order[0].cylinder, 55);
        check("T15 LOOK total seek",           r.totalSeek, 10);
        check("T15 LOOK wait of request 2",    r.order[0].wait, 0);
    }
    // T16: duplicate cylinders are both served, second one costs 0
    {
        Result r = simulate(Algo::FCFS, {{1, 70, 0}, {2, 70, 0}}, L);
        check("T16 duplicates total seek", r.totalSeek, 17);
        check("T16 duplicates 2nd seek",   r.order[1].seek, 0);
    }
    // T17: skipped counts. FCFS never skips; SSTF on the textbook queue does
    {
        check("T17 FCFS max skipped", simulate(Algo::FCFS, book(), L).maxSkipped, 0);
        check("T17 SSTF max skipped > 0", simulate(Algo::SSTF, book(), L).maxSkipped > 0, 1);
    }
    // T18: SSTF starvation demo: far request waits far longer than under SCAN
    {
        Config c{200, 100, +1, true};
        vector<Request> q = generateWorkload("starvation", 100, 200, 100, 1, 0);
        Result sstf = simulate(Algo::SSTF, q, c);
        Result scan = simulate(Algo::SCAN, q, c);
        check("T18 SSTF max wait > SCAN max wait", sstf.maxWait > scan.maxWait, 1);
        check("T18 far request is served last by SSTF", sstf.order.back().cylinder, 199);
        check("T18 far request skipped by all 99 later ones", sstf.order.back().skipped, 99);
    }
    // T19: same seed gives the same generated workload
    {
        vector<Request> a = generateWorkload("uniform", 10, 200, 50, 7, 30);
        vector<Request> b = generateWorkload("uniform", 10, 200, 50, 7, 30);
        bool same = true;
        for (size_t i = 0; i < a.size(); i++)
            if (a[i].cylinder != b[i].cylinder || a[i].arrival != b[i].arrival) same = false;
        check("T19 generator is repeatable", same, 1);
    }
    // T20: input validation
    {
        Workload w{{200, 53, 1, true}, {{1, 250, 0}}};
        check("T20 cylinder out of range rejected", !validateWorkload(w).empty(), 1);
        w.reqs = {{1, 10, 0}}; w.cfg.head = 200;
        check("T20 head out of range rejected", !validateWorkload(w).empty(), 1);
        w.cfg.head = 5; w.reqs.clear();
        check("T20 empty queue rejected", !validateWorkload(w).empty(), 1);
        w.reqs = {{1, 10, -3}};
        check("T20 negative arrival rejected", !validateWorkload(w).empty(), 1);
    }
    // T21: the original queue is not changed by a run
    {
        vector<Request> q = book();
        simulate(Algo::SSTF, q, L);
        check("T21 input queue unchanged", q[0].cylinder, 98);
    }

    cout << "\n" << (total - failed) << " / " << total << " checks passed\n";
    return failed == 0 ? 0 : 1;
}
