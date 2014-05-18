#include <GL/glew.h>
#include <GL/glut.h>
#include "VBOcloth.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
VBOcloth::VBOcloth(GLuint loc)
{
    setVBO();
    setUniform();
    glslHandle.setLocShader(loc);
    this->loc_shader = loc;
}
void VBOcloth::setVBO()
{
    int width = 10;
    vector< vector<vertexInfo>> node = createNode(width);
    //indices
    this->indices = getIndices(width);
    //vertex
    vector<float> vertices = changeDimesion(node);
    //normal
    vector<float> normal = getNormal(indices, node);
    //neighbor polygon index data -> for surface normal purpose
    this->NeighborPoly = getNeighborPolygon(width);
    //neighbor vertex -> for spring connectivity
    this->NeighborVert = getNeighborVertex(width);

    this->num_indices = indices.size();
    this->num_normal = normal.size();
    this->num_vertices = vertices.size();

    /*Bind VAO*/
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glGenBuffers(3, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);  // Vertex Data

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &normal[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(1);  // Normal Data

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

}
void VBOcloth::setUniform()
{
    this->setLight();
    this->setMtrices();
}
void VBOcloth::setLight()
{
    //light position
    this->LightPosition = glm::vec4(3.0f, 4.0f, 0.0f, 1.0f);

    //light intensity
    this->Iads = glm::vec3(1.0f, 1.0f, 1.0f);

    //material reflectivity
    this->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    this->Kd = glm::vec3(0.4f, 0.3f, 0.0f);
    this->Ks = glm::vec3(0.6f, 0.6f, 0.0f);

    this->Shininess = 50;
}
void VBOcloth::setMtrices()
{
    //matrix model
    //translate
    glm::mat4 m_trans = glm::translate(glm::mat4(1.0f),glm::vec3 (-5.0f, -4.0f, -16.0f));
    //scale
//    glm::mat4 scal = glm::scale(glm::mat4(1.0f),glm::vec3(2.0f, 1.0f, 1.0f));
    //rotation
//    glm::mat4 rotat = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3 (-1.0f, 0.0f, 0.0f));
    //matrix view
    glm::mat4 m_view = glm::lookAt(glm::vec3 (0.0f, 0.0f, 0.0f),
                                    glm::vec3 (0.0f, 0.0f, -1.0f),
                                    glm::vec3 (0.0f, 1.0f, 0.0f));
    //ModelView Matrix
    this->ModelViewMatrix =  m_view * m_trans;// * rotat;// * scal;
    //matrix perspective
    glm::mat4 m_pers = glm::perspective(70.0f, 1.33f, 1.0f, 200.0f);
    this->MVP = m_pers * ModelViewMatrix ;//*scal;
    //normal matrix
    glm::mat3 norm= glm::mat3(glm::vec3(ModelViewMatrix[0]),
                              glm::vec3(ModelViewMatrix[1]),
                              glm::vec3(ModelViewMatrix[2]));


    this->NormalMatrix = glm::inverse(norm);
}
void VBOcloth::passUniform()
{
    glslHandle.sendUniform("MVP", this->MVP);
    glslHandle.sendUniform("ModelViewMatrix", this->ModelViewMatrix);
    glslHandle.sendUniform("NormalMatrix", this->NormalMatrix);

    //struct Light
    glslHandle.sendUniform("Iads", this->Iads);
    glslHandle.sendUniform("LightPosition", ModelViewMatrix* LightPosition);
    //struct Material
    glslHandle.sendUniform("Ka", this->Ka);
    glslHandle.sendUniform("Kd", this->Kd);
    glslHandle.sendUniform("Ks", this->Ks);
    glslHandle.sendUniform("Shininess", this->Shininess);
}
void VBOcloth::render()
{
    if(this->loc_shader >= 0){
        glUseProgram(this->loc_shader);
    }else{
        cerr<<"Location of shader used by VBO cloth is not found. loc shader: "
            <<this->loc_shader<<endl;
    }

//    glPolygonMode(GL_FRONT, GL_LINE);
//    glLineWidth(3);
    this->passUniform();
    glBindVertexArray(vaoHandle);

    glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));

//    ModifyBufferData();

}
void VBOcloth::ModifyBufferData()
{
//    cout<<indices[10]<<endl;

//    int numOfData = this->num_indices;
//    float * data = new GLfloat[numOfData];
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
//    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
//                       numOfData * sizeof(float), data);
//    for(int i = 0; i<numOfData; i+=3)
//    {
//        cout<<data[i]<<'\t'
//        <<data[i+1]<<'\t'
//        <<data[i+2]<<endl;
//
//    }
//    cout<<endl;
//    delete [] data;

//    cout<<glutGet(GLUT_ELAPSED_TIME) / 1000.0f<<endl;
}
///************mouse
void VBOcloth::setMouse_1(int x, int y)
{//beginning point
    this->Mouse1_x = x;
    this->Mouse1_y = y;
    cout<<Mouse1_x<<'\t'<<Mouse1_y<<endl;
}
///******************generate cloth |
vector<float> VBOcloth::changeDimesion(const vector< vector<vertexInfo>> input)
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
    return output;
}

vector<GLuint> VBOcloth::getIndices(const int width)
{//create indices for mesh
    vector<GLuint> indices;
    for(int i = 0; i< width -1; i++)
    {
        for(int j = 1; j< width; j++)
        {
            GLuint up = i* width +j;
            GLuint down = (i+1)* width +j-1;
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
vector<float> VBOcloth::vec_Xproduct3D(vector<float>&vector_1, vector<float>&vector_2)
{//compute cross product
    vector<float> output(3);
    output[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    output[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    output[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
    return output;
}
vector<vertexInfo> VBOcloth::changeDimesionNode(vector< vector<vertexInfo>> &input)
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
vector< vector<vertexInfo>> VBOcloth::createNode(const int width)
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
    return node;
}
vector<float> VBOcloth::nodeSubtraction(vertexInfo &node1, vertexInfo &node2)
{   //compute vector subtraction
    //vector_2 - vector_1
    vector<float> output(3);
    output[0] = node2.x - node1.x;
    output[1] = node2.y - node1.y;
    output[2] = node2.z - node1.z;
    return output;
}
vector<float> VBOcloth::getNormal(vector<GLuint> &indices, vector< vector<vertexInfo>> &node)
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
void VBOcloth::addIndex(vector<int> &temp,int a, int b, int c)
{
    temp.push_back(a);
    temp.push_back(b);
    temp.push_back(c);
}

vector< vector<int>> VBOcloth::getNeighborPolygon(int width)
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

    return NeighborPoly;
}
vector< vector<int>> VBOcloth::getNeighborVertex(int width)
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
}
VBOcloth::~VBOcloth()
{
    //dtor
}
