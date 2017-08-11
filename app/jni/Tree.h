//
// Created by leejb on 2017/8/5.
//

#ifndef TREELAUNCHER_TREE_H
#define TREELAUNCHER_TREE_H
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <vector>
#include "baseGraphics/NvMath.h"
#include "baseShapes/Quad.h"
using nv::vec4f;
using nv::vec3f;
using nv::matrix4f;
using std::vector;
using std::string;
class NvModelGL;
class NvGLSLProgram;
class Tree {
public:
    Tree();
    void initModel();
    void draw(GLint positionHandle);
    void draw(NvGLSLProgram* glslProgram, GLint positionHandle);
    vec4f unProject(matrix4f& mvp, vec4f& inVec, int viewWith, int viewHeight);
    matrix4f& getProjectionMatrix(float viewWith, float viewHeight);
    matrix4f& getViewMatrix(vec3f& eye, vec3f& at, vec3f& up);
    matrix4f& getModelMatrix(float angleX, float angleY, float angleZ);
    bool testHit(float screenX, float screenY);

private:
    NvModelGL* mTreeModel;
    NvModelGL* mQuadsModel;
    float mFovy;
    float mAspect;
    float mNear, mFar;
    float mViewWidth, mViewHeight;
    matrix4f mProjectionMatrix;
    matrix4f mViewMatrix;
    matrix4f mModelMatrix;
    vector<string> mQuadObjectNames;
    vector<Quad*>  mQuads;
};


#endif //TREELAUNCHER_TREE_H
