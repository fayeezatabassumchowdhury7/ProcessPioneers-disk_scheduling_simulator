#include<bits/stdc++.h>
#include "algorithms.h"
using namespace std;

void sstf(vector<int> req, int n, int head)
{
    int current=head;
    int total=0;

    vector<bool>visited(n,false);

    cout<<"\n===== SSTF ====="<<endl;
    cout<<"Head Movement: "<<head;

    for(int i=0; i<n; i++)
    {
        int minDistance=INT_MAX;
        int index=-1;

        for(int j=0; j<n; j++)
        {
            if(visited[j]==false)
            {
                int distance=abs(current-req[j]);

                if(distance<minDistance)
                {
                    minDistance=distance;
                    index=j;
                }
            }
        }

        visited[index]=true;

        cout<<" -> "<<req[index]<<" (Seek: "<<minDistance<<")";

        total=total+minDistance;
        current=req[index];
    }

    cout<<endl;

    cout<<"Total Seek Distance: "<<total<<endl;

    cout<<"Average Seek Distance: "
        <<(double)total/n<<endl;
}
