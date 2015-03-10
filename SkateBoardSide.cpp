#include "SkateBoardSide.h"
#include <cmath>

SkateBoardSide::~SkateBoardSide(){
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &color_buffer);
}

void SkateBoardSide::build(float length,float breadth, float height,
        float clr1, float clr2, float clr3, float ref_quotient){

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);

    num_divisions = 10;

    float increment_angle = M_PI/(2*num_divisions);

    float temp_angle = 0.0f;

    for (int k =0; k < num_divisions; k++){
        float x_co = height * cos(temp_angle);
        float y_co = -height * sin(temp_angle);

        temp_angle += increment_angle;

        vertices.push_back(x_co);
        vertices.push_back(y_co);
        vertices.push_back(breadth/2);
        color.push_back (clr1 -= ref_quotient);
        color.push_back (clr2 -= ref_quotient);
        color.push_back (clr3 -= ref_quotient);


        vertices.push_back(x_co);
        vertices.push_back(-height);
        vertices.push_back(breadth/2);
        color.push_back (clr1);
        color.push_back (clr2);
        color.push_back (clr3);


    }

    vertices.push_back(height + length_up);
    vertices.push_back(0);
    vertices.push_back(breadth/2);
    color.push_back (clr1 -= ref_quotient);
    color.push_back (clr2 -= ref_quotient);
    color.push_back (clr3 -= ref_quotient);

    vertices.push_back(height + length_up);
    vertices.push_back(-height);
    vertices.push_back(breadth/2);
    color.push_back (clr1 -= ref_quotient);
    color.push_back (clr2 -= ref_quotient);
    color.push_back (clr3 -= ref_quotient);



    temp_angle = 0.0f;
    clr1 += (num_divisions*2+2)*ref_quotient;
    clr2 += (num_divisions*2+2)*ref_quotient;
    clr3 += (num_divisions*2+2)*ref_quotient;
    for (int k =0; k < num_divisions; k++){
        float x_co = height * cos(temp_angle);
        float y_co = -height * sin(temp_angle);

        temp_angle += increment_angle;

        vertices.push_back(x_co);
        vertices.push_back(y_co);
        vertices.push_back(-breadth/2);
        color.push_back (clr1 -= ref_quotient);
        color.push_back (clr2 -= ref_quotient);
        color.push_back (clr3 -= ref_quotient);

        vertices.push_back(x_co);
        vertices.push_back(-height);
        vertices.push_back(-breadth/2);
        color.push_back (clr1);
        color.push_back (clr2);
        color.push_back (clr3);
    }

    vertices.push_back(height + length_up);
    vertices.push_back(0);
    vertices.push_back(-breadth/2);
    color.push_back (clr1 -= ref_quotient);
    color.push_back (clr2 -= ref_quotient);
    color.push_back (clr3 -= ref_quotient);

    vertices.push_back(height + length_up);
    vertices.push_back(-height);
    vertices.push_back(-breadth/2);
    color.push_back (clr1 -= ref_quotient);
    color.push_back (clr2 -= ref_quotient);
    color.push_back (clr3 -= ref_quotient);

    vertices.push_back(height+length_up/2);
    vertices.push_back(0);
    vertices.push_back(0);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    vertices.push_back((height+length_up)/2);
    vertices.push_back(-height);
    vertices.push_back(0);


    for (int k =0; k < num_divisions; k++){
        indices.push_back(2*k);
        indices.push_back(2*k+ 2*num_divisions+2);
    }

    for (int k = 0; k< num_divisions; k++){
        indices.push_back(2*k+1);
        indices.push_back(2*k);
    }

    for (int k = 0; k< num_divisions; k++){
        indices.push_back(2*k+2*num_divisions+2);
        indices.push_back(2*k+2*num_divisions+3);
    }

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2*num_divisions);
    indices.push_back(2*num_divisions+1);
    indices.push_back(4*num_divisions+2);
    indices.push_back(4*num_divisions+3);
    indices.push_back(2*num_divisions+2);
    indices.push_back(2*num_divisions+3);

    //Tri fan 1.
    indices.push_back(4*num_divisions+4);
    indices.push_back(0);
    indices.push_back(2*num_divisions);
    indices.push_back(4*num_divisions+2);
    indices.push_back(2*num_divisions+2);
    indices.push_back(0);

    //Tri fan 2.
    indices.push_back(4*num_divisions+5);
    indices.push_back(2*num_divisions - 1);
    indices.push_back(4*num_divisions+1);
    indices.push_back(4*num_divisions+3);
    indices.push_back(2*num_divisions+1);
    indices.push_back(2*num_divisions - 1);


    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glBufferData (GL_ARRAY_BUFFER,
            color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SkateBoardSide::render() const {
    glPushAttrib(GL_ENABLE_BIT);
    //glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glDrawElements(GL_QUAD_STRIP, 2*num_divisions, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_QUAD_STRIP, 2*num_divisions, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*2*num_divisions));
    glDrawElements(GL_QUAD_STRIP, 2*num_divisions, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*4*num_divisions));
    glDrawElements(GL_QUAD_STRIP, 8, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*6*num_divisions));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*(6*num_divisions+8)));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*(6*num_divisions+14)));
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}

