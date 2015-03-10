#include <iostream>
#include <cmath>



#define GLEW_STATIC
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#undef GLFW_DLL
#include <GLFW/glfw3.h>
#include <unistd.h>
#include "HalfPipe.h"
#include "Polygon.h"
#include "GenericObject2.h"
#include "GenericObject1.h"
/* define global variables here */


GenericObject2* wheels;
Polygon* deck;
GenericObject1* truckBase;
GenericObject1* truckAxle;
HalfPipe* h_pipe;
GenericObject2* base;

glm::mat4 wheel_cf;
glm::mat4 board_cf;
glm::mat4 camera_cf, light1_cf, light0_cf;
glm::mat4 *active;

const float INIT_X_POS = 0.0;
const float GRAVITY = 9.8;   /* m/sec^2 */
bool is_anim_running = false;

/* light source setting */
GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};   /* color */
GLfloat light1_color[] = {1.0, 1.0, 0.6, 1.0};  /* color */
GLfloat black_color[] = {0.0, 0.0, 0.0, 1.0};   /* color */

/*--------------------------------*
 * GLUT Reshape callback function *
 *--------------------------------*/
void reshapeCallback (GLFWwindow *win, int w, int h)
{
    glViewport (0, 0, w, h);

    /* switch to Projection matrix mode */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    gluPerspective (60, (float) w / (float) h, 5.0, 100.0);

    /* switch back to Model View matrix mode */
    glMatrixMode (GL_MODELVIEW);
    camera_cf = glm::lookAt(glm::vec3(25,20,20), glm::vec3(0,0,10), glm::vec3(0,0,1));
}

/*================================================================*
 * Idle Callback function. This is the main engine for simulation *
 *================================================================*/
void updateCoordFrames()
{
    static double last_timestamp = 0;
    static float anim_time = 0;
    static float distance = 0;
    static float wheel_angle = 0;
    static int deg = 0;
    const float SPEED = 0.02;
    const float WHEEL_SPEED = 180/ M_PI * SPEED/0.0045; /* in degrees per second */
    float delta, current;


    current = glfwGetTime();
    if (is_anim_running) {
        delta = (current - last_timestamp);
        distance = SPEED * delta;
        board_cf *= glm::translate( glm::vec3{distance, 0.0f, 0.0f});
    }
    last_timestamp = current;
}

void myGLInit ()
{
    glClearColor (0.0, 0.0, 0.0, 1.0); /* black background */

    /* fill front-facing polygon */
    glPolygonMode (GL_FRONT, GL_FILL);
    /* draw outline of back-facing polygon */
    glPolygonMode (GL_BACK, GL_LINE);

    /* Enable depth test for hidden surface removal */
    glEnable (GL_DEPTH_TEST);

    /* enable back-face culling */
    glEnable (GL_CULL_FACE);
    //glCullFace (GL_BACK);

    /* Enable shading */
    glEnable (GL_LIGHTING);
    glEnable (GL_NORMALIZE); /* Tell OpenGL to renormalize normal vector
                              after transformation */
    /* initialize two light sources */
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT0, GL_AMBIENT, light0_color);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light0_color);
    glEnable (GL_LIGHT1);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_color);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_color);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_color);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 40);

    glEnableClientState(GL_VERTEX_ARRAY);
}

void render_skateBoard(){
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.3,0 , 0);
    wheels->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3,0 , -0.2);
    glRotatef(180, 1, 0, 0);
    wheels->render();
    glPopMatrix();

    /*Back Trucks and Wheels*/
    glPushMatrix();
    glTranslatef(0.2, 0, 0);

    glPushMatrix();
    glTranslatef(0.60,0 , 0);

    wheels->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.60,0 , -0.2);
    glRotatef(180, 1, 0, 0);
    wheels->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.60,0 , -0.1);
    truckAxle->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6,0.02 , -0.1);
    glRotatef(45, 0, 0, 1);
    truckBase->render();
    glPopMatrix();

    glPopMatrix();
    /*Back Trucks and Wheels End*/


    glPushMatrix();
    glTranslatef(0.3,0 , -0.1);
    //glRotatef(90, 1, 0, 0);
    truckAxle->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3,0.02 , -0.1);
    glRotatef(45, 0, 0, 1);
    truckBase->render();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0.55,0.07 , -0.1);
    //glRotatef(45, 0, 0, 1);
    deck->render();
    glPopMatrix();

    glPopMatrix();
}
void render_HalfPipe (){
    glPushMatrix();
    glTranslatef(1.3, 0, 0);
    h_pipe->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base->render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3, 0, 0);
    glRotatef(180, 0, 1, 0);
    h_pipe->render();
    glPopMatrix();
}

