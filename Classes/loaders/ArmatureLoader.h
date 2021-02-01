//
//  ArmatureLoader.h
//  QSMY
//
//  Created by wanghejun on 13-6-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ArmatureLoader__
#define __QSMY__ArmatureLoader__

#include "AppInclude.h"
#include "DragonBone.h"

class ArmatureLoader;
class ArmatureLoaderDelegate
{
public:
	virtual void loadingFinished(ArmatureLoader *pLoader) = 0;
};

class ArmatureLoader:public cocos2d::CCNode
{
    CC_SYNTHESIZE(ArmatureLoaderDelegate *, m_pDelegate, Delegate);
public:
    
    ArmatureLoader();
    virtual ~ArmatureLoader();
    CREATE_FUNC(ArmatureLoader);
    
    virtual bool init();
    
    void load(const char *_imagePath, const char *_skeletonName, const char* _armatureName,bool HighQuality);
    
    sp::Armature * getArmature()
    {
        return m_pArmature;
    }
private:
    void loadingCallBack();
    void showLoading();
    void hideLoading();
private:
    sp::Armature * m_pArmature;
    std::string m_skeletonName;
    std::string m_armatureName;
    CCNode* m_pLoading;
};

#endif /* defined(__QSMY__ArmatureLoader__) */
