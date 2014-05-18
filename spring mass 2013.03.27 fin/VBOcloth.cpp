#include <GL/glew.h>
#include <GL/glut.h>
#include "VBOcloth.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include "TrivialOutput.h"
#include "include/VectorOperation.h"
#include <fstream>

fstream fout ("test.txt", ios::out);
VBOcloth::VBOcloth(GLuint loc)
{
    setUniform();
    setVBO();
    glslHandle.setLocShader(loc);
    this->loc_shader = loc;
}
void VBOcloth::setVBO()
{
    int width = 10;

    VertexODEInfo = vector<MotionInfo>(width*width);
    //use for ODE, store position, velocity, acceleration, force ..on vertex

    vector< vector<vertexInfo>> node = createNode(width);
    this->node_screen = ScreenNode(node);//transfer vertex to screen space
    //indices
    this->indices = getIndices(width);
    //vertices
    this->vertices = changeDimesion(node);
    iniODEPosition();
    //normal
    this->normal = getNormal(width);

    //neighbor polygon index data -> for surface normal purpose
    this->indices2D = changeDimesionIndex(this->indices);
    this->NeighborPolyIndex = getNeighborPolygonIndex(width);
    //neighbor vertex -> for spring connectivity
    this->NeighborVert = getNeighborVertex(width);

    this->num_indices = indices.size();
    this->num_normal = normal.size();

    /*Bind VAO*/
    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray(vaoHandle);

    glGenBuffers(3, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    //The GL_ARRAY_BUFFER​ binding is NOT part of the VAO's state!
    //I know that's confusing, but that's the way it is.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);  // Vertex Data

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), &normal[0], GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(1);  // Normal Data

    //index data
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
    this->LightPosition = glm::vec4(3.0f, 4.0f, 1.0f, 1.0f);

    //light intensity
    this->Iads = glm::vec3(1.0f, 1.0f, 1.0f);

    //material reflectivity
    this->Ka = glm::vec3(0.1f, 0.1f, 0.1f);
    this->Kd = glm::vec3(0.6f, 0.6f, 0.2f);
    this->Ks = glm::vec3(0.3f, 0.3f, 0.3f);

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
    this->ModelViewMatrix =  m_view * m_trans;
    //matrix perspective
    glm::mat4 m_pers = glm::perspective(70.0f, 1.33f, 1.0f, 200.0f);
    this->MVP = m_pers * ModelViewMatrix ;
    //inverse MVP
    this->iMVP = glm::inverse(this->MVP);
    //normal matrix
    glm::mat3 norm= glm::mat3(glm::vec3(ModelViewMatrix[0]),
                              glm::vec3(ModelViewMatrix[1]),
                              glm::vec3(ModelViewMatrix[2]));


    this->NormalMatrix = glm::inverse(norm);

    this->m_screen = mat4(400, 0, 0, 0,
                     0,  300, 0, 0,
                     0, 0, 1, 0,
                     400, 300, 0, 1);
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
//    glLineWidth(2);
    this->passUniform();
//    glBindVertexArray(vaoHandle);

    glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));

    if(this->mouse_pick){//modify buffer data when mouse pick event is true
        //this->mouse_pick initial false
        ModifyBufferData();
    }
}

///*********ODE****************
void VBOcloth::updateTimeStep()
{//put next info to now
    for(int i= 0; i<(int)this->VertexODEInfo.size(); ++i)
    {
        VertexODEInfo[i].x_now = VertexODEInfo[i].x_next;
        VertexODEInfo[i].v_now = VertexODEInfo[i].v_next;
        VertexODEInfo[i].force_now = VertexODEInfo[i].force_next;
    }
}

