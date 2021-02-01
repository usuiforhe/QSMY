//
//  Navigation.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Navigation.h"
#include "../Game.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace navigation;

#define LABEL_OFFSET ccp(-20,-10)
#define ICON_OFFSET ccp(5,10)

Navigation::Navigation()
:m_pDelegate(NULL)
,m_pNavigationModel(NULL)
,m_pContainer(NULL)
,m_kNav(kNavNIL)

,m_pArmature(NULL)
{
    
}

Navigation::~Navigation()
{
    CC_SAFE_RELEASE(m_pContainer);
    UNBINDING(m_pNavigationModel, this, D_EVENT_NAV_LOCK_STATUS_UPDATE);
    CC_SAFE_RELEASE(m_pNavigationModel);
}

bool Navigation::init()
{
    if (!DLayer::init())
    {
        return false;
    }
    
    m_pNavigationModel = DM_GET_NAVIGATION_MODEL;
    CC_SAFE_RETAIN(m_pNavigationModel);
    /* Read a ccbi file. */
    CCB_READER_AND_ADDCHILD("ui/ccb/nav");
    
	m_pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("activeness");
	addChild(m_pArmature);

    return true;
}

bool Navigation::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    return false;
}

SEL_MenuHandler Navigation::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menuCallBack", Navigation::menuCallBack);
    return NULL;
}

void Navigation::menuCallBack(CCObject *sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	int tag = item->getTag();
    if(tag<=0)
    {
        CCLOG("nav error nav");
        return;
    }
    if(m_pDelegate!=NULL)
    {
		m_pDelegate->NavigateTo((NavigationType)tag);
    }
}

void Navigation::setVisible(bool b)
{
    if(b)
    {
        BINDING_ONLY(m_pNavigationModel, this, Navigation::updateNav, D_EVENT_NAV_LOCK_STATUS_UPDATE);
    }else
    {
        UNBINDING(m_pNavigationModel, this, D_EVENT_NAV_LOCK_STATUS_UPDATE);
    }
    DLayer::setVisible(b);
}

void Navigation::updateNav()
{
    if(m_kNav==kNavNIL) return;
    setNavigateDisplay(m_kNav);
}

void Navigation::setNavigateDisplay(NavigationType value)
{
    m_kNav = value;
    CCArray* pChildren = m_pNavigationModel->getNavigationBrothersByNavType(value);
    m_pContainer->removeAllChildren();

    CCMenu* pMenu = CCMenu::create();
    pMenu->setTouchEnabled(false);
    pMenu->setPosition(CCPointZero);
    m_pContainer->addChild(pMenu);
    
    
    CCObject* obj = NULL;
    NavigationNode* pChild = NULL;
    CCSprite*pSprite = NULL;
    CCMenuItem * pChildItem = NULL;
    int8_t index =0;
    CCARRAY_FOREACH(pChildren, obj)
    {
        pChild = (NavigationNode*)obj;
        if(!pChild->isUnlocked()) continue;
        
        CCNode* pNormal = CCNode::create();
        
//        pSprite = CCSprite::create("ui/nav/bg_btn_n.png");
//        pSprite->setAnchorPoint(CCPointZero);
//        pNormal->addChild(pSprite);
//        CCSize menuSize = pSprite->getContentSize();
        
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/icon_%d.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ICON_OFFSET);
        pNormal->addChild(pSprite);
        
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/img_%d_n.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(LABEL_OFFSET);
        pNormal->addChild(pSprite);
        
        
        
        
        CCNode* pSelected  = CCNode::create();
        pSprite = CCSprite::create("ui/nav/bg_btn.png");
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ICON_OFFSET);
        pSelected->addChild(pSprite);
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/icon_%d.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ICON_OFFSET);
        pSelected->addChild(pSprite);
        
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/img_%d_h.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(LABEL_OFFSET);
        pSelected->addChild(pSprite);
        
        
        CCNode* pDisabled  = CCNode::create();
        pSprite = CCSprite::create("ui/nav/bg_btn.png");
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ICON_OFFSET);
        pDisabled->addChild(pSprite);
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/icon_%d.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ICON_OFFSET);
        pDisabled->addChild(pSprite);
        
        pSprite = CCSprite::create(CCString::createWithFormat("ui/nav/img_%d_h.png",pChild->m_nId)->getCString());
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(LABEL_OFFSET);
        pDisabled->addChild(pSprite);
        
        
        
        pChildItem= CCMenuItemSprite::create(pNormal,pSelected,pDisabled,this,menu_selector(Navigation::menuCallBack));
        pChildItem->setContentSize(CCSizeMake(120, 110));
        pChildItem->setTag(pChild->m_kNavigationType);
        pChildItem->setPosition(ccp(index*130,-5));
        pMenu->addChild(pChildItem);
        
        pChildItem->setEnabled(pChild->m_kNavigationType!=value);
        
        ++index;
    }
    
    //TODO: 导航分页分类型显示
    setVisible(true);
    switch (value) {
        case kNavHome:              //首页
            setVisible(false);
            break;
        case kNavPve:               //pve
            
            break;
        case kNavFormation:         //阵容
            
            break;
		case kNavExperience:
			
			break;
        case kNavWonder:            //奇遇（活动）
            
            break;
        case kNavMall:              //市集（商城）
			
            break;
        case kNavLadder:            //论剑 pvp
            
            break;
        case kNavInventory:         //背包
            
            break;
        //case kNavBook:              //武林谱（图鉴）
        //
        //    break;
        //case kNavAnnounce:            //公告
        //    setVisible(false);
            break;
        case kNavSetting:           //设置
            setVisible(false);
            break;
        case kNavWarrior:           //弟子
            
			break;
        case kNavEquip:             //装备
            
            break;
        case kNavTreasure:          //心法
            
            break;
        case kNavDestiny:           //意志
            
            break;
        case kNavLottery:           //抽卡
            
            break;
        case kNavMatch:             //杯赛
           
            break;
        case kNavActiveness:        //活跃度
            setVisible(false);
            break;
        case kNavMail:              //邮件
            setVisible(false);
            break;
        case kNavWish:              //许愿
            break;
        case kNavPrize:             //领奖
            setVisible(false);
            break;
        case kNavChargePrize:       //累充
            setVisible(false);
            break;
        case kNavStrategy:       //累充
            setVisible(false);
            break;
		case kNavAccount:
			break;
        default:
            break;
    }

	CCNode *pItem = pMenu->getChildByTag(kNavChangeFormation);
	if (pItem && value != kNavChangeFormation && !DM_GET_FORMATION_MODEL->isFormationFull())
	{
		m_pArmature->setOpacity(255);
		CCPoint pt = pMenu->convertToWorldSpace(pItem->getPosition());
		CCPoint pos = convertToNodeSpace(pt);
		m_pArmature->setPosition(pos);
	}
	else
	{
		m_pArmature->setOpacity(0);
	}
	
}
