#include "TrivialOutput.h"

void vout (const vector< vector<float>> &vec)
{
    int it=0;
    for(int i =0; i< (int)vec.size(); i++)
    {
        cout<<it<<'\t';
        for(int j =0; j<(int)vec[i].size(); j++)
            cout<<vec[i][j]<<'\t';
        cout<<endl;
        it++;
    }
}
void vout (const vector< vector<int>> &vec)
{
    for(int i =0; i< (int)vec.size(); i++)
    {
        for(int j =0; j<(int)vec[i].size(); j++)
            cout<<vec[i][j]<<'\t';
        cout<<endl;
    }
}
void vout (const vector<int> &vec)
{
    for(int i =0; i< (int)vec.size(); i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}
void vout (const vector<GLuint> &vec)
{
    for(int i =0; i< (int)vec.size(); i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}
void vout (const vector<float> &vec)
{
    for(int i =0; i< (int)vec.size(); i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}
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
