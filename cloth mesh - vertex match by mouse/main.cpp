/**
    Tasks: Match mouse location to vertex
      / |  counter clockwise triangles
     /  |
    /___|
*/
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using glm::mat4;
using glm::vec4;
mat4 MVP;
mat4 m_screen;
vector< vector<int>> MainIndices;
struct vertexInfo{
    float x,y,z;
};
void vout(vector< vector<int>> in)
{
    for(int i=0; i<(int)in.size(); i++)
    {
        for(int j = 0; j <(int)in[i].size(); j++)
            cout<<in[i][j]<<'\t';
        cout<<endl;
    }
}
void vout(vector<int> in)
{
    for(int i=0; i<(int)in.size(); i++)
        cout<<in[i]<<'\t';
    cout<<endl;
}
void vout(vector< vector<float>> in)
{
    int it = 0;
    for(int i=0; i<(int)in.size(); i++)
    {
        cout<<it<<'\t';
        it++;
        for(int j = 0; j <(int)in[i].size(); j++)
            cout<<in[i][j]<<'\t';
        cout<<endl;

    }
}
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
//            cout<< i*length + j<<'\t';
//            cout<<node[i][j].x<<'\t';
//            cout<<node[i][j].y<<'\t';
//            cout<<node[i][j].z<<endl;
//        }
//    }

    return node;
}
vector< vector<int>> changeDimesionIndex(vector<int> indices)
{
    vector< vector<int>> indices2D;
    vector<int> temp;
    for(int i = 0; i< (int)indices.size(); i+=3)
    {
        temp.clear();
        temp.push_back(indices[i]);
        temp.push_back(indices[i+1]);
        temp.push_back(indices[i+2]);
        indices2D.push_back(temp);
    }
//    for(int i =0; i< (int)indices2D.size(); i++)
//    {
//        for(int j = 0 ; j < (int)indices2D[i].size(); j++)
//            cout<<indices2D[i][j]<<'\t';
//        cout<<endl;
//    }
    return indices2D;
}
vector<int> getIndices(const int width)
{//create indices for mesh
    vector<int> indices;
    for(int i = 0; i< width -1; i++)
    {
        for(int j = 1; j< width; j++)
        {
            int now = i* width +j;
            int down = (i+1)* width +j-1;
            indices.push_back(now);
            indices.push_back(now -1);
            indices.push_back(down);

            indices.push_back(down);
            indices.push_back(down+1);
            indices.push_back(now);
        }
    }
//    for(int i = 0, it =0; i< (int)indices.size(), it< width*width; i+=3, it ++)
//    {
//        cout<<it<<'\t';
//
//        cout<<indices[i]<<'\t';
//        cout<<indices[i+1]<<'\t';
//        cout<<indices[i+2]<<'\t';
//        cout<<(int)indices.size() - i;
//        cout<<endl;
//    }
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
float vec_length(const vector<float> &input)
{//compute vector length
    float output = 0;
    for(int i = 0 ; i < (int)input.size() ; i++)
        output+= pow(input[i],2);
    return pow(output,0.5);
}
vector<float> vec_normalize(const vector<float> &input)
{//output normalized vector
    float length = vec_length(input);
    vector<float> output((int)input.size());
    for(int i = 0 ; i < (int)input.size() ; i++)
        output[i] = input[i]/length;

    return output;
}
vector<float> getNormal(int width)
{
    width = width * width;
    float nor[] = {0, 1, 0};
    vector<float> normal;
    for(int i=0; i< width; i++)
    {
        normal.insert(normal.end(),nor, nor+3);
    }
    int it=0;
    for(int i=0; i< (int)normal.size(); i+=3)
    {
        cout<<it<<'\t';
        it++;
        cout<<normal[i]<<'\t';
        cout<<normal[i+1]<<'\t';
        cout<<normal[i+2]<<endl;
    }
    return normal;
}

void addVertexIndex(vector<int> &VertIndex,int a, int b, int c)
{
    vector< vector<int>> temp_1;
    for(int i = 0; i<(int)MainIndices.size(); i++)
    {
        for(int j = 0; j<(int)MainIndices[i].size(); j++)
        {
            if(a == MainIndices[i][j])
            {
                vector<int> temp = MainIndices[i];
                temp.push_back(i);//add index number
                temp_1.push_back(temp);
                break;
            }
        }
    }
    vector< vector<int>> temp_2;
    for(int i = 0; i<(int)temp_1.size(); i++)
    {
        for(int j = 0; j<(int)temp_1[i].size() -1; j++)
        {
            if(b == temp_1[i][j])
            {
                temp_2.push_back(temp_1[i]);
                break;
            }
        }
    }
    vector< vector<int>> temp_3;
    for(int i = 0; i<(int)temp_2.size(); i++)
    {
        for(int j = 0; j<(int)temp_2[i].size() -1; j++)
        {
            if(c == temp_2[i][j])
            {
                temp_3.push_back(temp_2[i]);
                break;
            }
        }
    }
    VertIndex.push_back(temp_3[0][3]);//polygon index of following vertex
    VertIndex.push_back(temp_3[0][0]);
    VertIndex.push_back(temp_3[0][1]);
    VertIndex.push_back(temp_3[0][2]);

}

vector< vector<int>> getNeighborPolygonIndex(int width)
{//for compute surface normal

    vector< vector<int>> NeighborPolyIndex;
    //0) polygon index 123)vertex index
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
                    addVertexIndex(temp, now, down, now +1);
                }
                else{
                    addVertexIndex(temp, now, now -1, down -1);
                    addVertexIndex(temp, now, down -1, down);
                    if(j != width -1)
                        addVertexIndex(temp, now, down, now +1);
                }
            }
            else if(i == width -1)//bottom row
            {
                if(j == width -1){//bottom right corner
                    addVertexIndex(temp, now, up, now - 1);
                }
                else{
                    addVertexIndex(temp, now, up + 1, up);
                    addVertexIndex(temp, now, now + 1, up + 1);
                    if(j!= 0)
                        addVertexIndex(temp, now, up, now - 1);
                }
            }
            else if(j == 0)//left column
            {
                addVertexIndex(temp, now, up + 1, up);
                addVertexIndex(temp, now, now + 1, up + 1);
                addVertexIndex(temp, now, down, now + 1);
            }
            else if(j == width -1)//right column
            {
                addVertexIndex(temp, now, up, now -1);
                addVertexIndex(temp, now, now -1, down - 1);
                addVertexIndex(temp, now, down - 1, down);
            }
            else//in the midst
            {
                addVertexIndex(temp, now, up, now -1);//1
                addVertexIndex(temp, now, up + 1, up);//2
                addVertexIndex(temp, now, now + 1, up + 1);//3
                addVertexIndex(temp, now, down, now + 1);//4
                addVertexIndex(temp, now, down - 1, down);//5
                addVertexIndex(temp, now, now - 1, down - 1);//6
            }
            NeighborPolyIndex.push_back(temp);
        }
    }

    for(int i = 0; i< (int)NeighborPolyIndex.size(); i++)
    {
        cout<<i<<'\t';
        for(int j = 0; j< (int)NeighborPolyIndex[i].size(); j++)
        {
            cout<<NeighborPolyIndex[i][j]<<"  ";
        }
        cout<<endl;
    }

    return NeighborPolyIndex;
}
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


