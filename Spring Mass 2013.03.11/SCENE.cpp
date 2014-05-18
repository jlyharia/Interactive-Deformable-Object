
#include <GL/glew.h>
#include <GL/glut.h>
#include "SCENE.h"
#include "shaderloader.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
SCENE::SCENE()
{
    this->MouseLeftClick = false;
}
void SCENE::setVBO()/*only set once*/
{
    obj_floor = new VBOfloor(CreateShaders("floor"));
    obj_cloth = new VBOcloth(CreateShaders("phong"));
}

void SCENE::render()
{
    obj_floor->render();
    obj_cloth->render();
}
void SCENE::setMouse_1(int x, int y)
{//beginning point
    this->Mouse1_x = x;
    this->Mouse1_y = y;
    obj_cloth->setMouse_1(x, y);
}
void SCENE::setMouse_2(int x, int y)
{//ending point
    this->Mouse2_x = x;
    this->Mouse2_y = y;
}
SCENE::~SCENE()
{
    //dtor
}
