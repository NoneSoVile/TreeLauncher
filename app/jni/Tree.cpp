//
// Created by leejb on 2017/8/5.
//

#include "Tree.h"
#include "Model/NvModelGL.h"
#include "baseGraphics/NvAssetLoader.h"
#define PI (3.1415926f)
Tree::Tree() :
        mTreeModel(NULL)
{}
void Tree::initModel(){
    int length = 0;
    char *modelData = NvAssetLoaderRead("models/tree2.obj", length);
    mTreeModel = NvModelGL::CreateFromObj(
            (uint8_t *)modelData, -1.0f, true, false);
    NvAssetLoaderFree(modelData);
}
void Tree::draw(GLint positionHandle){
    if(mTreeModel != NULL){
        mTreeModel->drawElements(positionHandle);
    }
}

vec4f Tree::unProject(matrix4f& mvp, vec4f& inVec, int viewWith, int viewHeight){
    matrix4f invMvp = nv::inverse(mvp);
    vec4f in, out;
    in[0] = (inVec.x / viewWith) * 2 - 1;
    in[1] = (inVec.y / viewHeight) * 2 - 1;
    in[2] = 2 * inVec.z - 1;
    in[3] = 1;
    out = invMvp * in;
    out.x *= out.w;
    out.y *= out.w;
    out.z *= out.w;
    return out;
}

matrix4f& Tree::getProjectionMatrix(float viewWith, float viewHeight){
    glViewport(0, 0, viewWith, viewHeight);
    //setting the perspective projection matrix
    nv::perspective(mProjectionMatrix, PI / 3.0f,
                    viewWith / viewHeight,
                    1.0f, 150.0f);

    return mProjectionMatrix;
}