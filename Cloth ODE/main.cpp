#include <iostream>
#include <vector>
#include "TrivialOutput.h"
using namespace std;
class vertexInfo{
    public:
        float x,y,z;
};
class MotionInfo{
    //use for ODE
    public:
        vector<float> x_now{0, 0, 0};
        vector<float> x_next{0, 0, 0};
        vector<float> v_now{0, 0, 0};
        vector<float> v_next{0, 0, 0};
        vector<float> a{0, 0, 0};
        vector<float> force_now{0, 0, 0};
        vector<float> force_next{0, 0, 0};
};
const int width = 5;
vector<MotionInfo> VertexODEInfo(width*width);
//neighbor vertex -> for spring connectivity
vector< vector<int>> NeighborVert;
vector<float> vertices;
int index_pick = 0;
///---------------------------------------------------
vector< vector<vertexInfo>> createNode(const int width)
{//create 10 by 10 vertex
    int length = width;//10
    vector< vector<vertexInfo>> node(length, vector<vertexInfo>(length));
    for(int i = 0; i< length; i++)
    {
        for(int j = 0; j< length; j++)
        {
            node[i][j].x = j;
            node[i][j].y = 0;
            node[i][j].z = i;
        }
    }
    return node;
}
vector<float> changeDimesion(const vector< vector<vertexInfo>> input)
{//change from 2d to 1d
    vector<float> output;
    for(int i = 0; i< (int)input.size(); i++)
    {
        for(int j = 0; j < (int)input[i].size(); j++)
        {
            output.push_back(input[i][j].x);
            output.push_back(input[i][j].y);
            output.push_back(input[i][j].z);
        }
    }
    return output;
}
void Preprocessing()
{
    ::NeighborVert = getNeighborVertex(width);
    vector< vector<vertexInfo>> node = createNode(width);
    ::vertices = changeDimesion(node);
}

void VBOcloth::ODE()
{

    if(this->MSFStatus)//exist mouse external force status
    {/*work once at each pick*/
        //calculate destination point
        vector<float> VertexNew = InversePerspective();
        //new vertex in in local space
        int index = ::index_pick * 3;
        vector<float> VertexOld{vertices[index],vertices[index+1],vertices[index+2]};
        //original vertex in in local space

        vector<float> Fe = vec_subtraction(VertexOld, VertexNew);//external force come from mouse
        VertexODEInfo[this->index_pick].force_next = Fe;
        //put force in picked vertex
        //no damping and hooke's force


        cout<<"VertexNew: "; vout(VertexNew);
        cout<<"VertexOld: "; vout(VertexOld);
        cout<<"Fe: "; vout(Fe);
        this->MSFStatus = false;
    }
    else//! this->MSFStatus ~~ doesn't exist mouse external force
    {
        updateTimeStep();
        updateForcePropagation();
        updateMotion();
    }

}
int main()
{
    Preprocessing();
    ODE();
    return 0;
}
