//
//  VipView.cpp
//  分享
//
//  Created by mzp on 14-04-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "ShareAward.h"
#include "model/ShareModel.h"
#include "managers/ShareManager.h"


USING_NS_CC;



ShareAward* ShareAward::create(const std::string& sShareMsg, const std::string& openUrl, cocos2d::CCArray* pSharePlugins)
{
	ShareAward* pRet = new ShareAward();
	if (pRet && pRet->init(sShareMsg, openUrl, pSharePlugins))
	{
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return NULL;
}

ShareAward::ShareAward()
	: m_pTableView(NULL)
	, m_pContainer(NULL)
	, m_pSharePlugins(NULL)
{

}

ShareAward::~ShareAward()
{
	CC_SAFE_RELEASE(m_pTableView);
	CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pSharePlugins);
}

bool ShareAward::init(const std::string& sShareMsg, const std::string& openUrl, cocos2d::CCArray* pSharePlugins)
{
	if (!DPopup::init())
	{
		return false;
	}

	m_sShareMsg = sShareMsg;
	m_sOpenUrl = openUrl;
    CC_SAFE_RETAIN(pSharePlugins);
    CC_SAFE_RELEASE(m_pSharePlugins);
	m_pSharePlugins = pSharePlugins;

	CCB_READER_AND_ADDCHILD("ui/ccb/account/shareAward");

	m_pTableView = DTableView::create(this, m_pContainer->getContentSize());
	m_pTableView->setDirection(kCCScrollViewDirectionHorizontal);
    m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pTableView->setDelegate(this);
	m_pContainer->addChild(m_pTableView);
	CC_SAFE_RETAIN(m_pTableView);

	return true;
}

bool ShareAward::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);

	return true;
}

cocos2d::SEL_MenuHandler ShareAward::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", ShareAward::closeCallBack);

	return NULL;
}

void ShareAward::closeCallBack(cocos2d::CCObject* obj)
{
	closePopup();
}

void ShareAward::tableCellTouched(cocos2d::extension::CCTableView* table,cocos2d::extension::CCTableViewCell* cell)
{
	if (!cell)
	{
		return;
	}
	
	unsigned int index = cell->getIdx();
	share::ShareInfo* pShareInfo= static_cast<share::ShareInfo*>(m_pSharePlugins->objectAtIndex(index));
	if (!pShareInfo)
	{
		return;
	}
	ShareManager::sharedShareManager()->doShare(pShareInfo->getKey().c_str(), m_sShareMsg.c_str(),m_sOpenUrl.c_str());
    closeCallBack(NULL);
}

cocos2d::CCSize ShareAward::tableCellSizeForIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	return CCSizeMake(130, 110);
}

cocos2d::extension::CCTableViewCell* ShareAward::tableCellAtIndex(cocos2d::extension::CCTableView* table, unsigned int idx)
{
	CCTableViewCell* cell = (CCTableViewCell*)table->dequeueCell();
	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();
	}
    
    cell->removeAllChildren();
    
    share::ShareInfo* pShareInfo= static_cast<share::ShareInfo*>(m_pSharePlugins->objectAtIndex(idx));
    if (pShareInfo)
    {
        CCSprite* pSprite = CCSprite::create(pShareInfo->getIconPath().c_str());
        pSprite->setAnchorPoint(CCPointZero);
        cell->addChild(pSprite);
    }
	return cell;
}

unsigned int ShareAward::numberOfCellsInTableView(cocos2d::extension::CCTableView* table)
{
	if (!m_pSharePlugins)
	{
		return 0;
	}
	
	return m_pSharePlugins->count();
}
