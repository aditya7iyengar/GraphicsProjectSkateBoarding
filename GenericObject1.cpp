#include "GenericObject1.h"
#include <cmath>

GenericObject1::~GenericObject1(){
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &normal_buffer);
}

void GenericObject1::build(float l, float b, float h,  int num) {
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &normal_buffer);

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
        normals.push_back(x/(sqrt(x*x+y*y+height*height/4)));
        normals.push_back(y/(sqrt(x*x+y*y+height*height/4)));
        normals.push_back(height/(2*sqrt(x*x+y*y+height*height/4)));
    }

    for ( int k = 0; k < num_sides; k++){
        float x = bottom_radius * cos(angle);
        float y = bottom_radius * sin(angle);
        angle += theta;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(-height/2);
        normals.push_back(x/(sqrt(x*x+y*y+height*height/4)));
        normals.push_back(y/(sqrt(x*x+y*y+height*height/4)));
        normals.push_back(-height/(2*sqrt(x*x+y*y+height*height/4)));
    }

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(height/2);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(1.0f);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-height/2);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(-1.0f);

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

    glBindBuffer (GL_ARRAY_BUFFER, normal_buffer);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ARRAY_BUFFER, normals.size() * sizeof(float),
            normals.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GenericObject1::render() const {
    glPushAttrib(GL_ENABLE_BIT);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glNormalPointer(GL_FLOAT, 0, 0);
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