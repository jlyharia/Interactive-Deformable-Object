
#include <GL/glew.h>
#include <GL/glut.h>
#include "SCENE.h"
#include "shaderloader.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
SCENE::SCENE()
{

}
void SCENE::setVBO()/*only set once*/
{
//    obj_floor = new VBOfloor(CreateShaders("floor"));
    obj_cloth = new VBOcloth(CreateShaders("phong"));
}

void SCENE::render()
{
//    obj_floor->render();
    obj_cloth->render();
}
void SCENE::setMouse_1(int x, int y)
{//beginning point
    obj_cloth->setMouse_1(x, y);
}
void SCENE::setMouse_2(int x, int y)
{//ending point
    obj_cloth->setMouse_2(x, y);
}
void SCENE::setMouseRight()
{//release picked vertex by click right button
    obj_cloth->setMouseRight();
}
SCENE::~SCENE()
{
    //dtor
}
