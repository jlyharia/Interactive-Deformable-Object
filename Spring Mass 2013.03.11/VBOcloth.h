#ifndef VBOCLOTH_H
#define VBOCLOTH_H
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLSLProgram.h"
#include <vector>
using namespace std;
using glm::mat4;
using glm::vec3;
struct vertexInfo{
    float x,y,z;
};
class VBOcloth
{
    public:
        VBOcloth(GLuint);
        void render();
        void setMouse_1(int, int);
        virtual ~VBOcloth();
    private:
        /*GLSL set up*/
        void setUniform();
        void passUniform();
        void setVBO();
        void setMtrices();
        void setLight();


        GLuint vboHandles[3];
        GLSLProgram glslHandle;
        GLuint vaoHandle;
        GLuint loc_shader;
        /*uniform variable*/

        mat4 ModelViewMatrix;
        mat3 NormalMatrix;
        mat4 MVP;
        /*light*/
        vec4 LightPosition;
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        vec3 Iads;
        float Shininess;
        /*generate cloth geometry*/
        vector<float> changeDimesion(const vector< vector<vertexInfo>>);
        vector<GLuint> getIndices(const int);
        vector<float> getNormal(vector<GLuint> &, vector< vector<vertexInfo>> &);
        vector<float> nodeSubtraction(vertexInfo &, vertexInfo &);
        vector< vector<vertexInfo>> createNode(const int );
        vector<vertexInfo> changeDimesionNode(vector< vector<vertexInfo>> &);
        vector<float> vec_Xproduct3D(vector<float>&, vector<float>&);

        vector< vector<int>> getNeighborVertex(int);
        vector< vector<int>> getNeighborPolygon(int);
        void addIndex(vector<int> &,int, int, int);
        /*Geometry data*/
        int num_indices;
        int num_normal;
        int num_vertices;
        vector<GLuint> indices;
        vector< vector<int>> NeighborPoly;
        vector< vector<int>> NeighborVert;
        /*Mouse information*/
        int Mouse1_x, Mouse1_y;

        /*modify buffer Data*/
        void ModifyBufferData();

};

#endif // VBOCLOTH_H
