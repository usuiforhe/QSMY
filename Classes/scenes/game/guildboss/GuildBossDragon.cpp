//
//  GuildBossDragon.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "GuildBossDragon.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "GuildBossMenu.h"
#include "GuildBoss.h"

#include "../battle/BattleDefine.h"

using namespace sp;

GuildBossDragon::GuildBossDragon()
	: m_pReader(NULL)
	, m_pBtnBack(NULL)
	, m_pBtnRescue(NULL)
	, m_pBtnBattle(NULL)
	, m_pBMFCountFree(NULL)
	, m_pBMFCountPayed(NULL)
	, m_pBMFGold(NULL)
	, m_pBMFTime(NULL)
	, m_pBMFFight(NULL)
	, m_pBMFSelfStar(NULL)
	, m_pBMFRescue(NULL)
	, m_pTTFName(NULL)
	, m_pAnimateAttack(NULL)
	, m_pAnimateAttack10(NULL)
	, m_pAnimateDragon(NULL)
	, m_pAnimateAttackOther(NULL)
	, m_pAnimateAttackName(NULL)
	, m_pAnimateDamage(NULL)
	, m_pAnimateRescue(NULL)
	, m_pNodeTen(NULL)
	, m_pNodeFree(NULL)
	, m_pNodePay(NULL)
	, m_pBMFCountDown(NULL)
	, m_pTxtZhan(NULL)
	, m_pNodeAttack(NULL)
	, m_pNodeHealthy(NULL)
	, m_pNodeWounded(NULL)
	, m_pNodeRescue(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_fWaitTime(0.f)
	, m_fColdDelay(0.f)
	, m_pBMFOtherDamage(NULL)
	, m_pTTFOtherName(NULL)
	, m_pBMFDamage(NULL)
	, m_pTTFGBMessage(NULL)
	, m_pNodeNormalAttack(NULL)
	, m_pNodeAutoAttack(NULL)
	, m_pTouchFlashNode(NULL)
	, m_pBtnAutoAttack(NULL)
	, m_pPBLeft(NULL)
	, m_pPBRight(NULL)
	, m_bRunningOtherAnimation(false)
	, m_bProgressActionShown(false)
	, m_fLastPercent(0.f)
	, m_pBtnGuildInfo(NULL)
	, m_pBtnPersonInfo(NULL)
	, m_pCriticalNode(NULL)
{
}

GuildBossDragon::~GuildBossDragon()
{
	CC_SAFE_RELEASE(m_pReader);
	CC_SAFE_RELEASE(m_pBtnBack);
	CC_SAFE_RELEASE(m_pBtnRescue);
	CC_SAFE_RELEASE(m_pBtnBattle);
	CC_SAFE_RELEASE(m_pBMFCountFree);
	CC_SAFE_RELEASE(m_pBMFCountPayed);
	CC_SAFE_RELEASE(m_pBMFGold);
	CC_SAFE_RELEASE(m_pBMFTime);
	CC_SAFE_RELEASE(m_pBMFFight);
	CC_SAFE_RELEASE(m_pBMFSelfStar);
	CC_SAFE_RELEASE(m_pBMFRescue);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pAnimateAttack);
	CC_SAFE_RELEASE(m_pAnimateAttack10);
	CC_SAFE_RELEASE(m_pAnimateDragon);
	CC_SAFE_RELEASE(m_pAnimateAttackOther);
	CC_SAFE_RELEASE(m_pAnimateAttackName);
	CC_SAFE_RELEASE(m_pAnimateDamage);
	CC_SAFE_RELEASE(m_pAnimateRescue);
	CC_SAFE_RELEASE(m_pNodeTen);
	CC_SAFE_RELEASE(m_pNodeFree);
	CC_SAFE_RELEASE(m_pNodePay);
	CC_SAFE_RELEASE(m_pBMFCountDown);
	CC_SAFE_RELEASE(m_pTxtZhan);
	CC_SAFE_RELEASE(m_pNodeNormalAttack);
	CC_SAFE_RELEASE(m_pNodeAttack);
	CC_SAFE_RELEASE(m_pNodeHealthy);
	CC_SAFE_RELEASE(m_pNodeWounded);
	CC_SAFE_RELEASE(m_pNodeRescue);
	CC_SAFE_RELEASE(m_pBMFOtherDamage);
	CC_SAFE_RELEASE(m_pTTFOtherName);
	CC_SAFE_RELEASE(m_pBMFDamage);
	CC_SAFE_RELEASE(m_pTTFGBMessage);
	CC_SAFE_RELEASE(m_pNodeAutoAttack);
	CC_SAFE_RELEASE(m_pTouchFlashNode);
	CC_SAFE_RELEASE(m_pBtnAutoAttack);
	CC_SAFE_RELEASE(m_pPBLeft);
	CC_SAFE_RELEASE(m_pPBRight);
	CC_SAFE_RELEASE(m_pBtnGuildInfo);
	CC_SAFE_RELEASE(m_pBtnPersonInfo);
	CC_SAFE_RELEASE(m_pCriticalNode);
}

