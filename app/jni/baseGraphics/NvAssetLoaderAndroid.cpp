#include "NvAssetLoader.h"
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#define  LOG_TAG    "AssetLoaderAndroid"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include <android/log.h>
static AAssetManager* s_assetManager = NULL;

bool NvAssetLoaderInit(void* platform)
{
    if (!platform)
        return false;

    s_assetManager = (AAssetManager*)platform;

    return true;
}

bool NvAssetLoaderShutdown()
{
    s_assetManager = NULL;
    return true;
}

bool NvAssetLoaderAddSearchPath(const char *)
{
    return true;
}

bool NvAssetLoaderRemoveSearchPath(const char *)
{
    return true;
}

char *NvAssetLoaderRead(const char *filePath, int32_t &length)
{
    char *buff = NULL;

    if (!s_assetManager)
        return NULL;

    AAsset *fileAsset = AAssetManager_open(s_assetManager, filePath, AASSET_MODE_BUFFER);

    if(fileAsset != NULL)
    {
        length = AAsset_getLength(fileAsset);
        buff = new char[length+1];
        int32_t numBytes = AAsset_read(fileAsset, buff, length);
        buff[length] = '\0';

        LOGI("Read asset '%s', %d bytes", filePath, numBytes);
        //LOGI(" %s\n", buff);

        AAsset_close(fileAsset);
    }

    return buff;
}

bool NvAssetLoaderFree(char* asset)
{
    delete[] asset;
    return true;
}

bool NvAssetLoaderFileExists(const char *filePath) {
    NvAssetFilePtr fp = NvAssetLoaderOpenFile(filePath);
    if (fp != NULL) {
        NvAssetLoaderCloseFile(fp);
        return true;
    }
    return false;
}

NvAssetFilePtr NvAssetLoaderOpenFile(const char* name) {
    if (!s_assetManager)
        return NULL;

    return (NvAssetFilePtr)AAssetManager_open(s_assetManager, name, AASSET_MODE_BUFFER);
}

void NvAssetLoaderCloseFile(NvAssetFilePtr fp) {
    AAsset_close((AAsset*)fp);
}

int32_t NvAssetFileGetSize(NvAssetFilePtr fp) {
    return AAsset_getLength((AAsset*)fp);
}

int32_t NvAssetFileSeek(NvAssetFilePtr fp, int32_t offset, NvAssetSeekBase whence) {
    return AAsset_seek((AAsset*)fp, offset, (int32_t)whence);
}

int32_t NvAssetFileRead(NvAssetFilePtr file, int32_t size, void* data) {
    return AAsset_read((AAsset*)file, data, size);
}

int64_t NvAssetFileGetSize64(NvAssetFilePtr fp) {
    return AAsset_getLength64((AAsset*)fp);
}

int64_t NvAssetFileRead64(NvAssetFilePtr file, int64_t size, void* data) {
    return AAsset_read((AAsset*)file, data, size);
}

int64_t NvAssetFileSeek64(NvAssetFilePtr fp, int64_t offset, NvAssetSeekBase whence) {
    return AAsset_seek64((AAsset*)fp, offset, (int32_t)whence);
}