/*--------------------------------*
 * GLUT display callback function *
 *--------------------------------*/
void displayCallback (GLFWwindow *win)
{
    /* clear the window */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadMatrixf(glm::value_ptr(camera_cf));

    glBegin (GL_LINES);
    glColor3ub (255, 0, 0);
    glVertex3i (0, 0, 0);
    glVertex3i (2, 0, 0);
    glColor3ub (0, 255, 0);
    glVertex3i (0, 0, 0);
    glVertex3i (0, 2, 0);
    glColor3ub (0, 0, 255);
    glVertex3i (0, 0, 0);
    glVertex3i (0, 0, 2);
    glEnd();


    /* Specify the reflectance property of the ground using glColor
       (instead of glMaterial....)
     */
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3ub (29, 100, 56);

    glBegin (GL_QUADS);
    const int GROUND_SIZE = 40;
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (-GROUND_SIZE, -GROUND_SIZE);
    glNormal3f (0.0f, 0.0f, 1.0f); /* normal vector for the ground */
    glVertex2i (GROUND_SIZE, -GROUND_SIZE);
    glEnd();
    glDisable (GL_COLOR_MATERIAL);

    /* place the light source in the scene. */
    glLightfv (GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::column(light0_cf, 3)));

    /* recall that the last column of a CF is the origin of the CF */
    glLightfv (GL_LIGHT1, GL_POSITION, glm::value_ptr(glm::column(light1_cf, 3)));

    /* we use the Z-axis of the light CF as the spotlight direction */
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::column(light1_cf, 2)));

    /* the curly bracket pairs below are only for readability */
//    glPushMatrix();
//    {
//        glMultMatrixf(glm::value_ptr(light0_cf));
//
//        /* Render light-0 as an emmisive object */
//        if (glIsEnabled(GL_LIGHT0))
//            glMaterialfv(GL_FRONT, GL_EMISSION, light0_color);
//        sphere.render();
//        glMaterialfv(GL_FRONT, GL_EMISSION, black_color);
//    }
//    glPopMatrix();

//    /* render the spot light using its coordinate frame */
//    glPushMatrix();
//    glMultMatrixf(glm::value_ptr(light1_cf));
//    spot->render();
//    glPopMatrix();


    /* The following nesting of push-pop pairs create an easy
     * way to render different object w.r.t other coordinate
     * frame.
     *
     * The swingarm is rendered w.r.t the swing base frame
     * The wheel is rendered w.r.t the swing arm frame
     */
    render_HalfPipe();
    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(board_cf));
        render_skateBoard();
    }
    glPopMatrix();
    /* to make smooth transition between frame */
    glfwSwapBuffers(win);
}

