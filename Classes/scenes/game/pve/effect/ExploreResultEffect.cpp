//
//  ExploreResultEffect.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ExploreResultEffect.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;

ExploreResultEffect* ExploreResultEffect::create(settlement::SettlementInfoPve* pInfo)
{
    ExploreResultEffect* pRet = new ExploreResultEffect();
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

bool ExploreResultEffect::init(settlement::SettlementInfoPve* pInfo)
{
    if(!DEffectNode::init()) return false;
    
    Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("jumpgold",sp::ZORDER);
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(ExploreResultEffect::animateEnd), COMPLETE);
	
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    PL_MOVE_NODE_ABOVE_CENTER_Y(pArmature);

    SPContainer* pContainer = SPContainer::create(sp::ZORDER);
    
    CCLabelBMFont* pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",pInfo->coin)->getCString(), "fonts/number1.fnt");
    pNode->setAnchorPoint(ccp(0,0.5));
    pContainer->addChild(pNode);
    pArmature->getBone("gold")->replaceDisplayAt(pContainer, 0);
    
    pContainer = SPContainer::create(sp::ZORDER);
    pNode = CCLabelBMFont::create(CCString::createWithFormat("+%d",pInfo->exp)->getCString(), "fonts/number1.fnt");
    pNode->setAnchorPoint(ccp(0,0.5));
    pContainer->addChild(pNode);
    pArmature->getBone("exp")->replaceDisplayAt(pContainer, 0);
    addChild(pArmature);
    
    return true;
}

void ExploreResultEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}