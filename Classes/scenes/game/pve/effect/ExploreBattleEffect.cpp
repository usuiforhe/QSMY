//
//  ExploreBattleEffect.cpp
//  发生战斗
//
//  Created by wanghejun on 13-8-6.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ExploreBattleEffect.h"
#include "managers/ResourceManager.h"
#include "managers/DataManager.h"

USING_NS_CC;
using namespace sp;

ExploreBattleEffect* ExploreBattleEffect::create(pve::MapInfo* pMapInfo)
{
    ExploreBattleEffect* pRet = new ExploreBattleEffect();
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

bool ExploreBattleEffect::init(pve::MapInfo* pMapInfo)
{
    if(!DEffectNode::init()) return false;

	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("pve_explore_r",sp::ZORDER);
	//	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(ExploreBattleEffect::animateEnd), COMPLETE);
//	pArmature->setPosition(ccpSub(D_DESIGN_POINT_CENTER, ccp(0, 18)));

	CCSprite* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(pMapInfo->enemy_id, 2.0);
	pNode->setFlipX(true);
	pNode->setPosition(ccp(200, -800));
	pArmature->getBone("character")->replaceDisplayAt(pNode, 0);


	CCClippingNode* pCNode = CCClippingNode::create();
	pCNode->addChild(pArmature);
	CCNode *stencil = CCNode::create();
	CCSprite *pTop = CCSprite::create("PL_ui/map/battle_ani_cover_r.png");
	pTop->setPositionY(30);
	stencil->addChild(pTop);
	pCNode->setStencil(stencil);
	pCNode->setAlphaThreshold(0.5);
	addChild(pCNode);
	pCNode->setPosition(ccpAdd(D_DESIGN_POINT_CENTER, ccp(0, -18)));
//	addChild(pArmature);

    pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("pve_explore_l",sp::ZORDER);
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(ExploreBattleEffect::animateEnd), COMPLETE);
 //   pArmature->setPosition(ccpSub(D_DESIGN_POINT_CENTER, ccp(0, 18)));
    
	pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(DM_GET_FORMATION_MODEL->getFormationLeader(), 2.0);
	pNode->setPosition(ccp(-200, -450));
	pArmature->getBone("character")->replaceDisplayAt(pNode, 0);


	pCNode = CCClippingNode::create();
	pCNode->addChild(pArmature);
	stencil = CCNode::create();
	CCSprite *pBtm = CCSprite::create("PL_ui/map/battle_ani_cover_l.png");
	stencil->addChild(pBtm);
	pCNode->setStencil(stencil);
	pCNode->setAlphaThreshold(0.5);
	addChild(pCNode);
	pCNode->setPosition(ccpAdd(D_DESIGN_POINT_CENTER, ccp(4, -8)));
//    addChild(pArmature);

	pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("pve_explore_c",sp::ZORDER);
	pArmature->setPosition(ccpAdd(D_DESIGN_POINT_CENTER, ccp(0, -18)));
	addChild(pArmature);

    return true;
}

void ExploreBattleEffect::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    callback();
    removeFromParent();
}


