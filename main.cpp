
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
#include <OpenGL/OpenGL.h>
#include "HalfPipe.h"
#include "Polygon.h"
#include "GenericObject2.h"
#include "GenericObject1.h"
#include "Shapes/Cylinder.h"
#include "Shapes/Sphere.h"
using namespace std;

GenericObject2 wheels;
Polygon deck;
GenericObject1 truckBase;
GenericObject1 truckAxle;
HalfPipe pipe;
GenericObject2 base;
GenericObject1 body;
GenericObject1 back_arm;
GenericObject1 fore_arm;
GenericObject1 thigh;
GenericObject1 leg;
Polygon foot;
Polygon head;
Cylinder spot;
Sphere sphere;
//gluDisk floor;
Polygon temp_floor;
Polygon ramp;
Polygon block;
Polygon stair1;
Polygon stair2;
Polygon stair3;
Polygon stair4;
GLUquadric *rails  = gluNewQuadric();

static float COPPER_AMBIENT[] = {0.191250, 0.073500, 0.022500, 1.000000};
static float COPPER_DIFFUSE[] = {0.703800, 0.270480, 0.082800, 1.000000};
static float COPPER_SPECULAR[] = {0.256777, 0.137622, 0.086014, 1.000000};

static float SILVER_AMBIENT[] = {0.192250, 0.192250, 0.192250, 1.000000};
static float SILVER_DIFFUSE[] = {0.507540, 0.507540, 0.507540, 1.000000};
static float SILVER_SPECULAR[] = {0.508273, 0.508273, 0.508273, 1.000000};

static float EMERALD_AMBIENT[] = {0.021500, 0.174500, 0.021500, 0.550000};
static float EMERALD_DIFFUSE[] = {0.075680, 0.614240, 0.075680, 0.550000};
static float EMERALD_SPECULAR[] = {0.633000, 0.727811, 0.633000, 0.550000};

static float BLACKPLASTIC_AMBIENT[] = {0.000000, 0.000000, 0.000000, 1.000000};
static float BLACKPLASTIC_DIFFUSE[] = {0.010000, 0.010000, 0.010000, 1.000000};
static float BLACKPLASTIC_SPECULAR[] = {0.500000, 0.500000, 0.500000, 1.000000};

static float RUBY_AMBIENT[] = {0.174500, 0.011750, 0.011750, 0.550000};
static float RUBY_DIFFUSE[] = {0.614240, 0.041360, 0.041360, 0.550000};
static float RUBY_SPECULAR[] = {0.727811, 0.626959, 0.626959, 0.550000};

static float BLACKRUBBER_AMBIENT[] = {0.020000, 0.020000, 0.020000, 1.000000};
static float BLACKRUBBER_DIFFUSE[] = {0.010000, 0.010000, 0.010000, 1.000000};
static float BLACKRUBBER_SPECULAR[] = {00.400000, 0.400000, 0.400000, 1.000000};

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
double board_x = 16.0;
double  board_y = 0.0;
double board_z = 0.0;
float radians = -M_PI/2;
float sec_angle = M_PI/50;
float total_spin = 0.0;
bool go_forward = false;
float done_angle = 0.0;
float proj_speed_x = 0.0;
float proj_speed_y = 0.0;
float rail_angle = M_PI/2;
float SKATEBOARD_SPEED = 2.6; /* in units per second */

