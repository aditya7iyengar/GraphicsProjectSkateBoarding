#include "Polygon.h"
#include <iostream>

Polygon::~Polygon() {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &color_buffer);
}

void Polygon::build(float l, float b, float h,
        float l2, float b2,
        float clr1, float clr2, float clr3, float shiny_quotient) {
    length = l;
    breadth = b;
    height = h;
    length2 = l2;
    breadth2 = b2;

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &color_buffer);
    /* Top vertices of the polygon */


    vertices.push_back(length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    vertices.push_back(-length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    vertices.push_back(-length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    vertices.push_back(length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    /* Bottom vertices the polygon */

    vertices.push_back(length2/2);
    vertices.push_back(breadth2/2);
    vertices.push_back(-height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    vertices.push_back(-length2/2);
    vertices.push_back(breadth2/2);
    vertices.push_back(-height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 -= (shiny_quotient*0.03);
    clr2 -= (shiny_quotient*0.02);
    clr3 -= (shiny_quotient*0.03);

    vertices.push_back(-length2/2);
    vertices.push_back(-breadth2/2);
    vertices.push_back(-height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 += (shiny_quotient*0.03);
    clr2 += (shiny_quotient*0.02);
    clr3 += (shiny_quotient*0.03);

    vertices.push_back(length2/2);
    vertices.push_back(-breadth2/2);
    vertices.push_back(-height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 += (shiny_quotient*0.03);
    clr2 += (shiny_quotient*0.02);
    clr3 += (shiny_quotient*0.03);


    /* Center of the top face */

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);

    clr1 += (shiny_quotient*0.03);
    clr2 += (shiny_quotient*0.02);
    clr3 += (shiny_quotient*0.03);


    /* Center of the bottom face */

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-height/2);

    color.push_back (clr1);
    color.push_back (clr2);
    color.push_back (clr3);



    for (int k = 0; k < 4; k++){
        indices.push_back(k);
        indices.push_back(k+4);
    }

    indices.push_back(0);
    indices.push_back(4);

    indices.push_back(8);

    for (int k = 0; k < 4; k++){
        indices.push_back(k);
    }

    indices.push_back(0);

    for (int i = 3; i > -1; i--){
        indices.push_back(i+4);
    }

    indices.push_back(7);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, color_buffer);
    glBufferData (GL_ARRAY_BUFFER,
            color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Polygon::render() const {
    /* binding the vertex buffer*/
    glPushAttrib(GL_ENABLE_BIT);
    //glDisableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    /* render the polygon */
    glDrawElements(GL_QUAD_STRIP, 10, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*10));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_SHORT,
            (void*)(sizeof(GLushort)*16));

    /* unbind the buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopAttrib();


}