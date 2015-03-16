#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <GL/glew.h>

using namespace std;
class Polygon {
private:
    GLuint vertex_buffer, index_buffer, normal_buffer;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLushort> indices;
public:
    float length, breadth, height, length2, breadth2;
    ~Polygon();
    void build(float l, float b, float h, float l2, float b2);
    void render() const;
};
#endif