const float GRAVITY = .9;   /* m/sec^2 */
bool is_anim_running = true;
bool pos = true;
bool jumped = false;
float jump_angle = 0.0;
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
    float delta, current;

    current = glfwGetTime();
    if (is_anim_running) {
        if (board_x > 3.8){
            delta = (current - last_timestamp);
            if (board_x >= 11.8) {
                delta = (current - last_timestamp);
                board_cf *= glm::translate(glm::vec3{-2.6 * delta, 0, 0});
                boarder_cf *= glm::translate(glm::vec3{-2.6 * delta, 0, 0});
                board_x -= 2.6 * delta;
            }
            else {
                if (board_y < 0.5) {
                    board_x += SKATEBOARD_SPEED * delta * cos(33.7);
                    board_y += SKATEBOARD_SPEED * delta * sin(33.7);
                    SKATEBOARD_SPEED -= GRAVITY * delta * sin (33.7);
                    board_cf = glm::translate(glm::vec3{board_x-1,board_y,0});
                    boarder_cf = glm::translate(glm::vec3{board_x-1,board_y+.4,0});
                    float angle_ramp = 0.588;
                    board_cf *= glm::rotate(angle_ramp, glm::vec3{0.0, 0.0, -1.0});
                    boarder_cf *= glm::rotate(angle_ramp, glm::vec3{0.0, 0.0, -1.0});
                    proj_speed_y = SKATEBOARD_SPEED * sin(33.7);
                    proj_speed_x = SKATEBOARD_SPEED * cos(33.7);
                }
                else {
                    proj_speed_y -= delta * GRAVITY/1.2;
                    board_y += proj_speed_y * delta;
                    board_x += proj_speed_x * delta;
                    board_cf = glm::translate(glm::vec3{board_x-1,board_y,0});
                    boarder_cf = glm::translate(glm::vec3{board_x-1,board_y+.4,0});
                    cout << "board_x" << board_x << endl;
                    cout << "board_y" << board_y << endl;
                    if (proj_speed_y < 0){
                        SKATEBOARD_SPEED = -2.6;
                        radians = 0.0;
                    }
                }

            }
        }
        else if ( board_x>= -1.3 && board_x <= 1.3) {
            if (SKATEBOARD_SPEED > 0)
                SKATEBOARD_SPEED = 2.6;
            else
                SKATEBOARD_SPEED = -2.6;
            delta = (current - last_timestamp);
            position = SKATEBOARD_SPEED * delta;
            board_cf *= glm::translate(glm::vec3{position, 0, 0});
            boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
            board_x += position;
            total_spin = 0.0;
            cout << "board_x" << board_x << endl;
        }
        else if(board_x > 0){
            delta = (current - last_timestamp);
            SKATEBOARD_SPEED -= GRAVITY*delta;
            position = SKATEBOARD_SPEED*delta;
           if (SKATEBOARD_SPEED > 0){
               if (radians >= 0){
                   board_cf *= glm::translate(glm::vec3{position, 0, 0});
                   boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
                   board_y += position;
                   if ( total_spin <= 2* M_PI) {
                       board_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                       boarder_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                       total_spin += sec_angle;
                   }
                   if (board_y >= 1.5){

                   }
                   cout << "board_y" << board_x << endl;
               }
               else{
                   radians += SKATEBOARD_SPEED*delta;
                   board_cf = glm::translate(glm::vec3{1.2+ cos(radians), sin(radians), 0});
                   boarder_cf = glm::translate(glm::vec3{1.2+ cos(radians), sin(radians)+0.4, 0});
                   float angle = radians + M_PI/2;
                   board_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,1.0f});
                   board_cf *= glm::translate( glm::vec3{0,0.05,0});
                   boarder_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,1.0f});
                   boarder_cf *= glm::translate( glm::vec3{0,0.05,0});
                   //board_cf *= glm::rotate(glm::radians(0.5f), glm::vec3{0.0f, 0.0f, 1.0f});
                   board_y += position;
                   cout << "radians" << radians << endl;
               }
           }
           else{
               if (board_y >=1.5){
                   board_cf *= glm::translate(glm::vec3{position, 0, 0});
                   boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
                   if ( total_spin <= 2* M_PI) {
                       board_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                       boarder_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                       total_spin += sec_angle;
                   }
                   if (board_y >= 1.5){

                   }
                   board_y += position;
                   cout << "board_y" << board_x << endl;
               }
               else {
                   radians += SKATEBOARD_SPEED*delta;
                   board_cf = glm::translate(glm::vec3{1.2+ cos(radians), sin(radians), 0});
                   boarder_cf = glm::translate(glm::vec3{1.2+ cos(radians), sin(radians)+0.4, 0});
                   float angle = radians + M_PI/2;
                   board_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,1.0f});
                   boarder_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,1.0f});
                   board_y = 0;
                   if  (radians <= -M_PI/2){
                        board_x = 1.29;
                        board_y = 0.0;
                        board_cf = glm::translate(glm::vec3{1.29, -0.95, 0});
                        boarder_cf = glm::translate(glm::vec3{1.29, -0.5, 0});
                   }
                   cout << "radians" << radians << endl;
               }
           }
        }
        else if (!go_forward){
            delta = (current - last_timestamp);
            SKATEBOARD_SPEED += GRAVITY*delta;
            position = SKATEBOARD_SPEED*delta;
            if (SKATEBOARD_SPEED < 0){
                if (radians >= 0){
                    board_cf *= glm::translate(glm::vec3{position, 0, 0});
                    boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
                    board_y -= position;
                    if ( total_spin <= 2* M_PI) {
                        board_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                        boarder_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                        total_spin += sec_angle;
                    }
                    if (board_y >= 1.5){

                    }
                    cout << "board_y" << board_y << endl;
                }
                else{
                    radians -= SKATEBOARD_SPEED*delta;
                    board_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians), 0});
                    boarder_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians)+0.4, 0});
                    float angle = radians + M_PI/2;
                    board_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                    boarder_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                    //board_cf *= glm::rotate(glm::radians(0.5f), glm::vec3{0.0f, 0.0f, 1.0f});
                    board_y -= position;
                    cout << "radians" << radians << endl;
                }
            }
            else{
                if (board_y >=1.5){
                    board_cf *= glm::translate(glm::vec3{position, 0, 0});
                    boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
                    board_y -= position;
                    if ( total_spin <= 2* M_PI) {
                        board_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                        boarder_cf *= glm::rotate(sec_angle, glm::vec3{1.0, 0.0, 0.0});
                        total_spin += sec_angle;
                    }
                    if (board_y >= 1.5){

                    }
                    cout << "board_y" << board_x << endl;
                }
                else {
                    radians -= SKATEBOARD_SPEED*delta;
                    board_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians), 0});
                    boarder_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians)+0.4, 0});
                    float angle = radians + M_PI/2;
                    board_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                    boarder_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                    board_y -= position;
                    if  (radians <= -M_PI/2){
                        board_x = -1.29;
                        board_y = 0.0;
                        board_cf = glm::translate(glm::vec3{-1.29, -0.95, 0});
                        boarder_cf = glm::translate(glm::vec3{-1.29, -0.5, 0});
                    }
                }
            }
        }
        else if (done_angle < M_PI/2){
            delta = (current - last_timestamp);
            SKATEBOARD_SPEED += GRAVITY*delta;
            position = SKATEBOARD_SPEED*delta;
            if (radians >= 0){
                if (board_y <= 1.0) {
                    board_cf = glm::translate(glm::vec3{-2.5, 0.05, 0});
                    boarder_cf = glm::translate(glm::vec3{-2.5, 0.45, 0});
                    board_y = 1.0;
                }
                board_cf *= glm::rotate(sec_angle, glm::vec3{0.0, 0.0, 1.0});
                boarder_cf *= glm::rotate(sec_angle, glm::vec3{0.0, 0.0, 1.0});
                done_angle += sec_angle;
            }
            else{
                radians -= SKATEBOARD_SPEED*delta;
                board_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians), 0});
                boarder_cf = glm::translate(glm::vec3{-1.2- cos(radians), sin(radians)+0.4, 0});
                float angle = radians + M_PI/2;
                board_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                boarder_cf *= glm::rotate((angle), glm::vec3{0.0,0.0,-1.0f});
                //board_cf *= glm::rotate(glm::radians(0.5f), glm::vec3{0.0f, 0.0f, 1.0f});
                board_y -= position;
                cout << "radians" << radians << endl;
            }
        }
        else if (board_x > -5.3) {
            if (rail_angle != 0.0) {
                board_cf = glm::translate(glm::vec3{-2.5, 0.05, 0});
                boarder_cf = glm::translate(glm::vec3{-2.5, 0.45, 0});
                board_x = -2.5;
                rail_angle = 0.0;
            }
            else {
                delta = (current - last_timestamp);
                position = SKATEBOARD_SPEED * delta;
                board_cf *= glm::translate(glm::vec3{position, 0, 0});
                boarder_cf *= glm::translate(glm::vec3{position, 0, 0});
                board_x += position;
                jumped = false;
                jump_angle = 0.0;
            }
        }
        else{
            delta = (current - last_timestamp);
            position = SKATEBOARD_SPEED * delta;
            if (!jumped){
                board_cf *= glm::translate(glm::vec3{position, -position/3 ,0});
                board_cf *= glm::rotate(sec_angle, glm::vec3{0.0, 1.0, 0.0});
                jump_angle +=sec_angle;
                if (jump_angle >= M_PI/2) {
                    jumped = true;
                    board_cf = glm::translate(glm::vec3{-6,0.5,0});
                    float pi_ov_four = M_PI/4;
                    board_cf *= glm::rotate(pi_ov_four, glm::vec3{0,0,1.0});
                    board_cf *= glm::rotate(2*pi_ov_four, glm::vec3{0,1.0,0});
                    board_x = -6;
                    SKATEBOARD_SPEED = -2.6;
                }
            }
            else if (board_x > -7){
                SKATEBOARD_SPEED += delta * GRAVITY;
                board_cf *= glm::translate(glm::vec3{0,0,position});
                board_x += position*cos(M_PI/4);
                if (board_x <= -7){
                    board_cf = glm::translate(glm::vec3{-8.2,-0.95,0});
                    board_x = -8.2;
                }
            }

            else {
                if ( board_x > - 10){
                    SKATEBOARD_SPEED += delta * GRAVITY;
                    board_cf *= glm::translate(glm::vec3{position,0,0});
                    board_x += position;
                }
                else {
                    board_x = 16.0;
                    board_y = 0.0;

                    radians = -M_PI/2;
                    sec_angle = M_PI/50;
                    total_spin = 0.0;
                    go_forward = false;
                    done_angle = 0.0;
                    proj_speed_x = 0.0;
                    proj_speed_y = 0.0;
                    rail_angle = M_PI/2;
                    SKATEBOARD_SPEED = 2.6; /* in units per second */
                    jumped = false;
                    jump_angle = 0.0;
                    board_cf = glm::translate( glm::vec3{16,-0.95,0});
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

    glMaterialfv(GL_FRONT, GL_AMBIENT, RUBY_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, RUBY_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, RUBY_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

    glPushMatrix();
    glTranslatef(0, 0.07, 0);
    deck.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, COPPER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, COPPER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, COPPER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.0);

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
    glTranslatef(0.60,0 , 0.1);

    wheels.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.60,0 , -0.1);
    glRotatef(180, 1, 0, 0);
    wheels.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);

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

void render_boarder(){
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(boarder_cf));

    glMaterialfv(GL_FRONT, GL_AMBIENT, BLACKPLASTIC_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKPLASTIC_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR,BLACKPLASTIC_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);

    glPushMatrix();
    glRotatef(90,-1,0,0);

    body.render();

    glMaterialfv(GL_FRONT, GL_AMBIENT, COPPER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, COPPER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR,COPPER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

    glPushMatrix();
    glTranslatef(-0.12,0,0.05);
    glRotatef(30,0,1,0);
    back_arm.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.12,0,0.05);
    glRotatef(30,0,-1,0);
    back_arm.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, EMERALD_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, EMERALD_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR,EMERALD_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 80.0);

    glPushMatrix();
    glTranslatef(0.06,0,-0.24);
    glRotatef(10,0,-1,0);
    back_arm.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.06,0,-0.24);
    glRotatef(10,0,1,0);
    back_arm.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, COPPER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, COPPER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR,COPPER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 30.0);

    glPushMatrix();
    glTranslatef(0.0,0.0,0.2);
    head.render();
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

    glPushMatrix();
    glTranslatef(-4.3,-0.5,0);
    block.render();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.95,-0.6,0);
    stair1.render();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-6.2,-0.7,0);
    stair2.render();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-6.45,-0.8,0);
    stair3.render();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-6.7,-0.9,0);
    stair4.render();
    glPopMatrix();

}


