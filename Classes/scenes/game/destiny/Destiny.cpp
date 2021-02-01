//
//  Destiny.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Destiny.h"
#include "DestinyView.h"
#include "DestinyInfoView.h"
#include "DestinyWarriorView.h"
#include "../../../components/components.h"
#include "../../../managers/GameManager.h"
#include "../user/UserInfo.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  TAB_RECT		CCRectMake(0, 170, 620, 580)

#pragma mark - -------------------------------------------------
#pragma mark - ---------------------Destiny---------------------
#pragma mark - -------------------------------------------------
#pragma mark - - init etc.
Destiny::Destiny()
:m_pContainer(NULL)
,m_pDestinyInfoViewNoAward(NULL)
,m_pDestinyInfoViewAward(NULL)
,m_pDestinyInfoViewAny(NULL)
,m_pDestinyWarriorView(NULL)
,m_pDTabView(NULL)
,m_pWarriorModel(NULL)
,m_pDestinyModel(NULL)
,m_pFormationModel(NULL)
,m_pInfoContainer(NULL)
,m_pItemInfo(NULL)
{
    
}
Destiny::~Destiny()
{
    CC_SAFE_RELEASE(m_pDestinyInfoViewNoAward);
    CC_SAFE_RELEASE(m_pDestinyInfoViewAward);
    CC_SAFE_RELEASE(m_pDestinyInfoViewAny);
    CC_SAFE_RELEASE(m_pDestinyWarriorView);
    CC_SAFE_RELEASE(m_pDTabView);
    CC_SAFE_RELEASE(m_pItemInfo);
}
bool Destiny::init()
{
    if(!DLayer::init()) return false;
    m_pContainer = CCNode::create();
    addChild(m_pContainer,1);
    
    m_pInfoContainer = CCNode::create();
    addChild(m_pInfoContainer,2);
    
    
    m_pDestinyModel = DM_GET_DESTINY_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    
    CCB_READER_AND_ADDCHILD("ui/ccb/destiny/destiny");
    
    
    m_pDTabView = DTabView::create(TAB_RECT,360);
	m_pDTabView->retain();
    addChild(m_pDTabView);
    
	std::string tabName[] = {"font_sheet_tianzhi","font_sheet_wangming"};
    
    //初始化天命界面
	for(uint8_t i = (uint8_t)destiny::kDestinyRequireAssign; i <= (uint8_t)destiny::kDestinyRequireAny; ++i)
	{
        DestinyView *pDestinyView = DestinyView::create((destiny::DestinyRequireType)i);
		pDestinyView->setDelegate(this);
		m_pDTabView->addTabWithFontImg(pDestinyView,CCString::createWithFormat("ui/common/%s.png",tabName[i-1].c_str())->getCString());
	}
    
    
    UserInfoView::sharedUserInfoView()->removeFromParent();
    addChild(UserInfoView::sharedUserInfoView());
    
    return true;
}

void Destiny::onEnter()
{
    //BINDING_EXEC(m_pDestinyModel, this, Destiny::updateView, D_EVENT_DESTINYS_UPDATED);
    BINDING_ONLY(m_pWarriorModel, this, Destiny::updateCanCollectStatus, D_EVENT_WARRIORLIST_UPDATE);
    BINDING_ONLY(m_pWarriorModel, this, Destiny::updateCanCollectStatus, D_EVENT_SOULLIST_UPDATE);
    BINDING_ONLY(m_pFormationModel, this, Destiny::updateCanCollectStatus, D_EVENT_FORMATION_UPDATE);
    updateCanCollectStatus();
    updateSoulList();
    DLayer::onEnter();
}

void Destiny::onExit()
{
    DLayer::onExit();
    //UNBINDING_ALL(m_pDestinyModel, this);
    UNBINDING_ALL(m_pWarriorModel, this);
    UNBINDING_ALL(m_pFormationModel, this);
    
}


void Destiny::updateSoulList()
{
    HTTP_CLIENT->getSouls();
}

void Destiny::updateCanCollectStatus()
{
    CCObject* obj = NULL;
    destiny::DestinyInfo* pDestinyInfo = NULL;
    cocos2d::CCArray* pArray = m_pDestinyModel->getDestinyListByRequireType(destiny::kDestinyRequireAssign);
    
    CCARRAY_FOREACH(pArray, obj)
    {
        pDestinyInfo = (destiny::DestinyInfo*)obj;
        pDestinyInfo->setCanCollectDirty();
    }
    
    pArray = m_pDestinyModel->getDestinyListByRequireType(destiny::kDestinyRequireAny);
    CCARRAY_FOREACH(pArray, obj)
    {
        pDestinyInfo = (destiny::DestinyInfo*)obj;
        pDestinyInfo->setCanCollectDirty();
     }
}

