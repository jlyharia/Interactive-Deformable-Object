#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::mat2;
using glm::mat3;
mat4 ModelViewMatrix;
mat4 MVP;
mat4 m_pers;
mat4 m_trans;
mat4 m_screen;
mat4 m_view;
vec4 vert_device;
int winWidth = 800, winHeight = 600;
void vout(glm::vec3 in)
{
    for(int i = 0; i< 3; i++)
        cout<<in[i]<<'\t';
    cout<<endl;
}
void vout(glm::vec4 in)
{
    for(int i = 0; i< 4; i++)
        cout<<in[i]<<'\t';
    cout<<endl;
}
void vout(glm::mat4 in)
{
    for(int i = 0; i< 4; i++)
    {
        for(int j = 0; j< 4; j++)
            cout<<in[i][j]<<'\t';
        cout<<endl;
    }
}
void setMVP()
{
    //translate
    ::m_trans = glm::translate(glm::mat4(1.0f),glm::vec3 (-5.0f, -4.0f, -16.0f));
    //matrix view
    ::m_view = glm::lookAt(glm::vec3 (0.0f, 0.0f, 0.0f),
                                    glm::vec3 (0.0f, 0.0f, -1.0f),
                                    glm::vec3 (0.0f, 1.0f, 0.0f));
    //matrix perspective
    ::m_pers = glm::perspective(70.0f, 1.33f, 1.0f, 200.0f);
    //ModelView Matrix
    ::ModelViewMatrix =  m_view * m_trans;

    ::MVP = m_pers * ModelViewMatrix ;//*scal;
    ::m_screen = mat4(400, 0, 0, 0,
                     0,  300, 0, 0,
                     0, 0, 1, 0,
                     400, 300, 0, 1);
    /*Inverse matrix*/

}
void forwardPorcess()
{
//    vec3 vert_model= vec3(3.0, 4.0, 0.0);
    vec3 vert_model= vec3(9.0, 0.0, 0.0);
    cout<<"vertex in Model: ";
    vout(vert_model);

    vec4 vert_world = m_trans * vec4(vert_model, 1.0);
    cout<<"vertex in World: ";
    vout(vert_world);

    vec4 vert_view = m_view * vert_world;
    cout<<"vertex in View: ";
    vout(vert_view);

    vec4 vert_pres = m_pers * vert_view;
    cout<<"vertex in Perspective: ";
    vout(vert_pres);

    vec4 vert_normal = vert_pres / vert_pres[3];
    cout<<"vertex in Normal: ";
    vout(vert_normal);

    vert_device = m_screen * vert_normal;
    cout<<"vertex in Device: ";
    vout(vert_device);
    cout<<endl<<endl;
}

void InversePorcess()
{
    cout<<"Inverse Process"<<endl;
    cout<<"vertex in Device: ";

    vert_device.x = 547;
    vert_device.y = 213;
    vout(vert_device);

    vec4 vert_normal = vert_device;
    vert_normal[0] = (vert_device[0] - 400)/400;
    vert_normal[1] = (vert_device[1] - 300)/300;
    cout<<"vertex in Normal: ";
    vout(vert_normal);

    vec4 vert_pres = glm::inverse(::MVP) * vert_normal;
    cout<<"vertex in Perspective: ";
    vout(vert_pres);

    vec4 vert_Model = vert_pres/vert_pres[3];
    cout<<"vertex in Model: ";
    vout(vert_Model);

//    vec4 vert_pres = glm::inverse(::m_pers) * vert_normal;
//    cout<<"vertex in Perspective: ";
//    vout(vert_pres);
//
//    vec4 vert_view = vert_pres/vert_pres[3];
//    cout<<"vertex in View: ";
//    vout(vert_view);
//
//    vec4 vert_model = glm::inverse(::ModelViewMatrix) * vert_view;
//    cout<<"vertex in Model: ";
//    vout(vert_model);
}
int main()
{
    setMVP();
    forwardPorcess();
    InversePorcess();
    return 0;
}
