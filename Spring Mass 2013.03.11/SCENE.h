#ifndef SCENE_H
#define SCENE_H
#include <glm/glm.hpp>
#include "loadModel.h"
#include <GL/glut.h>
#include "GLSLProgram.h"
#include "VBOfloor.h"
#include "VBOcloth.h"
class SCENE
{
    public:
        SCENE();
        virtual ~SCENE();
        void setVBO();
        void render();

        void setMouse_1(int, int);
        void setMouse_2(int, int);
    private:
        /*object*/
        //floor
        VBOfloor *obj_floor;
        //cloth
        VBOcloth *obj_cloth;

        /*Mouse Information*/
        bool MouseLeftClick;
        int Mouse1_x, Mouse1_y, Mouse2_x, Mouse2_y;
};

#endif // SCENE_H
