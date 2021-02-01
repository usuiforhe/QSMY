//
//  MatchView.cpp
//  比赛结果列表
//
//  Created by cll on 13-4-16
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchView.h"
#include "Match.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "utils/TimeUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  MATCH_LIST_SIZE   CCSizeMake(630, 730)
#define  MATCH_CELL_SIZE    CCSizeMake(630, 128)


/************************ match select class begin*******************/


MatchView* MatchView::create(MatchDelegate* pDelegate)
{
	MatchView *pRet = new MatchView();
	if (pRet && pRet->init(pDelegate))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
    
	return pRet;
}

MatchView::MatchView()
:m_pListView(NULL)
,m_pDelegate(NULL)
,m_pMatchModel(NULL)
,m_pMatchInfoVec(NULL)
,m_pSignMenu(NULL)
,m_pCCData(NULL)
,m_pContainer(NULL)
,m_pTimeLabel(NULL)
,m_pTime(NULL)
,m_pVipLabel(NULL)
,m_fTimer(0.f)
,m_nLastIndex(0)
,m_nWatchingIndex(0)
,m_pNodeNoMatch(NULL)
,m_pCompete(NULL)
,m_pCompeted(NULL)
,m_pCompeteMenu(NULL)
{
};

MatchView::~MatchView()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pSignMenu);
    CC_SAFE_RELEASE(m_pCCData);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pTime);
	CC_SAFE_RELEASE(m_pVipLabel);
    CC_SAFE_RELEASE(m_pTimeLabel);
    CC_SAFE_RELEASE(m_pMatchInfoVec);
	CC_SAFE_RELEASE(m_pNodeNoMatch);
	CC_SAFE_RELEASE(m_pCompete);
	CC_SAFE_RELEASE(m_pCompeted);
	CC_SAFE_RELEASE(m_pCompeteMenu);
}
bool MatchView::init(MatchDelegate* pDelegate)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pDelegate = pDelegate;
    
	m_pMatchModel = DM_GET_MATCH_MODEL;
    
    m_pMatchInfoVec = CCArray::create();
    CC_SAFE_RETAIN(m_pMatchInfoVec);
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/match/matchView");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_beisai/PL_beisai_saichang2");

    //m_pCCData = readCCData("ui/ccb/match/matchViewCell");
	m_pCCData = readCCData("PL_ui/ccb/pl_beisai/PL_beisai_saichang_cell");
	
    CC_SAFE_RETAIN(m_pCCData);
    
    
	m_pListView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(MATCH_LIST_SIZE, -PL_MAX_HEIGHT_DIFF));
	m_pListView->setFlyInStyle(kCCTableViewFlyInOnEnter);
	m_pListView->setDirection(kCCScrollViewDirectionVertical);
	m_pListView->setDelegate(this);
	m_pListView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pListView->setAnchorPoint(CCPointZero);
	m_pContainer->addChild(m_pListView);
    m_Pos = CCPoint(0.f,-9999999.f);
    
    //m_pTime->setVisible(false);
	//m_pVipLabel->setVisible(false);
	m_pNodeNoMatch->setVisible(false);
	return true;
    
}

void MatchView::onEnter()
{
    DLayer::onEnter();
	BINDING_ONLY(m_pMatchModel,this,MatchView::updateMatchInfo,D_EVENT_MATCHLIST_UPDATE);
	BINDING_ONLY(m_pMatchModel,this,MatchView::updateMatchInfo,D_EVENT_MATCHINFO_UPDATE);
    updateMatchInfo();
	
}

void MatchView::onExit()
{
	UNBINDING_ALL(m_pMatchModel,this);
    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	DLayer::onExit();
    m_pMatchInfoVec->removeAllObjects();
    m_Pos = m_pListView->getContentOffset();
    m_pListView->reloadData();
    m_pListView->releaseCellsFreed();
}

void MatchView::update(float dt)
{
    m_fTimer-=dt;
    if(m_fTimer<0.f) m_fTimer=0.f;
    //m_pTimeLabel->setString(TimeUtil::getSeconds2String(m_fTimer));
  
}


void MatchView::updateMatchInfo()
{
    if(!m_pMatchModel->isSynced) return;
    
    
    
    m_pMatchModel->getMatchInfoVec(m_pMatchInfoVec);

	m_pCompete->setVisible(!m_pMatchModel->getIsSigned());
	m_pCompeted->setVisible(m_pMatchModel->getIsSigned());
	if(m_pMatchModel->getIsSigned()){
		m_pCompeteMenu->setShaderProgram(D_SHADER_GARY);
		m_pCompeteMenu->setEnabled(false);
	}
    
    //m_pSignMenu->setEnabled(!m_pMatchModel->getIsSigned());
    //m_pTime->setVisible(m_pMatchModel->getIsSigned());
	//m_pVipLabel->setVisible(!m_pMatchModel->getIsSigned());
    
    //if(m_pTime->isVisible())
    //{
    //    m_fTimer = TimeUtil::getDiff(GameManager::sharedGameManager()->getServerTime(),
    //                                 TimeUtil::getTimeToday(TimeUtil::getStartTimeOfTomorrow(GameManager::sharedGameManager()->getServerTime()),8));
    //    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
    //    TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
    //    update(0.f);
    //}
	m_pListView->reloadData();
	/************************************************************************/
	/* 避免出現tablecell fly顯示異常，刪除以下兩條語句                                 */
	/************************************************************************/
    //m_pListView->setContentOffset(m_Pos);
    //m_pListView->updateContentOffset();
	m_pNodeNoMatch->setVisible(!m_pMatchInfoVec->count());
}

