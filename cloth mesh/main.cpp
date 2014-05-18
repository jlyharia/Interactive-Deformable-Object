/**
    Tasks: Create vertex and polygon data
      / |  counter clockwise triangles
     /  |
    /___|
*/
#include <iostream>
#include <vector>
using namespace std;
struct vertexInfo{
    float x,y,z;
};
struct IndexInfo{
    float x,y,z;
};
vector<float> vec_Xproduct3D(vector<float>&vector_1, vector<float>&vector_2)
{//compute cross product
    vector<float> output(3);
    output[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    output[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    output[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
    return output;
}
vector<float> changeDimesion(vector< vector<vertexInfo>> input)
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

//    for(int i = 0; i< (int)output.size(); i+=3)
//    {
//        cout<<output[i]<<'\t'
//            <<output[i+1]<<'\t'
//            <<output[i+2]<<endl;
//    }
    return output;
}
vector<vertexInfo> changeDimesionNode(vector< vector<vertexInfo>> &input)
{
    vector<vertexInfo> output;
    for(int i = 0; i < (int)input.size(); i++)
    {
        for(int j = 0 ; j < (int)input[i].size(); j++)
        {
            output.push_back(input[i][j]);
        }
    }
    return output;

}
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
//    for(int i = 0; i< length; i++)
//    {
//        for(int j = 0; j< length; j++)
//        {
//            cout<<node[i][j].x<<'\t';
//            cout<<node[i][j].y<<'\t';
//            cout<<node[i][j].z<<endl;
//        }
//    }

    return node;
}

vector<int> getIndices(const int width)
{//create indices for mesh
    vector<int> indices;
    for(int i = 0; i< width -1; i++)
    {
        for(int j = 1; j< width; j++)
        {
            int up = i* width +j;
            int down = (i+1)* width +j-1;
            indices.push_back(up);
            indices.push_back(up -1);
            indices.push_back(down);
            indices.push_back(down);
            indices.push_back(down+1);
            indices.push_back(up);
        }
    }
    return indices;
}

vector<float> nodeSubtraction(vertexInfo &node1, vertexInfo &node2)
{   //compute vector subtraction
    //vector_2 - vector_1
    vector<float> output(3);
    output[0] = node2.x - node1.x;
    output[1] = node2.y - node1.y;
    output[2] = node2.z - node1.z;
    return output;
}
vector<float> getNormal(vector<int> indices, vector< vector<vertexInfo>> node)
{
    vector<float> normal;
    vector<vertexInfo> node_1d = changeDimesionNode(node);
    for(int i = 0; i< (int)indices.size(); i+=3)
    {
        vector<float> v1;//vector
        vector<float> v2;
        v1 = nodeSubtraction( node_1d[indices[i+1]], node_1d[indices[i]]);
        v2 = nodeSubtraction( node_1d[indices[i+1]], node_1d[indices[i+2]]);
        vector<float> xprod = vec_Xproduct3D(v2, v1);
        normal.insert(normal.begin(), xprod.begin(), xprod.end());
    }
    return normal;
}
void addIndex(vector<int> &temp,int a, int b, int c)
{
    temp.push_back(a);
    temp.push_back(b);
    temp.push_back(c);
}

vector< vector<int>> getNeighborPolygon(int width)
{//for compute surface normal
    vector< vector<int>> NeighborPoly;
    vector<int> temp;
    for(int i = 0; i< width; i++)
    {
        for(int j = 0; j< width; j++)
        {
            temp.clear();
            int now = i* width +j;
            int up = (i-1)* width +j;
            int down = (i+1)* width +j;
            if(i == 0)//top row
            {
                if(j ==0){//top-left corner
                    addIndex(temp, now, down, now +1);
                }
                else{
                    addIndex(temp, now, down, now +1);
                    addIndex(temp, now, down -1, down);
                }
            }
            else if(i == width -1)//bottom row
            {
                if(j == width -1)//bottom right corner
                    addIndex(temp, now, up, now - 1);
                else{
                    addIndex(temp, now, up, now - 1);
                    addIndex(temp, now, up + 1, up);
                }
            }
            else if(j == 0)//left column
            {
                addIndex(temp, now, up + 1, up);
                addIndex(temp, now, now + 1, up + 1);
                addIndex(temp, now, down, now + 1);
            }
            else if(j == width -1)//right column
            {
                addIndex(temp, now, up, now -1);
                addIndex(temp, now, now -1, down - 1);
                addIndex(temp, now, down - 1, down);
            }
            else//in the midst
            {
                addIndex(temp, now, up, now -1);//1
                addIndex(temp, now, up + 1, up);//2
                addIndex(temp, now, now + 1, up + 1);//3
                addIndex(temp, now, down, now + 1);//4
                addIndex(temp, now, down - 1, down);//5
                addIndex(temp, now, now - 1, down - 1);//6
            }
            NeighborPoly.push_back(temp);
        }
    }

//    for(int i = 0; i< (int)NeighborPoly.size(); i++)
//    {
//        cout<<i<<'\t';
//        for(int j = 0; j< (int)NeighborPoly[i].size(); j++)
//        {
//            cout<<NeighborPoly[i][j]<<"  ";
//        }
//        cout<<endl;
//    }
    return NeighborPoly;
}
vector< vector<int>> getNeighborVertex(int width)
{
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

//    for(int i = 0; i< (int)NeighborVert.size(); i++)
//    {
//        cout<<i<<'\t';
//        for(int j = 0; j< (int)NeighborVert[i].size(); j++)
//        {
//            cout<<NeighborVert[i][j]<<"  ";
//        }
//        cout<<endl;
//    }
}

int main()
{
    int width = 10;
    //2d node
    vector< vector<vertexInfo>> node = createNode(width);
    //indices
    vector<int> indices = getIndices(width);
    //vertex
    vector<float> vertex = changeDimesion(node);
    //normal
    vector<float> normal = getNormal(indices, node);
    //neighbor polygon index data -> for surface normal purpose
    vector< vector<int>> NeighborPoly = getNeighborPolygon(width);
    //neighbor vertex -> for spring connectivity
    vector< vector<int>> NeighborVert = getNeighborVertex(width);

    return 0;
}
