#include <string.h>
#include <jni.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <baseGraphics/NvAssetLoader.h>
#include "launcher-jni.h"
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#define  LOG_TAG    "JNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include <android/log.h>
#include <baseGraphics/NvMatrix.h>
#include <baseGraphics/NvMath.h>
#include <baseGraphics/NvGLSLProgram.h>
#include <baseGraphics/NvImageGL.h>
#include "Tree.h"


using nv::matrix4f;
using nv::vec4f;
Tree tree;
matrix4f  mvp;
matrix4f scaleM;
matrix4f viewMatrix;
matrix4f modelMatrix;
vec3f eye(0, 0, 0), at(0, 0, -1), up(0, 1, 0);
float m_PreviousX, m_PreviousY, m_DeltaX, m_DeltaY;
float mAngleY = 0.0f, mAngleX = 0.0f;
int m_ScreenWidth, m_ScreenHeight;
int m_TexID;

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}


void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}



class BaseShader : public NvGLSLProgram
{
public:
    BaseShader(const char *vertexProgramPath,
               const char *fragmentProgramPath) :
            positionAHandle(-1)
    {
        bool success = setSourceFromFiles(vertexProgramPath, fragmentProgramPath);
        LOGI("setSourceFromFiles result = %d", success);
        positionAHandle = getAttribLocation("a_Position");
        texCoordAHandle = getAttribLocation("a_TexCoord");
        colorAHandle = getAttribLocation("a_Color");
        texUHandle = getUniformLocation("u_tTex");

        checkGlError("BaseShader");
    }

    GLint positionAHandle;
    GLint texCoordAHandle;
    GLint colorAHandle;
    GLint texUHandle;
};

BaseShader* mTriangleProgram;
GLuint gvPositionHandle;

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    mTriangleProgram = new BaseShader("shaders/triangle.vert", "shaders/triangle.frag");
    gvPositionHandle = mTriangleProgram->getAttribLocation("a_Position");
    checkGlError("glGetAttribLocation");

    glViewport(0, 0, w, h);

    m_ScreenWidth = w;
    m_ScreenHeight = h;

    m_TexID =
            NvImageGL::UploadTextureFromDDSFile("textures/android_log.dds");
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGlError("xx");
    tree.initModel();

    glClearDepthf(1.0f);         //  深度缓存设置
    glEnable(GL_DEPTH_TEST);       //  打开深度测试
    glDepthFunc(GL_LEQUAL);        //  设置深度测试类型
    glDepthMask(GL_TRUE);
    glDepthRangef(0.0f, 1.0f);

    return true;
}


void updateTouchParameters(int x, int y, int deltaX, int deltaY)
{
    LOGI("touchEvent(%d, %d, %d, %d)", x, y, deltaX, deltaY);

    m_PreviousX = x;
    m_PreviousY = y;
    m_DeltaX = deltaX;
    m_DeltaY = deltaY;
    mAngleX += deltaY * 0.01f;
    mAngleY -= deltaX * 0.01f;

    tree.testHit(x, y);

}

static const float TRIANGLE_COORDS[] = {
        0.0f, 1.0f, -0.0f, 1.0f,         0, 1.0f,
        -1.0f, -1.0f, -0.0f, 1.0f,          0.0f, 0.0f,
        1.0f,  -1.0f, -0.0f, 1.0f,         1.0f, 0.0f, };

static const float RECT_COORDS[] = {
        -1.0f, -1.0f, -0.0f, 1.0f,            0, 0.0f,
        1.0f, -1.0f, -0.0f, 1.0f,          1.0f, 0.0f,
        -1.0f,  1.0f, -0.0f, 1.0f,          0.0f, 1.0f,
         1.0f,  1.0f, -0.0f, 1.0f,          1.0f, 1.0f,};
