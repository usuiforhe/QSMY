//
//  DMenuItemSound.h
//  QSMY
//
//  Created by wanghejun on 13-11-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DMenuItemSound__
#define __QSMY__DMenuItemSound__

#include "AppInclude.h"

#include "AppInclude.h"
#include "utils/Notification.h"


class DMenuItemImageSound:public cocos2d::CCMenuItemImage
{
    
public:
    
    DMenuItemImageSound();
    virtual ~DMenuItemImageSound();
    CREATE_FUNC(DMenuItemImageSound);
    virtual bool init();

	/** creates a menu item with a normal and selected image*/
	static DMenuItemImageSound* create(const char *normalImage, const char *selectedImage);
	/** creates a menu item with a normal,selected  and disabled image*/
	static DMenuItemImageSound* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
	/** creates a menu item with a normal and selected image with target/selector */
	static DMenuItemImageSound* create(const char *normalImage, const char *selectedImage, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static DMenuItemImageSound* create(const char *normalImage, const char *selectedImage, const char *disabledImage, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

    /** Creates a CCMenuItem with a target/selector */
    static DMenuItemImageSound* create(CCObject *rec, cocos2d::SEL_MenuHandler selector);
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_sSound, Sound);
    
    virtual void activate();
};

class DMenuItemImageSoundLoader : public cocos2d::extension::CCMenuItemImageLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DMenuItemImageSoundLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DMenuItemImageSound);
    virtual void onHandlePropTypeString(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, const char * pString, cocos2d::extension::CCBReader * pCCBReader);
};


#endif /* defined(__QSMY__DMenuItemSound__) */
