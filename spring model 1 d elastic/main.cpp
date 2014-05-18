#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

class MotionInfo{
    public:
        float x_rest = 0;//rest position
        float x_now = 0;
        float x_next = 0;
        float v_now = 0;
        float v_next = 0;
        float a = 0;
        float force_now =0;
        float force_next =0;
        const float k = 0.8; //spring constant
        const float d = -0.3; //damping constant
}mass[2]; //mass i
// screen width and height

int screenWidth;
int screenHeight;

// rotation angle
bool exfStatus = true;//for initial force
float del_t = 0.03;

// user initialization
void init( void )
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth (1.0);
}

float Force_hooke(int i)
{
    float del_x = mass[i].x_rest - mass[i].x_next;
    float Fh = mass[i].k * del_x;
    return Fh;
}
float Force_damping(int i)
{
    float Fd = mass[i].d * mass[i].v_now;
    return Fd;
}
void updateMotion()
{//update 1)position, 2)velocity, 3)acceleration
    for(int i =0 ;i < 2; i++)
    {
        float Fh = Force_hooke(i);//hooke's law
        float Fd = Force_damping(i);//damping forces
        float Fe = mass[i].force_next;
        float netForce = Fh + Fd + Fe;
        mass[i].force_next = netForce;
        mass[i].a = netForce;
        //v1 = v0 + at
        mass[i].v_next = mass[i].v_now + mass[i].a * ::del_t;
        //x1 = x0 + (v1+v0)/2
        mass[i].x_next = mass[i].x_now + (mass[i].v_now + mass[i].v_next) * 0.5 *::del_t;
    }

}
void NextTimeStep()
{//put next info to now
    for(int i=0 ; i< 2; i++)
    {
        ::mass[i].x_now = ::mass[i].x_next;
        ::mass[i].v_now = ::mass[i].v_next;
        ::mass[i].force_now = ::mass[i].force_next;
    }
}
void ForcePropagation()
{//! importance: force_next should clear before force propagation
    //node 0 to node 1
    ::mass[1].force_next = ::mass[0].force_now * 0.5;
    //node 1 to node 0
    ::mass[0].force_next = ::mass[1].force_now * 0.5;
}
void SpringODE()
{
    if(exfStatus){//applied external force
        mass[1].force_next = 100;
        exfStatus = false;
    }else{// no external force
        NextTimeStep();
        ForcePropagation();
        updateMotion();
    }


}
void preprocessing()
{//set geometry
    mass[0].x_next = 0;
    mass[1].x_next = 3;

    mass[0].x_rest = 0;
    mass[1].x_rest = 3;

}
void drawLine()
{
    SpringODE();
//    cout<<mass[0].x_next<<'\t'<<mass[1].x_next<<endl;
    glPushMatrix();
    glColor3f(0.0, 0.7, 1.0);
    glTranslatef(0.0, 0.0, -5.0);
    glBegin(GL_LINES);
        //line 1
        glVertex3f(-3,0,0);
        glVertex3f(::mass[0].x_next, 0, 0);
        //line 2
        glColor3f(1.0, 0.0, 1.0);
        glVertex3f(::mass[0].x_next , 0, 0);
        glVertex3f(::mass[1].x_next ,0.0 ,0.0);
    glEnd();
    glPopMatrix();
}
void drawLine_2()
{// for comparison purpose
    glPushMatrix();
    glColor3f(1.0, 0.7, 0.0);
    glTranslatef(0.0, -2.0, -5.0);
    glBegin(GL_LINES);
        //front
        glVertex3f(-3,0,0);
        glVertex3f(0 ,0.0f ,0.0);

        //line 2
        glColor3f(0.5, 1.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(3 ,0.0 ,0.0);

    glEnd();
    glPopMatrix();
}
void drawScene( void )
{
    // clear buffer
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth (1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render state
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // light source attributes


    // modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    drawLine();
    drawLine_2();

       // swap back and front buffers
    glutSwapBuffers();

    // use the codes below to capture frame images of animation

}

// update viewport and projection matrix when the window is resized
void reshape( int w, int h )
{
    // viewport
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // projection matrix
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0);

    screenWidth = w;
    screenHeight = h;
}

// keyboard input
void keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
    case VK_ESCAPE:
        exit(0);
        break;

    default:
        break;
    }
}

// timer
void timer( int value )
{
    // render
    glutPostRedisplay();

    // reset timer
    // 16 ms per frame ( about 60 frames per second )
    glutTimerFunc( 16, timer, 0 );
}

int main( int argc, char** argv )
{
    preprocessing();
    // create GL window
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize(936, 528);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    // user initialization
    init();

    // set callback functions
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    // main loop
    glutMainLoop();

    return 0;
}