//void Destiny::updateView()
//{
//    
//}

#pragma mark - - DestinyDelegate
void Destiny::showDestinyInfoView(destiny::DestinyInfo*pDestinyInfo)
{
    m_pContainer->removeAllChildren();
    DestinyInfoView* pInfoView = NULL;
    
    switch (pDestinyInfo->getRequireType())
    {
        case destiny::kDestinyRequireAny:
            if(!m_pDestinyInfoViewAny)
            {
                //初始化天命信息界面
                m_pDestinyInfoViewAny = DestinyInfoViewAny::create();
                m_pDestinyInfoViewAny->retain();
            }
            pInfoView = m_pDestinyInfoViewAny;
            break;
        case destiny::kDestinyRequireAssign:
            if(pDestinyInfo->isNeedReAward())
            {
                if(!m_pDestinyInfoViewAward)
                {
                    //初始化天命信息界面
                    m_pDestinyInfoViewAward = DestinyInfoViewAward::create();
                    m_pDestinyInfoViewAward->retain();
                }
                pInfoView = m_pDestinyInfoViewAward;
            }
            else
            {
                if(!m_pDestinyInfoViewNoAward)
                {
                    //初始化天命信息界面
                    m_pDestinyInfoViewNoAward = DestinyInfoViewNoAward::create();
                    m_pDestinyInfoViewNoAward->retain();
                }
                pInfoView = m_pDestinyInfoViewNoAward;
            }
            break;
        default:
            break;
    }
    pInfoView->setDelegate(this);
    pInfoView->setDestinyInfo(pDestinyInfo);
    m_pContainer->addChild(pInfoView);
}

void Destiny::showDestinyWarriorView(destiny::DestinyInfo* pDestinyInfo, uint8_t slotIndex)
{
    m_pContainer->removeAllChildren();
    
    if(!m_pDestinyWarriorView)
    {
        m_pDestinyWarriorView = DestinyWarriorView::create();
        m_pDestinyWarriorView->retain();
    }
    m_pDestinyWarriorView->setDelegate(this);
    m_pDestinyWarriorView->setDestinyInfo(pDestinyInfo, slotIndex);
    m_pContainer->addChild(m_pDestinyWarriorView);
}

void Destiny::improveDestinys(uint32_t did, Json::Value &warriors , Json::Value &souls,cocos2d::CCObject* pSelectorTarget, SEL_CallFuncJson selector)
{
    HTTP_CLIENT->improveDestinys(did,warriors,souls,pSelectorTarget,selector);
}

void Destiny::reward(uint32_t id)
{
    HTTP_CLIENT->destinyReward(id);
}


void Destiny::closeInfoView(InventoryType type)
{
    m_pInfoContainer->removeAllChildren();
}

void Destiny::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
	
	m_pInfoContainer->removeAllChildren();
    
	if(m_pItemInfo == NULL)
	{
		m_pItemInfo = ItemInfoView::create();
		m_pItemInfo->setDelegate(this);
		CC_SAFE_RETAIN(m_pItemInfo);
	}
    
	m_pItemInfo->setItemInfo(pItemInfo);
	m_pInfoContainer->addChild(m_pItemInfo);
    
}

void Destiny::closeInfoView(EquipViewType)
{
    m_pInfoContainer->removeAllChildren();
}


void Destiny::showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param)
{
    m_pInfoContainer->removeAllChildren();
    switch (type) {
        case kEquipInfo:
			{
				//if(m_pEquip==NULL)
				//{
				//    m_pEquip = EquipInfoView::create();
				//    m_pEquip->setDelegate(this);
				//    CC_SAFE_RETAIN(m_pEquip);
				//}
				EquipInfoView* m_pEquip = EquipInfoView::create();
				m_pEquip->setDelegate(this);

				m_pEquip->setEquip(pEquip);
				m_pInfoContainer->addChild(m_pEquip);
				break;
			}
        default:
            break;
    }
}

void Destiny::onBackKey()
{
	if (m_pInfoContainer && m_pInfoContainer->getChildrenCount())
	{
		m_pInfoContainer->removeAllChildren();
		return;
	}
	if (m_pContainer && m_pContainer->getChildrenCount())
	{
		m_pContainer->removeAllChildren();
		return;
	}
	
	BackKeyInterface::onBackKey();
}