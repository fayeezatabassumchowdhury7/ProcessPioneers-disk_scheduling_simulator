#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

void look(vector<int> req, int n, int head, char direction)
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

    cout<<"\n===== LOOK ====="<<endl;
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
