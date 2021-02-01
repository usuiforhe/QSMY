//
//  GuildBossFight.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossFight.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"
#include "GuildBossFightCell.h"

using namespace sp;

GuildBossFight::GuildBossFight()
	: m_pContainer(NULL)
	, m_pBtnBack(NULL)
	, m_pDBtnCheck(NULL)
	, m_pBMFTime(NULL)
	, m_pTTFName(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFCount(NULL)
	, m_pAnimateStar(NULL)
	, m_pBtnRefresh(NULL)
	, m_pBMFRefreshCD(NULL)
	, m_pNodeFreeCount(NULL)
	, m_pNodePayedCount(NULL)
	, m_pDelegate(NULL)
	, m_pCell(NULL)
	, m_pTableView(NULL)
	, m_iSelected(-1)
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_bOnlyDeclared(false)
	, m_fWaitTime(0.f)
	, m_fColdDelay(0.f)
	, m_bReloadCountDown(false)
	, m_iRobDamage(0)
	, m_fRefreshCD(0.f)
	, m_pBtnGuildInfo(NULL)
	, m_pBtnPersonInfo(NULL)
	, m_pTxtEmpty(NULL)
{
}

GuildBossFight::~GuildBossFight()
{
	CC_SAFE_RELEASE(m_pContainer);
	CC_SAFE_RELEASE(m_pBtnBack);
	CC_SAFE_RELEASE(m_pDBtnCheck);
	CC_SAFE_RELEASE(m_pBMFTime);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFCount);
	CC_SAFE_RELEASE(m_pAnimateStar);
	CC_SAFE_RELEASE(m_pBtnRefresh);
	CC_SAFE_RELEASE(m_pBMFRefreshCD);
	CC_SAFE_RELEASE(m_pNodeFreeCount);
	CC_SAFE_RELEASE(m_pNodePayedCount);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pBtnGuildInfo);
	CC_SAFE_RELEASE(m_pBtnPersonInfo);
	CC_SAFE_RELEASE(m_pTxtEmpty);
}

GuildBossFight* GuildBossFight::create()
{
	GuildBossFight *pRet = new GuildBossFight();
	if (pRet && pRet->init())
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

bool GuildBossFight::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	m_pList = CCArray::create();
	CC_SAFE_RETAIN(m_pList);


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("GuildBossMenu", GuildBossMenuLoader::loader());
	//CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/guildBoss/guildBossFight", pNodeLoaderLibrary);
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_fenzheng", pNodeLoaderLibrary);

	CCB_READER("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_fz_cell", m_pCell);
	CC_SAFE_RETAIN(m_pCell);

	m_pTableView = DTableView::create(this, PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pContainer->addChild(m_pTableView);

 	m_pBtnBack->setTarget(this, menu_selector(GuildBossFight::callBackClose));

	m_pDBtnCheck->getButton()->setTarget(this, menu_selector(GuildBossFight::callBackCheck));

	m_pBtnGuildInfo->getButton()->setTarget(this, menu_selector(GuildBossFight::callBackRank));
	m_pBtnPersonInfo->getButton()->setTarget(this, menu_selector(GuildBossFight::callBackContribution));

	SPContainer* pContainer = SPContainer::create(sp::ZORDER);
	m_pBMFStar = CCLabelBMFont::create("", "fonts/plycs60(tsz).fnt");
	CC_SAFE_RETAIN(m_pBMFStar);
	m_pBMFStar->setAnchorPoint(ccp(0.5,0.5));
	pContainer->addChild(m_pBMFStar);
	m_pAnimateStar->replaceNodeInBone(pContainer, "shuzi");
	m_pAnimateStar->setDelegate(this);

	m_pBtnRefresh->getButton()->setTarget(this, menu_selector(GuildBossFight::callBackRefresh));
	//m_pBMFRefreshCD->setVisible(false);

	m_bOnlyDeclared = !m_pModel->getDeclaredShown();
	callBackCheck(NULL);

	m_pNodePayedCount->setVisible(false);

	return true;
}

void GuildBossFight::onEnter()
{
	BINDING_ONLY(m_pModel, this, GuildBossFight::updateTable, D_EVENT_GUILDBOSS_FIGHT_LIST);
	BINDING_EXEC(m_pModel, this, GuildBossFight::updateView, D_EVENT_GUILDBOSS_REFRESH);
	m_pModel->syncAttackTargets();
	m_pModel->syncSelfInfo();
	if (m_pModel->getRobResult()->getRobDamage() >= 0)
	{
		showRobDamage();
	}
	DLayer::onEnter();
}

void GuildBossFight::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossFight::updateRefresh), this);
	m_pModel->setDeclaredShown(m_bOnlyDeclared);
}

