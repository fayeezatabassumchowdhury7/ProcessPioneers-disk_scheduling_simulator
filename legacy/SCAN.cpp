#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

void scan(vector<int> req, int n, int head, int disksize, char direction)
{
    vector<int>left,right;

    for(int i:req)
    {
        if(i<head)
            left.push_back(i);
        else
            right.push_back(i);
    }

    sort(left.begin(),left.end(),greater<int>());
    sort(right.begin(),right.end());

    int current=head;
    int total=0;

    cout<<"\n===== SCAN ====="<<endl;
    cout<<"Head Movement: "<<head;

    if(direction=='R' || direction=='r')
    {
        for(int i:right)
        {
            int distance=abs(current-i);

            cout<<" -> "<<i<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=i;
        }

        int distance=abs(current-(disksize-1));

        if(distance>0)
        {
            cout<<" -> "<<disksize-1<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=disksize-1;
        }

        for(int i:left)
        {
            int distance=abs(current-i);

            cout<<" -> "<<i<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=i;
        }
    }
    else
    {
        for(int i:left)
        {
            int distance=abs(current-i);

            cout<<" -> "<<i<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=i;
        }

        int distance=abs(current-0);

        if(distance>0)
        {
            cout<<" -> 0 (Seek: "<<distance<<")";

            total=total+distance;
            current=0;
        }

        for(int i:right)
        {
            int distance=abs(current-i);

            cout<<" -> "<<i<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=i;
        }
    }

    cout<<endl;

    cout<<"Total Seek Distance: "<<total<<endl;

    cout<<"Average Seek Distance: "
        <<(double)total/n<<endl;
}
