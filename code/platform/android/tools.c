#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>

#include <unistd.h>

//#include <android/sensor.h>           // Required for: Android sensors functions (accelerometer, gyroscope, light...)
#include <android/window.h>             // Required for: AWINDOW_FLAG_FULLSCREEN definition and others
#include <android_native_app_glue.h>    // Required for: android_app struct and activity management
#include <jni.h>                        // Required for: JNIEnv and JavaVM [Used in OpenURL()]

//#include <EGL/egl.h>                    // Native platform windowing system interface
//#include <GLES2/gl2.h>                // OpenGL ES 2.0 library (not required in this module, only in rlgl)

struct android_app* GetAndroidApp(void);
//use GetAndroidApp from raylib rcore.c to get struct android_app

void AutoHideNavBar(struct android_app* state)
{
	JNIEnv* env = NULL;
	JavaVM* vm = state->activity->vm;
	(*vm)->AttachCurrentThread(vm, &env, NULL);

	jclass activityClass = (*env)->FindClass(env, "android/app/NativeActivity");
	jmethodID getWindow = (*env)->GetMethodID(env, activityClass, "getWindow", "()Landroid/view/Window;");

	jclass windowClass = (*env)->FindClass(env, "android/view/Window");
	jmethodID getDecorView = (*env)->GetMethodID(env, windowClass, "getDecorView", "()Landroid/view/View;");

	jclass viewClass = (*env)->FindClass(env, "android/view/View");
	jmethodID setSystemUiVisibility = (*env)->GetMethodID(env, viewClass, "setSystemUiVisibility", "(I)V");

	jobject window = (*env)->CallObjectMethod(env, state->activity->clazz, getWindow);

	jobject decorView = (*env)->CallObjectMethod(env, window, getDecorView);

	jfieldID flagID1 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_LAYOUT_STABLE", "I");
	jfieldID flagID2 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION", "I");
	jfieldID flagID3 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN", "I");
	jfieldID flagID4 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_HIDE_NAVIGATION", "I");
	jfieldID flagID5 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_FULLSCREEN", "I");
	jfieldID flagID6 = (*env)->GetStaticFieldID(env, viewClass, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I");

	int flag1 = (*env)->GetStaticIntField(env, viewClass, flagID1);
	int flag2 = (*env)->GetStaticIntField(env, viewClass, flagID2);
	int flag3 = (*env)->GetStaticIntField(env, viewClass, flagID3);
	int flag4 = (*env)->GetStaticIntField(env, viewClass, flagID4);
	int flag5 = (*env)->GetStaticIntField(env, viewClass, flagID5);
	int flag6 = (*env)->GetStaticIntField(env, viewClass, flagID6);

	int flags = flag1 | flag2 | flag3 | flag4 | flag5 | flag6;

	(*env)->CallVoidMethod(env, decorView, setSystemUiVisibility, flags);

	(*vm)->DetachCurrentThread(vm);
}

void AutoHideNavBarOnGainedFocus(void)
{
	AutoHideNavBar(GetAndroidApp());
}

void AndroidTestFun(const char* className, const char* fieldName, char* val256buf)
{
	JNIEnv* env = NULL;
	JavaVM* vm = GetAndroidApp()->activity->vm;

	(*vm)->AttachCurrentThread(vm, &env, NULL);

	strcpy(val256buf, "");

	jclass clazz = (*env)->FindClass(env, className);

	if(clazz != NULL)
	{
		jfieldID fid = (*env)->GetStaticFieldID(env, clazz, fieldName, "Ljava/lang/String;");

		if(fid != NULL)
		{
			jstring jstr = (jstring)(*env)->GetStaticObjectField(env, clazz, fid);

			const jint jstr_len = (*env)->GetStringLength(env, jstr);

			if(jstr_len > 0)
			{
				jboolean blnIsCopy;
				const char* str = (*env)->GetStringUTFChars(env, jstr, &blnIsCopy);

				if(str != NULL)
				{
					strcpy(val256buf, str);
					(*env)->ReleaseStringUTFChars(env, jstr, str);
				}
			}
		}
	}

	(*vm)->DetachCurrentThread(vm);
}

/*
#if defined(PLATFORM_ANDROID)
		JNIEnv *env = NULL;
		JavaVM *vm = CORE.Android.app->activity->vm;
		(*vm)->AttachCurrentThread(vm, &env, NULL);

		jstring urlString = (*env)->NewStringUTF(env, url);
		jclass uriClass = (*env)->FindClass(env, "android/net/Uri");
		jmethodID uriParse = (*env)->GetStaticMethodID(env, uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
		jobject uri = (*env)->CallStaticObjectMethod(env, uriClass, uriParse, urlString);

		jclass intentClass = (*env)->FindClass(env, "android/content/Intent");
		jfieldID actionViewId = (*env)->GetStaticFieldID(env, intentClass, "ACTION_VIEW", "Ljava/lang/String;");
		jobject actionView = (*env)->GetStaticObjectField(env, intentClass, actionViewId);
		jmethodID newIntent = (*env)->GetMethodID(env, intentClass, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
		jobject intent = (*env)->AllocObject(env, intentClass);

		(*env)->CallVoidMethod(env, intent, newIntent, actionView, uri);
		jclass activityClass = (*env)->FindClass(env, "android/app/Activity");
		jmethodID startActivity = (*env)->GetMethodID(env, activityClass, "startActivity", "(Landroid/content/Intent;)V");
		(*env)->CallVoidMethod(env, CORE.Android.app->activity->clazz, startActivity, intent);

		(*vm)->DetachCurrentThread(vm);
#endif
*/