void drawQuad(){
    glVertexAttribPointer(mTriangleProgram->positionAHandle, 4, GL_FLOAT, GL_FALSE, 6* sizeof(float), RECT_COORDS);
    glVertexAttribPointer(mTriangleProgram->texCoordAHandle, 2, GL_FLOAT, GL_FALSE, 6* sizeof(float), RECT_COORDS + 4);
    glEnableVertexAttribArray(mTriangleProgram->positionAHandle);
    glEnableVertexAttribArray(mTriangleProgram->texCoordAHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(mTriangleProgram->positionAHandle);
    glDisableVertexAttribArray(mTriangleProgram->texCoordAHandle);
}
void renderFrame() {
    mvp.make_identity();
    viewMatrix.make_identity();
    mvp = tree.getProjectionMatrix(m_ScreenWidth, m_ScreenHeight);
    viewMatrix = tree.getViewMatrix(eye, at, up);
    modelMatrix = tree.getModelMatrix(mAngleX, mAngleY, mAngleY);
    viewMatrix *= modelMatrix;
    mvp *= viewMatrix;

    float depth = 0.0;

    glClearColor(0.0, 0, 0, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


    mTriangleProgram->enable();
    checkGlError("glUseProgram");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    glUniform1i(mTriangleProgram->texUHandle, 0);
    mTriangleProgram->setUniformMatrix4fv("uMVP", mvp._array, 1, GL_FALSE);

    tree.draw(mTriangleProgram, mTriangleProgram->positionAHandle);
    mTriangleProgram->disable();

}

#ifdef __cplusplus
extern "C" {
#endif


    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initScreen(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_renderFrame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_touchParameters(JNIEnv * env, jobject obj, jfloat mPreviousX, jfloat mPreviousY, jfloat mDeltaX, jfloat mDeltaY);
    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initAssetManager(JNIEnv * env, jobject o, jobject obj);
    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initAppList(JNIEnv * env, jobject o, jobject arrayList);

    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initScreen(JNIEnv * env, jobject obj,  jint width, jint height)
    {
        setupGraphics(width, height);
    }

    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_renderFrame(JNIEnv * env, jobject obj)
    {
       renderFrame();
    }

    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_touchParameters(JNIEnv * env, jobject obj, jfloat mPreviousX, jfloat mPreviousY, jfloat mDeltaX, jfloat mDeltaY)
    {
        updateTouchParameters(mPreviousX, mPreviousY, mDeltaX, mDeltaY);
    }

    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initAssetManager(JNIEnv * env, jobject o, jobject obj)
    {
        AAssetManager* mgr = AAssetManager_fromJava(env, obj);
        if(mgr!=NULL)
        {
            NvAssetLoaderInit(mgr);
            LOGI(" %s", "AAssetManager!=NULL");
        }

    }

int JNICALL Java_com_visual_treelauncher_GL2JNILib_showBitmap(JNIEnv* env, jobject thiz, jobject jbitmap)
    {
        int ret = 0;
        AndroidBitmapInfo bitmapInfo;
        void *pixels = NULL;
        int imgWidth = 2;
        int imgHeight = 2;

    LOGI("Java_com_example_hellojni_HelloJni_showBitmap enter");

    if ((ret = AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return -1;
    }

    LOGI("Java_com_example_hellojni_HelloJni_showBitmap width %d, height %d, format %d",
     bitmapInfo.width, bitmapInfo.height, bitmapInfo.format);
    imgWidth = bitmapInfo.width;
    imgHeight = bitmapInfo.height;

    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Java_com_example_hellojni_HelloJni_showBitmap invalid rgb format");
        return -1;
    }

    if ((ret = AndroidBitmap_lockPixels(env, jbitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    AndroidBitmap_unlockPixels(env, jbitmap);

    return 0;
}

    JNIEXPORT void JNICALL Java_com_visual_treelauncher_GL2JNILib_initAppList(JNIEnv * env, jobject thiz, jobject arrayList)
    {
        jclass clazz = (env)->GetObjectClass(arrayList);
        jmethodID getMethodID = (env)->GetMethodID(clazz, "get", "(I)Ljava/lang/Object;");
        jmethodID sizeMethodID = (env)->GetMethodID(clazz, "size", "()I");
        int size = (env)->CallIntMethod(arrayList, sizeMethodID);
        LOGI("arrayList's size is : %d", size);
        for (int i = 0; i < size; i++)
        {
            jobject appItem = (jobject)(env)->CallObjectMethod(arrayList, getMethodID, i);
            jclass clazz = (env)->GetObjectClass(appItem);
            jfieldID fid = (env)->GetFieldID(clazz, "packageName", "Ljava/lang/String;");
            jstring jstr_packageName =  (jstring)env->GetObjectField(appItem, fid);
            jboolean isCpopy;
            const char *strPackageName = env->GetStringUTFChars(jstr_packageName, &isCpopy);
            LOGI("packageName name is : %s", strPackageName);

             fid = (env)->GetFieldID(clazz, "bitmap", "Landroid/graphics/Bitmap;");
            jobject appBitmap =  env->GetObjectField(appItem, fid);
            Java_com_visual_treelauncher_GL2JNILib_showBitmap(env, thiz, appBitmap);
        }
    }
#ifdef __cplusplus
}
#endif