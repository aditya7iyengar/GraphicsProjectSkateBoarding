#include "Polygon.h"
#include <iostream>
#include <cmath>
#include <OpenGL/OpenGL.h>

Polygon::~Polygon() {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &normal_buffer);
}

void Polygon::build(float l, float b, float h,
        float l2, float b2 ){
    length = l;
    breadth = b;
    height = h;
    length2 = l2;
    breadth2 = b2;

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &index_buffer);
    glGenBuffers(1, &normal_buffer);
    /* Top vertices of the polygon */


    vertices.push_back(length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);

    normals.push_back(length/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));
    normals.push_back(breadth/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));
    normals.push_back(height/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));



    vertices.push_back(-length/2);
    vertices.push_back(breadth/2);
    vertices.push_back(height/2);
    normals.push_back(-length/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));
    normals.push_back(breadth/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));
    normals.push_back(height/(2*sqrt(length/2*length/2 + breadth/2*breadth/2 + height/2*height/2)));



    vertices.push_back(-length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);
    normals.push_back(-length/2);
    normals.push_back(-breadth/2);
    normals.push_back(height/2);




    vertices.push_back(length/2);
    vertices.push_back(-breadth/2);
    vertices.push_back(height/2);
    normals.push_back(length/2);
    normals.push_back(-breadth/2);
    normals.push_back(height/2);




    /* Bottom vertices the polygon */

    vertices.push_back(length2/2);
    vertices.push_back(breadth2/2);
    vertices.push_back(-height/2);
    normals.push_back(length2/2);
    normals.push_back(breadth2/2);
    normals.push_back(-height/2);




    vertices.push_back(-length2/2);
    vertices.push_back(breadth2/2);
    vertices.push_back(-height/2);
    normals.push_back(-length2/2);
    normals.push_back(breadth2/2);
    normals.push_back(-height/2);





    vertices.push_back(-length2/2);
    vertices.push_back(-breadth2/2);
    vertices.push_back(-height/2);
    normals.push_back(-length2/2);
    normals.push_back(-breadth2/2);
    normals.push_back(-height/2);


    vertices.push_back(length2/2);
    vertices.push_back(-breadth2/2);
    vertices.push_back(-height/2);
    normals.push_back(length2/2);
    normals.push_back(-breadth2/2);
    normals.push_back(-height/2);




    /* Center of the top face */

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(height/2);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(height/2);



    /* Center of the bottom face */

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-height/2);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(-height/2);





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

    /* select the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, normal_buffer);
    /* allocate in GPU and copy from CPU */
    glBufferData (GL_ARRAY_BUFFER, normals.size() * sizeof(float),
            normals.data(), GL_STATIC_DRAW);
    /* deselect the buffer */
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLushort),indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Polygon::render() const {
    /* binding the vertex buffer*/
    glPushAttrib(GL_ENABLE_BIT);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glNormalPointer(GL_FLOAT, 0, 0);

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