//
//  WeeklyMenu.cpp
//  周活动按钮
//
//  Created by wanghejun on 13-7-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "WeeklyMenu.h"
#include "../Game.h"

using namespace navigation;
USING_NS_CC;

#define LABEL_WEEKLY_OFFSET ccp(5,3)
#define LABEL_WEEKLY_WIDTH 69

enum ChildrenWeekTag
{
    kWeekBg = 0,
    kWeekLabel,
    kWeekIcon
};


WeeklyMenu::WeeklyMenu()
:m_bShowing(false)
{
    
}

WeeklyMenu::~WeeklyMenu()
{
}


WeeklyMenu* WeeklyMenu::create()
{
    WeeklyMenu *pRet = new WeeklyMenu();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void WeeklyMenu::updateNavNodes()
{
    removeAllChildren();
   
    //CC_SAFE_RELEASE(m_pItems);
    //m_pItems = CCArray::createWithCapacity(pChildren->count());
    //CC_SAFE_RETAIN(m_pItems);
    
    CCObject* obj = NULL;
    CCMenuItem * pChildItem = NULL;
    CCSprite* pSprite = NULL;
    int8_t index = 0;
	CCDictionary* weeklyList = DM_GET_WEEKLY_MODEL->getWeeklyList();
	unsigned int nId = 0;
	CCDictElement* pEle = NULL;
	CCDICT_FOREACH(weeklyList, pEle)
	{
		weekly::WeeklyTypeInfo *pInfo = dynamic_cast<weekly::WeeklyTypeInfo *>(pEle->getObject());
		if (!pInfo)
		{
			continue;
		}
		
		nId = pInfo->getNav();
		
		navigation::NavigationType navType = static_cast<navigation::NavigationType>(nId);
		
		if (DM_GET_WEEKLY_MODEL->isWeekly(navType) && DM_GET_WEEKLY_MODEL->isWeeklyActive(navType) == false)
		{
			continue;
		}

		CCNode* pNormal = CCNode::create();
		pNormal->setAnchorPoint(CCPointZero);
		pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/home/icon_weekly%d.png",navType)->getCString());

		pSprite->setTag(kWeekIcon);
		pSprite->ignoreAnchorPointForPosition(true);
		pNormal->addChild(pSprite);

		CCSize menuSize = pSprite->getContentSize();

		/*pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/home/hd_icon_wenzi_weekly_%d.png",navType)->getCString());
		pSprite->setTag(kWeekLabel);
		pSprite->ignoreAnchorPointForPosition(true);
		pSprite->setPosition(LABEL_WEEKLY_OFFSET);
		pSprite->setPositionX(LABEL_WEEKLY_OFFSET.x - (pSprite->getContentSize().width - LABEL_WEEKLY_WIDTH)/2);
		pNormal->addChild(pSprite);*/

		CCNode* pSelected  = CCNode::create();
		pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/home/icon_weekly%d_up.png",navType)->getCString());

		pSprite->setTag(kWeekIcon);
		pSprite->ignoreAnchorPointForPosition(true);
		pSelected->addChild(pSprite);        

		//pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/home/hd_icon_wenzi_weekly_%d.png",navType)->getCString());
		//if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
		//pSprite->setTag(kWeekLabel);
		//pSprite->ignoreAnchorPointForPosition(true);
		//pSprite->setPosition(LABEL_WEEKLY_OFFSET);
		//pSprite->setPositionX(LABEL_WEEKLY_OFFSET.x - (pSprite->getContentSize().width - LABEL_WEEKLY_WIDTH)/2);
		//pSelected->addChild(pSprite);

		pChildItem= CCMenuItemSprite::create(pNormal,pSelected,this,menu_selector(WeeklyMenu::menuCallBack));
		pChildItem->setContentSize(menuSize);
		pChildItem->setTag(navType);

		//m_pItems->addObject(pChildItem);
		addChild(pChildItem,index);
	}
}


bool WeeklyMenu::init()
{
    if(!CCMenu::init()) return false;
    updateNavNodes();
    return true;
    
}

//bool WeeklyMenu::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
//{
//    CC_UNUSED_PARAM(event);
//    if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !isEnabled())
//    {
//        hide();
//        return false;
//    }
//    
//    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
//    {
//        if (c->isVisible() == false)
//        {
//            hide();
//            return false;
//        }
//    }
//    
//    m_pSelectedItem = this->itemForTouch(touch);
//    if (m_pSelectedItem)
//    {
//        m_eState = kCCMenuStateTrackingTouch;
//        m_pSelectedItem->selected();
//        return true;
//    }
//    hide();
//    return false;
//}

//void WeeklyMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
//{
//    CC_UNUSED_PARAM(event);
//    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
//    CCMenuItem *currentItem = this->itemForTouch(touch);
//    if (currentItem != m_pSelectedItem)
//    {
//        if (m_pSelectedItem)
//        {
//            m_pSelectedItem->unselected();
//        }
//        m_pSelectedItem = currentItem;
//        if (m_pSelectedItem)
//        {
//            m_pSelectedItem->selected();
//        }
//    }
//}

void WeeklyMenu::hide()
{
    //if(!m_bShowing) return;
    //m_bShowing = false;
    //updateView(true);
}

void WeeklyMenu::menuCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
	int tag = item->getTag();
    if(tag<=0)
    {
        CCLOG("WeeklyMenu error nav");
        return;
    }
    if(m_pDelegate!=NULL)
    {
		m_pDelegate->NavigateTo((NavigationType)tag);
    }
}

void WeeklyMenu::onEnter()
{
    //BINDING_ONLY(DM_GET_NAVIGATION_MODEL, this, WeeklyMenu::updateNavNodes, D_EVENT_NAV_LOCK_STATUS_UPDATE);
	NotificationCenter::sharedNotificationCenter()->addObserver(this, callfunc_selector(WeeklyMenu::updateNavNodes), D_EVENT_WEEKLY_UPDATE);
    CCMenu::onEnter();
}

void WeeklyMenu::onExit()
{
    //UNBINDING_ALL(DM_GET_NAVIGATION_MODEL, this);
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, D_EVENT_WEEKLY_UPDATE);
    CCMenu::onExit();
}


/*
 * override add:
 */
void WeeklyMenu::addChild(CCNode * child)
{
    CCLayer::addChild(child);
}

void WeeklyMenu::addChild(CCNode * child, int zOrder)
{
    CCLayer::addChild(child, zOrder);
}

void WeeklyMenu::addChild(CCNode * child, int zOrder, int tag)
{
    CCLayer::addChild(child, zOrder, tag);
}