GuildBossDragon* GuildBossDragon::create()
{
	GuildBossDragon *pRet = new GuildBossDragon();
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

bool GuildBossDragon::init()
{
	if(!DLayer::init())
	{
		return false;
	}

	m_pModel = DM_GET_GUILDBOSS_MODEL;

	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("GuildBossMenu", GuildBossMenuLoader::loader());

	cocos2d::CCNode *__pNode__ = NULL;
	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(pNodeLoaderLibrary);
	__pNode__ = ___ccbReader__->readNodeGraphFromFile("PL_ui/ccb/pl_banghui/pl_banghui_jianghu_chue", this);
	___ccbReader__->autorelease();
	if(__pNode__) addChild(__pNode__);

	m_pReader = ___ccbReader__;
	CC_SAFE_RETAIN(m_pReader);

	m_pBtnBack->setTarget(this, menu_selector(GuildBossDragon::callBackBack));
	m_pBtnRescue->getButton()->setTarget(this, menu_selector(GuildBossDragon::callBackRescue));
	m_pBtnBattle->setTarget(this, menu_selector(GuildBossDragon::callBackBattle));

	m_pBtnGuildInfo->getButton()->setTarget(this, menu_selector(GuildBossDragon::callBackRank));
	m_pBtnPersonInfo->getButton()->setTarget(this, menu_selector(GuildBossDragon::callBackContribution));

	m_pAnimateAttack->setDelegate(this);
	m_pAnimateAttack10->setDelegate(this);

	uint32_t leaderID = DM_GET_FORMATION_MODEL->getFormationLeader();
	uint32_t avatarID = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(leaderID)->getAvatarId();

	//TODO? replace bone!
	//for (int i = 1; i <= 4; i++)
	//{
	//	CCSprite* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon50PForLottery(avatarID);
	//	const char *pBoneName = CCString::createWithFormat("unit%d", i)->getCString();
	//	m_pAnimateAttack->replaceNodeInBone(pNode, pBoneName);
	//	pNode = ResourceManager::sharedResourceManager()->getWarriorIcon50PForLottery(avatarID);
	//	m_pAnimateAttack10->replaceNodeInBone(pNode, pBoneName);
	//}

	m_pAnimateDragon->setDelegate(this);
	m_pAnimateAttack->setDelegate(this);
	m_pAnimateAttack10->setDelegate(this);
	//m_pAnimateAttackOther->setDelegate(this);
	//m_pAnimateAttackName->setDelegate(this);


	SPContainer* pContainer = SPContainer::create(sp::ZORDER);
	m_pBMFOtherDamage = CCLabelBMFont::create("", "fonts/battle_dmg.fnt");
	CC_SAFE_RETAIN(m_pBMFOtherDamage);
	m_pBMFOtherDamage->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(m_pBMFOtherDamage);
	//m_pAnimateAttackName->replaceNodeInBone(pContainer, "damage");

	pContainer = SPContainer::create(sp::ZORDER);
	m_pTTFOtherName = CCLabelTTF::create("", "Arial", 24);
	CC_SAFE_RETAIN(m_pTTFOtherName);
	m_pTTFOtherName->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(m_pTTFOtherName);
	//m_pAnimateAttackName->replaceNodeInBone(pContainer, "name");

	pContainer = SPContainer::create(sp::ZORDER);
	m_pBMFDamage = CCLabelBMFont::create("", "fonts/battle_dmg.fnt");
	CC_SAFE_RETAIN(m_pBMFDamage);
	m_pBMFDamage->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(m_pBMFDamage);
	//m_pAnimateDamage->replaceNodeInBone(pContainer, "num");
	//m_pAnimateDamage->setDelegate(this);

	pContainer = SPContainer::create(sp::ZORDER);
	m_pTTFGBMessage = CCLabelTTF::create("", D_FONT_DEFAULT,D_FONT_SIZE_10);
	CC_SAFE_RETAIN(m_pTTFGBMessage);
	m_pTTFGBMessage->setAnchorPoint(ccp(0.5,0.5));
	pContainer->addChild(m_pTTFGBMessage);
	//m_pAnimateRescue->replaceNodeInBone(pContainer, "GBMessage");
	//m_pAnimateRescue->setDelegate(this);

	m_pBtnAutoAttack->setTarget(this, menu_selector(GuildBossDragon::callBackAutoAttack));

	guildboss::ColorMap m_mAngerColor = m_pModel->getAngerColor();
	for (guildboss::ColorMap::iterator i = m_mAngerColor.begin(); i != m_mAngerColor.end(); i++)
	{
		m_pPBLeft->setColorWithPercent(i->second, i->first);
		m_pPBRight->setColorWithPercent(i->second, i->first);
	}

	return true;
}
 
void GuildBossDragon::onEnter()
{
	m_pModel->syncWounded();
	m_pModel->syncSelfInfo();
	m_pModel->setDragonShowing(true);
	m_pModel->syncNotices(0.f);
	m_bProgressActionShown = false;
	BINDING_EXEC(m_pModel, this, GuildBossDragon::updateRescue, D_EVENT_GUILDBOSS_WOUNDED_LIST);
	BINDING_EXEC(m_pModel, this, GuildBossDragon::updateView, D_EVENT_GUILDBOSS_REFRESH);
	BINDING_EXEC(m_pModel, this, GuildBossDragon::updateStar, D_EVENT_GUILDBOSS_ADD_DAMAGE)
	BINDING_EXEC(m_pModel, this, GuildBossDragon::playNoticeAnimation, D_EVENT_GUILDBOSS_NOTICE);
	BINDING_ONLY(m_pModel, this, GuildBossDragon::playAutoAttackAnimation, D_EVENT_GUILDBOSS_AUTO_ATTACK);
	BINDING_ONLY(m_pModel, this, GuildBossDragon::playAttackAnimation, D_EVENT_GUILDBOSS_ATTACK);
	DLayer::onEnter();
}

void GuildBossDragon::onExit()
{
	DLayer::onExit();
	UNBINDING_ALL(m_pModel, this);
	TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
	m_pModel->setDragonShowing(false);
}

bool GuildBossDragon::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBack", CCMenuItem *, m_pBtnBack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRescue", DButton *, m_pBtnRescue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnBattle", CCMenuItem *, m_pBtnBattle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCountFree", CCLabelBMFont *, m_pBMFCountFree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCountPayed", CCLabelBMFont *, m_pBMFCountPayed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFGold", CCLabelBMFont *, m_pBMFGold);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFTime", CCLabelBMFont *, m_pBMFTime);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFFight", CCLabelBMFont *, m_pBMFFight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFSelfStar", CCLabelBMFont *, m_pBMFSelfStar);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFRescue", CCLabelBMFont *, m_pBMFRescue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateAttack", UIAnimate *, m_pAnimateAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateAttack10", UIAnimate *, m_pAnimateAttack10);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateDragon", UIAnimate *, m_pAnimateDragon);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateAttackOther", UIAnimate *, m_pAnimateAttackOther);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateAttackName", UIAnimate *, m_pAnimateAttackName);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateDamage", UIAnimate *, m_pAnimateDamage);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTen", CCNode *, m_pNodeTen);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFree", CCNode *, m_pNodeFree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePay", CCNode *, m_pNodePay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBMFCountDown", CCLabelBMFont *, m_pBMFCountDown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtZhan", CCNode *, m_pTxtZhan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAttack", CCNode *, m_pNodeAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeHealthy", CCNode *, m_pNodeHealthy);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeWounded", CCNode *, m_pNodeWounded);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeRescue", CCNode *, m_pNodeRescue);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateRescue", UIAnimate *, m_pAnimateRescue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNormalAttack", CCNode *, m_pNodeNormalAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeAutoAttack", CCNode *, m_pNodeAutoAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTouchFlash", CCNode *, m_pTouchFlashNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAutoAttack", CCMenuItem *, m_pBtnAutoAttack);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBLeft", DProgressBar *, m_pPBLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPBRight", DProgressBar *, m_pPBRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnPersonInfo", DButton *, m_pBtnPersonInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnGuildInfo", DButton *, m_pBtnGuildInfo);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCriticalNode", CCNode *, m_pCriticalNode);
	return false;
}


