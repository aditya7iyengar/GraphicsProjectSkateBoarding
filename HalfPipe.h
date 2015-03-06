#ifndef HALFPIPE_H
#define HALFPIPE_H

#include <vector>
#include <GL/glew.h>

using namespace std;

class HalfPipe {

private:
    GLuint vertex_buffer, index_buffer, color_buffer;
    vector<float> vertices;
    vector<GLushort> indices;
    vector<GLfloat> color;
    int num_divisions;

public:
    ~HalfPipe();
    void build (float length_up, float breadth, float height,
            float clr1, float clr2, float clr3, float ref_quotient);
    void render() const;
};
#endif