#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

int main()
{
    cout<<"===== Disk Scheduling Simulator ====="<<endl;

    int n, disksize, head;
    char direction;

    cout<<"Enter n (number of requests): ";
    cin>>n;

    cout<<"Enter disksize (number of tracks): ";
    cin>>disksize;

    cout<<"Enter head: ";
    cin>>head;

    if(n<=0 || disksize<=0 || head<0 || head>=disksize)
    {
        cout<<"Invalid input!"<<endl;
        return 0;
    }

    vector<int>req(n);

    cout<<"Enter request: "<<endl;

    for(int i=0; i<n; i++)
    {
        cin>>req[i];

        if(req[i]<0 || req[i]>=disksize)
        {
            cout<<"Invalid request!"<<endl;
            return 0;
        }
    }

    cout<<"Enter direction (R/L): ";
    cin>>direction;

    if(direction!='R' && direction!='r' &&
       direction!='L' && direction!='l')
    {
        cout<<"Invalid direction!"<<endl;
        return 0;
    }

    cout<<endl;
    cout<<"===== All Algorithm Results ====="<<endl;

    fcfs(req,n,head);
    sstf(req,n,head);
    scan(req,n,head,disksize,direction);
    cscan(req,n,head,disksize,direction);
    look(req,n,head,direction);
    clook(req,n,head,direction);

    return 0;
}
