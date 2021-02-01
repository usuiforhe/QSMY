//
//  Win.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Win.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
using namespace sp;


Win* Win::create(uint8_t score)
{
    Win* pRet = new Win();
    if(pRet&& pRet->init(score))
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

bool Win::init(uint8_t score)
{
    if(!DEffectNode::init()) return false;
    
	ccColor4B c = ccc4(0,0,0,150);
	CCLayerColor* l = CCLayerColor::create(c);
	l->setContentSize(CCSizeMake(l->getContentSize().width,l->getContentSize().height+200));
	l->setPosition(0.f,-50.0f);
	addChild(l);
	//CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_common/ALL_mengban");	

	/*if(score>0)
	{

	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("win");
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	addChild(pArmature);

	pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("battlestar");
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Win::animateEnd), COMPLETE);
	pArmature->getAnimation()->playByIndex(score-1,-1,-1,true);
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	addChild(pArmature);

	}else
	{
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("win");
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Win::animateEnd), COMPLETE);
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	addChild(pArmature);
	}*/
	
	Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("battleover");
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	pArmature->getBone("star1")->setOpacity(score>0?255:0);
	pArmature->getBone("star2")->setOpacity(score>1?255:0);
	pArmature->getBone("star3")->setOpacity(score>2?255:0);
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Win::animateEnd), COMPLETE);
	pArmature->getAnimation()->playByIndex(0);	
	addChild(pArmature);

	//this->runAction(CCSequence::create(CCDelayTime::create(2.0f),CCCallFunc::create(this, callfunc_selector(Win::close)),NULL));
	//preEnd();
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setTouchEnabled(false);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);
    
    CCMenuItem* pItem = DMenuItemImageSound::create(this, menu_selector(Win::menuCallBack));
    pItem->setContentSize(DESIGN_SIZE);
    pItem->setAnchorPoint(CCPointZero);
    pMenu->addChild(pItem);
    
    return true;
}

void Win::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    close();
}

void Win::menuCallBack(cocos2d::CCObject* obj)
{
    close();
}

void Win::close()
{
    removeAllChildren();
    callback();
    removeFromParent();
}