cocos2d::SEL_MenuHandler GuildBossDragon::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackRank", GuildBossDragon::callBackRank);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackContribution", GuildBossDragon::callBackContribution);
	return NULL;
}

void GuildBossDragon::callBackBack(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavHome);
	}
}

void GuildBossDragon::callBackRescue(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRescue);
	}
}

void GuildBossDragon::callBackBattle(CCObject *pSender)
{
	if (m_fColdDelay > 0.f)
	{
		Tips_Alert(D_LOCAL_STRING("GuildBossErrorColdDelay").c_str());
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossBattleShow(this);
		playTouchAnimation();
	}
}

void GuildBossDragon::callBackRank(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankGuild);
	}
}

void GuildBossDragon::callBackContribution(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossNavigateTo(GuildBossNavRankContribution);
	}
}

void GuildBossDragon::updateView()
{

	uint32_t nServerTime = GameManager::sharedGameManager()->getServerTime();
	m_fColdDelay = 0;
	if (nServerTime < m_pModel->getBossSelf()->getNextTime())
	{
		m_fColdDelay = m_pModel->getBossSelf()->getNextTime() - nServerTime;
	}
	m_fWaitTime = 0;
	m_fWaitTime = m_pModel->getCountingDownOfBoss();
	if(m_fWaitTime >= 0.f)
	{
		TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
		TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
	}
	else
	{
		update(0.f);
	}
	m_pBMFCountFree->setString(D_CSTRING_FROM_INT(m_pModel->getBossSelf()->getAvailTimes()));
	m_pBMFCountPayed->setString(D_CSTRING_FROM_INT(m_pModel->getBossSelf()->getPayedTimes()));
	if (m_pModel->getBossSelf()->getAvailTimes())
	{
		m_pBMFGold->setString("0");
	}
	else
	{
		m_pBMFGold->setString(D_CSTRING_FROM_INT(m_pModel->getConfig()->getAttackCost()));
	}
	//m_pBMFFight->setString(stringForUIntWith10K(m_pModel->getBossSelf()->getUserFight()));
	m_pBMFFight->setString(stringForUIntWith10K(DM_GET_FORMATION_MODEL->getFormationTotalForce()));
	m_pTTFName->setString(m_pModel->getBoss()->getTargetName().c_str());
	m_pNodeTen->setVisible(!m_pModel->getBossSelf()->getAvailTimes() && m_pModel->getBossSelf()->getPayedTimes());
	m_pNodeFree->setVisible(m_pModel->getBossSelf()->getAvailTimes() > 0);
	m_pNodePay->setVisible(!m_pNodeFree->isVisible());
	int attackCount = m_pModel->getBossSelf()->getAvailTimes() + m_pModel->getBossSelf()->getPayedTimes();
	m_pBtnBattle->setEnabled(m_fColdDelay <= 0.f && attackCount);
	m_pNodeWounded->setVisible(!m_pModel->getBossSelf()->getRobBlood());
	m_pNodeHealthy->setVisible(m_pModel->getBossSelf()->getRobBlood() > 0);

	m_pNodeNormalAttack->setVisible(!m_pModel->isSelfAngry());
	m_pNodeAutoAttack->setVisible(m_pModel->isSelfAngry());

	if (!m_bProgressActionShown || m_fLastPercent > m_pModel->getPercentOfSelfAnger())
	{
		m_pPBLeft->setPercent(m_pModel->getPercentOfSelfAnger(), 0.);
		m_pPBRight->setPercent(m_pModel->getPercentOfSelfAnger(), 0.);
		m_bProgressActionShown = true;
	}
	else
	{
		m_pPBLeft->setPercent(m_pModel->getPercentOfSelfAnger(), 0.5);
		m_pPBRight->setPercent(m_pModel->getPercentOfSelfAnger(), 0.5);
	}
	m_fLastPercent = m_pModel->getPercentOfSelfAnger();
}