bool GuildBossFight::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBack", CCMenuItem *, m_pBtnBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCheck", DButton *, m_pDBtnCheck);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, m_pBMFTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFGuildName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFUserFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCount", CCLabelBMFont *, m_pBMFCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateStar", UIAnimate *, m_pAnimateStar);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRefresh", DButton *, m_pBtnRefresh);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRefreshCD", CCLabelBMFont *, m_pBMFRefreshCD);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFreeCount", CCNode *, m_pNodeFreeCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePayedCount", CCNode *, m_pNodePayedCount);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPersonInfo", DButton *, m_pBtnPersonInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnGuildInfo", DButton *, m_pBtnGuildInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtEmpty", CCNode *, m_pTxtEmpty);
	return false;
}

cocos2d::SEL_MenuHandler GuildBossFight::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", GuildBossFight::callBackClose);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRank", GuildBossFight::callBackRank);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackContribution", GuildBossFight::callBackContribution);
	return NULL;
}

#pragma mark CCTableViewDataSource
CCTableViewCell* GuildBossFight::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	GuildBossFightCell *cell = (GuildBossFightCell*)table->dequeueCell();
	if (!cell)
	{
		cell = GuildBossFightCell::create();
		cell->setDelegate(this);
	}
	if (!m_bReloadCountDown)
	{
		cell->setData((guildboss::GuildBossUserBasic *)m_pList->objectAtIndex(idx));
	}
	cell->setCountDown(m_fColdDelay);
	return cell;
}

uint32_t GuildBossFight::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}

CCSize GuildBossFight::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

#pragma mark CCTableViewDelegate
void GuildBossFight::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}

void GuildBossFight::GuildBossFightUser(GuildBossFightCell *pNode, guildboss::GuildBossUserBasic *pTarget)
{
	if (m_fColdDelay > 0.f)
	{
		Tips_Alert(D_LOCAL_STRING("GuildBossErrorColdDelay").c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossFightUser(pNode, pTarget);
	}

}

void GuildBossFight::animationComplete(UIAnimate *pAnimate)
{
	m_pBMFStar->setVisible(false);
	m_pAnimateStar->setVisible(false);
}

#pragma mark GuildBossFightCellDelegate
void GuildBossFight::callBackClose(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
	
}

void GuildBossFight::callBackRank(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankGuild);
	}
}

void GuildBossFight::callBackContribution(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankContribution);
	}
}

void GuildBossFight::updateTable()
{
	if (m_bOnlyDeclared)
	{
		m_pList = m_pModel->getDeclaredUsers();
	}
	else
	{
		m_pList = m_pModel->getAttackTargets();
	}
	//默认显示
	unsigned int count = 0;
	if(m_pList)count = m_pList->count();
	m_pTxtEmpty->setVisible(count == 0);

	m_bReloadCountDown = false;
	m_pTableView->reloadData();
}

