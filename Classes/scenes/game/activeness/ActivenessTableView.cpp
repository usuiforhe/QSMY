//
//  ActivenessTableView.cpp
//  QSMY
//
//  Created by cll on 13-6-26.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ActivenessTableView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "managers/TipsManager.h"
#include "Activeness.h"
#include "../Game.h"
#include "../signin/Signin.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace activeness;
using namespace navigation;

#define  ACTIVENESS_CELL_SIZE		CCSizeMake(527, 101)

#pragma mark - ActivenessCell -

ActivenessCell::ActivenessCell():
m_pCompletedLabel(NULL),
//m_pBgInProgress(NULL),
m_pPoint(NULL),
m_pProgress(NULL),
//m_pContainer(NULL),
m_pButton(NULL),
m_pTitle(NULL),
m_pRichTitle(NULL),
m_pInfo(NULL),
m_pUa(NULL)
{
};

ActivenessCell::~ActivenessCell()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pCompletedLabel);
    //CC_SAFE_RELEASE(m_pBgInProgress);
    CC_SAFE_RELEASE(m_pPoint);
    CC_SAFE_RELEASE(m_pProgress);
    //CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pButton);
    CC_SAFE_RELEASE(m_pTitle);
	CC_SAFE_RELEASE(m_pRichTitle);
    CC_SAFE_RELEASE(m_pInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pUa, this);
};

ActivenessCell * ActivenessCell::create(cocos2d::extension::CCData *pData)
{
    ActivenessCell * pRet = new ActivenessCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool ActivenessCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompletedLabel", CCSprite *, this->m_pCompletedLabel);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgInProgress", CCScale9Sprite *, this->m_pBgInProgress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPoint", CCLabelBMFont *, this->m_pPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgress", CCLabelBMFont *, this->m_pProgress);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", CCMenuItem *, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTitle", CCLabelTTF *, this->m_pTitle);
    
    return false;
}

bool ActivenessCell::init(cocos2d::extension::CCData *pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);

	if (NULL == m_pRichTitle) {
		m_pRichTitle = CCRichLabelTTF::create();
		CC_SAFE_RETAIN(m_pRichTitle);
		replaceLabel(this->m_pTitle, m_pRichTitle);
	}

    return CCTableViewCell::init();
}

void ActivenessCell::setActivenessInfo(ActivenessInfo * pInfo , UserActiveness * pUa)
{
    if(m_pInfo == pInfo && m_pUa == pUa)
    {
        return;
	}
	CC_SAFE_RETAIN(pInfo);
    CC_SAFE_RELEASE(m_pInfo);
    m_pInfo = pInfo;
    
    CC_SAFE_RETAIN(pUa);
    SAFE_RELEASE_UNBINDING_ALL(m_pUa, this);
    m_pUa = pUa;
    
    BINDING_EXEC(m_pUa, this, ActivenessCell::updateView, D_EVENT_ACTIVENESS_USER_UPDATE);
}

void ActivenessCell::updateView()
{
    bool completed(false);
    uint32_t times(0);
    if(m_pUa != NULL)
    {
        times = m_pUa->getTimes();
        completed = m_pUa->isTargetAchieved();
    }

	m_pRichTitle->setFontFillColor(m_pTitle->getColor());
	m_pRichTitle->enableStroke(m_pTitle->getColor(), 1.0);
    m_pRichTitle->setString(D_LOCAL_STRING(m_pInfo->getName().c_str()).c_str());
    m_pPoint->setString(CCString::createWithFormat("%d", m_pInfo->getPoint())->getCString());
    m_pProgress->setString(CCString::createWithFormat("%d/%d", times, m_pInfo->getTarget())->getCString());
    
    m_pCompletedLabel->setVisible(completed);
    //m_pBgInProgress->setVisible(!completed);
    //m_pContainer->setVisible(!completed);
    if(!completed)
    {
        m_pButton->setEnabled(true);
        m_pButton->setTarget(this, menu_selector(ActivenessCell::onGotoClick));
    }
}

void ActivenessCell::onGotoClick(cocos2d::CCObject * sender)
{   
	if(m_pInfo->getNavType() == navigation::kNavSignin)
	{
		Game::sharedGame()->openPopup(Signin::create());
	} else 
	{
		Game::sharedGame()->NavigateTo(m_pInfo->getNavType(),false,m_pInfo->getNavTag()>0?CCInteger::create(m_pInfo->getNavTag()):NULL);
	}
    
}

/************************ recharge select class begin*******************/

ActivenessTableView::ActivenessTableView():
m_pListView(NULL),
m_pActivenessList(NULL),
m_pData(NULL)
{};

ActivenessTableView::~ActivenessTableView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pActivenessList);
    CC_SAFE_RELEASE(m_pData);
}

ActivenessTableView * ActivenessTableView::create(const cocos2d::CCSize &size)
{
    ActivenessTableView * pRet = new ActivenessTableView;
    if(pRet && pRet->init(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        pRet = NULL;
    }
    
    return pRet;
}

bool ActivenessTableView::init(const cocos2d::CCSize & size)
{
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pActivenessList = DM_GET_ACTIVENESS_MODEL->getActivenessInfoList();
    CC_SAFE_RETAIN(m_pActivenessList);
    
    m_pData = readCCData("PL_ui/ccb/pl_huoyuedu/PL_huoyuedu_cell");
    CC_SAFE_RETAIN(m_pData);
    
	m_pListView = DTableView::create(this, size);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setPosition(CCPointZero);
	addChild(m_pListView);
    
	return true;    
}

void ActivenessTableView::onEnter()
{    
	BINDING_EXEC(DM_GET_ACTIVENESS_MODEL, this, ActivenessTableView::updateView,D_EVENT_ACTIVENESS_UPDATE);
    
    DLayer::onEnter();
}

void ActivenessTableView::onExit()
{
    
	UNBINDING_ALL(DM_GET_ACTIVENESS_MODEL, this);
	DLayer::onExit();
}

void ActivenessTableView::updateView()
{
	if(!DM_GET_ACTIVENESS_MODEL->isSynced ) return;
    
	m_pListView->reloadData();   
}

CCSize ActivenessTableView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return ACTIVENESS_CELL_SIZE;
}

void ActivenessTableView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{    
    CCLOG("award cell touched at index: %i", cell->getIdx());	
}

CCTableViewCell* ActivenessTableView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	ActivenessCell *cell = (ActivenessCell*)table->dequeueCell();
	if (!cell)
	{
		cell = ActivenessCell::create(m_pData);		
	}
    ActivenessInfo* pInfo = static_cast<ActivenessInfo *>(m_pActivenessList->objectAtIndex(idx));
    ActivenessModel * pActiveness = DM_GET_ACTIVENESS_MODEL;
    UserActiveness* pUa = pActiveness->getActiveness(pInfo->getId());
    cell->setActivenessInfo(pInfo,pUa);
	return cell;
}

uint32_t ActivenessTableView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pActivenessList->count();
    
}

/************************ recharge select class end*******************/