void win_refresh (GLFWwindow *win) {
//    cout << __PRETTY_FUNCTION__ << endl;
    glClearColor(0, 0.0, 0.0, 1);
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

<<<<<<< HEAD
    glDisable (GL_COLOR_MATERIAL);
=======
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, -1.001);
    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glColor3ub (29, 100, 56);

    glBegin (GL_QUADS);
    const int GROUND_SIZE = 10;
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
    glPopMatrix();

>>>>>>> origin/master
    /* The following two groups of GL_LINE_LOOP and GL_LINES draw the square block
     * whose 4 vertices make the tetrahedron */

    //enable_contents();

    /* place the light source in the scene. */
    glLightfv (GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::column(light0_cf, 3)));

    /* recall that the last column of a CF is the origin of the CF */
    glLightfv (GL_LIGHT1, GL_POSITION, glm::value_ptr(glm::column(light1_cf, 3)));

    /* we use the Z-axis of the light CF as the spotlight direction */
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::column(light1_cf, 2)));

    glPushMatrix();
    {
        glMultMatrixf(glm::value_ptr(light0_cf));

        /* Render light-0 as an emmisive object */
        if (glIsEnabled(GL_LIGHT0))
            glMaterialfv(GL_FRONT, GL_EMISSION, light0_color);
        sphere.render();
        glMaterialfv(GL_FRONT, GL_EMISSION, black_color);
    }
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 1.0);

    glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(0,-15,1.05);
    temp_floor.render();
    glPopMatrix();


    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 80.0);

    glPushMatrix();
    glMultMatrixf(glm::value_ptr(light1_cf));
    spot.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0);

    glPushMatrix();
    glRotatef(90,0,1,0);
    glRotatef(-90,1,0,0);
    glTranslatef(0,-10,-0.5);
    ramp.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0);

    glPushMatrix();
    glTranslatef(4, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0);

    glPushMatrix();
    glTranslatef(7, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0);

    glPushMatrix();
    glTranslatef(-8.5, -1, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
    base.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, SILVER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SILVER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SILVER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0);

    /* Half Pipe */
    render_HalfPipe();
    /* HalfPipe ends */

    /*SkateBoard Start*/
    render_skateBoard();
    /*SkateBoard End*/

    boarder_cf = board_cf * glm::translate(glm::vec3{0,.45,0});
    render_boarder();

    glMaterialfv(GL_FRONT, GL_AMBIENT, BLACKRUBBER_AMBIENT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKRUBBER_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, BLACKRUBBER_SPECULAR);
    glMaterialf(GL_FRONT, GL_SHININESS, 80.0);

    glPushMatrix();
    glTranslatef(-7.5,-1.0,0);
    glRotatef(90,0,1,0);
    glRotatef(-45,1,0,0);
    gluCylinder(rails,0.05,0.05,2.2,10,10);
    glTranslatef(-1,0,0);
    gluCylinder(rails,0.05,0.05,2.2,10,10);
    glTranslatef(2,0,0);
    gluCylinder(rails,0.05,0.05,2.2,10,10);
    glPopMatrix();
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
<<<<<<< HEAD
//            case GLFW_KEY_SPACE:
//                glPolygonMode(GL_FRONT, GL_LINE);
//                break;
//            case GLFW_KEY_C:
//                camera_cf = camera_cf * glm::translate(glm::vec3{-5, -5, 5});
//                break;
//            case GLFW_KEY_S:
//                camera_cf = board_cf* glm::translate(glm::vec3{0, 0, -2});
//                camera_cf *= glm::rotate(1.57f, glm::vec3{1,0,0});
//                break
            case GLFW_KEY_ESCAPE:
                exit(0);
            case GLFW_KEY_0:
