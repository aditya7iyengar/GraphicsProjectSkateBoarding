
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
#include <sys/time.h>
#include <math.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>
#undef GLFW_DLL
#include <GLFW/glfw3.h>
#include "HalfPipe.h"
#include "Polygon.h"
#include "GenericObject2.h"
#include "GenericObject1.h"
using namespace std;

GenericObject2 wheels;
Polygon deck;
GenericObject1 truckBase;
GenericObject1 truckAxle;
HalfPipe pipe;
GenericObject2 base;

glm::mat4 wheel1_cf;
glm::mat4 wheel2_cf;
glm::mat4 wheel3_cf;
glm::mat4 wheel4_cf;
glm::mat4 board_cf, lambo_cf, boarder_cf;
glm::mat4 camera_cf, light1_cf, light0_cf;
glm::mat4 *active;

void win_refresh(GLFWwindow*);
void render_skateBoard();
float arc_ball_rad_square;
int screen_ctr_x, screen_ctr_y;
double overall_x = 0.0;
double overall_y = 0.0;
double angle = 0.0;
double down_angle = 90.0;
float SKATEBOARD_SPEED = 0.5; /* in units per second */

const float GRAVITY = 9.8;   /* m/sec^2 */
bool is_anim_running = true;
bool pos = true;

/* light source setting */
GLfloat light0_color[] = {1.0, 1.0, 1.0, 1.0};   /* color */
GLfloat light1_color[] = {1.0, 1.0, 0.6, 1.0};  /* color */
GLfloat black_color[] = {0.0, 0.0, 0.0, 1.0};   /* color */

/*================================================================*
 * Idle Callback function. This is the main engine for simulation *
 *================================================================*/
void simulate()
{
    float position = 0;
    static double last_timestamp = 0;

    const float SPEED_RAMP = 0.5/1;
    float delta, current;
    bool contact = true;

    current = glfwGetTime();
    if (is_anim_running) {
        if (overall_x >= -1.3 && overall_x <= 1.3) {
            delta = (current - last_timestamp);
            position = SKATEBOARD_SPEED * delta;
            board_cf *= glm::translate(glm::vec3{position, 0, 0});
            overall_x += position;
            cout << "overallx" << overall_x <<endl;
        }
        else {
            if (angle <= 90) {
                delta = (current - last_timestamp);
                if (SKATEBOARD_SPEED > 0)
                    board_cf *= glm::rotate(glm::radians(0.5f), glm::vec3{0.0f, 0.0f, 1.0f});
                else
                    board_cf *= glm::rotate(glm::radians(0.5f), glm::vec3{0.0f, 0.0f, -1.0f});
                position = SKATEBOARD_SPEED * delta;
                board_cf *= glm::translate(glm::vec3{position / 1.6, 0, 0});
                angle += 0.5;
                overall_y = 0.5;
            }
            else {
                if ( overall_y >= 0.5) {
                    down_angle = 90.0;
                    delta = (current - last_timestamp);
                    if (pos) {
                        SKATEBOARD_SPEED -= (0.098 * delta);
                        position = SKATEBOARD_SPEED * delta;
                        board_cf *= glm::translate(glm::vec3{position, 0, 0});
                        overall_y += position;
                    }
                    else{
                        SKATEBOARD_SPEED += (0.098 * delta);
                        position = SKATEBOARD_SPEED * delta;
                        board_cf *= glm::translate(glm::vec3{-position, 0, 0});
                        overall_y -= position;
                    }
                }
                else {
                    if (down_angle > 0) {
                        if (SKATEBOARD_SPEED < 0)
                            board_cf *= glm::rotate(glm::radians(0.9f), glm::vec3{0.0f, 0.0f, -1.0f});
                        else
                            board_cf *= glm::rotate(glm::radians(0.9f), glm::vec3{0.0f, 0.0f, 1.0f});
                        delta = (current - last_timestamp);
                        position = SKATEBOARD_SPEED * delta;
                        board_cf *= glm::translate(glm::vec3{position, 0, 0});
                        down_angle -= 0.9;
                    }
                    else {
                        overall_y = 0;
                        angle = 0;
                        if (SKATEBOARD_SPEED < 0) {
                            SKATEBOARD_SPEED = -0.5;
                            overall_x = 1.3;
                            pos = false;
                        }
                        else{
                            SKATEBOARD_SPEED = 0.5;
                            overall_x= -1.3;
                            pos = true;
                        }
                    }
                }
            }
        }

    }
    last_timestamp = current;
}

