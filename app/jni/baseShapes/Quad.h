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
using nv::vec4f;
class NvGLSLProgram;
class Quad {
public:
    vec3f V[4];
    Triangle tri[2];
    float V_Buffer[4*3];
    vec4f V_Color;
    string mObjectName;

    Quad(string& objectName, vec3f& v0, vec3f& v1, vec3f& v2, vec3f& v3);
    void draw(GLint positionHandle);
    void draw(NvGLSLProgram* glslProgram, GLint positionHandle);
    void onPressed(bool pressed);
};


#endif //TREELAUNCHER_TRIANGLE_H
