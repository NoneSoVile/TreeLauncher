package com.visual.treelauncher;

import java.util.Collection;
import java.util.ArrayList;
import java.util.List;

import java.util.ListIterator;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;

import android.graphics.Bitmap;
import android.util.Log;


public class AppsDataSource   {
	private static final String TAG = "AppsDataSource";
	private Context mContext;
	private PackageManager mPackageManager;
	private List<ResolveInfo> apps = new ArrayList<ResolveInfo>();
	private ArrayList<AppItem> app_Items = new ArrayList<AppItem>() ;

	public AppsDataSource(final Context context) {
		mContext = context;
	}

    public ArrayList<AppItem>  getAppItems(){return app_Items;}
	public void loadApps() {
		final PackageManager packageManager = mPackageManager = mContext
				.getPackageManager();
		if(apps != null){
			apps.clear();
		}		
		apps = null;
		final Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
		mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		apps = packageManager.queryIntentActivities(mainIntent, 0);
        loadAppItems();
	}

	public void loadAppItems() {
		app_Items.clear();
		int size = apps.size();
		Log.d(TAG, "loadAppItems apps total = " + size);


		for (int i = 0; i < size; i++) {
			ResolveInfo info = apps.get(i);
			AppItem item = new AppItem();

			item.drawable = getFullResIcon(info);
            item.bitmap = getFullBitmap(info.activityInfo);
			item.iconID = getFullResIconID(info.activityInfo);
			final String packageName = info.activityInfo.applicationInfo.packageName;
			ComponentName componentName = new ComponentName(packageName,
					info.activityInfo.name);
			item.packageName = packageName;

			item.intent = setActivity(componentName,
					Intent.FLAG_ACTIVITY_NEW_TASK
							| Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
			app_Items.add(item);
            Log.d(TAG, "load AppItem : package name = " + packageName + " bitmap object" +
            item.bitmap + " width = " + ((item.bitmap != null) ? item.bitmap.getWidth() : 0));
		}

	}
	
    final Intent setActivity(ComponentName className, int launchFlags)
    {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.setComponent(className);
        intent.setFlags(launchFlags);
        return intent;
    }
	
    public Drawable getFullResIcon(ResolveInfo info)
    {
        return getFullResIcon(info.activityInfo);
    }

    public Drawable getFullResIcon(ActivityInfo info)
    {

        Resources resources;
        try
        {
            resources = mPackageManager
                    .getResourcesForApplication(info.applicationInfo);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            resources = null;
        }
        if (resources != null)
        {
            int iconId = info.getIconResource();
            if (iconId != 0)
            {
                return getFullResIcon(resources, iconId);
            }
        }
        return getFullResDefaultActivityIcon();
    }
    
	public int getFullResIconID(ActivityInfo info) {
		Resources resources;
		int iconId = 0;
		try {
			resources = mPackageManager
					.getResourcesForApplication(info.applicationInfo);
		} catch (PackageManager.NameNotFoundException e) {
			resources = null;
		}
		if (resources != null) {
			iconId = info.getIconResource();
		}
		return iconId;
	}
    
    public Drawable getFullResIcon(Resources resources, int iconId)
    {
        Drawable d;
        try
        {
            d = resources.getDrawable(iconId);
            //resources.getdr
        }
        catch (Resources.NotFoundException e)
        {
            d = null;
        }
        catch(NoSuchMethodError e)
        {
        	d = resources.getDrawable(iconId);
        }

        return (d != null) ? d : getFullResDefaultActivityIcon();
    }
    
    public Drawable getFullResDefaultActivityIcon()
    {
        return getFullResIcon(Resources.getSystem(),
                android.R.drawable.sym_action_email);
    }

    public Bitmap getFullBitmap(ActivityInfo info){
        Resources resources;
        Bitmap iconBmp = null;
        try {
            resources = mPackageManager
                    .getResourcesForApplication(info.applicationInfo);
        } catch (PackageManager.NameNotFoundException e) {
            resources = null;
        }
        if (resources != null) {
            int iconId = info.getIconResource();
            iconBmp = BitmapFactory.decodeResource(resources, iconId);
        }
        return iconBmp;
    }

}
