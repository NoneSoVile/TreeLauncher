//
// Created by leejb on 2017/8/6.
//

#ifndef TREELAUNCHER_TRIANGLE_H
#define TREELAUNCHER_TRIANGLE_H
#include "../baseGraphics/Common.h"
#include <string>
using std::string;
using nv::vec3f;
class Triangle {
public:
    vec3f V[3];
    float V_Buffer[9];
    string mObjectName;
    Triangle(){}
    Triangle(string& objectName, vec3f& v0, vec3f& v1, vec3f& v2);
    void draw(GLint positionHandle);
};


#endif //TREELAUNCHER_TRIANGLE_H
