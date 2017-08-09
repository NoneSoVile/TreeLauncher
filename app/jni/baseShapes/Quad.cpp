//
// Created by leejb on 2017/8/6.
//

#include "Quad.h"

Quad::Quad(vec3f& v0, vec3f& v1, vec3f& v2, vec3f& v3){
    V[0] = v0;
    V[1] = v1;
    V[2] = v2;
    V[3] = v3;
    tri[0].V[0] = v0;tri[0].V[1] = v1;tri[0].V[2] = v2;
    tri[1].V[0] = v2;tri[1].V[1] = v1;tri[1].V[2] = v3;
}
