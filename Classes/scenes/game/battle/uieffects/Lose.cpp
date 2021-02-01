//
//  Lose.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Lose.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;


bool Lose::init()
{
    if(!DEffectNode::init()) return false;

	ccColor4B c = ccc4(0,0,0,150);
	CCLayerColor* l = CCLayerColor::create(c);
	l->setContentSize(CCSizeMake(l->getContentSize().width,l->getContentSize().height+200));
	l->setPosition(0.f,-50.0f);
	addChild(l);
	//CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_common/ALL_mengban");	

    //Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("lose");
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("battleover");
	pArmature->getAnimation()->playByIndex(1);

    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Lose::animateEnd), COMPLETE);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    addChild(pArmature);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setTouchEnabled(false);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);
    
    CCMenuItem* pItem = DMenuItemImageSound::create(this, menu_selector(Lose::menuCallBack));
    pItem->setContentSize(DESIGN_SIZE);
    pItem->setAnchorPoint(CCPointZero);
    pMenu->addChild(pItem);
    
    return true;
}

void Lose::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    close();
}

void Lose::menuCallBack(cocos2d::CCObject* obj)
{
    close();
}

void Lose::close()
{
    removeAllChildren();
    callback();
    removeFromParent();
}
