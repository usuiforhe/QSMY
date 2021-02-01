//
//  Prize.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "managers/GameManager.h"
#include "Prize.h"
#include "PrizeCell.h"
#include "../Game.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PRIZE_CELL_SIZE CCSizeMake(640, 110)

bool Prize::init()
{
    if(DM_GET_PRIZE_MODEL->isDone())
    {
        return false;
    }
    
    if(!DLayer::init())
    {
        return false;
    }
    
    CCB_READER_AND_ADDCHILD("ui/ccb/activity/getAward");
    
    m_pDTableView = DTableView::create(this, m_pContainer->getContentSize());
    m_pDTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pDTableView->setDelegate(this);
	m_pDTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pDTableView->setPosition(CCPointZero);
	m_pContainer->addChild(m_pDTableView);
    
    m_pData = readCCData("ui/ccb/activity/getAwardCell");
    CC_SAFE_RETAIN(m_pData);
    
    m_pCloseButton->getButton()->setTarget(this, menu_selector(Prize::closeCallBack));
    
    m_pInfoContainer = CCNode::create();
    addChild(m_pInfoContainer,1);
    CC_SAFE_RETAIN(m_pInfoContainer);
    
    return true;
}

void Prize::closeInfoView(WarriorViewType type)
{
    if(type==kSkillInfoView && m_lastType==kWarriorInfo)
    {
        showInfoView(kWarriorInfo, WarriorDelegate::m_pWarrior);
    }else
    {
        m_pInfoContainer->removeAllChildren();
        WarriorDelegate::closeInfoView(type);
    }
}

void Prize::closeInfoView(EquipViewType)
{
    m_pInfoContainer->removeAllChildren();
}

void Prize::closeInfoView(InventoryType type)
{
    m_pInfoContainer->removeAllChildren();
}

void Prize::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
	
	m_pInfoContainer->removeAllChildren();
    
	if(m_pItemInfoView == NULL)
	{
		m_pItemInfoView = ItemInfoView::create();
		m_pItemInfoView->setDelegate(this);
		CC_SAFE_RETAIN(m_pItemInfoView);
	}
    
	m_pItemInfoView->setItemInfo(pItemInfo);
	m_pInfoContainer->addChild(m_pItemInfoView);
    
}

void Prize::showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param)
{
    WarriorDelegate::showInfoView(type, pWarrior, param);
    m_pInfoContainer->removeAllChildren();
    switch (type) {
        case kSkillInfoView:
        {
            if(m_pSkillInfo==NULL)
            {
                m_pSkillInfo = SkillInfoView::create();
                m_pSkillInfo->setDelegate(this);
                CC_SAFE_RETAIN(m_pSkillInfo);
            }
            CCInteger* pSkillId = static_cast<CCInteger*>(param);
            m_pSkillInfo->setSkillInfo(pWarrior,pSkillId->getValue());
            m_pInfoContainer->addChild(m_pSkillInfo);
            break;
        }
        case kWarriorInfo:
        {
            if(m_pWarrior==NULL)
            {
                m_pWarrior = WarriorInfoView::create();
                m_pWarrior->setDelegate(this);
                CC_SAFE_RETAIN(m_pWarrior);
            }
            m_pWarrior->setWarrior(pWarrior);
            m_pInfoContainer->addChild(m_pWarrior);
            break;
        }
        default:
            break;
    }
}

void Prize::showInfoView(EquipViewType type, equip::EquipCurInfo * pEquip,cocos2d::CCObject* param)
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

void Prize::onEnter()
{
    DLayer::onEnter();
    DM_GET_PRIZE_MODEL->syncFromServer();
    
    BINDING_EXEC(DM_GET_PRIZE_MODEL, this, Prize::updateView, D_EVENT_PRIZE_INIT);
    BINDING_ONLY(DM_GET_PRIZE_MODEL, this, Prize::updateView, D_EVENT_PRIZE_UPDATE);
}

void Prize::onExit()
{
    DLayer::onExit();
    UNBINDING_ALL(DM_GET_PRIZE_MODEL, this);
}

bool Prize::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", cocos2d::CCNode *, m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, m_pCloseButton);
    
    return false;
}

cocos2d::CCSize Prize::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return PRIZE_CELL_SIZE;
}

cocos2d::extension::CCTableViewCell* Prize::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    PrizeCell * pCell = static_cast<PrizeCell *>(table->dequeueCell());
    if(pCell == NULL)
    {
        pCell = PrizeCell::create(m_pData);
        pCell->setDelegate(this);
    }
    prize::PrizeItem * pPrizeItem = (prize::PrizeItem *) m_pPrizeList->objectAtIndex(idx);
    pCell->setPrizeItem(pPrizeItem);
    
    return pCell;
}

unsigned int Prize::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    if(m_pPrizeList == NULL)
    {
        return 0;
    }
    
    return m_pPrizeList->count();
}

void Prize::updateView()
{
    if(!DM_GET_PRIZE_MODEL->isSynced)
    {
        return;
    }

    CC_SAFE_RELEASE(m_pPrizeList);
    m_pPrizeList = DM_GET_PRIZE_MODEL->getNotDrawnPrizeList();
    CC_SAFE_RETAIN(m_pPrizeList);
    while (true)
    {
        CCTableViewCell * pCell = m_pDTableView->dequeueCell();
        if(pCell == NULL) break;
    }
    m_pDTableView->reloadData();
}

void Prize::closeCallBack(cocos2d::CCObject *sender)
{
    Game::sharedGame()->NavigateTo(navigation::kNavHome);
}

void Prize::onBackKey()
{
	if (m_pInfoContainer && m_pInfoContainer->getChildrenCount())
	{
		m_pInfoContainer->removeAllChildren();
		return;
	}
	
	BackKeyInterface::onBackKey();
}