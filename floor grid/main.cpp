/**
    Tasks: Create vertex and polygon data
*/
#include <iostream>
#include <vector>
using namespace std;
vector< vector<float>> interpolation(float corner1[3], float corner2[3], int width)
{
    int step_x = (corner2[0] - corner1[0])/(width*2);
    int step_z = (corner2[2] - corner1[2])/(width*2);
    int length = width*2;
    vector< vector<float>> edge(length , vector<float> (3));

    for(int i = 0; i< (int)edge.size(); i++)
    {
        edge[i][0] = corner1[0] + i*step_x;
        edge[i][1] = 0;
        edge[i][2] = corner1[2] + i*step_z;

    }
    return edge;
}
vector<float> changeDimesion(vector< vector<float>>input)
{
    int length = input.size()*input[0].size();
    vector<float> output(length);
    for(int i = 0; i< (int)input.size(); i++)
    {
        int it = i * 3;
        output[it] = input[i][0];
        output[it + 1] = input[i][1];
        output[it + 2] = input[i][2];
    }
    return output;
}
vector<float> getVertex(int input)
{
    float width = (float)input;
    float corner_1 [3] = {-width, 0, -width};
    float corner_2 [3] = {-width, 0, width};
    float corner_3 [3] = {width, 0, width};
    float corner_4 [3] = {width, 0, -width};

    vector<float> edge1 = changeDimesion(interpolation(corner_1, corner_2, width));
    vector<float> edge2 = changeDimesion(interpolation(corner_2, corner_3, width));
    vector<float> edge3 = changeDimesion(interpolation(corner_4, corner_3, width));
    vector<float> edge4 = changeDimesion(interpolation(corner_1, corner_4, width));
    vector<float> vertex = edge1;
    vertex.insert(vertex.end(), edge2.begin(), edge2.end());
    vertex.insert(vertex.end(), edge3.begin(), edge3.end());
    vertex.insert(vertex.end(), edge4.begin(), edge4.end());
    return vertex;
}

vector<int> getIndices(int width)
{
    vector<int> indices(width * 8);
    for(int i = 0; i< width * 4; i++)
    {
        int it = i*2;
        indices[it] = i;
        indices[it +1] =  (width *4) + i;
    }
    return indices;
}
int main()
{
    int width = 10;
    vector<float> vertex = getVertex(width);
    vector<int> indices = getIndices(width);
    int it = 0;
    for(int i = 0; i< (int)vertex.size(); i+=3)
    {
        cout<<it<<'\t';
        it++;
        cout<<vertex[i]<<'\t';
        cout<<vertex[i+1]<<'\t';
        cout<<vertex[i+2]<<endl;
    }
    return 0;
}
