//
// Created by leejb on 2017/8/5.
//

#include "Tree.h"
#include "Model/NvModelGL.h"
#include "baseGraphics/NvAssetLoader.h"
#include "Model/NvModelObj.h"
#include <string>
#include <vector>
#define  LOG_TAG    "tree"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include <android/log.h>

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

    //test
    NvModelObj* model = (NvModelObj*)mTreeModel->getModel();
    vector<string> allObjectNames = model->getObjectList();
    for(int i = 0; i < allObjectNames.size(); i++){
        if(allObjectNames[i].find("Plane")){
            mQuadObjectNames.push_back(allObjectNames[i]);
        }
    }
    //std::string objectName("Plane.006_Plane.028");
    //std::vector<float> positions = model->getPositionsByObjectName(objectName);
    //LOGI("Plane.004_Plane.026 position is :");
    //for(int i = 0; i < positions.size(); i++)
      //  LOGI("%f ", positions[i]);
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

matrix4f& Tree::getViewMatrix(vec3f& eye, vec3f& at, vec3f& up){
    nv::lookAt(mViewMatrix, eye, at, up);
    return mViewMatrix;
}

matrix4f& Tree::getModelMatrix(float angleX, float angleY, float angleZ){
    matrix4f rop, top, tmp;
    tmp = nv::rotationY(tmp, angleY);
    rop = tmp;
    top.set_translate(nv::vec3f(0, 0, -2.0f + angleX));
    mModelMatrix = top * rop;
    return mModelMatrix;
}