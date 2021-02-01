//
//  ArmatureLoader.cpp
//  QSMY
//
//  Created by wanghejun on 13-6-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ArmatureLoader.h"
#include "AppMacros.h"
#include "../managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;


ArmatureLoader::ArmatureLoader()
:m_pArmature(NULL)
,m_pLoading(NULL)
,m_pDelegate(NULL)
{
    
}

ArmatureLoader::~ArmatureLoader()
{
    CC_SAFE_RELEASE(m_pArmature);
    CC_SAFE_RELEASE(m_pLoading);
}


bool ArmatureLoader::init()
{
    if(!CCNode::init()) return false;    
    return true;
}

void ArmatureLoader::showLoading()
{
    CC_SAFE_RELEASE_NULL(m_pLoading);
    CCNode* pLoading = ResourceManager::sharedResourceManager()->getUIAnimate("loading");
    pLoading->setPosition(ccp(0,100));
    pLoading->setScale(0.5f);
    addChild(pLoading);
    m_pLoading = pLoading;
    CC_SAFE_RETAIN(m_pLoading);
}

void ArmatureLoader::hideLoading()
{
    if(m_pLoading && m_pLoading->getParent()!=NULL) m_pLoading->removeFromParent();
    CC_SAFE_RELEASE_NULL(m_pLoading);
}


void ArmatureLoader::load(const char *_imagePath, const char *_skeletonName, const char *_armatureName,bool HighQuality)
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pArmature);
    
    m_skeletonName = _skeletonName;
    m_armatureName = _armatureName;
    
    if(ArmatureDataManager::sharedArmatureDataManager()->hasCache(_imagePath,_skeletonName))
    {
        loadingCallBack();
        return;
    }
    
    showLoading();
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(_imagePath,_skeletonName,this,callfunc_selector(ArmatureLoader::loadingCallBack),HighQuality);
}

void ArmatureLoader::loadingCallBack()
{
    hideLoading();
    
    Armature *armature = Armature::create(m_skeletonName.c_str(),m_armatureName.c_str());
    armature->getAnimation()->playByIndex(0);
    
    CC_SAFE_RELEASE(m_pArmature);
    CC_SAFE_RETAIN(armature);
    m_pArmature = armature;
    addChild(m_pArmature);
	if (m_pDelegate)
	{
		m_pDelegate->loadingFinished(this);
	}
	
}

