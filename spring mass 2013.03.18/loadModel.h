/**
    Load Geometry file with .d extension
    This file is been modified in order to fulfill OpenGL 4.3 standard
    Update Feb.19.2013
**/
#ifndef LOADMODEL_H
#define LOADMODEL_H
#include <string>
#include <vector>
using namespace std;
class loadModel
{
    public:
        loadModel();
        //constructor and set up default parameter

        void loadFile(string);
        //input file name and load file

        int getPolyNum();
        float *getNormPtr();
        float *getVerPtr();
        virtual ~loadModel();
    private:
        int numOfPolygon;

        float *vertex = nullptr;
        float *normal = nullptr;
        char *fileName;
        void LoadData();
        //load geometric data from file to program

        vector<float> vec_Xproduct3D(vector<float>, vector<float>);
        vector<float> vec_normalize(vector<float> );
        vector<float> vec_subtraction(vector<float> , vector<float> );
        float vec_length(vector<float> );
};

#endif // LOADMODEL_H