//    for(int i = 0; i< (int)NeighborVert.size(); i++)
//    {
//        cout<<i<<'\t';
//        for(int j = 0; j< (int)NeighborVert[i].size(); j++)
//        {
//            cout<<NeighborVert[i][j]<<"  ";
//        }
//        cout<<endl;
//    }
    return NeighborVert;
}

void setMatrix()
{
    //translate
    glm::mat4 m_trans = glm::translate(glm::mat4(1.0f),glm::vec3 (-5.0f, -4.0f, -16.0f));
    //matrix view
    glm::mat4 m_view = glm::lookAt(glm::vec3 (0.0f, 0.0f, 0.0f),
                                    glm::vec3 (0.0f, 0.0f, -1.0f),
                                    glm::vec3 (0.0f, 1.0f, 0.0f));
    //matrix perspective
    glm::mat4 m_pers = glm::perspective(70.0f, 1.33f, 1.0f, 200.0f);
    //ModelView Matrix
    glm::mat4 ModelViewMatrix =  m_view * m_trans;

    ::MVP = m_pers * ModelViewMatrix ;//*scal;
    ::m_screen = glm::mat4(400, 0, 0, 0,
                             0,  300, 0, 0,
                             0, 0, 1, 0,
                             400, 300, 0, 1);
//    for(int i = 0; i< 4; i++)
//    {
//        for(int j = 0 ; j < 4; j ++)
//        {
//            MVP[i][j] = glMVP[j][i];
//            m_sre[i][j] = m_screen[j][i];
//        }
//    }

}
vector< vector<vertexInfo>> ScreenNode(const vector< vector<vertexInfo>> &node)
{//compute node in screen space

    setMatrix();
    vector< vector<vertexInfo>> node_screen = node;
    for(int i = 0; i < (int)node.size(); i++)
    {
        for(int j = 0 ; j <(int)node[i].size(); j++)
        {
            vec4 v_pers = ::MVP * vec4(node[i][j].x, node[i][j].y, node[i][j].z, 1.0);
            vec4 v_screen = ::m_screen * vec4(v_pers/v_pers[3]);

            node_screen[i][j].x = v_screen.x;
            node_screen[i][j].y = v_screen.y;
            node_screen[i][j].z = v_screen.z;
        }
    }
//    for(int i = 0; i< (int)node_screen.size(); i++)
//    {
//
//        for(int j = 0; j< (int)node_screen[i].size(); j++)
//        {
//            cout<<i*(int)node_screen.size() + j<<'\t';
//            cout<<node_screen[i][j].x<<'\t'<<'\t';
//            cout<<node_screen[i][j].y<<'\t'<<'\t';
//            cout<<node_screen[i][j].z<<endl;
//        }
//    }
    return node_screen;
}
float Length_MsVt(float p1x, float p1y, float p2x, float p2y)
{//output two point pseudo length (without radical sign)
    float a = p1x - p2x;
    float b = p1y - p2y;
    return a*a + b*b;
}