=======
            case GLFW_KEY_1:
>>>>>>> origin/master
                active = &light0_cf;
                if (glIsEnabled(GL_LIGHT0))
                    glDisable(GL_LIGHT0);
                else
                    glEnable(GL_LIGHT0);
                break;
<<<<<<< HEAD
            case GLFW_KEY_1:
=======
            case GLFW_KEY_2:
>>>>>>> origin/master
                active = &light1_cf;
                if (glIsEnabled(GL_LIGHT1))
                    glDisable(GL_LIGHT1);
                else
                    glEnable(GL_LIGHT1);
                break;
<<<<<<< HEAD
            case GLFW_KEY_DOWN:
                SKATEBOARD_SPEED -= 0.1;
                break;
            case GLFW_KEY_UP:
                SKATEBOARD_SPEED += 0.1;
                break;
            case GLFW_KEY_BACKSPACE:
=======
            case GLFW_KEY_W:
>>>>>>> origin/master
                glPolygonMode(GL_FRONT, GL_LINE);
                break;
            case GLFW_KEY_F:
                go_forward = true;
                break;
            case GLFW_KEY_ENTER:
                is_anim_running ^= true;
                break;
            case GLFW_KEY_L:
                camera_cf *= glm::translate(glm::vec3{-1,0,0});
                break;
            case GLFW_KEY_J:
                camera_cf *= glm::translate(glm::vec3{1,0,0});
                break;
            case GLFW_KEY_I:
                camera_cf *= glm::translate(glm::vec3{0,1,0});
                break;
            case GLFW_KEY_K:
                camera_cf *= glm::translate(glm::vec3{0,-1,0});
                break;
            case GLFW_KEY_M:
                camera_cf *= glm::translate(glm::vec3{0,0,1});
                break;
            case GLFW_KEY_N:
                camera_cf *= glm::translate(glm::vec3{0,0,-1});
                break;
            case GLFW_KEY_Z:
                camera_cf *= glm::rotate(0.314f, glm::vec3{1,0,0});
                break;
            case GLFW_KEY_X:
                camera_cf *= glm::rotate(0.314f, glm::vec3{0,1,0});
                break;
            case GLFW_KEY_C:
                camera_cf *= glm::rotate(0.314f, glm::vec3{0,0,1});
                break;
            case GLFW_KEY_8:
                camera_cf = glm::translate(glm::vec3{0, 0, -5});
                light0_cf = glm::translate(glm::vec3{10, 10, 10});
                light1_cf = glm::translate(glm::vec3{-10, 10, 10});
                light1_cf = light1_cf * glm::rotate (glm::radians(120.0f), glm::vec3{1,0,0});
                break;
            case GLFW_KEY_2:
                light0_cf *= glm::translate(glm::vec3{1,0,0});
                break;
            case GLFW_KEY_3:
                light0_cf  *= glm::translate(glm::vec3{0,1,0});
                break;
            case GLFW_KEY_4:
                light0_cf  *= glm::translate(glm::vec3{0,0,1});
                break;
            case GLFW_KEY_5:
                light1_cf *= glm::translate(glm::vec3{1,0,0});
                break;
            case GLFW_KEY_6:
                light1_cf  *= glm::translate(glm::vec3{0,1,0});
                break;
            case GLFW_KEY_7:
                light1_cf  *= glm::translate(glm::vec3{0,0,1});
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

    /* Enable shading */
    glEnable (GL_LIGHTING);
