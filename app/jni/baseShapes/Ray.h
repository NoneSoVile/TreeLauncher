//
// Created by leejb on 2017/8/6.
//

#ifndef TREELAUNCHER_RAY_H
#define TREELAUNCHER_RAY_H

#include "../baseGraphics/Common.h"
using nv::vec3f;

class Ray {
public:
    Ray(vec3f o, vec3f d);
public:
    vec3f Origin;
    vec3f Direction;
};


#endif //TREELAUNCHER_RAY_H
