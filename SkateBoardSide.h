#ifndef SKATEBOARDSIDE_H
#define SKATEBOARDSIDE_H

#include <vector>
#include <GL/glew.h>

using namespace std;

class SkateBoardSide {

private:
    GLuint vertex_buffer, index_buffer, color_buffer;
    vector<float> vertices;
    vector<GLushort> indices;
    vector<GLfloat> color;
    int num_divisions;

public:
    ~SkateBoardSide();
    void build (float length, float breadth, float height,
            float clr1, float clr2, float clr3, float ref_quotient);
    void render() const;
};
#endif