void err_function (int what, const char *msg) {
    cerr << what << " " << msg << endl;
}
void win_resize (GLFWwindow * win, int width, int height)
{
#ifdef DEBUG
    cout << __FUNCTION__ << " " << width << "x" << height << endl;
#endif
    int w, h;
    glfwGetWindowSize(win, &w, &h);
    screen_ctr_x = w / 2.0;
    screen_ctr_y = h / 2.0;
    float rad = min(h,w)/2;
    arc_ball_rad_square = rad * rad;
    /* Use the entire window for our view port */
    glViewport(0, 0, width, height);
    /* Use GL_PROJECTION to select the type of synthetic camera */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* near-plane(1) & far-plane(10) are always POSITIVE and they measure
     * the distances along the Z-axis in front of the camera */
    gluPerspective(60.0, static_cast<float> (width)/ static_cast<float> (height), 1, 100);
}

void render_skateBoard(){
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(board_cf));

    glPushMatrix();
    glTranslatef(0, 0.07, 0);
    deck.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.55,0,0);
    glPushMatrix();
    glTranslatef(0.3,0 ,0.1);
    wheels.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3,0 , -0.1);
    glRotatef(180, 1, 0, 0);
    wheels.render();
    glPopMatrix();

    /*Back Trucks and Wheels*/
    glPushMatrix();
    glTranslatef(0.2, 0, 0);

    glPushMatrix();
    glTranslatef(0.60,0 , 0);

    wheels.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.60,0 , -0.1);
    glRotatef(180, 1, 0, 0);
    wheels.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.60,0 , 0);
    truckAxle.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6,0.02 , 0);
    glRotatef(45, 0, 0, 1);
    truckBase.render();
    glPopMatrix();

    glPopMatrix();
    /*Back Trucks and Wheels End*/


    glPushMatrix();
    glTranslatef(0.3,0 , 0);
    //glRotatef(90, 1, 0, 0);
    truckAxle.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3,0.02 , 0);
    glRotatef(45, 0, 0, 1);
    truckBase.render();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

}
void render_HalfPipe (){
    glPushMatrix();
    glTranslatef(1.3, 0, 0);
    pipe.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3, 0, 0);
    glRotatef(180, 0, 1, 0);
    pipe.render();
    glPopMatrix();
}
void win_refresh (GLFWwindow *win) {
//    cout << __PRETTY_FUNCTION__ << endl;
    glClearColor(0, 0.7, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    /* place the camera using the camera coordinate frame */
    glMultMatrixf (glm::value_ptr(camera_cf));

    const float& S = 1;
    /* draw the axes */
    glBegin(GL_LINES);
    glColor3ub (255, 0, 0);
    glVertex2i (0, 0);
    glVertex2f (S * 1.1, 0);
    glColor3ub (0, 255, 0);
    glVertex2i (0, 0);
    glVertex2f (0, S * 1.1);
    glColor3ub (0, 0, 255);
    glVertex2i (0, 0);
    glVertex3f (0, 0, S * 1.1);
    glEnd();

    /* The following two groups of GL_LINE_LOOP and GL_LINES draw the square block
     * whose 4 vertices make the tetrahedron */

    //enable_contents();
    /* Half Pipe */
    render_HalfPipe();
    /* HalfPipe ends */

    /*SkateBoard Start*/
    render_skateBoard();
    /*SkateBoard End*/

    /* must swap buffer at the end of render function */
    glfwSwapBuffers(win);
}



/* action: GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT */
void key_handler (GLFWwindow *win, int key, int scan_code, int action, int mods)
{
    const float FIRST_VIEW = 0.5f;
    const float SECOND_VIEW = 1.5f;
    glm::vec3 N;
    float theta;
    glm::quat q;
    static glm::mat4 current_cam;
    glm::vec3 this_vec;
    cout << __FUNCTION__ << endl;
    if (action != GLFW_PRESS) return;
    if (mods == GLFW_MOD_SHIFT) {
        switch (key) {
            case GLFW_KEY_D: /* Uppercase 'D' */
                /* pre mult: trans  Z-ax of the world */
//                hex1_cf = glm::translate(glm::vec3{0, +0.5f, 0}) * hex1_cf;
                break;
        }
    }
    else {
        switch (key) {
            case GLFW_KEY_W:
                glPolygonMode(GL_FRONT, GL_LINE);
                break;
            case GLFW_KEY_C:
                camera_cf = camera_cf * glm::translate(glm::vec3{-5, -5, 5});
                break;
            case GLFW_KEY_S:
                camera_cf = board_cf* glm::translate(glm::vec3{0, 0, -2});
                camera_cf *= glm::rotate(1.57f, glm::vec3{1,0,0});
                break;
            case GLFW_KEY_SPACE:
                is_anim_running ^= true;
                break;

        }
    }
    win_refresh(win);
}

/*
    The virtual trackball technique implemented here is based on:
    https://www.opengl.org/wiki/Object_Mouse_Trackball
*/
void cursor_handler (GLFWwindow *win, double xpos, double ypos) {
    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    static glm::vec3 first_click;
    static glm::mat4 current_cam;
    static bool is_tracking = false;

    glm::vec3 this_vec;
    if (state == GLFW_PRESS) {
        /* TODO: use glUnproject? */
        float x = (xpos - screen_ctr_x);
        float y = -(ypos - screen_ctr_y);
        float hypot_square = x * x + y * y;
        float z;

        /* determine whether the mouse is on the sphere or on the hyperbolic sheet */
        if (2 * hypot_square < arc_ball_rad_square)
            z = sqrt(arc_ball_rad_square - hypot_square);
        else
            z = arc_ball_rad_square / 2.0 / sqrt(hypot_square);
        if (!is_tracking) {
            /* store the mouse position when the button was pressed for the first time */
            first_click = glm::normalize(glm::vec3{x, y, z});
            current_cam = camera_cf;
            is_tracking = true;
        }
        else {
            /* compute the rotation w.r.t the initial click */
            this_vec = glm::normalize(glm::vec3{x, y, z});
            /* determine axis of rotation */
            glm::vec3 N = glm::cross(first_click, this_vec);

            /* determine the angle of rotation */
            float theta = glm::angle(first_click, this_vec);

            /* create a quaternion of the rotation */
            glm::quat q{cos(theta / 2), sin(theta / 2) * N};
            /* apply the rotation w.r.t to the current camera CF */
            camera_cf = current_cam * glm::toMat4(glm::normalize(q));
        }
        win_refresh(win);
    }
    else {
        is_tracking = false;
    }
}

void scroll_handler (GLFWwindow *win, double xscroll, double yscroll) {
    /* translate along the camera Z-axis */
    glm::mat4 z_translate = glm::translate((float)yscroll * glm::vec3{0, 0, 1});
    camera_cf =  z_translate * camera_cf;
    win_refresh(win);

}

void init_gl() {
    glEnable (GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_CULL_FACE);
    glLineWidth(3.0);

    /* place the camera at Z=+5 (notice that the sign is OPPOSITE!) */
    camera_cf *= glm::translate(glm::vec3{0, 0, -5});
}

void make_model() {
    board_cf = glm::translate( glm::vec3{0,-0.95,0});
    pipe.build(0.5, 3, 1, 0.6, 0.6, 0.6, 0.02);
    base.build(3.0/1.414, 0, 3.0/1.414, 0, 0, 4, 0.6, 0.6, 0.6, 0.02);

    wheels.build(0.045, 0.01, 0.045, 0.04, 0.04, 30, 0.9, 0.3, 0.4, 1);
    truckAxle.build(0.01 , 0.01, .2, 20, 0.4, 0.4, 0.2);
    truckBase.build(.055 , .055, .05, 4, 0.4, 0.1, 0.2);
    deck.build(0.65, 0.03 , 0.2, 0.65 , 0.03, 0.1, 0.1, 0, 0.2);
}

int main() {

    if(!glfwInit()) {
        cerr << "Can't initialize GLFW" << endl;
        glfwTerminate();
        exit (EXIT_FAILURE);
    }

    GLFWwindow * win;
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    win = glfwCreateWindow(50, 50, "BoarderAnimation", NULL, NULL);

    glfwSetWindowRefreshCallback(win, win_refresh);
    /* On Mac with Retina display there is a discrepancy between units measured in
     * "screen coordinates" and "pixels" */
    glfwSetWindowSizeCallback(win, win_resize);  /* use this for non-retina displays */
    glfwSetFramebufferSizeCallback(win, win_resize); /* use this for retina displays */
    glfwSetKeyCallback(win, key_handler);
    glfwSetCursorPosCallback(win, cursor_handler);
    glfwSetScrollCallback(win, scroll_handler);
    glfwMakeContextCurrent(win);

    /* glewInit must be invoked AFTER glfwMakeContextCurrent() */
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf (stderr, "GLEW init error %s\n", glewGetErrorString(err));
        exit (EXIT_FAILURE);
    }

    /* GL functions can be called, only AFTER the window is created */
    const GLubyte *version = glGetString (GL_VERSION);
    printf ("GL Version is %s\n", version);


    glfwSetWindowSize(win, 800, 800);
    glfwSwapInterval(1);
    init_gl();
    make_model();

    win_refresh(win);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        simulate();
        win_refresh(win);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
