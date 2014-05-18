#include <GL/glew.h>
#include <GL/glut.h>
#include "VBOfloor.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace std;
VBOfloor::VBOfloor(GLuint loc)
{//Constructor with shader location for VBO floor

    setVBOgrid();
    setUniform();
    glslHandle.setLocShader(loc);
    this->loc_shader = loc;
}
void VBOfloor::setVBOgrid()
{
    int width = 20;
    vector<float> vertex = getVertex(width);
    vector<GLuint> indices = getIndices(width);
    this->num_indices = indices.size();
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);//vbo

    glGenVertexArrays( 1, &vaoHandle );//vao
    glBindVertexArray(vaoHandle);//vao

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), &vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Vertex position
}

void VBOfloor::setUniform()
{//set MVP & Line color
    glm::mat4 m_trans = glm::translate(glm::mat4(1.0f),glm::vec3 (0.0f, -5.0f, -5.0f));
    //matrix view
    glm::mat4 m_view = glm::lookAt(glm::vec3 (0.0f, 0.0f, 0.0f),
                        glm::vec3 (0.0f, 0.0f, -1.0f),
                        glm::vec3 (0.0f, 1.0f, 0.0f));

    //matrix perspective
    glm::mat4 m_pers = glm::perspective(70.0f, 1.33f, 1.0f, 200.0f);
    this->MVP = m_pers * m_view* m_trans;
    this->LineColor = vec3(0.1f, 0.5f, 0.1f);
}
void VBOfloor::render()
{
    if(this->loc_shader != 0){
        glUseProgram(this->loc_shader);
    }else{
        cerr<<"Location of floor shader is not found"<<endl;
    }

    this->passUniform();
    glBindVertexArray(vaoHandle);
    glLineWidth(1);
    glDrawElements(GL_LINES, this->num_indices, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}
void VBOfloor::passUniform()
{
    glslHandle.sendUniform("MVP", this->MVP);
    glslHandle.sendUniform("LineColor", this->LineColor);
}
vector< vector<float>> VBOfloor::interpolation(float corner1[3], float corner2[3], int width)
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
vector<float> VBOfloor::changeDimesion(vector< vector<float>>input)
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
vector<float> VBOfloor::getVertex(int input)
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

vector<GLuint> VBOfloor::getIndices(int width)
{
    vector<GLuint> indices(width * 8);
    for(GLuint i = 0; i< (GLuint)width * 4; i++)
    {
        int it = i*2;
        indices[it] = i;
        indices[it +1] =  (width *4) + i;
    }
    return indices;
}
VBOfloor::~VBOfloor()
{
    //dtor
}
