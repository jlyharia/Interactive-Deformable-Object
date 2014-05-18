#include <GL/freeglut.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 400,winHeight = 300;

void displayFcn()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(3.0);
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

void plotPoint(GLint x, GLint y)
{
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}
void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
//    cout<<"test"<<endl;
    if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
        plotPoint(xMouse, winHeight - yMouse);
    glFlush();
}
void init()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0 , 200.0, 0.0, 150);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Mouse Plot Points");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(mousePtPlot);

    glutMainLoop();
    return 0;
}
