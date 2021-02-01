//
//  BattleStart.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BattleStart.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;


bool BattleStart::init()
{
    if(!DEffectNode::init()) return false;
    Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("battlestart");
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleStart::animateEnd), COMPLETE);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    addChild(pArmature);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setTouchEnabled(false);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);
    
    CCMenuItem* pItem = DMenuItemImageSound::create(this, menu_selector(BattleStart::menuCallBack));
    pItem->setContentSize(DESIGN_SIZE);
    pItem->setAnchorPoint(CCPointZero);
    pMenu->addChild(pItem);
    return true;
}

void BattleStart::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    close();
}

void BattleStart::menuCallBack(cocos2d::CCObject* obj)
{
    close();
}

void BattleStart::close()
{
    removeAllChildren();
    callback();
    removeFromParent();
}


