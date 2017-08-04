package com.visual.treelauncher;

import android.app.Activity;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.util.ArrayList;


public class MainActivity extends Activity {

    GL2JNIView mView;
    AppsDataSource mAppsDataSource;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView  tv = new TextView(this);
        mAppsDataSource = new AppsDataSource(this);
        mAppsDataSource.loadApps();
        mView = new GL2JNIView(getApplication());
        GL2JNILib.initAssetManager(getAssets());
        ArrayList<AppItem> appItems = mAppsDataSource.getAppItems();
        GL2JNILib.initAppList(appItems);
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