void GuildBossDragon::animationComplete(UIAnimate *pAnimate)
{
	if (pAnimate == m_pAnimateDragon)
	{
		//m_pAnimateDragon->playWithName("stand");
		m_pAnimateDragon->playAtIndex(0);
		return;
	}
	if (pAnimate == m_pAnimateAttackOther)
	{
		m_pBMFOtherDamage->setVisible(false);
		updateView();
		playOtherAnimation();
		return;
	}
	if (pAnimate == m_pAnimateRescue)
	{
		playRescueAnimation();
		return;
	}
	if (pAnimate == m_pAnimateDamage)
	{
		m_pBMFDamage->setVisible(false);
	}
	updateStar();
}

void GuildBossDragon::updateStar()
{
	m_pBMFSelfStar->setString(stringForUIntWith10K(m_pModel->getBossSelf()->getDamageDaily()));
	m_pBMFCountFree->setString(D_CSTRING_FROM_INT(m_pModel->getBossSelf()->getAvailTimes()));
	m_pBMFCountPayed->setString(D_CSTRING_FROM_INT(m_pModel->getBossSelf()->getPayedTimes()));
}


void GuildBossDragon::update(float dt)
{
	m_fWaitTime-=dt;
	m_fColdDelay -= dt;
	if (m_fColdDelay > 0.f)
	{
		m_pBMFCountDown->setString(D_CSTRING_FROM_INT((int)m_fColdDelay));
	}
	int attackCount = m_pModel->getBossSelf()->getAvailTimes() + m_pModel->getBossSelf()->getPayedTimes();
	m_pBtnBattle->setEnabled(m_fColdDelay <= 0.f && attackCount);
	m_pBMFCountDown->setVisible(m_fColdDelay > 0.f);
	m_pTxtZhan->setVisible(!m_pBMFCountDown->isVisible());
	m_pNodeAttack->setVisible(m_fColdDelay <= 0.f && m_pBtnBattle->isEnabled());
	
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

void GuildBossDragon::playAutoAttackAnimation()
{
	CCArray *pArray = m_pModel->getDamage();
	if (!pArray)
	{
		return;
	}
	updateView();
	{
		m_pAnimateAttack10->playAtIndex(0);
		//m_pAnimateDragon->playWithName("hard straight1");
		m_pAnimateDragon->playAtIndex(1);
	}
	for (uint32_t i = 0; i < pArray->count(); i++)
	{
		guildboss::GuildBossDamage *pDamage = (guildboss::GuildBossDamage *)pArray->objectAtIndex(i);
		CC_SAFE_RETAIN(pDamage);
		CCAction* pAction = CCSequence::create(CCDelayTime::create(i * 0.1),
			CCCallFuncO::create(this, callfuncO_selector(GuildBossDragon::playAutoAttackPart), pDamage),
			NULL
			);
		pAction->setTag(i);
		runAction(pAction);
	}
}

void GuildBossDragon::playAttackAnimation()
{
	CCArray *pArray = m_pModel->getDamage();
	if (!pArray)
	{
		return;
	}
	updateView();
	{
		m_pAnimateAttack->playAtIndex(0);
	//	m_pAnimateDragon->playWithName("hard straight");
		m_pAnimateDragon->playAtIndex(1);
	}
	for (uint32_t i = 0; i < pArray->count(); i++)
	{
		guildboss::GuildBossDamage *pDamage = (guildboss::GuildBossDamage *)pArray->objectAtIndex(i);
		CC_SAFE_RETAIN(pDamage);
		CCAction* pAction = CCSequence::create(CCDelayTime::create(i * 0.1),
			CCCallFuncO::create(this, callfuncO_selector(GuildBossDragon::playAutoAttackPart), pDamage),
			NULL
			);
		pAction->setTag(i);
		runAction(pAction);
	}
}

void GuildBossDragon::playAutoAttackPart(CCObject *pObj)
{
	guildboss::GuildBossDamage *pDamage = NULL;
	pDamage = (guildboss::GuildBossDamage *)pObj;

	//伤害数字
	SPContainer* pContainer = SPContainer::create(sp::ZORDER);
	CCLabelBMFont *pBMFDamage = CCLabelBMFont::create("", "fonts/battle_dmg.fnt");
	//CC_SAFE_RETAIN(pBMFDamage);
	pBMFDamage->setAnchorPoint(ccp(0,0.5));
	pContainer->addChild(pBMFDamage);
	pBMFDamage->setString(stringForUIntWith10K(pDamage->getDamage()));

	//飘起来伤害数字
	sp::Armature *pArmatureNumber = ResourceManager::sharedResourceManager()->getUIAnimateOnly("guildwars_number", sp::ZORDER);
	pArmatureNumber->getBone("shuzi_1")->replaceDisplayAt(pContainer, 0);
	//pArmatureNumber->setPosition(m_pAnimateDragon->getPosition());
	m_pCriticalNode->addChild(pArmatureNumber);
	pArmatureNumber->getAnimation()->playByIndex(0);
	pArmatureNumber->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildBossDragon::armatureComplete), COMPLETE);

	//暴击
	if(pDamage->getCritical() > 1)
	{
		std::string critical = (pDamage->getCritical() == 2 ? std::string("critical2") : std::string("critical3"));
		sp::Armature *pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly(critical.c_str(), sp::ZORDER);
		//pArmature->getBone("num")->replaceDisplayAt(pContainer, 0);
		//pArmature->setPosition(m_pAnimateDragon->getPosition());
		m_pCriticalNode->addChild(pArmature);
		pArmature->getAnimation()->playByIndex(0);
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildBossDragon::armatureComplete), COMPLETE);
	}
	
	m_pModel->addDamageAndRemove();
	CC_SAFE_RELEASE(pDamage);
}

