// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidRewardedAdHelper.h"

#include "AndroidRewardedAdHelperBPLibrary.h"

#define LOCTEXT_NAMESPACE "FAndroidRewardedAdHelperModule"

#if PLATFORM_ANDROID

#endif

void FAndroidRewardedAdHelperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UAndroidRewardedAdHelperBPLibrary::InitRewardedAd();
}

void FAndroidRewardedAdHelperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAndroidRewardedAdHelperModule, AndroidRewardedAdHelper)