<<<<<<< HEAD
    glEnable (GL_NORMALIZE); /* Tell OpenGL to renormalize normal vector
                              after transformation */
=======
    glEnable (GL_NORMALIZE);
    // /* Tell OpenGL to renormalize normal vector after transformation */
>>>>>>> origin/master
    /* initialize two light sources */
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT0, GL_AMBIENT, light0_color);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light0_color);
    glEnable (GL_LIGHT1);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_color);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_color);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_color);
<<<<<<< HEAD
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 40);

    //glEnableClientState(GL_VERTEX_ARRAY);
=======
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 10);
>>>>>>> origin/master

    /* place the camera at Z=+5 (notice that the sign is OPPOSITE!) */
    camera_cf *= glm::translate(glm::vec3{0, 0, -5});
    light0_cf = glm::translate(glm::vec3{10, 10, 10});
    light1_cf = glm::translate(glm::vec3{-10, 10, 10});
    light1_cf = light1_cf * glm::rotate (glm::radians(120.0f), glm::vec3{1,0,0});
}

void make_model() {
    sphere.build(15, 20);
    spot.build(1 + tan(glm::radians(40.0f)), 1, 2);

    boarder_cf = glm::translate(glm::vec3{16,-0.5,0});
    board_cf = glm::translate( glm::vec3{16,-0.95,0});
    temp_floor.build(40,40,0.01,40,40);
    ramp.build(3,0.5,1,3,3);
    pipe.build(0.5, 3, 1);
    base.build(3.0/1.414, 0, 3.0/1.414, 0, 0, 4);

    wheels.build(0.045, 0.01, 0.045, 0.04, 0.04, 30);
    truckAxle.build(0.01 , 0.01, .2, 20);
    truckBase.build(.055 , .055, .05, 4);
    deck.build(0.65, 0.03 , 0.2, 0.65 , 0.03);
    block.build(3,1,3,3,1);
    stair1.build(0.5,0.8,3,0.5,0.8);
    stair2.build(0.5,0.6,3,0.5,0.6);
    stair3.build(0.5,0.4,3,0.5,0.4);
    stair4.build(0.5,0.2,3,0.5,0.2);


    body.build(0.12,0.1,0.30,20);
    head.build(0.16,0.16,0.16,0.10,0.10);
    back_arm.build(0.06,0.04,0.20,20);

    gluQuadricOrientation (rails, GLU_OUTSIDE);
    gluQuadricNormals (rails, GLU_SMOOTH);


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
