//
// Created by leejb on 2017/8/5.
//

#ifndef TREELAUNCHER_TREE_H
#define TREELAUNCHER_TREE_H
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "baseGraphics/NvMath.h"
using nv::vec4f;
using nv::matrix4f;
class NvModelGL;
class Tree {
public:
    Tree();
    void initModel();
    void draw(GLint positionHandle);
    vec4f unProject(matrix4f& mvp, vec4f& inVec, int viewWith, int viewHeight);
    matrix4f& getProjectionMatrix(float viewWith, float viewHeight);

private:
    NvModelGL* mTreeModel;
    matrix4f mProjectionMatrix;
};


#endif //TREELAUNCHER_TREE_H
