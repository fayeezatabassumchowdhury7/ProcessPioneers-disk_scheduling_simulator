#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include<bits/stdc++.h>
using namespace std;

void fcfs(vector<int> req, int n, int head);
void sstf(vector<int> req, int n, int head);
void scan(vector<int> req, int n, int head, int disksize, char direction);
void cscan(vector<int> req, int n, int head, int disksize, char direction);
void look(vector<int> req, int n, int head, char direction);
void clook(vector<int> req, int n, int head, char direction);

#endif