void MatchView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}


CCSize MatchView::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return MATCH_CELL_SIZE;
}

CCTableViewCell* MatchView::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	
	MatchViewCell *cell = (MatchViewCell*)table->dequeueCell();
    
	match::MatchInfo *pMatch =  dynamic_cast<match::MatchInfo*>(m_pMatchInfoVec->objectAtIndex(m_pMatchInfoVec->count() - 1 - idx));
    
	if (!cell)
	{
		cell = MatchViewCell::create(m_pCCData);
        //cell->m_pMenuItemReplay->setTarget(this,menu_selector(MatchView::replayCallback));
        //cell->m_pMenuItemAward->setTarget(this,menu_selector(MatchView::awardCallback));
		//cell->m_pButton->getButton()->setTarget(this,menu_selector(MatchView::replayCallback));

		cell->m_pBtnAward->getButton()->setTarget(this,menu_selector(MatchView::awardCallback));
		cell->m_pBtnWatch->getButton()->setTarget(this,menu_selector(MatchView::replayCallback));
	}
	cell->m_pBtnAward->getButton()->setTag(m_pMatchInfoVec->count() - 1 - idx);
	cell->m_pBtnWatch->getButton()->setTag(m_pMatchInfoVec->count() - 1 - idx);

    //cell->m_pButton->getButton()->setTag(m_pMatchInfoVec->count() - 1 - idx);
    //cell->m_pMenuItemReplay->setTag(m_pMatchInfoVec->count() - 1 - idx);
    cell->setRound(idx + 1);
    //cell->m_pMenuItemAward->setTag(m_pMatchInfoVec->count() - 1 - idx);
	cell->setMatchInfo(pMatch);
	return cell;
    
}

uint32_t MatchView::numberOfCellsInTableView(CCTableView *table)
{
	return m_pMatchInfoVec->count();
}

void MatchView::awardCallback(cocos2d::CCObject *sender)
{
    CCMenuItem * pMenuItem = (CCMenuItem*) sender;
    match::MatchInfo *pMatch =  dynamic_cast<match::MatchInfo*>(m_pMatchInfoVec->objectAtIndex(pMenuItem->getTag()));
    HTTP_CLIENT->getCupMatchPrize(pMatch->finalNum, pMatch->mid);
}
void MatchView::replayCallback(cocos2d::CCObject *sender)
{
    CCMenuItem * pMenuItem = (CCMenuItem*) sender;
	m_nWatchingIndex = pMenuItem->getTag();
    match::MatchInfo *pMatch =  dynamic_cast<match::MatchInfo*>(m_pMatchInfoVec->objectAtIndex(pMenuItem->getTag()));
	HTTP_CLIENT->getCupMatchReplay(pMatch->finalNum, pMatch->mid);
    DM_GET_SETTLEMENT_MODEL->setSettlementInfoMatch(pMatch);
}

bool MatchView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSignMenu", CCMenuItem*, this->m_pSignMenu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTime", CCNode*, this->m_pTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipLabel", CCNode*, this->m_pVipLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTimeLabel", CCLabelBMFont*, this->m_pTimeLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNoMatch", CCNode *, m_pNodeNoMatch);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompete", CCSprite *, m_pCompete);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompeted", CCSprite *, m_pCompeted);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCompeteMenu", CCMenuItemImage *, m_pCompeteMenu);
    return false;
}


SEL_MenuHandler MatchView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "signCallBack", MatchView::signCallBack);
    return NULL;
}

void MatchView::signCallBack(cocos2d::CCObject *obj)
{
    if(m_pDelegate)
    {
        m_pDelegate->signUpCup();
    }
}

/************************ match select class end*******************/






/************************ match cell class begin**********************/

MatchViewCell::MatchViewCell()
:m_pMatchInfo(NULL)
,m_pMenuItemReplay(NULL)
,m_pMenuItemAward(NULL)
,m_pTfLevel(NULL)
,m_pImgLose(NULL)
,m_pImgWin(NULL)
,m_pAward(NULL)
,m_pReplay(NULL)
,m_pLive(NULL)
,m_pHeadContainer(NULL)
,m_pTfName(NULL)
,m_pRound(NULL)
//,m_pButton(NULL)
,m_pNodeAward(NULL)
,m_pNodeWatch(NULL)
,m_pBtnAward(NULL)
,m_pBtnWatch(NULL)
{
}


