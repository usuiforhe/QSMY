//
//  PartyAndroidHelper.h
//
//
//  Created by Uno on 2013/11/1.
//
//

#include "platform/android/jni/JniHelper.h"

#ifndef PartyAndroidHelper_h
#define PartyAndroidHelper_h

class  PartyAndroidHelper {
        
	public:
		JNIEnv* getJNIEnv();
		void trackStart(jint appId, jstring appKey);
		void trackEvent(jint eventId);
		void trackPayment(jstring itemName, jfloat itemPrice, jstring itemPriceCurrency, jint itemNum);
};

#endif
