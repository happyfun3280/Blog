// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AndroidRewardedAdHelperBPLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE(FAndroidRewardedAdDelegate);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAndroidRewardedAdRewardDelegate, const FString&, name, int32, count);


/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UAndroidRewardedAdHelperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	friend class FAndroidRewardedAdHelperModule;

private:
	static FString GetAdUnitIDStringByIndex(int32 adUnitIndex);
	static void InitRewardedAd();

public:
	static FAndroidRewardedAdDelegate OnLoaded_RewardedAd;
	static FAndroidRewardedAdDelegate OnLoadFailed_RewardedAd;
	static FAndroidRewardedAdRewardDelegate OnSucceeded_RewardedAd;
	static FAndroidRewardedAdDelegate OnFailed_RewardedAd;

	UFUNCTION(BlueprintCallable, Category = "Android Rewarded AD")
	static void LoadRewardedAd(int32 adUnitIndex, const FAndroidRewardedAdDelegate& onLoaded, const FAndroidRewardedAdDelegate& onLoadFailed);

	UFUNCTION(BlueprintCallable, Category = "Android Rewarded AD")
	static void ShowRewardedAd(const FAndroidRewardedAdRewardDelegate& onSucceeded, const FAndroidRewardedAdDelegate& onFailed);

	UFUNCTION(BlueprintCallable, Category = "Android Rewarded AD")
	static bool IsRewardedAdAvailable();
};
