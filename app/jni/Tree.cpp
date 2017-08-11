//
// Created by leejb on 2017/8/5.
//

#include "Tree.h"
#include "Model/NvModelGL.h"
#include "baseGraphics/NvAssetLoader.h"
#include "Model/NvModelObj.h"
#include "baseShapes/Ray.h"
#include "baseShapes/IntrRayTriangle.h"
#include "baseGraphics/NvGLSLProgram.h"
#include <string>
#include <vector>
#define  LOG_TAG    "tree"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include <android/log.h>

#define PI (3.1415926f)
Tree::Tree() :
        mTreeModel(NULL),
        mFovy(PI / 2.0f),
        mAspect(1.0f),
        mNear(1.0f),
        mFar(150.0f),
        mViewWidth(1080),
        mViewHeight(1920)
{}
void Tree::initModel(){
    int length = 0;
    char *modelData = NvAssetLoaderRead("models/tree2.obj", length);
    mTreeModel = NvModelGL::CreateFromObj(
            (uint8_t *)modelData, -1.0f, true, false);
    NvAssetLoaderFree(modelData);

    modelData = NvAssetLoaderRead("models/quads.obj", length);
    mQuadsModel = NvModelGL::CreateFromObj(
            (uint8_t *)modelData, -1.0f, true, false);
    NvAssetLoaderFree(modelData);

    //test
    NvModelObj* model = (NvModelObj*)mQuadsModel->getModel();
    vector<string> allObjectNames = model->getObjectList();
    for(int i = 0; i < allObjectNames.size(); i++){
        if(allObjectNames[i].find("Plane") != -1){
            mQuadObjectNames.push_back(allObjectNames[i]);
            std::vector<float> positions = model->getPositionsByObjectName(allObjectNames[i]);
            int size = positions.size();

            if(size == 4*3){
                vec3f  vertices[4];
                for(int j = 0; j < 4; j++)
                    vertices[j] = vec3f(positions[j*3], positions[j*3 + 1], positions[j*3 + 2]);
                Quad* quad = new Quad(allObjectNames[i], vertices[0], vertices[1], vertices[2], vertices[3]);
                mQuads.push_back(quad);
                LOGI("Plane  %s  added to scene", allObjectNames[i].c_str());
            }else{
                LOGE("Plane  %s 's vertex is not 4", allObjectNames[i].c_str());
            }
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
    for (int i = 0; i < mQuads.size(); ++i) {
        mQuads[i]->draw(positionHandle);
    }
}

void Tree::draw(NvGLSLProgram* glslProgram, GLint positionHandle){
    if(mTreeModel != NULL){
        glslProgram->setUniform4fv("u_color", vec4f(0, 0, 1, 1), 1);
        mTreeModel->drawElements(positionHandle);
    }
    for (int i = 0; i < mQuads.size(); ++i) {
        mQuads[i]->draw(glslProgram, positionHandle);
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

matrix4f& Tree::getProjectionMatrix(float viewWidth, float viewHeight){
    glViewport(0, 0, viewWidth, viewHeight);
    mViewWidth = viewWidth;
    mViewHeight = viewHeight;
    mAspect = viewWidth / viewHeight;
    //setting the perspective projection matrix
    nv::perspective(mProjectionMatrix, mFovy,
                    mAspect,
                    mNear, mFar);

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

bool Tree::testHit(float screenX, float screenY){
    float x = (screenX / mViewWidth) * 2.0f - 1.0f;
    float y = ((mViewHeight - screenY) / mViewHeight) * 2.0f - 1.0f;
    float l, r, t, b;
    t = mNear*tan(mFovy/2.0f);
    b = -t;
    r = mAspect*t;
    l = -r;
    //x = (r - l) * x / 2.0f;
    //y = (t - b) * y / 2.0f;

    vec3f target(x, y, 0);
    matrix4f invProj = nv::inverse(mProjectionMatrix);
    target = (vec3f)(invProj * vec4f(target, 1));
    x = target.x;
    y = target.y;

    float z = -mNear;
    LOGI("l,r,b,t  : %f, %f, %f, %f", l, r, b, t);
    LOGI("Ray x : %f, y: %f, z: %f", x, y, z);
    vec3f direction(x, y, z);
    direction = nv::normalize(direction);
    Ray ray(vec3f(0.0f, 0.0f, 0.0f), direction);
    matrix4f v = mViewMatrix;
    matrix4f m = mModelMatrix;
    matrix4f mv = v * m;
    for(int i = 0; i < mQuads.size(); i++){
        Quad& quad = *(mQuads[i]);
        bool pressed = false;
        for(int j = 0; j < 2; j++) {
            if(pressed)
                continue;
            vec3f v0 ((mv * vec4f(quad.tri[j].V[0], 1)));
            vec3f v1 ( (mv * vec4f(quad.tri[j].V[1], 1)));
            vec3f v2 ( (mv * vec4f(quad.tri[j].V[2], 1)));
            Triangle T1(quad.mObjectName, v0, v1, v2);
            IntrRayTriangle intersectTest(ray, T1);
            LOGI("Triangle %s (%f %f , %f %f , %f %f )", quad.mObjectName.c_str(),
               v0.x, v0.y,   v1.x, v1.y,   v2.x, v2.y);
            if (intersectTest.Test()) {
                quad.onPressed(true);
                pressed = true;
                LOGI("Hit quad object %s", quad.mObjectName.c_str());
            }else{
                quad.onPressed(false);
            }
        }
    }
    return false;
}