//
//  PartyAndroidHelper.cpp
//
//
//  Created by Uno on 2013/11/1.
//
//

#include "PartyAndroidHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "PartyAndroidHelper", __VA_ARGS__)


jobject m_context = NULL;
extern "C"
{
  void Java_com_wegames_pilicb_and_PiliTaiwan_passAppContext(JNIEnv* env, jobject thiz, jobject context)
    {
	  m_context = context;
    }
}

JNIEnv* PartyAndroidHelper::getJNIEnv()
{
	JNIEnv *env  =NULL;
	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (NULL == jvm) {
		//return NULL;
	}

	//setup env
	jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
	if (ret !=JNI_OK) {
		env =NULL;
		//return NULL;
	}

	return env;
}

void PartyAndroidHelper::trackStart(jint appId, jstring appKey)
{
	JNIEnv *env = PartyAndroidHelper::getJNIEnv();

	jclass cls;
	jmethodID mid;
	jboolean b;
	jobject jobj;
	jint square;
	//LOGI("start trackStart");

	cls = env->FindClass("it/partytrack/sdk/Track");

	if(cls != 0) {
		mid = env->GetStaticMethodID(cls, "start", "(Landroid/content/Context;ILjava/lang/String;)V");

		if(mid != 0) {
			env->CallStaticVoidMethod(cls, mid, m_context, appId, appKey);
			//LOGI("pass call trackStart");
		}
	}
}

void PartyAndroidHelper::trackEvent(jint eventId)
{
	JNIEnv *env = PartyAndroidHelper::getJNIEnv();

	jclass cls;
	jmethodID mid;
	jboolean b;
	jobject jobj;
	jint square;
	//LOGI("start trackEvent");

	cls = env->FindClass("it/partytrack/sdk/Track");

	if(cls != 0) {
		mid = env->GetStaticMethodID(cls, "event", "(I)V");

		if(mid != 0) {
			env->CallStaticVoidMethod(cls, mid, eventId);
			//LOGI("pass call trackEvent");
		}
	}
}

void PartyAndroidHelper::trackPayment(jstring itemName, jfloat itemPrice, jstring itemPriceCurrency, jint itemNum)
{
	JNIEnv *env = PartyAndroidHelper::getJNIEnv();

	jclass cls;
	jmethodID mid;
	jboolean b;
	jobject jobj;
	jint square;
	//LOGI("start trackPayment");

	cls = env->FindClass("it/partytrack/sdk/Track");

	if(cls != 0) {
		mid = env->GetStaticMethodID(cls, "payment", "(Ljava/lang/String;FLjava/lang/String;I)V");

		if(mid != 0) {
			env->CallStaticVoidMethod(cls, mid, itemName, itemPrice, itemPriceCurrency, itemNum);
			//LOGI("pass call trackPayment");
		}
	}
}
