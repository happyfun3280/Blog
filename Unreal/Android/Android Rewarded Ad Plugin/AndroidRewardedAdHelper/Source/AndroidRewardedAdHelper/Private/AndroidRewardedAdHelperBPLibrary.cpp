// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidRewardedAdHelperBPLibrary.h"


#if PLATFORM_ANDROID

#include "Android/AndroidPlatform.h"
#include "Android/AndroidApplication.h"
#include "Launch/Public/Android/AndroidJNI.h"
#include "Android/AndroidJavaEnv.h"

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeOnLoadedRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz)
{
	AsyncTask(ENamedThreads::GameThread, []() {
		UAndroidRewardedAdHelperBPLibrary::OnLoaded_RewardedAd.ExecuteIfBound();
		});
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeOnLoadFailedRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz)
{
	AsyncTask(ENamedThreads::GameThread, []() {
		UAndroidRewardedAdHelperBPLibrary::OnLoadFailed_RewardedAd.ExecuteIfBound();
		});
}


JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeOnSucceededRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz, jstring type, jint amount)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FString rewardName = FJavaHelper::FStringFromParam(Env, type);
		int rewardCount = (int)amount;

		AsyncTask(ENamedThreads::GameThread, [rewardName, rewardCount]() {
			UAndroidRewardedAdHelperBPLibrary::OnSucceeded_RewardedAd.ExecuteIfBound(rewardName, rewardCount);
			});

	}
}

JNI_METHOD void Java_com_epicgames_unreal_GameActivity_nativeOnFailedRewardedAd(JNIEnv* LocalJNIEnv, jobject LocalThiz)
{
	AsyncTask(ENamedThreads::GameThread, []() {
		UAndroidRewardedAdHelperBPLibrary::OnFailed_RewardedAd.ExecuteIfBound();
		});
}

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


UAndroidRewardedAdHelperBPLibrary::UAndroidRewardedAdHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FAndroidRewardedAdDelegate UAndroidRewardedAdHelperBPLibrary::OnLoaded_RewardedAd = FAndroidRewardedAdDelegate();
FAndroidRewardedAdDelegate UAndroidRewardedAdHelperBPLibrary::OnLoadFailed_RewardedAd = FAndroidRewardedAdDelegate();
FAndroidRewardedAdRewardDelegate UAndroidRewardedAdHelperBPLibrary::OnSucceeded_RewardedAd = FAndroidRewardedAdRewardDelegate();
FAndroidRewardedAdDelegate UAndroidRewardedAdHelperBPLibrary::OnFailed_RewardedAd = FAndroidRewardedAdDelegate();

FString UAndroidRewardedAdHelperBPLibrary::GetAdUnitIDStringByIndex(int32 adUnitIndex)
{
	TArray<FString> AdUnitIDs;
	int32 count = GConfig->GetArray(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitIDs"), AdUnitIDs, GEngineIni);

	if (adUnitIndex >= 0 && adUnitIndex < count && !AdUnitIDs[adUnitIndex].IsEmpty())
		return AdUnitIDs[adUnitIndex];

	return FString();
}

void UAndroidRewardedAdHelperBPLibrary::InitRewardedAd()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		AndroidThunkJava_LoadRewardedAd = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_LoadRewardedAd", "(Ljava/lang/String;)V", false);
		AndroidThunkJava_ShowRewardedAd = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_ShowRewardedAd", "()V", false);
		AndroidThunkJava_IsRewardedAdAvailable = FJavaWrapper::FindMethod(Env, FJavaWrapper::GoogleServicesClassID, "AndroidThunkJava_IsRewardedAdAvailable", "()Z", false);
	}
#endif
}

void UAndroidRewardedAdHelperBPLibrary::LoadRewardedAd(int32 adUnitIndex,
	const FAndroidRewardedAdDelegate& onLoaded, const FAndroidRewardedAdDelegate& onLoadFailed)
{
	OnLoaded_RewardedAd.Clear();
	OnLoadFailed_RewardedAd.Clear();
	OnSucceeded_RewardedAd.Clear();
	OnFailed_RewardedAd.Clear();

	OnLoaded_RewardedAd = onLoaded;
	OnLoadFailed_RewardedAd = onLoadFailed;

#if PLATFORM_ANDROID
	FString adUnitID = GetAdUnitIDStringByIndex(adUnitIndex);

	if (!adUnitID.IsEmpty())
	{
		AndroidThunkCpp_LoadRewardedAd(adUnitID);
	}
#endif
}

void UAndroidRewardedAdHelperBPLibrary::ShowRewardedAd(const FAndroidRewardedAdRewardDelegate& onSucceeded,
	const FAndroidRewardedAdDelegate& onFailed)
{
	OnSucceeded_RewardedAd = onSucceeded;
	OnFailed_RewardedAd = onFailed;

#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowRewardedAd();
#endif
}

bool UAndroidRewardedAdHelperBPLibrary::IsRewardedAdAvailable()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsRewardedAdAvailable();
#endif
	return false;
}
