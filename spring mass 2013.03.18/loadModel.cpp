/**
    Load Geometry file with .d extension
    This file is been modified in order to fulfill OpenGL 4.3 standard
    Update Feb.19.2013
**/
#include "loadModel.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>/* malloc exit*/
#include <math.h>
#include <stdio.h>/* free */
//#include "Trivial.h"
using namespace std;

loadModel::loadModel(){}
void loadModel::loadFile(string name)
{/*set geometric file name & control loading data*/
    fileName = new char[name.length() + 1];
    strcpy(fileName, name.c_str());

    LoadData();
}
void loadModel::LoadData()
{//load geometric data from file to program

    //initialize default parameter
    ifstream inGeometricData(fileName, ios::in);
    //load Geometric data *.d, constructor name - inGeometricData
    if(!inGeometricData)
    {
        cerr << "File could not be opened" <<endl;
        exit(1); //#include <cstdlib>
    }

    int num_vertex, num_polygon;
    // store 1)number of vertex and 2)number of polygon
    string first_word;
    //store first word - data

    string headline_string;
    char headline[256];
    inGeometricData.getline(headline, 256, '\n');
    inGeometricData.seekg(0L, ios::beg);//put cursor back to beginning
    headline_string = headline; //convert to string type
    if(!headline_string.find("data"))
    {
        inGeometricData >>first_word >> num_vertex >> num_polygon;
        //first line include word "data"
    }
    else
    {
        inGeometricData >> num_vertex >> num_polygon;
    }

    numOfPolygon = num_polygon;
    vector< vector<float> > vec_vertex(num_vertex, vector<float>(4));
    //declare 2 dimensional vec_vertex to store vertices data
    for(int i = 0; i< num_vertex; i++)
    {
        //put vertex data to vector - vec_vertex
        inGeometricData >> vec_vertex[i][0]
                        >> vec_vertex[i][1]
                        >> vec_vertex[i][2];
        vec_vertex[i][3] = 1;
    }

    vector< vector<int> > vec_polygon;
    //declare 2 dimensional vector to store polygon data
    //dynamic array to fit different number of vertex index

    for(int i = 0; i< num_polygon; i++)
    {
        //put element to vector vec_polygon
        int element,index;
        inGeometricData>>index; //get number of vertex index
        vector<int> row;
        row.push_back(index);//put in a new vector
        for (int j = 0; j < index  ; j++)
        {
            inGeometricData>>element;
            row.push_back(element); // Add an element to the row
        }
        vec_polygon.push_back(row);
        // Add the vector to the main vector (vec_polygon)
    }
    inGeometricData.close(); // close file
    //end of load vertices from file
    /*Compute_Poly_Normal *************************/
    vector< vector<float> > vec_normal(vec_vertex.size(),vector<float>(3));

    vector <float> poly_vector_1(3);
    vector <float> poly_vector_2(3);
    vector <float> poly_normal(3);
    for(int i=0; i<(int)vec_polygon.size(); i++)
    {//compute polygon normal
        poly_vector_1 = vec_subtraction(vec_vertex[vec_polygon[i][1]-1],
                           vec_vertex[vec_polygon[i][2]-1]);
        poly_vector_2 = vec_subtraction(vec_vertex[vec_polygon[i][1]-1],
                           vec_vertex[vec_polygon[i][3]-1]);

        //calculus cross product
        poly_normal = vec_Xproduct3D(poly_vector_1, poly_vector_2);

        for(int j=1; j<(int)vec_polygon[i][0]+1 ;j++)
        {//add polygon normal to vertex
            for (int p=0; p<3 ;p++ )
                vec_normal[vec_polygon[i][j]-1][p] += poly_normal[p];
        }
    }
    for(int i=0; i<(int)vec_normal.size() ;i++ )
    {//normalize the normal on vertex
        vec_normal[i] = vec_normalize(vec_normal[i]);
    }
    //vertex normals as average of surrounding neighbor polygon's normal

    /*compute vertex array and normal array in pointer*/
    float *vert = new float [numOfPolygon * 9];
    float *norm = new float [numOfPolygon * 9];
    int it = 0;
    for(int i = 0 ; i < numOfPolygon; i++)//(int)vec_polygon.size(); i++)
    {
        for(int j = 1 ; j < 4; j ++)
        {
            //vertex
            vert[it]     = vec_vertex[vec_polygon[i][j]-1][0];
            vert[it + 1] = vec_vertex[vec_polygon[i][j]-1][1];
            vert[it + 2] = vec_vertex[vec_polygon[i][j]-1][2];
            //normal
            norm[it]     = vec_normal[vec_polygon[i][j]-1][0];
            norm[it + 1] = vec_normal[vec_polygon[i][j]-1][1];
            norm[it + 2] = vec_normal[vec_polygon[i][j]-1][2];
            it += 3;
        }
    }
    vertex = vert;
    normal = norm;
}
int loadModel::getPolyNum()
{//return number of Polygon
    return numOfPolygon;
}
float *loadModel::getVerPtr()
{/*return a pointer point to polygonal vertex data*/
    return vertex;
}
float *loadModel::getNormPtr()
{/*return a pointer point to polygonal vertex normal data*/
    return normal;
}
vector<float> loadModel::vec_Xproduct3D(vector<float>vector_1, vector<float>vector_2)
{//compute cross product
    vector<float> output(3);
    output[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    output[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    output[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
    return output;
}
vector<float> loadModel::vec_normalize(vector<float> input)
{//output normalized vector
    float length = vec_length(input);
    int range = (int)input.size();
    vector<float> output(range);
    for(int i = 0 ; i < range ; i++)
        output[i] = input[i]/length;

    return output;
}
float loadModel::vec_length(vector<float> input)
{//compute vector length
    float output = 0;
    for(int i = 0 ; i < (int)input.size() ; i++)
        output+= pow(input[i],2);
    return pow(output,0.5);
}
vector<float> loadModel::vec_subtraction(vector<float> vector_1, vector<float> vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    vector<float> output((int)vector_1.size());
    for(int i = 0 ; i < (int)vector_1.size() ; i++)
        output[i] = vector_2[i] - vector_1[i];
    return output;
}
loadModel::~loadModel()
{
    //deconstructor
}
