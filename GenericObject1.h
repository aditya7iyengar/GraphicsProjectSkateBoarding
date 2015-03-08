#ifndef ADI_GEN_OBJECT_ONE_H
#define ADI_GEN_OBJECT_ONE_H

#include <vector>
#include <GL/glew.h>

using namespace std;

class GenericObject1 {

private:
    GLuint vertex_buffer, index_buffer, color_buffer;
    vector<float> vertices;
    vector<GLushort> indices;
    vector<GLfloat> color;

public:
    int num_sides;
    float top_radius;
    float bottom_radius;
    float height;
    ~GenericObject1();
    void build (float l, float b, float h, int num, float clr1, float clr2, float clr3);
    void render() const;
};
#endif