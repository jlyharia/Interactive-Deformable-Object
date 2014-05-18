#include "../include/VectorOperation.h"
#include <math.h>
//#include <iostream>
//#include <vector>
//using namespace std;
vector<float> vec_cross(const vector<float>&vector_1, const vector<float>&vector_2)
{//compute cross product
    vector<float> output(3);
    output[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    output[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    output[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
    return output;
}

float vec_dot(const vector<float>&vector_1, const vector<float>&vector_2)
{//input 2 vector, output dot product
    float dotProduct = 0;
    for(int i = 0 ; i <(int)vector_1.size(); i++)
        dotProduct += vector_1[i]*vector_2[i];
    return dotProduct;
}
vector<float> vec_normalize(const vector<float> &input)
{//output normalized vector
    float length = vec_length(input);
    vector<float> output((int)input.size());
    for(int i = 0 ; i < (int)input.size() ; i++)
        output[i] = input[i]/length;

    return output;
}
float vec_length(const vector<float> &input)
{//compute vector length
    float output = 0;
    for(int i = 0 ; i < (int)input.size() ; i++)
        output+= pow(input[i],2);
    return pow(output,0.5);
}
vector<float> vec_subtraction(const vector<float>&vector_1, const vector<float>&vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    vector<float> output((int)vector_1.size());
    for(int i = 0 ; i < (int)output.size() ; i++)
        output[i] = vector_2[i] - vector_1[i];
    return output;
}

vector<float>vec_dim2to1(const vector< vector<float>> &input)
{
    vector<float> output;
    for(int i =0; i<(int)input.size(); i++)
    {
        output.insert(output.end(), input[i].cbegin(), input[i].cend());
    }
    return output;
}
