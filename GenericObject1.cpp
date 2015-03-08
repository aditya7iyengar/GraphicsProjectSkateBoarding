#include "GenericObject1.h"
#include <cmath>

GenericObject1::~GenericObject1(){
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &color_buffer);
}

void GenericObject1::build(float l, float b, float h,  int num, float clr1, float clr2, float clr3) {
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);

    num_sides = num;
    top_radius = l;
    bottom_radius = b;
    height = h;

    float theta = 2 * M_PI/num_sides;

    float angle = 0.0f;

    for ( int k = 0; k < num_sides; k++){
        float x = top_radius * cos(angle);
        float y = top_radius * sin(angle);
        angle += theta;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(height/2);
        color.push_back (clr1);
        color.push_back (clr2);
        color.push_back (clr3);
    }

    for ( int k = 0; k < num_sides; k++){
        float x = bottom_radius * cos(angle);
        float y = bottom_radius * sin(angle);
        angle += theta;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(-height/2);
        color.push_back (clr1);
        color.push_back (clr2);
        color.push_back (clr3);
    }

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(height/2);
    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-height/2);
    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    for (int k = 0; k < num_sides; k++){
        indices.push_back(k);
        indices.push_back(k + num_sides);
    }

    indices.push_back(0);
    indices.push_back(num_sides);

    indices.push_back(2*num_sides);
    for (int k = 0; k < num_sides ; k++){
        indices.push_back(k);
    }
    indices.push_back(0);

    indices.push_back(2*num_sides+1);
    for (int k = num_sides - 1; k > -1 ; k--){
        indices.push_back(k+num_sides);
    }
    indices.push_back(2*num_sides-1);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glBufferData (GL_ARRAY_BUFFER,
            color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GenericObject1::render() const {
    glPushAttrib(GL_ENABLE_BIT);
    //glDisableClientState(GL_COLOR_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glDrawElements(GL_QUAD_STRIP, 2*num_sides + 2, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_TRIANGLE_FAN, num_sides + 2, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*2*(num_sides + 1) ));
    glDrawElements(GL_TRIANGLE_FAN, num_sides + 2, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*(3*(num_sides + 1)+1) ));
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();
}