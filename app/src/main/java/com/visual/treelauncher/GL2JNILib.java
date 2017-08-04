
package com.visual.treelauncher;

// Wrapper for native library

import android.content.res.AssetManager;
import java.util.ArrayList;

public class GL2JNILib {

    static {
        System.loadLibrary("treelaunchergraphics");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void initScreen(int width, int height);
     public static native void renderFrame();
     public static native void touchParameters(float mPreviousX, float mPreviousY, float mDeltaX, float mDeltaY);
     public static native void initAssetManager(AssetManager ass);
     public static native void initAppList(ArrayList<AppItem> array);
}
