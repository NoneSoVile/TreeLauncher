//
// Created by leejb on 2017/8/6.
//

#include "Quad.h"

Quad::Quad(string& objectName, vec3f& v0, vec3f& v1, vec3f& v2, vec3f& v3){
    V[0] = v0;
    V[1] = v1;
    V[2] = v2;
    V[3] = v3;
    tri[0].V[0] = v0;tri[0].V[1] = v1;tri[0].V[2] = v2;
    tri[1].V[0] = v2;tri[1].V[1] = v1;tri[1].V[2] = v3;
    for(int i = 0; i < 2; i++)
        for (int j = 0; j < 3; ++j) {
            memcpy(V_Buffer + i*3*3 + j*3, (float*)(tri[i].V[j]), sizeof(float)*3);
        }

    tri[0].mObjectName = objectName;
    tri[1].mObjectName = objectName;
    mObjectName = objectName;
}

void Quad::draw(GLint positionHandle){
    glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), V_Buffer);
    glEnableVertexAttribArray(positionHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(positionHandle);
}
