#ifndef VBOCLOTH_H
#define VBOCLOTH_H
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "GLSLProgram.h"
#include <vector>
using namespace std;
using glm::mat4;
using glm::vec3;
using glm::vec4;
class vertexInfo{
    public:
        float x,y,z;
};
class MotionInfo{
    //use for ODE
    public:
        vector<float> x_rest{0, 0, 0};
        vector<float> x_now{0, 0, 0};
        vector<float> x_next{0, 0, 0};
        vector<float> v_now{0, 0, 0};
        vector<float> v_next{0, 0, 0};
        vector<float> a{0, 0, 0};
        vector<float> force_now{0, 0, 0};
        vector<float> force_next{0, 0, 0};
};
class VBOcloth
{
    public:
        VBOcloth(GLuint);
        virtual ~VBOcloth();
        void render();
        /*mouse interface*/
        void setMouse_1(int, int);
        void setMouse_2(int, int);
        void setMouseRight();

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
        mat4 MVP, iMVP;
        mat4 m_screen;

        /*light*/
        vec4 LightPosition;
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        vec3 Iads;
        float Shininess;

        /*generate cloth geometry*/
        vector<GLuint> getIndices(const int);
        vector<float> getNormal(int);
        vector<float> nodeSubtraction(vertexInfo &, vertexInfo &);
        vector< vector<vertexInfo>> createNode(const int );
        vector<float> changeDimesion(const vector< vector<vertexInfo>>);
        vector<vertexInfo> changeDimesionNode(vector< vector<vertexInfo>> &);
        vector< vector<int>> changeDimesionIndex(vector<GLuint> &indices);
        vector< vector<float>> changeDimesionVertex(const vector<float> &);
        vector< vector<int>> getNeighborVertex(int);
        vector< vector<int>> getNeighborPolygonIndex(int);
        void addVertexIndex(vector<int> &,int, int, int);

        /*Geometry data*/
        int num_indices;
        int num_normal;
        vector<GLuint> indices;
        vector< vector<int>> indices2D;
        vector< vector<int>> NeighborPolyIndex;
        vector< vector<int>> NeighborVert;
        vector< vector<vertexInfo>> node_screen;
        vector<float> vertices;
        vector< vector<float>> vertices2D;
        vector<float> normal;

        /*ODEs*/
        const float hooke_k = 0.8f; //spring constant
        const float damp_d = -0.4f; //damping constant
        float del_t = 0.08f; //time step;
        bool MSFStatus = true;//mouse external force status
        vector<MotionInfo> VertexODEInfo;
        void ODE();
        void updateMotion();
        vector<float> Force_damping(int );
        vector<float> Force_hooke(int );
        void updateForcePropagation();
        void updateTimeStep();
        vector<float> getMotionData();
        void iniODEPosition();

        /*Mouse information*/
        int Mouse1_x, Mouse1_y;
        int Mouse2_x, Mouse2_y;
        bool mouse_pick = false;

        /*modify buffer Data*/
        void CheckBufferVert();
        void CheckBufferNorm();
        void ModifyBufferData();
        vector<float> InversePerspective();
        void updateNewVertex();
        void updateNewNormal();

        /*Vertex Matching*/
        int index_pick;
        vector< vector<vertexInfo>> ScreenNode(const vector< vector<vertexInfo>> &);
        float Length_MsVt(float, float, float, float);
        int QuickPartition(vector< vector<float>> &, int, int, int);
        void QuickSort(vector< vector<float>> &, int, int, int);
        int MatchVertex(const vector< vector<vertexInfo>> &);

};

#endif // VBOCLOTH_H