void GuildBossFight::updateView()
{
	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	m_fColdDelay = 0;
	if (nServerTime <= m_pModel->getBossSelf()->getNextTime())
	{
		m_fColdDelay = m_pModel->getBossSelf()->getNextTime() - nServerTime + 1;
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossFight::updateCountDown), this);
		TimerUtil::sharedTimerUtil()->scheduleSelector(schedule_selector(GuildBossFight::updateCountDown), this, 0.5, false);
	}
	m_fWaitTime = m_pModel->getCountingDownOfRob();
	if(m_fWaitTime >= 0.f)
	{
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
		TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	}
	else
	{
		update(0.f);
	}
	m_pNodeFreeCount->setVisible(m_pModel->getBossSelf()->getAvailTimes() > 0);
	m_pNodePayedCount->setVisible(m_pModel->getBossSelf()->getAvailTimes() <= 0);
	int count = m_pModel->getBossSelf()->getAvailTimes();
	if (count <= 0)
	{
		count = m_pModel->getBossSelf()->getPayedTimes();
	}
	m_pBMFCount->setString(D_CSTRING_FROM_INT(count));
	//m_pBMFFight->setString(stringForUIntWith10K(m_pModel->getBossSelf()->getUserFight()));
	m_pBMFFight->setString(stringForUIntWith10K(DM_GET_FORMATION_MODEL->getFormationTotalForce()));
	m_pTTFName->setString(m_pModel->getBoss()->getTargetName().c_str());
	updateTable();
}



void GuildBossFight::updateCountDown(float dt)
{
	m_fColdDelay -= dt;
	if (m_fColdDelay < 0.f)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossFight::updateCountDown), this);
		CCLOG("cold delay cleared!!!!");
	}
	m_bReloadCountDown = true;
	updateTable();
	m_bReloadCountDown = false;
}

void GuildBossFight::update(float dt)
{
	m_fWaitTime-=dt;
	if(m_fWaitTime <= 0.f)
	{
		m_fWaitTime = 0.f;
		TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);

		if (m_pDelegate)
		{
			//m_pModel->resetStageData();
			m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
			return;
		}
	}
	const char *pStr = TimeUtil::getSeconds2String(m_fWaitTime, "%H:%M:%S");
	m_pBMFTime->setString(pStr);
}

void GuildBossFight::callBackCheck(CCObject *pSender)
{
	m_bOnlyDeclared = !m_bOnlyDeclared;
	if (m_bOnlyDeclared)
	{
		m_pDBtnCheck->getButton()->selected();
	}
	else
	{
		m_pDBtnCheck->getButton()->unselected();
	}
	updateTable();
}

void GuildBossFight::showRobDamage()
{
	m_pBMFStar->setVisible(true);
	m_pAnimateStar->setVisible(true);
	m_pBMFStar->setString(stringForUIntWith10K(m_pModel->getRobResult()->getRobDamage()));
	m_pAnimateStar->playAtIndex(0);
	m_iRobDamage = 0;
	m_pModel->getRobResult()->setRobDamage(-1);
}

void GuildBossFight::callBackRefresh(CCObject *pSender)
{
	HTTP_CLIENT->guildBossGetTargetList(m_pModel->getBoss()->getGuildID(), this, callfuncJson_selector(GuildBossFight::callBackRefresh));
}

void GuildBossFight::callBackRefresh(const Json::Value &,const Json::Value &responseData)
{
	m_fRefreshCD = m_pModel->getConfig()->getRefreshCD();
	TimerUtil::sharedTimerUtil()->scheduleSelector(schedule_selector(GuildBossFight::updateRefresh), this, 0.1, false);
}

void GuildBossFight::updateRefresh(float dt)
{
	m_fRefreshCD -= dt;
	if (m_fRefreshCD < 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(GuildBossFight::updateRefresh), this);
	}
	m_pBtnRefresh->getButton()->setEnabled(m_fRefreshCD <= 0.f);
	//m_pBMFRefreshCD->setString(D_CSTRING_FROM_INT(int(m_fRefreshCD)));
	//m_pBMFRefreshCD->setVisible(m_fRefreshCD > 0.f);
}