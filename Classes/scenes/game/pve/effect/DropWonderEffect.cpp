//
//  DropWonderEffect.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DropWonderEffect.h"
#include "managers/ResourceManager.h"
#include "managers/DataManager.h"

USING_NS_CC;
using namespace sp;

DropWonderEffect* DropWonderEffect::create(settlement::WonderInfo* pInfo)
{
    DropWonderEffect* pRet = new DropWonderEffect();
    if(pRet&& pRet->init(pInfo))
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

bool DropWonderEffect::init(settlement::WonderInfo* pInfo)
{
    if(!DEffectNode::init()) return false;
    Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate(CCString::createWithFormat("wonder%d",pInfo->wonderType)->getCString(),sp::ZORDER);
    if(pInfo->wonderType==settlement::kWonderTypeWarrior)
    {
        CCSprite* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(DM_GET_MALL_MODEL->getProductBaseByID(pInfo->productId)->getItemId());
        pNode->setFlipX(true);
        pArmature->getBone("character")->replaceDisplayAt(pNode, 0);
    }
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(DropWonderEffect::animateEnd), COMPLETE);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    addChild(pArmature);
    return true;
}

void DropWonderEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}


