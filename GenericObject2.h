#ifndef GENERIC_OBJECT_TWO_H
#define GENERIC_OBJECT_TWO_H

#include <vector>
#include <GL/glew.h>

using namespace std;

class GenericObject2{
private:
    GLuint vertex_buffer, index_buffer, normal_buffer;
    vector<float> vertices;
    vector<GLushort> indices;
    vector<GLfloat> normals;

public:
    int num_sides;
    float outer_top_radius, inner_top_radius, outer_bottom_radius, inner_bottom_radius, height;
    void build (float outer_top_radius, float inner_top_radius, float outer_bottom_radius,float inner_bottom_radius,
            float height, int num_sides);
    void render() const;
    ~GenericObject2();
};
#endif