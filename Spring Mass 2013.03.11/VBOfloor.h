#ifndef VBOFLOOR_H
#define VBOFLOOR_H
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLSLProgram.h"
#include <vector>
using namespace std;
using glm::mat4;
using glm::vec3;
class VBOfloor
{
    public:
        VBOfloor(GLuint);
        void render();
        bool isCreate();
        virtual ~VBOfloor();
    private:
        /*generate floor grid*/
        int num_indices;
        vector<GLuint> getIndices(int );
        vector<float> getVertex(int );
        vector<float> changeDimesion(vector< vector<float>>);
        vector< vector<float>> interpolation(float [3], float [3], int );

        /*GLSL set up*/
        void passUniform();
        void setUniform();
        void setVBOgrid();
        GLSLProgram glslHandle;
        GLuint vaoHandle;
        GLuint loc_shader;
        bool life;
        /*uniform variable*/
        mat4 MVP;
        vec3 LineColor;
};

#endif // VBOFLOOR_H
