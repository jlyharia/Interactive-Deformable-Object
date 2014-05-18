#include <GL/freeglut.h>
#include <iostream>

using namespace std;
class scrPt{
public:
    GLint x, y;
};

GLsizei winWidth = 400,winHeight = 300;
bool msLEFTclick = false;

scrPt endPt1, endPt2;
void init()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0 , 200.0, 0.0, 150);
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
    /*reset viewport and projection parameter*/
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

    winWidth = newWidth;
    winWidth = newHeight;
}
void drawLineSegment()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3);
    if(::msLEFTclick)
    {
        glBegin(GL_LINES);
            glVertex2i(endPt1.x, endPt1.y);
            glVertex2i(endPt2.x, endPt2.y);
        glEnd();
    }
}

void line(GLint button, GLint state, GLint xMouse, GLint yMouse)
{

    if(!::msLEFTclick)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {//store beginning point
            endPt1.x = xMouse;
            endPt1.y = winHeight - yMouse;
            ::msLEFTclick = true;
        }
    }
    else//::msLEFTclick == true
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        {
            ::msLEFTclick = false;
        }
    }
    glFlush();
}
void mouseMotion(int xMouse, int yMouse)
{/*when the mouse is moved within the display window with one or more buttons activated*/

    if(::msLEFTclick)
    {//store ending point
        endPt2.x = xMouse;
        endPt2.y = winHeight - yMouse;
        drawLineSegment();
    }

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Draw Interactive lines");

    init();
    glutDisplayFunc(drawLineSegment);
    glutReshapeFunc(winReshapeFcn);
    /*mouse function*/
    glutMouseFunc(line);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}
