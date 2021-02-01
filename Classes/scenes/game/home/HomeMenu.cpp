//
//  HomeMenu.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "HomeMenu.h"
#include "../Game.h"

using namespace navigation;
USING_NS_CC;


#define LABEL_OFFSET ccp(8,-10)
#define ICON_OFFSET ccp(-5,-10)
#define LABEL_WIDTH 69

static const cocos2d::CCPoint MENU_POS[] =
{
	ccp(-80,80),    //一
	ccp(80,80),    //二
	ccp(-80,-70),   //三
	ccp(80,-70)    //四
};

enum ChildrenTag
{
    kTagBg = 0,
    kTagLabel,
    kTagIcon
};


HomeMenu::HomeMenu()
:m_pNode(NULL)
,m_pBg(NULL)
,m_pItems(NULL)
,m_bShowing(false)
,m_pToggleCallBackTarget(NULL)
,m_pToggleCallBackSelector(NULL)
{
    
}

HomeMenu::~HomeMenu()
{
    CC_SAFE_RELEASE(m_pItems);
    CC_SAFE_RELEASE(m_pNode);
}


HomeMenu* HomeMenu::create(navigation::NavigationNode *pNode)
{
    HomeMenu *pRet = new HomeMenu();
    if(pRet && pRet->init(pNode))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void HomeMenu::updateNavNodes()
{
    removeAllChildren();
    if(!m_pNode->isUnlocked())
    {
        return;
    }
    CC_SAFE_RELEASE(m_pItems);
    CCArray* pChildren = m_pNode->getChildren();
    m_pItems = CCArray::createWithCapacity(pChildren->count());
    CC_SAFE_RETAIN(m_pItems);
    
    
    DMenuItemImageSound*pItem = DMenuItemImageSound::create(CCString::createWithFormat("PL_ui/home/img_%d_n.png",m_pNode->m_nId)->getCString(),
                                               CCString::createWithFormat("PL_ui/home/img_%d_h.png",m_pNode->m_nId)->getCString(),
                                               this,menu_selector(HomeMenu::toggleMenu));
	pItem->setSound(DM_GET_CONFIG_MODEL->getConfigByKey(MAIN_BUTTON_SOUND_KEY)->getCString());
    pItem->setContentSize(CCSizeMake(79, 146));
    addChild(pItem);
    
    m_pBg = CCSprite::create("PL_ui/home/quan2.png");
    addChild(m_pBg);
    
    CCObject* obj = NULL;
    NavigationNode* pChild = NULL;
    CCMenuItem * pChildItem = NULL;
    CCSprite* pSprite = NULL;
    int8_t index = 0;
    CCARRAY_FOREACH(pChildren, obj)
    {
        pChild = (NavigationNode*)obj;
        
        CCNode* pNormal = CCNode::create();
        pNormal->setAnchorPoint(CCPointZero);
        pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/nav/icon_%d.png",pChild->m_nId)->getCString());//CCSprite::create("ui/nav/bg_btn_n.png");
        if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
        pSprite->setPosition(ICON_OFFSET);
        pSprite->setTag(kTagIcon);
        pSprite->ignoreAnchorPointForPosition(true);
        pNormal->addChild(pSprite);
        
        CCSize menuSize = pSprite->getContentSize();
        
        pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/nav/img_%d_n.png",pChild->m_nId)->getCString());
        if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
        pSprite->setTag(kTagLabel);
        pSprite->ignoreAnchorPointForPosition(true);
        pSprite->setPosition(LABEL_OFFSET);
		pSprite->setPositionX(LABEL_OFFSET.x - (pSprite->getContentSize().width - LABEL_WIDTH)/2);
        pNormal->addChild(pSprite);
        
        
        
        CCNode* pSelected  = CCNode::create();
//         pSprite = CCSprite::create("ui/nav/bg_btn_h.png");
//         if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
//         pSprite->setPosition(ICON_OFFSET);
//         pSprite->setTag(kTagBg);
//         pSprite->ignoreAnchorPointForPosition(true);
//         pSelected->addChild(pSprite);



		pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/nav/icon_%d_down.png",pChild->m_nId)->getCString());
		if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
		pSprite->setPosition(ICON_OFFSET);
		pSprite->setTag(kTagIcon);
		pSprite->ignoreAnchorPointForPosition(true);
		pSelected->addChild(pSprite);
        
//         pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/icon_%d.png",pChild->m_nId)->getCString());
//         if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
//         pSprite->setTag(kTagIcon);
//         pSprite->ignoreAnchorPointForPosition(true);
//         pSelected->addChild(pSprite);
        
        
        pSprite = CCSprite::create(CCString::createWithFormat("PL_ui/nav/img_%d_h.png",pChild->m_nId)->getCString());
        if(!pChild->isUnlocked()) pSprite->setShaderProgram(D_SHADER_GARY);
        pSprite->setTag(kTagLabel);
        pSprite->ignoreAnchorPointForPosition(true);
        pSprite->setPosition(LABEL_OFFSET);
		pSprite->setPositionX(LABEL_OFFSET.x - (pSprite->getContentSize().width - LABEL_WIDTH)/2);
        pSelected->addChild(pSprite);
        
        pChildItem= CCMenuItemSprite::create(pNormal,pSelected,this,menu_selector(HomeMenu::menuCallBack));
        pChildItem->setContentSize(menuSize);
        pChildItem->setTag(pChild->m_kNavigationType);
        pChildItem->setPosition(MENU_POS[index]);
        m_pItems->addObject(pChildItem);
        addChild(pChildItem,index);
        
        ++index;
    }
    //setPosition(CCPointZero);
    updateView(false);
    
}


bool HomeMenu::init(NavigationNode* pNode)
{
    if(!CCMenu::init()) return false;
    CC_SAFE_RETAIN(pNode);
    CC_SAFE_RELEASE(m_pNode);
    m_pNode = pNode;
    updateNavNodes();
    return true;
    
}

bool HomeMenu::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !isEnabled())
    {
        hide();
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            hide();
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    hide();
    return false;
}

void HomeMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem)
    {
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->selected();
        }
    }
}

void HomeMenu::setToggleCallBack(cocos2d::CCObject * target, cocos2d::SEL_CallFuncO selector)
{
    m_pToggleCallBackTarget = target;
    m_pToggleCallBackSelector = selector;
}


void HomeMenu::hide()
{
    if(!m_bShowing) return;
    m_bShowing = false;
    updateView(true);
}

void HomeMenu::menuCallBack(cocos2d::CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
	int tag = item->getTag();
    if(tag<=0)
    {
        CCLOG("HomeMenu error nav");
        return;
    }
    if(m_pDelegate!=NULL)
    {
		//特殊处理苍龙现 有工会且 今天创建的
		if (navigation::kNavGuildBoss == (NavigationType)tag && DM_GET_GUILD_MODEL->getGuildInfo()->getIsCreatedToday() && DM_GET_GUILD_MODEL->getSelfInfo()->getGuildID())
		{
			Tips_Alert(D_LOCAL_STRING("GuildErrorNewGuild").c_str());
		}
		else
		{
			m_pDelegate->NavigateTo((NavigationType)tag);
		}
    }
}

void HomeMenu::toggleMenu(cocos2d::CCObject* obj)
{
    m_bShowing =!m_bShowing;
    updateView(true);
}

void HomeMenu::updateView(bool animate)
{
    if(m_pToggleCallBackTarget != NULL && m_pToggleCallBackSelector!= NULL)
    {
        (m_pToggleCallBackTarget->*m_pToggleCallBackSelector)(this);
    }
    
    if(m_bShowing && getParent())
    {
        static int32_t zOrder = 0;
        getParent()->reorderChild(this, ++zOrder); //确保正在显示的节点能够收到点击事件
    }
    CCObject* obj= NULL;
    CCMenuItemSprite * pItem = NULL;
    CCSprite* pSprite = NULL;
    
    if(!animate)
    {
        if(m_pBg) m_pBg->setScale(m_bShowing?1:0);
        
        CCARRAY_FOREACH(m_pItems, obj)
        {
            pItem = (CCMenuItemSprite*)obj;
            pItem->setVisible(m_bShowing);
            
//             pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagBg);
//             pSprite->setScale(m_bShowing?1:0);
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagIcon);
            pSprite->setScale(m_bShowing?1:0);
            
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagLabel);
            pSprite->setOpacity(m_bShowing?255:0);
        }
        return;
    }
    if(m_bShowing)
    {
        
        if(m_pBg) m_pBg->runAction(CCSequence::create(CCDelayTime::create(0.f),CCEaseBackOut::create(CCScaleTo::create(0.3f,1.f)),NULL));
        uint32_t index = 0;
        CCARRAY_FOREACH(m_pItems, obj)
        {
            pItem = (CCMenuItemSprite*)obj;
            pItem->setVisible(m_bShowing);
            
//             pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagBg);
//             pSprite->runAction(CCSequence::create(CCDelayTime::create(0.02f+0.03f*index),
//                                                   CCSpawn::create(CCFadeIn::create(0.3f),
//                                                                   CCEaseBackOut::create(CCScaleTo::create(0.3f,1.f)),
//                                                                   NULL)
//                                                   ,NULL));
            
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagIcon);
            pSprite->runAction(CCSequence::create(CCDelayTime::create(0.06f+0.03f*index),CCEaseBackOut::create(CCScaleTo::create(0.2f,1.f)),NULL));
            
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagLabel);
            pSprite->runAction(CCSequence::create(CCDelayTime::create(0.06f+0.03f*index),CCFadeIn::create(0.3f),NULL));
            ++index;
        }
    }
    else
    {
        if(m_pBg) m_pBg->stopAllActions();
        if(m_pBg) m_pBg->setScale(m_bShowing?1:0);
        
        CCARRAY_FOREACH(m_pItems, obj)
        {
            pItem = (CCMenuItemSprite*)obj;
            pItem->setVisible(m_bShowing);
            
//             pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagBg);
//             pSprite->stopAllActions();
//             pSprite->setScale(m_bShowing?1:0);
            
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagIcon);
            pSprite->stopAllActions();
            pSprite->setScale(m_bShowing?1:0);
            
            pSprite = (CCSprite*)pItem->getNormalImage()->getChildByTag(kTagLabel);
            pSprite->stopAllActions();
            pSprite->setOpacity(m_bShowing?255:0);
        }
        return;
    }
    
}

void HomeMenu::onEnter()
{
    BINDING_ONLY(DM_GET_NAVIGATION_MODEL, this, HomeMenu::updateNavNodes, D_EVENT_NAV_LOCK_STATUS_UPDATE);
    CCMenu::onEnter();
}

void HomeMenu::onExit()
{
    UNBINDING_ALL(DM_GET_NAVIGATION_MODEL, this);
    CCMenu::onExit();
}


/*
 * override add:
 */
void HomeMenu::addChild(CCNode * child)
{
    CCLayer::addChild(child);
}

void HomeMenu::addChild(CCNode * child, int zOrder)
{
    CCLayer::addChild(child, zOrder);
}

void HomeMenu::addChild(CCNode * child, int zOrder, int tag)
{
    CCLayer::addChild(child, zOrder, tag);
}