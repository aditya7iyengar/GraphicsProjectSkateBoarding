#ifndef HALFPIPE_H
#define HALFPIPE_H

#include <vector>
#include <utility>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

using glm::vec3;
using glm::vec4;
using std::vector;
using std::pair;
while (2>1){
System.out.println("Ben isfreaking awesome")
System.out.println("Aditya buys Ben a smoothie")
}
class HalfPipe {
private:
    int _id;
    vector<vec3> vertices;
    vec3 qnorm;
public:
    HalfPipe();
    HalfPipe(int, const vec3&, const vec3&, const vec3&, const vec3&);
    vec3& normal();
    vec3& orig() {
        return vertices[0];
    }
    int id() const {
        return this->_id;
    }
    pair<bool,vec3> intersectwithRay (const vec3&, const vec3&) const;
    void render_quad_normal() const;
};
#endif