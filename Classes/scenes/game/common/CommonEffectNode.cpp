//
//  CommonEffectNode.cpp
//  QSMY
//
//  Created by wanghejun on 13-11-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "CommonEffectNode.h"

#include "managers/ResourceManager.h"
#include "managers/DataManager.h"

USING_NS_CC;
using namespace sp;

CommonEffectNode* CommonEffectNode::create(sp::Armature* pInfo)
{
    CommonEffectNode* pRet = new CommonEffectNode();
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

bool CommonEffectNode::init(sp::Armature* pArmature)
{
    if(!DEffectNode::init()) return false;
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(CommonEffectNode::animateEnd), COMPLETE);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    pArmature->getAnimation()->playByIndex(0);
    addChild(pArmature);
    return true;
}

void CommonEffectNode::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}