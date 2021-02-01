//
//  Guide.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Guide.h"
#include "managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

Guide::Guide()
:m_pContainer(NULL)
,m_pCellData(NULL)
,m_pTableView(NULL)
{
    
}

Guide::~Guide()
{
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pCellData);
    CC_SAFE_RELEASE(m_pTableView);
}

bool Guide::init()
{
    if(!DLayer::init()) return false;
    CCB_READER_AND_ADDCHILD("ui/ccb/settings/guide");
    m_pCellData =readCCData("ui/ccb/settings/guideCell");
    
    CC_SAFE_RETAIN(m_pCellData);
    
    
    m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
    m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setDelegate(this);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pContainer->addChild(m_pTableView);
    CC_SAFE_RETAIN(m_pTableView);
    
    return true;
}

bool Guide::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    return false;
}

cocos2d::CCSize Guide::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return CCSizeMake(580, 255);
}

void Guide::tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell)
{
    
}

cocos2d::extension::CCTableViewCell* Guide::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
    GuideCell* pCell =  static_cast<GuideCell*>(table->dequeueCell());
    if(!pCell)
    {
        pCell = GuideCell::create(m_pCellData);
    }
    
    settings::GuideInfo* pInfo = static_cast<settings::GuideInfo*>(DM_GET_SETTING_MODEL->getGuides()->objectAtIndex(idx));
    pCell->setGuideInfo(pInfo);
    return pCell;
}


uint32_t Guide::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return DM_GET_SETTING_MODEL->getGuides()->count();
}



#pragma mark -

GuideCell::GuideCell()
:m_pInfoTTF(NULL)
,m_pGuideInfo(NULL)
,m_pTitleTTF(NULL)
{
}

GuideCell::~GuideCell()
{
    CC_SAFE_RELEASE(m_pInfoTTF);
    CC_SAFE_RELEASE(m_pTitleTTF);
}

GuideCell* GuideCell::create(cocos2d::extension::CCData* pData)
{
    GuideCell* pRet = new GuideCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
    
}

bool GuideCell::init(cocos2d::extension::CCData* pData)
{
    if(!CCTableViewCell::init()) return false;
    CCB_READER_DATA_AND_ADDCHILD(pData);
    return true;
    
}


bool GuideCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pInfoTTF",CCLabelTTF*,m_pInfoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pTitleTTF",CCLabelTTF*,m_pTitleTTF);
    return false;
}

void GuideCell::setGuideInfo(settings::GuideInfo* pInfo)
{
    if(m_pGuideInfo == pInfo) return;
    m_pGuideInfo = pInfo;
    updateView();
}


void GuideCell::updateView()
{
    if(m_pGuideInfo ==NULL) return;
    m_pInfoTTF->setString(m_pGuideInfo->getInfo().c_str());
    m_pTitleTTF->setString(m_pGuideInfo->getTitle().c_str());
}