MatchViewCell::~MatchViewCell()
{
    removeAllChildren();
	SAFE_RELEASE_UNBINDING_ALL(m_pMatchInfo, this);
    CC_SAFE_RELEASE(m_pMenuItemReplay);
    CC_SAFE_RELEASE(m_pMenuItemAward);
    CC_SAFE_RELEASE(m_pTfLevel);
    CC_SAFE_RELEASE(m_pImgLose);
    CC_SAFE_RELEASE(m_pImgWin);
    CC_SAFE_RELEASE(m_pAward);
    CC_SAFE_RELEASE(m_pReplay);
	CC_SAFE_RELEASE(m_pLive);
    CC_SAFE_RELEASE(m_pHeadContainer);
    CC_SAFE_RELEASE(m_pRound);
	//CC_SAFE_RELEASE(m_pButton);

	CC_SAFE_RELEASE(m_pNodeAward);
	CC_SAFE_RELEASE(m_pNodeWatch);
	CC_SAFE_RELEASE(m_pBtnAward);
	CC_SAFE_RELEASE(m_pBtnWatch);
}


MatchViewCell* MatchViewCell::create(cocos2d::extension::CCData* pData)
{
    MatchViewCell* pRet = new MatchViewCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {        
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool MatchViewCell::init(cocos2d::extension::CCData* pData)
{
    CCB_READER_DATA_AND_ADDCHILD(pData);
	return true;
}

void MatchViewCell::setMatchInfo(match::MatchInfo* pMatchInfo)
{
	if(pMatchInfo == m_pMatchInfo) return;
	SAFE_RELEASE_UNBINDING_ALL(m_pMatchInfo, this);
	CC_SAFE_RETAIN(pMatchInfo);
	m_pMatchInfo = pMatchInfo;
    BINDING_EXEC(m_pMatchInfo, this, MatchViewCell::updateCellView, D_EVENT_MATCH_AWARD_STATUS_UPDATE);
}

bool MatchViewCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButton", DButton*, this->m_pButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItemReplay", CCMenuItem*, this->m_pMenuItemReplay);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMenuItemAward", CCMenuItem*, this->m_pMenuItemAward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfLevel", CCLabelBMFont*, this->m_pTfLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfName", CCLabelTTF*, this->m_pTfName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgLose", CCSprite*, this->m_pImgLose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgWin", CCSprite*, this->m_pImgWin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAward", CCSprite*, this->m_pAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplay", CCSprite*, this->m_pReplay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "match_sprite_live",CCSprite*, this->m_pLive);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHeadContainer", CCNode*, this->m_pHeadContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRound", CCLabelBMFont*, this->m_pRound);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAward", CCNode*, this->m_pNodeAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWatch", CCNode*, this->m_pNodeWatch);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAward", DButton*, this->m_pBtnAward);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnWatch", DButton*, this->m_pBtnWatch);

    return false;
}

void MatchViewCell::updateCellView()
{
    this->m_pHeadContainer->removeAllChildren();
    this->m_pHeadContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForList(m_pMatchInfo->oppDesp->getAvatarId()));
    this->m_pTfLevel->setString(CCString::createWithFormat("%d",m_pMatchInfo->oppDesp->lv)->getCString());
    this->m_pTfName->setString(m_pMatchInfo->oppDesp->name.c_str());
    this->m_pImgLose->setVisible(!m_pMatchInfo->win);
    this->m_pImgWin->setVisible(m_pMatchInfo->win);
    /*this->m_pAward->setVisible(!m_pMatchInfo->status && m_pMatchInfo->win);
    this->m_pMenuItemAward->setVisible(!m_pMatchInfo->status && m_pMatchInfo->win);*/
	setWatched(m_pMatchInfo->watched);
}

void MatchViewCell::setRound(uint32_t index)
{
    m_pRound->setString(CCString::createWithFormat("%d",index)->getCString());
}


void MatchViewCell::setWatched(bool isWatched)
{
	if (isWatched == false)
	{
		m_pImgWin->setVisible(false);
		m_pImgLose->setVisible(false);
		/*m_pAward->setVisible(false);
		m_pMenuItemAward->setVisible(false);
		m_pLive->setVisible(true);
		m_pReplay->setVisible(false);*/

		m_pNodeAward->setVisible(false);
		m_pNodeWatch->setVisible(true);
	}
	else
	{
		m_pImgLose->setVisible(!m_pMatchInfo->win);
		m_pImgWin->setVisible(m_pMatchInfo->win);
		/*m_pAward->setVisible(!m_pMatchInfo->status && m_pMatchInfo->win);
		m_pMenuItemAward->setVisible(!m_pMatchInfo->status && m_pMatchInfo->win);
		m_pLive->setVisible(false);
		m_pReplay->setVisible(true);*/

		m_pNodeAward->setVisible(!m_pMatchInfo->status && m_pMatchInfo->win);
		m_pNodeWatch->setVisible(!m_pNodeAward->isVisible());
	}
}

/************************ match cell class end**********************/