int QuickPartition(vector< vector<float>> &A, int p, int q, int index)
{
    int x = A[p][index];//pivot //1 -> distance
    int i = p;
    for(int j = p+1; j < q; j++)
    {
        if(x >= A[j][index])
        {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[p], A[i]);
    return i;
}
void QuickSort(vector< vector<float>> &A, int p, int q, int index)
{
    if(p < q)
    {
        int r = QuickPartition(A, p, q, index);

        QuickSort(A, p, r, index);
        QuickSort(A, r+1, q, index);
    }

}
int MatchVertex(const vector< vector<vertexInfo>> &node_s)
{//match vertex by mouse
    int winHeight = 600;
//    float mx = 650, my = 547; //mouse position in scream
    float mx = 350, my = 546; //mouse position in scream
//    float mx = 91, my = 542; //mouse position in scream
//    float mx = 263, my = 405; //mouse position in scream
    my = winHeight - my;
    int threshold = 1000;
    int width = (int)node_s.size();

    vector< vector<float>> nearVertex;//1. index 2. distance 3. z value
    vector<float> temp;//0. index 1. distance 2. z value
    for(int i = 0; i < width; i++)
    {
        for(int j = 0 ; j < width; j++)
        {
            float dist = Length_MsVt(mx, my, node_s[i][j].x, node_s[i][j].y);
            if(threshold > dist)
            {//Vertex in reasonable distance between mouse
                int index = i * width + j;
                temp.clear();
                temp.push_back(index);
                temp.push_back(dist);
                temp.push_back(node_s[i][j].z);
                nearVertex.push_back(temp);
            }

        }
    }
    if(nearVertex.size() > 1)
    {//QuickSort nearVertex by distance "if" there exists more than one vertices
        QuickSort(nearVertex, 0, nearVertex.size(), 1);
        if(nearVertex[0][1] == nearVertex[1][1])
        {//if there are vertices show equally smallest distance
            //collect the data  and sort by z value
            int limitIndex = 2;
            for(int i = 2; i< (int)nearVertex.size(); i++)
            {
                if(nearVertex[0][1] == nearVertex[i][1]);
                    limitIndex++;
            }
            //sort by z value
            QuickSort(nearVertex, 0, limitIndex, 2);
        }
    }



//    for(int i = 0; i< (int)nearVertex.size(); i++)
//    {
//        cout<<i<<'\t';
//        for(int j = 0; j< (int)nearVertex[i].size(); j++)
//        {
//            cout<<nearVertex[i][j]<<'\t';
//        }
//        cout<<endl;
//    }
    return (int)nearVertex[0][0];
}
int main()
{
    int width = 10;
    //2d node
    vector< vector<vertexInfo>> node = createNode(width);
    //indices
    vector<int>indices = getIndices(width);
    ::MainIndices = changeDimesionIndex(indices);
    //vertex
    vector<float> vertex = changeDimesion(node);
    //normal
    vector<float> normal = getNormal(width);
    //neighbor polygon index data -> for surface normal purpose
//    vector< vector<int>> NeighborPolyIndex = getNeighborPolygonIndex(width);
//    //neighbor vertex -> for spring connectivity
//    vector< vector<int>> NeighborVert = getNeighborVertex(width);
//    vector<int> temp;
    /*Vertex matching process*/
    //1. transfer vertex to screen space
//    vector< vector<vertexInfo>> node_screen = ScreenNode(node);

    //2. use mouse position to match the nearest vertex's index
//    int index = MatchVertex(node_screen);
//    cout<<index<<endl;

    return 0;
}