void GuildBossDragon::armatureComplete(sp::Armature * target, const char* event,const char* data)
{
	if (target)
	{
		target->removeFromParent();
		updateStar();
	}
}

void GuildBossDragon::playNoticeAnimation()
{
	playOtherAnimation();
	playRescueAnimation();
}

void GuildBossDragon::playOtherAnimation()
{
	/*guildboss::GuildBossNotice *pNotice = m_pModel->topNoticeRetained();
	if (!pNotice)
	{
		return;
	}
	{
		m_pAnimateAttackOther->playRandomly();

		sp::Armature *pArmature = ResourceManager::sharedResourceManager()->getUIAnimateOnly(m_pAnimateAttackName->getAnimateName().c_str(), sp::ZORDER);
		SPContainer* pContainer = SPContainer::create(sp::ZORDER);
		CCLabelBMFont *pBMFOtherDamage = CCLabelBMFont::create("", "fonts/battle_dmg.fnt");
		pBMFOtherDamage->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(pBMFOtherDamage);
		pBMFOtherDamage->setString(stringForUIntWith10K(pNotice->getDamage()));
		pArmature->getBone("damage")->replaceDisplayAt(pContainer, 0);

		pContainer = SPContainer::create(sp::ZORDER);
		CCLabelTTF *TTFOtherName = CCLabelTTF::create("", "Arial", 24);
		TTFOtherName->setAnchorPoint(ccp(0,0.5));
		pContainer->addChild(TTFOtherName);
		TTFOtherName->setString(pNotice->getUserName().c_str());
		pArmature->getBone("name")->replaceDisplayAt(pContainer, 0);

		pArmature->setPosition(m_pModel->randomPosition());
		addChild(pArmature);
		pArmature->getAnimation()->playByIndex(pNotice->getIsAuto());
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(GuildBossDragon::otherArmatureComplete), COMPLETE);

		CCAction* pAction = CCSequence::create(CCDelayTime::create(0.5),
			CCCallFunc::create(this, callfunc_selector(GuildBossDragon::playOtherAnimation)),
			NULL
			);
		runAction(pAction);
		m_bRunningOtherAnimation = true;

	}
	CC_SAFE_RELEASE(pNotice);*/
}


