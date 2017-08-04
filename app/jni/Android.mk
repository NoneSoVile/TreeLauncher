
APP_ABI := armeabi-v7a
APP_PLATFORM := android-17
APP_STL := stlport_static
APP_ALLOW_MISSING_DEPS := true

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_CFLAGS    := -Werror
LOCAL_MODULE    := treelaunchergraphics
LOCAL_SRC_FILES := launcher-jni.cpp  \
                   baseGraphics\NvAssetLoaderAndroid.cpp  \
                   baseGraphics\NvGLSLProgram.cpp   \
                   baseGraphics\ColorBlock.cpp   \
                   baseGraphics\BlockDXT.cpp   \
                   baseGraphics\NvFilePtr.cpp  \
                   baseGraphics\NvImage.cpp  \
                   baseGraphics\NvImageDDS.cpp \
                   baseGraphics\NvImageGL.cpp  \

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -ljnigraphics
LOCAL_STATIC_LIBRARIES := cpufeatures


include $(BUILD_SHARED_LIBRARY)

#$(call import-module,android/ndk_helper)
#$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
