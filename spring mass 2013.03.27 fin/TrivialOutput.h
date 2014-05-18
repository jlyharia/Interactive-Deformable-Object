#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>
using namespace std;
using glm::vec3;
using glm::vec4;


template <typename T>
void vout (const vector< vector<T>> &vec)
{
//    int it=0;
    for(int i =0; i< (int)vec.size(); i++)
    {
//        cout<<it<<'\t';
        for(int j =0; j<(int)vec[i].size(); j++)
            cout<<vec[i][j]<<'\t';
        cout<<endl;
//        it++;
    }
}

template <typename T>
void vout (const vector<T> &vec)
{
    for(int i =0; i< (int)vec.size(); i++)
        cout<<vec[i]<<'\t';
    cout<<endl;
}



void vout (const vector<GLuint> &vec);

void vout (const vec3 &vec);
void vout (const vec4 &vec);
