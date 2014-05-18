#include <GL/glew.h>
#include <GL/freeglut.h>
void handleResize(int w, int h)
{// window resize
	glViewport(0, 0, w, h);
}
void update(int value)
{//update frame buffer
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}
void initRendering()
{// initialize rendering
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //clear the background color
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Enable back face culling
	glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);

}
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: //Escape key
			exit(0); //Exit the program
			break;
        default:
            break;
	}
}
void glslInit()
{//initialize glew function
    glewInit();
}

