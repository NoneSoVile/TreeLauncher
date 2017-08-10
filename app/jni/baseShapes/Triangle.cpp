//
// Created by leejb on 2017/8/6.
//

#include "Triangle.h"

Triangle::Triangle(string& objectName, vec3f& v0, vec3f& v1, vec3f& v2){
    V[0] = v0;
    V[1] = v1;
    V[2] = v2;
    mObjectName = objectName;
    for(int i = 0; i < 3; i++)
        memcpy(V_Buffer + i*3, (float*)V[i], sizeof(float)*3);
}

void Triangle::draw(GLint positionHandle){
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, V_Buffer);
    glEnableVertexAttribArray(positionHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    glDisableVertexAttribArray(positionHandle);
}
