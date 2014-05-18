#include "TrivialOutput.h"

void vout (const vec3 &vec)
{
    for(int i =0; i< 3; i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}
void vout (const vec4 &vec)
{
    for(int i =0; i< 4; i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}


///-------------------------------

vector< vector<int>> getNeighborVertex(int width)
{//compute spring force
    vector< vector<int>> NeighborVert;
    vector<int> temp;
    for(int i = 0; i< width; i++)
    {
        for(int j = 0; j< width; j++)
        {
            temp.clear();
            int now = i* width +j;
            int up = (i-1)* width +j;
            int down = (i+1)* width +j;

            if(i > 0 && j > 0)
                temp.push_back(up -1);//1
            if(j > 0)
                temp.push_back(now -1);//2
            if(i < width -1 && j > 0)
                temp.push_back(down -1);//3

            if(i < width -1)
                temp.push_back(down);//4

            if(i < width -1 && j < width -1)
                temp.push_back(down + 1);//5
            if(j < width -1)
                temp.push_back(now + 1);//6
            if(i > 0 && j < width -1)
                temp.push_back(up + 1);//7
            if(i > 0)
                temp.push_back(up);//8

            NeighborVert.push_back(temp);
        }
    }
    return NeighborVert;
}
