
# How to make Android Rewarded Ad in Unreal ?

- We have to control JNI (Java Native Interface) for implemeting rewarded ad functionality
- JNI helps you interact between C++ (Unreal Code) and Java (Android Code)

- This is based on Unreal Engine 5.2 version.

## 1. Add this Android JNI code in your project
```cpp
#if PLATFORM_ANDROID

#include "Engine/GameEngine.h"
#include "Android/AndroidApplication.h"
#include "Launch/Public/Android/AndroidJNI.h"
#include "Android/AndroidJavaEnv.h"

// Java to C++
extern "C" void Java_com_epicgames_unreal_GameActivity_SucceedRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz, jstring type, jint amount)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FString rewardType = FJavaHelper::FStringFromParam(Env, type);
		int rewardAmount = (int)amount;

		AsyncTask(ENamedThreads::GameThread, [rewardType, rewardAmount]() {
	
			// Implement your code when rewarded ad is succeeded.
			// FString 	rewardType 		: your reward type in AdMob
			// int 		rewardAmount 	: your reward amount in AdMob
	
		});

	}
}

// Java to C++
extern "C" void Java_com_epicgames_unreal_GameActivity_FailRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz)
{
	AsyncTask(ENamedThreads::GameThread, []() {

		// Implement your code when rewarded ad is Failed.
	
	});
}

// C++ to Java
// Call this function when you should load rewarded ad.
static jmethodID AndroidThunkJava_LoadRewardedAd;
void AndroidThunkCpp_LoadRewardedAd(const FString& adUnitID)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (AndroidThunkJava_LoadRewardedAd)
		{
			auto AdUnitIDArg = FJavaHelper::ToJavaString(Env, adUnitID);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GoogleServicesThis, AndroidThunkJava_LoadRewardedAd, *AdUnitIDArg);
		}
	}
}

// C++ to Java
// Call this function when you should show rewarded ad after loaded.
static jmethodID AndroidThunkJava_ShowRewardedAd;
void AndroidThunkCpp_ShowRewardedAd()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (AndroidThunkJava_ShowRewardedAd)
		{
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GoogleServicesThis, AndroidThunkJava_ShowRewardedAd);
		}
	}
}

// C++ to Java
// Call this function when you should check that rewarded ad is loaded.
static jmethodID AndroidThunkJava_IsRewardedAdAvailable;
bool AndroidThunkCpp_IsRewardedAdAvailable()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (AndroidThunkJava_ShowRewardedAd)
		{
			return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GoogleServicesThis, AndroidThunkJava_IsRewardedAdAvailable);
		}
	}

	return false;
}

#endif
```

## 2. Add this code where your game proceeds first (like game instance init function)
- This code must be called before using rewarded ad.
```cpp
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		AndroidThunkJava_LoadRewardedAd = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_LoadRewardedAd", "(Ljava/lang/String;)V", false);
		AndroidThunkJava_ShowRewardedAd = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_ShowRewardedAd", "()V", false);
		AndroidThunkJava_IsRewardedAdAvailable = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_IsRewardedAdAvailable", "()Z", false);
	}
#endif
```

## 3. Make this UPL file in your project
```xml
<?xml version="1.0" encoding="utf-8"?>
<root xmlns:android="http://schemas.android.com/apk/res/android">
  <gameActivityImportAdditions>
    <insert>
	// For Rewarded Ad
	    import androidx.annotation.NonNull;
	    import com.google.android.gms.ads.AdError;
	    import com.google.android.gms.ads.OnUserEarnedRewardListener;
	    import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;

      import com.google.android.gms.ads.rewarded.RewardedAd;
      import com.google.android.gms.ads.rewarded.RewardItem;
    </insert>
  </gameActivityImportAdditions>
  
  <gameActivityClassAdditions>
    <insert> <![CDATA[
	// For Rewarded Ad
      public RewardedAd 	rewardedAd;
      public AdRequest		rewardedAdRequest;
      public boolean		isRewardedAdLoaded;
      public RewardItem		rewardItem;

      public void AndroidThunkJava_LoadRewardedAd(String AdMobAdUnitID)
      {
      	rewardedAdRequest = new AdRequest.Builder().build();
      	rewardedAd = null;
      	isRewardedAdLoaded = false;

      	_activity.runOnUiThread(new Runnable() {
      		
			@Override
      		public void run() {
      			FullScreenContentCallback fullScreenContentCallback = new FullScreenContentCallback() {
      				
					@Override
      				public void onAdDismissedFullScreenContent() {

      					Log.debug("Rewarded Ad onAdDismissedFullScreenContent");
      					FailRewardedAd();

      				}

      				@Override
      				public void onAdShowedFullScreenContent() {

      					Log.debug("Rewarded Ad onAdShowedFullScreenContent");
      				}

      				@Override
      				public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
      					rewardedAd = null;
      					isRewardedAdLoaded = false;

      					Log.debug("Rewarded Ad onRewardedAdFailedToShow : " + adError.getCode());
      					FailRewardedAd();

      				}
      			};

      			RewardedAd.load(_activity, AdMobAdUnitID, rewardedAdRequest, new RewardedAdLoadCallback() {
      				
					@Override
      				public void onAdLoaded(RewardedAd ad) {
      					rewardedAd = ad;
      					isRewardedAdLoaded = true;

      					rewardedAd.setFullScreenContentCallback(fullScreenContentCallback);

      					Log.debug("Rewarded Ad successfully loaded.");
      				}

      				@Override
      				public void onAdFailedToLoad(LoadAdError adError) {
      					Log.debug("Rewarded Ad failed to load, errocode: " + adError.getCode());
     					isRewardedAdLoaded = false;
      					rewardedAd = null;
      				}

      			});
      		}
      	});
      }

      public boolean AndroidThunkJava_IsRewardedAdAvailable()
      {
      	return rewardedAd != null && isRewardedAdLoaded;
      }

      public void AndroidThunkJava_ShowRewardedAd()
      {
      	if(AndroidThunkJava_IsRewardedAdAvailable())
      	{
      		_activity.runOnUiThread(new Runnable() {
      			@Override
      			public void run() {

      				rewardedAd.show(_activity, new OnUserEarnedRewardListener() {
      					
						@Override
      					public void onUserEarnedReward(@NonNull RewardItem item) {
      						rewardItem = item;

      						Log.debug("User Earned Reward : " + rewardItem.getType() + " " + rewardItem.getAmount());
      						SucceedRewardedAd(rewardItem.getType(), rewardItem.getAmount());
      					}

      				});

      			}
      		});
      	}
      	else
      	{
      		Log.debug("Rewarded Ad is not available to show - call LoadRewardedAd or wait for it to finish loading");
      	}
      }

      public native void SucceedRewardedAd(String type, int amount);
      public native void FailRewardedAd();

    ]]></insert>
  </gameActivityClassAdditions>
</root>
```

## 4. Include the UPL file above from Build.cs file
```cs
using System.IO;
using UnrealBuildTool;

public class UserProject : ModuleRules
{
	public UserProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { /*...*/ });
		

        PrivateDependencyModuleNames.AddRange(new string[] { /*...*/ });

		// Add this code
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			var manifestFile = Path.Combine(ModuleDirectory, "/[UPL file path based Build.cs]/[UPL file name].xml");
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", manifestFile);
		}
	}
}
```