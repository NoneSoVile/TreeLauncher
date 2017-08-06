//
// Created by leejb on 2017/8/6.
//

#ifndef TREELAUNCHER_TRIANGLE_H
#define TREELAUNCHER_TRIANGLE_H
#include "../baseGraphics/Common.h"
using nv::vec3f;
class Triangle {
public:
    vec3f V[3];

    Triangle(vec3f& v0, vec3f& v1, vec3f& v2);
};


#endif //TREELAUNCHER_TRIANGLE_H
