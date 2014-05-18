#include "TrivialOutput.h"

void vout (const vector<GLuint> &vec)
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
