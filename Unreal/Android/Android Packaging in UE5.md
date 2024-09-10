# How to package for android in Unreal ?

### Android SDK Settings
- `Project Settings - Platforms - Android SDK`
	- `Location of Android SDK` : `[SDK Path]\AndroidSDK`
	- `Location of Android NDK` : `[SDK Path]\AndroidSDK\ndk\25.1.8937393`
	- `Location of JAVA` : `[Java Path]\Java\Jdk-1.8.411`
	- `SDK API Level` : `android-33`
	- `NDK API Level` : `android-29`

### Android Quick Launching
- `Project Settings - Project - Packaging - Project`
	- `Build Configuration` : `Development`
	- `Full Rebuild` : `false`
	- `For Distribution : false`
- `Project Settings - Platforms - Android`
	- `APK Packaging`
		- `Package game data inside .apk?` : `true`
		- `Disable verify OBB on first start/update` : `true`
		- `Use externalFilesDir for UnrealGame files?` : `true`

### Android Simple Packaging
- `Project Settings - Project - Packaging - Project`
	- `Build Configuration` : `Shipping`
	- `Full Rebuild` : `true`
	- `For Distribution` : `true`
- `Project Settings - Platforms - Android`
	- `APK Packaging`
		- `Package game data inside .apk?` : `true`
		- `Disable verify OBB on first start/update` : `true`
		- `Use externalFilesDir for UnrealGame files?` : `true`
	- `App Bundles`
		- `Generate Bundle (AAB)` : `true`

### Android Packaging with Google PAD
- `Project Settings - Project - Packaging - Project`
	- `Build Configuration` : `Shipping`
	- `Full Rebuild` : `true`
	- `For Distribution` : `true`
- `Project Settings - Platforms - Android`
	- `APK Packaging`
		- `Package game data inside .apk?` : `false`
		- `Disable verify OBB on first start/update` : `true`
		- `Use externalFilesDir for UnrealGame files?` : `true`
	- `App Bundles`
		- `Generate Bundle (AAB)` : `true`
- `Project Settings - Plugins - Google PAD`
	- `Enable Plugin` : `true`
	- `Only Distribution` : `true`
	- `Only Shipping` : `false`