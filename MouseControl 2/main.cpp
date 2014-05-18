#include <GL/freeglut.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 400,winHeight = 300;
GLint endPtCtr = 0;

class scrPt{
public:
    GLint x, y;
};
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

void displayFcn()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2i(endPt1.x, endPt1.y);
        glVertex2i(endPt2.x, endPt2.y);
    glEnd();
//    glFlush();
}
void polyline(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
    if(::endPtCtr == 0){
        if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
            endPt1.x = xMouse;
            endPt1.y = winHeight - yMouse;
            endPtCtr = 1;
        }else{
            if(button == GLUT_RIGHT_BUTTON)
                exit(0);
        }
    }else{
        if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
            endPt2.x = xMouse;
            endPt2.y = winHeight - yMouse;
            displayFcn();
            endPtCtr = 0;
        }else{
            if(button == GLUT_RIGHT_BUTTON)
                exit(0);
        }
    }

    glFlush();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Draw Interactive Polyline");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(polyline);


    glutMainLoop();
    return 0;
}
