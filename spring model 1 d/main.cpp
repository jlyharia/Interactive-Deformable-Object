#include <GL/glut.h>
#include <iostream>

using namespace std;

struct MotionInfo{
    const float x_rest = 0;//rest position
    float x_now = 0;
    float x_next = 0;
    float v_now = 0;
    float v_next = 0;
    float a = 0;
    const float k = 0.8; //spring constant
    const float d = -0.05; //damping constant
}mass; //mass i
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

float Force_hooke()
{
    float del_x = mass.x_rest - mass.x_next;
    float Fh = mass.k * del_x;
    return Fh;
}
float Force_damping()
{
    float Fd = mass.d * mass.v_now;
//    cout<<Fd<<endl;
    return Fd;
}
void updateMass(float Fh, float Fd, float Fe)
{//update 1)position, 2)velocity, 3)acceleration
    float netForce = Fh + Fd + Fe;
    mass.a = netForce;

    //v1 = v0 + at
    mass.v_next = mass.v_now + mass.a * ::del_t;
    //x1 = x0 + (v1+v0)/2
    mass.x_next = mass.x_now + (mass.v_now + mass.v_next) * 0.5 *::del_t;
}
void NextTimeStep()
{//put next info to now
    ::mass.x_now = ::mass.x_next;
    ::mass.v_now = ::mass.v_next;
}
void SpringODE()
{
    if(exfStatus){//applied external force
        float Fe = 20;
        updateMass(0, 0, Fe);
        exfStatus = false;

    }else{// no external force
        NextTimeStep();
        float Fh = Force_hooke();//hooke's law
        float Fd = Force_damping();//damping force
        updateMass(Fh, Fd, 0);
    }


}
void drawLine()
{
    SpringODE();

    glPushMatrix();
    glColor3f(0.0, 0.7, 1.0);
    glTranslatef(0.0, 0.0, -5.0);
    glBegin(GL_LINES);
        //front
        glVertex3f(-3,0,0);
        glVertex3f(::mass.x_next, 0, 0);
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
        glVertex3f(::mass.x_rest ,0.0f ,0.0);
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
