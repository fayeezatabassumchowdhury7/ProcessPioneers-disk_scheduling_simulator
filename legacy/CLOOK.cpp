#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

void clook(vector<int> req, int n, int head, char direction)
{
    vector<int>left,right;

    for(int i:req)
    {
        if(i<head)
            left.push_back(i);
        else
            right.push_back(i);
    }

    sort(left.begin(),left.end());
    sort(right.begin(),right.end());

    int current=head;
    int total=0;

    cout<<"\n===== C-LOOK ====="<<endl;
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

        if(!left.empty())
        {
            int distance=abs(current-left[0]);

            cout<<" -> "<<left[0]<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=left[0];
        }

        for(int i=1; i<(int)left.size(); i++)
        {
            int distance=abs(current-left[i]);

            cout<<" -> "<<left[i]<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=left[i];
        }
    }
    else
    {
        for(int i=(int)left.size()-1; i>=0; i--)
        {
            int distance=abs(current-left[i]);

            cout<<" -> "<<left[i]<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=left[i];
        }

        if(!right.empty())
        {
            int distance=abs(current-right.back());

            cout<<" -> "<<right.back()<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=right.back();
        }

        for(int i=(int)right.size()-2; i>=0; i--)
        {
            int distance=abs(current-right[i]);

            cout<<" -> "<<right[i]<<" (Seek: "<<distance<<")";

            total=total+distance;
            current=right[i];
        }
    }

    cout<<endl;

    cout<<"Total Seek Distance: "<<total<<endl;

    cout<<"Average Seek Distance: "
        <<(double)total/n<<endl;
}

