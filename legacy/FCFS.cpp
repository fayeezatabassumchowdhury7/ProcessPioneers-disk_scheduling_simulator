#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

void fcfs(vector<int> req, int n, int head)
{
    int current=head;
    int total=0;

    cout<<"\n===== FCFS ====="<<endl;
    cout<<"Head Movement: "<<head;

    for(int i:req)
    {
        int distance=abs(current-i);

        cout<<" -> "<<i<<" (Seek: "<<distance<<")";

        total=total+distance;
        current=i;
    }

    cout<<endl;

    cout<<"Total Seek Distance: "<<total<<endl;

    cout<<"Average Seek Distance: "
        <<(double)total/n<<endl;
}