void VBOcloth::updateForcePropagation()
{//force propagate on spring within dt
    vector<float> zero{0, 0, 0};
    for(int i= 0; i<(int)this->NeighborVert.size(); ++i)
    {

        vector<float> f_source = VertexODEInfo[i].force_now;
        for(int k =0; k< 3; ++k)//energy emission
            f_source[k] *= 0.3;

        fout<<i<<'\t';
        for(auto it:f_source)fout<<it<<'\t'; fout<<endl;
        for(int j= 0; j<(int)this->NeighborVert[i].size(); ++j)
        {
            int index = NeighborVert[i][j];

            VertexODEInfo[index].force_next = zero; /*make zero*/
            vector<float> &f_dest = VertexODEInfo[index].force_next;
            //propagate force from vertex
            for(int k =0; k< 3; ++k)
                f_dest[k] = f_dest[k] + f_source[k];
        }
    }
}
vector<float> VBOcloth::Force_hooke(int index)
{// hooke's law - spring force
    //vertex rest position (constant value)
    float *x_next = VertexODEInfo[index].x_next.data();
    float *x_rest = VertexODEInfo[index].x_rest.data();

    vector<float> Fh(3);
    for(int i =0; i< 3; ++i)
        Fh[i] = (x_rest[i] - x_next[i]) * this->hooke_k;
    return Fh;
}
vector<float> VBOcloth::Force_damping(int index)
{// damping force
    vector<float> Fd(3);
    float *v0 = VertexODEInfo[index].v_now.data();
    for(int i =0; i< 3; ++i)
        Fd[i] = v0[i] * this->damp_d;
    return Fd;
}
void VBOcloth::updateMotion()
{
    for(int i= 0; i<(int)this->VertexODEInfo.size(); ++i)
    {

        vector<float> Fe = VertexODEInfo[i].force_next;//external force
        vector<float> Fh = Force_hooke(i);//force hooke law - spring force
        vector<float> Fd = Force_damping(i);//damping force

        vector<float> FF(3);//final force
        for(int k =0; k<3; ++k)
            FF[k] = Fe[k] + Fh[k] + Fd[k];
        VertexODEInfo[i].force_next = FF;

        float *a = FF.data();
        float *v0 = VertexODEInfo[i].v_now.data();
        float *v1 = VertexODEInfo[i].v_next.data();
        float *x0 = VertexODEInfo[i].x_now.data();
        float *x1 = VertexODEInfo[i].x_next.data();

        //v1 = v0 + at
        for(int k = 0 ; k< 3; ++k)
        {
            v1[k] = v0[k] + a[k]*del_t;
        }
        //x1 = x0 + (v1+v0)t/2
        for(int k = 0 ; k< 3; ++k)
        {
            x1[k] = x0[k] + (v0[k] + v1[k])*del_t*0.5;
        }

    }
}
void VBOcloth::ODE()
{

    if(this->MSFStatus)//exist mouse external force status
    {/*work once at each pick*/
        //calculate destination point
        vector<float> VertexNew = InversePerspective();
        //new vertex in in local space
        int index = this->index_pick * 3;
        vector<float> VertexOld{vertices[index],vertices[index+1],vertices[index+2]};
        //original vertex in in local space

        vector<float> Fe = vec_subtraction(VertexOld, VertexNew);//external force come from mouse

        for(int i=0; i<(int)Fe.size(); ++i)
            Fe[i] *= 10;

        VertexODEInfo[this->index_pick].force_next = Fe;
        //put force in picked vertex
        //no damping and hooke's force


//        cout<<"VertexNew: "; vout(VertexNew);
//        cout<<"VertexOld: "; vout(VertexOld);
//        cout<<"Fe: "; vout(Fe);

        this->MSFStatus = false;
    }
    else//! this->MSFStatus ~~ doesn't exist mouse external force
    {
        updateTimeStep();
        updateForcePropagation();
        updateMotion();
    }

}
vector<float> VBOcloth::getMotionData()
{
    vector<float> output;
    for(int i= 0; i<(int)this->VertexODEInfo.size(); ++i)
    {
        float *pos = VertexODEInfo[i].x_next.data();
        for(int j =0 ; j<3; ++j)
            output.push_back(pos[j]);
    }
    return output;
}
void VBOcloth::iniODEPosition()
{
    int index;
    for(int i =0; i<(int)this->VertexODEInfo.size(); ++i)
    {
        index = i*3;
        VertexODEInfo[i].x_next =
            vector<float>{vertices[index], vertices[index+1], vertices[index+2]};
        VertexODEInfo[i].x_rest = VertexODEInfo[i].x_next;
    }
}

