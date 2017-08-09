//
// Created by leejb on 2017/8/6.
//

#ifndef TREELAUNCHER_QUAD_H
#define TREELAUNCHER_QUAD_H
#include "../baseGraphics/Common.h"
#include "Triangle.h"
using nv::vec3f;
class Quad {
public:
    vec3f V[4];
    Triangle tri[2];

    Quad(vec3f& v0, vec3f& v1, vec3f& v2, vec3f& v3);
};


#endif //TREELAUNCHER_TRIANGLE_H
