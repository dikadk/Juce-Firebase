# Juce-Firebase
How to integrate Firebase C++ SDK into iOS JUCE app

Based on https://forum.juce.com/t/juce-firebase-admob-lets-monetize-our-juce-ios-apps-with-ads-partial-how-to/29369/5
and https://www.youtube.com/watch?v=vBaQaqaJjZ8

But due to changes in latest Firebase iOS SDK. Tutorial required some additional steps.

This tutorial will show how to setup Analytics and Crashlytics

Pre-requisites

###IMPORTANT AUG 2022 Update

Due to changed in Firebase iOS SDK - it uses now swift binaries I didn't find any way to link it properly and make some with Projucer

So import to use Firebase 8.5.0
https://github.com/firebase/firebase-ios-sdk/releases/tag/8.5.0
https://github.com/firebase/firebase-cpp-sdk/releases/tag/v8.5.0

#-------------------------------------------------------------------------------------

1. Download Firebase C++ SDK
2. Download Firebase iOS Frameworks Zip ~1gb (https://firebase.google.com/docs/ios/setup)
3. Build custom Projucer from Source.

New Firebase iOS SDK consists of .framework and .xcframework. Bundled Juce Projucer version has few issues with .xcframework.

So we have to make few changes to Xcode Exporter and build Projucer from Source.
```jucer_ProjectExport_Xcode.h:```
1. Comment out line: ```writeWorkspaceSettings();```
(This will ensure that once we change build system in Xcode to NEW BUILD SYSTEM, it won't reset to Legacy on saving project. We have to use NEW BUILD system in XCode to use .xcframewoks)
2. Ignore addition of .framework extention to .xcframework paths
  ```
  String addCustomFramework(String frameworkPath){
            if (! frameworkPath.endsWithIgnoreCase (".framework") && !frameworkPath.endsWithIgnoreCase(".xcframework"))
            frameworkPath << ".framework";
            ...
   }
  ```
   This will ensure that we will be able to specify extension in Extra Custom Frameworks field in Project (aka. path/FirebaseAnAlytics.xcframework)
3. Build Projucer and open Juce Project using it

Project Config:
1. Setup Firebase project
2. Download GoogleServices-Info.plist into Source folder and add to project ass 'Xcode Resource'
3. Edit GoogleServices-Info.plist: change ```xml<key>IS_ANALYTICS_ENABLED</key>``` to ```true```
4. Add reference to Firebase.h to Sources
5. Configure Xcode (iOS) exporter:
   1. Shared:
      1. Extra Compiler Flags: ```-fno-aligned-allocation```
      2. Extra Linker Flags: ```$(OTHER_LDFLAGS) -ObjC -lsqlite3 -lz```
      3. Custom PList:
        ```xml
        <plist version="1.0">
            <dict>
                <key>NSAppTransportSecurity</key>
                    <dict>
                        <key>NSAllowsArbitraryLoads</key>
                            <true/>
                    </dict>
            </dict>
        </plist>
        ```
      4. Extra System Frameworks: 
      ```CoreMotion, MediaPlayer, AdSupport, MessageUI, CoreServices, CoreTelephony, Security, StoreKit,    SystemConfiguration, UIKit, Foundation, CoreMedia, GLKit```
      1. Frameworkds Search Path: ```/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics;
/Users/home/Documents/git_projects/Firebase/FirebaseCrashlytics;
/Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks```
      1. Extra Custom Frameworks: ```
/Users/home/Documents/git_projects/Firebase/FirebaseCrashlytics/FirebaseCrashlytics.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/FirebaseAnalytics.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/FirebaseCore.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/FirebaseCoreDiagnostics.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/FirebaseInstallations.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/GoogleAppMeasurement.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/GoogleDataTransport.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/GoogleUtilities.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/nanopb.xcframework
/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics/PromisesObjC.xcframework
/Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks/firebase_analytics.xcframework
/Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks/firebase.xcframework```
   1. Debug/Release
      1. Header Search Paths: ```/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics;
/Users/home/Documents/git_projects/Firebase/FirebaseCrashlytics;
/Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks```
      2. Extra Search Library Paths: ```/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics;
/Users/home/Documents/git_projects/Firebase/FirebaseCrashlytics;
/Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks```
      3. Custom XCode Flags: ```FRAMEWORK_SEARCH_PATHS="/Users/home/Documents/git_projects/Firebase/FirebaseAnalytics /Users/home/Documents/git_projects/Firebase/FirebaseCrashlytics /Users/home/Documents/git_projects/firebase_cpp_sdk/xcframeworks",
GCC_GENERATE_DEBUGGING_SYMBOLS=YES,
STRIP_INSTALLED_PRODUCT=NO,
COPY_PHASE_STRIP=NO```
6. Export iOS project to XCode
7. Under ```Project->Build Options->Debug Information Format``` change to ```DWARF with dSYM File``` (we will need these files to upload to Firebase for de-obfuscating crash reports)
8. Build app and check Verify that app showed up in Analytics

Few useful debug arguments for XCode Schemas (Arguments passed on Launch):
```-FIRAnalyticsDebugEnabled```
```-FIRDebugEnabled```


Links:
Firebase C++ SDK Guide: https://firebase.google.com/docs/cpp/setup?platform=ios
Firebase Setup without Cocaposds: https://firebase.google.com/docs/ios/setup