//
//  Announce.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Announce.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "AnnounceCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define ANNOUNCE_CELL_PADDING 5

Announce::Announce()
: m_pContainer(NULL)
, m_pTableView(NULL)
, m_pData(NULL)
{
    
}

Announce::~Announce()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pData);
}

bool Announce::init()
{
    if(!DPopup::init())
    {
        return false;
    }
    
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_gongkaiting/pl_gongkaiting");
    
    m_pData = readCCData("PL_ui/ccb/pl_gongkaiting/pl_gongkaiting_cell");
    CC_SAFE_RETAIN(m_pData);
    
	m_pTableView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setPosition(ccp(10,0));
	m_pContainer->addChild(m_pTableView);
    
	return true;

}


bool Announce::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", cocos2d::CCNode *, m_pContainer);
    return false;
}

cocos2d::SEL_MenuHandler Announce::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", Announce::closeCallBack);
    return NULL;
}

cocos2d::CCSize Announce::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    if(m_sizeMap.find(idx)==m_sizeMap.end())
    {
        announce::AnnounceInfo * pAnnounce = static_cast<announce::AnnounceInfo *>(DM_GET_ANNOUNCE_MODEL->getAnnounceList()->objectAtIndex(idx));
        AnnounceCell * pCell = AnnounceCell::create(m_pData);
        pCell->setAnnounce(pAnnounce);
        m_sizeMap.insert(std::make_pair(idx, CCSizeMake(ANNOUNCE_CELL_WIDTH, pCell->getHeight() + ANNOUNCE_CELL_HEIGHT_PLUS)));
    }
    return m_sizeMap[idx];
}

cocos2d::extension::CCTableViewCell * Announce::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    AnnounceCell * pCell = static_cast<AnnounceCell *>(table->dequeueCell());
    
    if(pCell == NULL)
    {
        pCell = AnnounceCell::create(m_pData);
    }
    
    announce::AnnounceInfo * pAnnounce = static_cast<announce::AnnounceInfo *>(DM_GET_ANNOUNCE_MODEL->getAnnounceList()->objectAtIndex(idx));
    pCell->setDelegate(this);
    pCell->setAnnounce(pAnnounce);
    
    return pCell;
}

unsigned int Announce::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return DM_GET_ANNOUNCE_MODEL->getAnnounceList()->count();
}

void Announce::closeCallBack(cocos2d::CCObject *sender)
{
    closePopup();
}

void Announce::NavigationTo(announce::AnnounceInfo * pAnnounce)
{
    if(!pAnnounce->isNeedNav()) return;
    if(!pAnnounce->getUrl().empty())
    {
        SystemUtil::openURL(pAnnounce->getUrl().c_str());
        return;
    }
    Game::sharedGame()->NavigateTo(pAnnounce->getNavType(), false, CCInteger::create(pAnnounce->getNavTag()));
    closeCallBack(NULL);
}