void myModelInit ()
{
//    sphere.build(15, 20);
//    spot = new Cylinder();
//    spot -> build(1 + tan(glm::radians(40.0f)), 1, 2);
    board_cf = glm::translate( glm::vec3{0,-0.95,0});
    //pipe.build(0.5, 3, 1, 0.6, 0.6, 0.6, 0.02);
    //base.build(3.0/1.414, 0, 3.0/1.414, 0, 0, 4, 0.6, 0.6, 0.6, 0.02);

    //wheels.build(0.045, 0.01, 0.045, 0.04, 0.04, 30, 0.9, 0.3, 0.4, 1);
    //truckAxle.build(0.01 , 0.01, .2, 20, 0.4, 0.4, 0.2);
    //truckBase.build(.055 , .055, .05, 4, 0.4, 0.1, 0.2);
    //deck.build(0.65, 0.03 , 0.2, 0.65 , 0.03, 0.1, 0.1, 0, 0.2);
    h_pipe = new HalfPipe();
    h_pipe->build(0.5, 3, 1, 0.6, 0.6, 0.6, 0.02);

    base = new GenericObject2;
    base->build(3.0/1.414, 0, 3.0/1.414, 0, 0, 4, 0.6, 0.6, 0.6, 0.02);

    wheels = new GenericObject2();
    wheels->build(0.045, 0.01, 0.045, 0.04, 0.04, 30, 0.9, 0.3, 0.4, 1);

    truckAxle = new GenericObject1();
    truckAxle->build(0.01 , 0.01, .2, 20, 0.4, 0.4, 0.2);

    truckBase = new GenericObject1();
    truckBase->build(.055 , .055, .05, 4, 0.4, 0.1, 0.2);

    deck = new Polygon();
    deck->build(0.65, 0.03 , 0.2, 0.65 , 0.03, 0.1, 0.1, 0, 0.2);


//    wheel_cf = glm::translate(glm::vec3{0.0f, 0.0f, -swingarm->length()});
//    wheel_cf *= glm::rotate(glm::radians(90.0f), glm::vec3{1,0,0});

//    frame.build();
//    frame_cf = glm::translate(glm::vec3{0, 0 , 25});
    active = &camera_cf;


    light0_cf = glm::translate(glm::vec3{-25, 8, 26});

    light1_cf = glm::translate(glm::vec3{0, -10, 18});
    light1_cf = light1_cf * glm::rotate (glm::radians(-120.0f), glm::vec3{1,0,0});
}

void keyCallback (GLFWwindow *win, int key, int scan_code, int action, int mods) {
    if (action == GLFW_RELEASE) return; /* ignore key release action */

    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_UP: /* tilt */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                break;
            case GLFW_KEY_DOWN: /* tilt */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{1.0f, 0.0f, 0.0f});
                break;
            case GLFW_KEY_LEFT: /* pan left */
                *active *= glm::rotate(glm::radians(-3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_RIGHT: /* pan right */
                *active *= glm::rotate(glm::radians(+3.0f), glm::vec3{0.0f, 1.0f, 0.0f});
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, 1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, 1});
                break;
            default:
                break;
        };

    }
    else {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                exit(0);
            case GLFW_KEY_0:
                active = &light0_cf;
                if (glIsEnabled(GL_LIGHT0))
                    glDisable(GL_LIGHT0);
                else
                    glEnable(GL_LIGHT0);
                break;
            case GLFW_KEY_1:
                active = &light1_cf;
                if (glIsEnabled(GL_LIGHT1))
                    glDisable(GL_LIGHT1);
                else
                    glEnable(GL_LIGHT1);
                break;

            case GLFW_KEY_SPACE: /* pause the animation */
                is_anim_running ^= true;
                break;
            case GLFW_KEY_C:
                active = &camera_cf;
                break;
            case GLFW_KEY_F:
                active = &board_cf;
                break;
            case GLFW_KEY_X:
                *active *= glm::translate(glm::vec3{-1, 0, 0});
                break;
            case GLFW_KEY_Y:
                *active *= glm::translate(glm::vec3{0, -1, 0});
                break;
            case GLFW_KEY_Z:
                *active *= glm::translate(glm::vec3{0, 0, -1});
                break;
        }
    }
}

int main (int argc, char **argv)
{

    glfwInit();
    GLFWwindow *win;
    win = glfwCreateWindow(100, 50, "Animation", NULL, NULL);

    glfwMakeContextCurrent(win);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n",
                glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    srand (time(0));

    myGLInit ();
    myModelInit ();

    /* setup display callback function */
    glfwSetFramebufferSizeCallback(win, reshapeCallback);
    glfwSetWindowRefreshCallback(win, displayCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetWindowSize(win, 800, 600);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        updateCoordFrames();
        displayCallback(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();

}