void GuildBossDragon::otherArmatureComplete(sp::Armature * target, const char* event,const char* data)
{
	if (target)
	{
		target->removeFromParent();
	}
	m_bRunningOtherAnimation = false;
}

void GuildBossDragon::updateRescue()
{
	CCArray *pArr = m_pModel->getWoundedUsers();
	//if (!pArr)
	//{
	//	m_pBMFRescue->setString("0");
	//}
	//else
	//{
	//	m_pBMFRescue->setString(D_CSTRING_FROM_INT(pArr->count()));
	//}
	//m_pNodeRescue->setVisible(pArr && pArr->count());
}

void GuildBossDragon::playRescueAnimation()
{
	/*if (m_pAnimateRescue->isRunningAnimation())
	{
		return;
	}
	guildboss::GuildBossNotice *pNotice = m_pModel->topRescueNoticeRetained();
	if (!pNotice)
	{
		return;
	}
	if (pNotice->getType() == guildboss::GuildBossNoticeTypeSelfRescued)
	{
		if (m_pDelegate)
		{
			m_pDelegate->GuildBossShowHealthView(pNotice);
		}
		m_pModel->rescued();
		m_pNodeWounded->setVisible(false);
		m_pNodeHealthy->setVisible(true);
		playRescueAnimation();
	}
	else if (pNotice->getType() == guildboss::GuildBossNoticeTypeRescue)
	{
		std::string sRescue = D_LOCAL_STRING("%sRescue%s", pNotice->getTargetName().c_str(), pNotice->getUserName().c_str());
		m_pTTFGBMessage->setString(sRescue.c_str());
		m_pAnimateRescue->playAtIndex(0);
	}
	CC_SAFE_RELEASE(pNotice);*/
}

void GuildBossDragon::playTouchAnimation()
{
	sp::Armature *pTouchFlash = ResourceManager::sharedResourceManager()->getUIAnimate("guildwars_touch", sp::ZORDER);
	m_pTouchFlashNode->addChild(pTouchFlash);
}

void GuildBossDragon::callBackAutoAttack(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->GuildBossAutoAttack(this);
		playTouchAnimation();
	}
}