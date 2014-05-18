/**
    Developer: Johnny Lee
    Date: Mar.12.2013

    Description: This is a project implement interactive deformable object
    Tasks:
        1) render floor grid
        2) render cloth
        3) add mouse detection
        4) match mouse location to vertex
*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ogl_trivialfunc.h"
#include "SCENE.h"

using namespace std;
///function declaration
void drawScene(void);
void openglInit(int argc, char** argv);
void glslInit(void);
void preProcessing(void);
///***controller****
SCENE scene;
///**Mouse control
GLsizei winWidth = 800,winHeight = 600;
bool MouseLeftClick1 = false;
bool MouseLeftClick2 = false;
///-------program begin--------
void preProcessing()
{
    scene.setVBO();
}
void drawScene()
{//main drawing function
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene.render();

    glutSwapBuffers();
}
void MouseFcn(int button, int state, int mx, int my)
{/*sets the mouse callback for the current window*/
    if(!::MouseLeftClick1)
    {//click left button to pick vertex
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {//store beginning point
            ::MouseLeftClick1 = true;
            scene.setMouse_1(mx, winHeight - my);
        }
    }
    else if(::MouseLeftClick1 && !::MouseLeftClick2)//::MouseLeftClick == true
    {//click left button again to set destination for the vertex
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            ::MouseLeftClick2 = true;
            scene.setMouse_2(mx, winHeight - my);
        }
    }
    else
    {
        if(::MouseLeftClick1 && ::MouseLeftClick2)
        {//only work if previous process have done
            if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
            {//click right button to release vertex
                scene.setMouseRight();
                ::MouseLeftClick1 = false;
                ::MouseLeftClick2 = false;
            }
        }

    }
}

void openglInit(int argc, char** argv)
{// initialize OpenGL function

    //Initialize GLUT
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(::winWidth , ::winHeight); //Set the window size

	//Create the window
	glutCreateWindow("Interactive Deformable Object ~ Cloth Simulation");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
    glutTimerFunc(25, update, 0); //Add a timer
    //keyboard
    glutKeyboardFunc(handleKeypress);
    //mouse
    glutMouseFunc(MouseFcn);
}
int main(int argc, char** argv)
{

    ///OpenGL initialization
    openglInit(argc, argv);
    ///glew function initialization
    glslInit();
    ///ante production
    preProcessing();


    glutMainLoop();
	return 0; //This line is never reached
}