///*********Modify Buffer Data************
void VBOcloth::CheckBufferVert()
{
    int numOfData = this->vertices.size();
    vector<float> data (numOfData);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);//vertices
    glGetBufferSubData(GL_ARRAY_BUFFER, 0,
                       numOfData * sizeof(float), &data[0]);
   int it = 0;
   cout<<endl<<endl;
    for(int i = 0; i<numOfData; i+=3)
    {
        cout<<it<<'\t';
        cout<<data[i]<<'\t'
        <<data[i+1]<<'\t'
        <<data[i+2]<<endl;
        it++;

    }
    cout<<endl;
}
vector<float> VBOcloth::InversePerspective()
{//inverse mouse location in screen space to model space
    int index = this->index_pick * 3;
    vec4 v_Model = vec4(vertices[index],vertices[index+1],vertices[index+2],1);
    vec4 v_pers = MVP * v_Model;
    vec4 v_screen = m_screen * (v_pers/v_pers[3]);
    //model space

    glm::vec4 vert_device = vec4(this->Mouse2_x, this->Mouse2_y,
                            v_screen[2], 1.0);
    //get the z value
    //transform to normal space
    glm::vec4 vert_normal = vert_device;
    vert_normal[0] = (vert_device[0] - 400)/400;
    vert_normal[1] = (vert_device[1] - 300)/300;

    glm::vec4 vert_fakeModel = this->iMVP * vert_normal;
    //divide by w
    glm::vec4 vert_Model = vert_fakeModel/ vert_fakeModel[3];

    return vector<float>{vert_Model[0], vert_Model[1], vert_Model[2]};
}
void VBOcloth::updateNewVertex()
{//update all vertex

    //get new vertex data from vector<MotionInfo>VertexODEInfo
    vector<float> newVertex = getMotionData();
    //replace vertex info in shader
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);//bind VertexPosition
    //modify one vertex(3 float value) per time
    //picked vertex index

    glBufferSubData(GL_ARRAY_BUFFER, 0, //offset
                    newVertex.size() * sizeof(float), &newVertex[0]);
}

void VBOcloth::CheckBufferNorm()
{
    int numOfData = this->normal.size();
    float * data = new GLfloat[numOfData];

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);//normal
    glGetBufferSubData(GL_ARRAY_BUFFER, 0,
                       numOfData * sizeof(float), data);
//    int it = 0;
//    cout<<endl<<endl;
//    for(int i = 0; i<numOfData; i+=3)
//    {
//        cout<<it<<'\t';
//        cout<<data[i]<<'\t'
//        <<data[i+1]<<'\t'
//        <<data[i+2]<<endl;
//        it++;
//
//    }
//    cout<<endl;
    delete [] data;
}
void VBOcloth::updateNewNormal()
{
    //1 get all vertices from shader
    int numOfData = this->vertices.size();
    vector<float> vertexData (numOfData);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);//vertices
    glGetBufferSubData(GL_ARRAY_BUFFER, 0,
                       numOfData * sizeof(float), &vertexData[0]);

    //2 calculate polygon normal
    vector< vector<float>> vertexData2D = changeDimesionVertex(vertexData);
    vector< vector<float>> VertNormal(normal.size()/3, vector<float> (3));
    vector <float> e1;
    vector <float> e2;
    vector <float> poly_normal;
    for(int i = 0; i< (int)this->indices2D.size(); i++)
    {
        e1 = vec_subtraction(vertexData2D[indices2D[i][1]],
                             vertexData2D[indices2D[i][2]]);
        e2 = vec_subtraction(vertexData2D[indices2D[i][1]],
                             vertexData2D[indices2D[i][0]]);
        poly_normal = vec_cross(e1, e2);

        //3 calculate vertex normal
        for(int j = 0; j < (int)indices2D[i].size(); j++)
        {
            for(int k=0; k < 3; k++)
                VertNormal[indices2D[i][j]][k] += poly_normal[k];
        }

    }

    for(int i=0; i<(int)VertNormal.size() ;i++ )
    {//normalize the normal on vertex
        VertNormal[i] = vec_normalize(VertNormal[i]);
    }
    vector<float> NewNormal = vec_dim2to1(VertNormal);

    //4 replace vertex normal in shader with new vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);//bind VertexNormal
    glBufferSubData(GL_ARRAY_BUFFER, 0, //offset
                NewNormal.size() * sizeof(float), &NewNormal[0]);

}
void VBOcloth::ModifyBufferData()
{
    ODE();
    updateNewVertex();
    updateNewNormal();

//    CheckBufferVert();
//    CheckBufferNorm();
}

