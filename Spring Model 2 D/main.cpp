#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using glm::mat4;
using glm::vec3;
using glm::vec4;

struct MotionInfo{
    const vec3 x_rest = vec3(0, 0, 0);//rest position
    vec3 x_now = vec3(0, 0, 0);
    vec3 x_next = vec3(0, 0, 0);
    vec3 v_now = vec3(0, 0, 0);
    vec3 v_next = vec3(0, 0, 0);
    vec3 a = vec3(0, 0, 0);
    const float k = 0.8; //spring constant
    const float d = -0.1; //damping constant
}mass; //mass i
// screen width and height

int screenWidth;
int screenHeight;

// rotation angle
bool exfStatus = true;//for initial force
float del_t = 0.09;

// user initialization
void init( void )
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth (1.0);
}

vec3 Force_hooke()
{
    vec3 del_x = mass.x_rest - mass.x_next;
    del_x *= mass.k;
    return del_x;
}
vec3 Force_damping()
{
    vec3 Fd = mass.v_now;
    Fd *= mass.d;
    return Fd;
}
void updateMass(vec3 Fh, vec3 Fd, vec3 Fe)
{//update 1)position, 2)velocity, 3)acceleration
    vec3 netForce = Fh + Fd + Fe;
    mass.a = netForce;

    //v1 = v0 + at
    vec3 adelt = mass.a;
    adelt *= ::del_t;
    mass.v_next = mass.v_now + adelt;
    //x1 = x0 + (v1+v0)t/2
    vec3 length = mass.v_now + mass.v_next;
    length *= ::del_t/2;
//    mass.x_next = mass.x_now + (mass.v_now + mass.v_next) * 0.5 *::del_t;
    mass.x_next = mass.x_now + length;
}
void NextTimeStep()
{//put next info to now
    ::mass.x_now = ::mass.x_next;
    ::mass.v_now = ::mass.v_next;
}
void SpringODE()
{
    vec3 nothing = vec3(0, 0, 0);
    if(exfStatus){//applied external force
        vec3 Fe = vec3(0, 20, 0);
        updateMass(nothing, nothing, Fe);
        exfStatus = false;

    }else{// no external force
        NextTimeStep();
        vec3 Fh = Force_hooke();//hooke's law
        vec3 Fd = Force_damping();//damping force
        updateMass(Fh, Fd, nothing);
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
        glVertex3f(mass.v_next.x, mass.v_next.y, mass.v_next.z);
    glEnd();
    glPopMatrix();
}
void drawLine_2()
{
    glPushMatrix();
    glColor3f(1.0, 0.7, 0.0);
    glTranslatef(0.0, -2.0, -5.0);
    glBegin(GL_LINES);
        //front
        glVertex3f(-3,0,0);
        glVertex3f(mass.x_rest.x, mass.x_rest.y, mass.x_rest.z);
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
//    vec3 x_now = vec3(1, 2, 3);
//    vec3 x_next = vec3(0, 4, 0);
//    x_next *= 3;
//    for(int i=0; i< 3; i++)
//        cout<<x_next[i]<<'\t';
    // set callback functions
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    // main loop
    glutMainLoop();

    return 0;
}
