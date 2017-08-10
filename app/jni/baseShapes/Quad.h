//
// Created by leejb on 2017/8/6.
//

#ifndef TREELAUNCHER_QUAD_H
#define TREELAUNCHER_QUAD_H
#include "../baseGraphics/Common.h"
#include "Triangle.h"
#include <string>
using std::string;
using nv::vec3f;
class Quad {
public:
    vec3f V[4];
    Triangle tri[2];
    float V_Buffer[6*3];
    string mObjectName;

    Quad(string& objectName, vec3f& v0, vec3f& v1, vec3f& v2, vec3f& v3);
    void draw(GLint positionHandle);
};


#endif //TREELAUNCHER_TRIANGLE_H