///************mouse*******
void VBOcloth::setMouse_1(int x, int y)
{//beginning point
    this->Mouse1_x = x;
    this->Mouse1_y = y;
    this->index_pick = MatchVertex(this->node_screen);
}
void VBOcloth::setMouse_2(int x, int y)
{//click left button again to set destination for the vertex
    this->Mouse2_x = x;
    this->Mouse2_y = y;
    if(this->index_pick >= 0) //valid pick
        this->mouse_pick = true;
    else//invalid pick
        cout<<"Match Vertex Failed"<<endl;
}
void VBOcloth::setMouseRight()
{//start ODE
    this->mouse_pick = false;//return to false
    this->MSFStatus = true;
    cout<<"Right click"<<endl;
}
///*******Vertex Matching*********
vector< vector<vertexInfo>> VBOcloth::ScreenNode(const vector< vector<vertexInfo>> &node)
{//compute node in screen space
    glm::mat4 m_screen = glm::mat4(400, 0, 0, 0,
                                     0,  300, 0, 0,
                                     0, 0, 1, 0,
                                     400, 300, 0, 1);
    vector< vector<vertexInfo>> node_screen = node;
    for(int i = 0; i < (int)node.size(); i++)
    {
        for(int j = 0 ; j <(int)node[i].size(); j++)
        {
            vec4 v_pers = this->MVP * vec4(node[i][j].x, node[i][j].y, node[i][j].z, 1.0);
            vec4 v_screen = m_screen * vec4(v_pers/v_pers[3]);

            node_screen[i][j].x = v_screen.x;
            node_screen[i][j].y = v_screen.y;
            node_screen[i][j].z = v_screen.z;
        }
    }

    return node_screen;
}
float VBOcloth::Length_MsVt(float p1x, float p1y, float p2x, float p2y)
{//output two point pseudo length (without radical sign)
    float a = p1x - p2x;
    float b = p1y - p2y;
    return a*a + b*b;
}
int VBOcloth::QuickPartition(vector< vector<float>> &A, int p, int q, int index)
{
    int x = A[p][index];//pivot //index = 1 -> distance, 2-> z value
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
void VBOcloth::QuickSort(vector< vector<float>> &A, int p, int q, int index)
{
    if(p < q)
    {
        int r = QuickPartition(A, p, q, index);
        QuickSort(A, p, r, index);
        QuickSort(A, r+1, q, index);
    }
}
int VBOcloth::MatchVertex(const vector< vector<vertexInfo>> &node_s)
{//match vertex by mouse
//    int winHeight = 600;
    int mx = this->Mouse1_x;
    int my = this->Mouse1_y;
    int threshold = 1000;
    int width = (int)node_s.size();
    int index;
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
        index = (int)nearVertex[0][0];
    }else{//no vertex match by mouse location
        index = -1;
    }
    //return the "index" of vertex select by mouse
    //if no vertex, return -1
    return index;
}


///******************generate Cloth Geometry******
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
vector<float> VBOcloth::getNormal(int width)
{
    width = width * width;
    float nor[] = {0, 1, 0};
    vector<float> normal;
    for(int i=0; i< width; i++)
    {
        normal.insert(normal.end(),nor, nor+3);
    }
    return normal;
}
vector< vector<int>> VBOcloth::changeDimesionIndex(vector<GLuint> &indices)
{//change indices from 1D to 2D
    vector< vector<int>> index2D;
    vector<int> temp;
    for(int i = 0; i< (int)indices.size(); i+=3)
    {
        temp.clear();
        temp.push_back(indices[i]);
        temp.push_back(indices[i+1]);
        temp.push_back(indices[i+2]);
        index2D.push_back(temp);
    }
    return index2D;
}
vector< vector<float>> VBOcloth::changeDimesionVertex(const vector<float> &vertex)
{//change vertex from 1D to 2D
    vector< vector<float>> vertex2D;
    vector<float> temp;
    for(int i = 0; i< (int)vertex.size(); i+=3)
    {
        temp.clear();
        temp.push_back(vertex[i]);
        temp.push_back(vertex[i+1]);
        temp.push_back(vertex[i+2]);
        vertex2D.push_back(temp);
    }
    return vertex2D;
}
void VBOcloth::addVertexIndex(vector<int> &VertIndex,int a, int b, int c)
{
    vector< vector<int>> temp_1;
    for(int i = 0; i<(int)this->indices2D.size(); i++)
    {
        for(int j = 0; j<(int)this->indices2D[i].size(); j++)
        {
            if(a == this->indices2D[i][j])
            {
                vector<int> temp = this->indices2D[i];
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
    VertIndex.push_back(temp_3[0][0]);//vertex 1
    VertIndex.push_back(temp_3[0][1]);//vertex 2
    VertIndex.push_back(temp_3[0][2]);//vertex 3
}

vector< vector<int>> VBOcloth::getNeighborPolygonIndex(int width)
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

//    for(int i = 0; i< (int)NeighborPolyIndex.size(); i++)
//    {
//        cout<<i<<'\t';
//        for(int j = 0; j< (int)NeighborPolyIndex[i].size(); j++)
//        {
//            cout<<NeighborPolyIndex[i][j]<<"  ";
//        }
//        cout<<endl;
//    }

    return NeighborPolyIndex;
}
vector< vector<int>> VBOcloth::getNeighborVertex(int width)
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
    return NeighborVert;
}
VBOcloth::~VBOcloth()
{
    //dtor
}
