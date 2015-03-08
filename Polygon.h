#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <GL/glew.h>

using namespace std;
class Polygon {
private:
    GLuint vertex_buffer, index_buffer, color_buffer;
    vector<GLfloat> vertices;
    vector<GLfloat> color;
    vector<GLushort> indices;
public:
    float length, breadth, height, length2, breadth2;
    ~Polygon();
    void build(float l, float b, float h, float l2, float b2, float clr1, float clr2, float clr3, float shiny_quotient);
    void render() const;
};
#endif