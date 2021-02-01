//
//  ExploreEffect.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#define Y_POS_PLUS	240

#include "ExploreEffect.h"
#include "ExploreEffect.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;

ExploreEffect::ExploreEffect()
:m_pNode(NULL)
{
    
}

ExploreEffect::~ExploreEffect()
{
    CC_SAFE_RELEASE(m_pNode);
}

ExploreEffect* ExploreEffect::create(pve::MapInfo* pMapInfo)
{
    ExploreEffect* pRet = new ExploreEffect();
    if(pRet&& pRet->init(pMapInfo))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}

bool ExploreEffect::init(pve::MapInfo* pMapInfo)
{
    if(!DEffectNode::init()) return false;
    
    Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("walk",sp::ZORDER);
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(ExploreEffect::animateEnd), COMPLETE);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    
    CCSprite *pNode = ResourceManager::sharedResourceManager()->getSceneIcon(pMapInfo->chapterInfo->getCid());
    CC_SAFE_RETAIN(pNode);
    CC_SAFE_RELEASE(m_pNode);
    m_pNode = pNode;
    pArmature->getBone("img1")->replaceDisplayAt(pNode, 0);
    
    
    CCClippingNode* pClippingNode = CCClippingNode::create();
    pClippingNode->addChild(pArmature);
    addChild(pClippingNode);
    
	CCLayerColor *stencil = CCLayerColor::create(ccc4(255,255,255,255),640,1136);
    stencil->setPosition(ccp(0,0));
    pClippingNode->setStencil(stencil);
    
    pArmature->setAnimationScale(pMapInfo->iTimes%pMapInfo->fight_open==pMapInfo->fight_open-1?0.8f:1.f);
    
    return true;
}

void ExploreEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}

void ExploreEffect::setOffset(const CCPoint &offset,float scale)
{
    m_pNode->setPosition(ccpAdd(offset, ccp(0, Y_POS_PLUS)));
    m_pNode->setScale(scale);
}






