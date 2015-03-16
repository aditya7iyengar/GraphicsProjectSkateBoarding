#include "HalfPipe.h"
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

HalfPipe::~HalfPipe(){
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteBuffers(1, &normal_buffer);
}

void HalfPipe::build(float length_up,float breadth, float height){

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);
    glGenBuffers(1, &normal_buffer);

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
        normals.push_back(x_co/(sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));
        normals.push_back(y_co/(sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));
        normals.push_back(breadth/(2*sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));



        vertices.push_back(x_co);
        vertices.push_back(-height);
        vertices.push_back(breadth/2);
        normals.push_back(x_co/(sqrt(x_co * x_co + height*height + breadth/2*breadth/2)));
        normals.push_back(-height/(sqrt(x_co*x_co + height*height + breadth/2*breadth/2)));
        normals.push_back(breadth/(2*sqrt(x_co*x_co + height*height + breadth/2*breadth/2)));

    }

    vertices.push_back(height + length_up);
    vertices.push_back(0);
    vertices.push_back(breadth/2);
    normals.push_back((height + length_up)/(sqrt((height + length_up)*(height + length_up)+breadth/2*breadth/2)));
    normals.push_back(0);
    normals.push_back(breadth/(2*sqrt((height + length_up)*(height + length_up) + breadth/2*breadth/2)));

    vertices.push_back(height + length_up);
    vertices.push_back(-height);
    vertices.push_back(breadth/2);
    normals.push_back((height + length_up)/(sqrt((height + length_up)*(height + length_up) + height*height + breadth/2*breadth/2)));
    normals.push_back(-height/(sqrt((height + length_up)*(height + length_up) +height*height+ breadth/2*breadth/2)));
    normals.push_back(breadth/(2*sqrt((height + length_up)*(height + length_up) + height*height + breadth/2*breadth/2)));

    temp_angle = 0.0f;
    for (int k =0; k < num_divisions; k++){
        float x_co = height * cos(temp_angle);
        float y_co = -height * sin(temp_angle);

        temp_angle += increment_angle;

        vertices.push_back(x_co);
        vertices.push_back(y_co);
        vertices.push_back(-breadth/2);
        normals.push_back(x_co/(sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));
        normals.push_back(y_co/(sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));
        normals.push_back(-breadth/(2*sqrt(x_co*x_co + y_co*y_co + breadth/2*breadth/2)));

        vertices.push_back(x_co);
        vertices.push_back(-height);
        vertices.push_back(-breadth/2);
        normals.push_back(x_co/(sqrt(x_co*x_co + height*height + breadth/2*breadth/2)));
        normals.push_back(-height/(sqrt(x_co*x_co + height*height + breadth/2*breadth/2)));
        normals.push_back(-breadth/(2*sqrt(x_co*x_co + height*height + breadth/2*breadth/2)));
    }

    vertices.push_back(height + length_up);
    vertices.push_back(0);
    vertices.push_back(-breadth/2);
    normals.push_back((height + length_up)/(sqrt((height + length_up)*(height + length_up)+breadth/2*breadth/2)));
    normals.push_back(0/(sqrt((height + length_up)*(height + length_up) + breadth/2*breadth/2)));
    normals.push_back(-breadth/(2*sqrt((height + length_up)*(height + length_up) + breadth/2*breadth/2)));

    vertices.push_back(height + length_up);
    vertices.push_back(-height);
    vertices.push_back(-breadth/2);
    normals.push_back((height + length_up)/(sqrt((height + length_up)*(height + length_up)+ height*height + breadth/2*breadth/2)));
    normals.push_back(-height/(sqrt((height + length_up)*(height + length_up) +height*height+ breadth/2*breadth/2)));
    normals.push_back(-breadth/(2*sqrt((height + length_up)*(height + length_up) + height*height + breadth/2*breadth/2)));

    vertices.push_back(height+length_up/2);
    vertices.push_back(0);
    vertices.push_back(0);

    normals.push_back(1.0f);
    normals.push_back(0.0);
    normals.push_back(0.0);


    vertices.push_back((height+length_up)/2);
    vertices.push_back(-height);
    vertices.push_back(0);
    normals.push_back((height + length_up)/(2*sqrt((height + length_up)*(height + length_up)/4+height*height)));
    normals.push_back(-height/sqrt((height + length_up)*(height + length_up)/4+height*height));
    normals.push_back(0);

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

void HalfPipe::render() const {
    glPushAttrib(GL_ENABLE_BIT);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glNormalPointer(GL_FLOAT, 0, 0);
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

