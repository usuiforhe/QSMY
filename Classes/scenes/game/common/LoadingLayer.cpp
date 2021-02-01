//
//  LoadingLayer.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#define AUTO_TOUCH_DELAY	3.f
#define LOADING_TAG_MAX	10

#include "LoadingLayer.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"

USING_NS_CC;

// singleton stuff
static LoadingLayer *s_SharedLoadingLayer = NULL;

LoadingLayer* LoadingLayer::sharedLoadingLayer(void)
{
    if (!s_SharedLoadingLayer)
    {
        s_SharedLoadingLayer = new LoadingLayer();
        s_SharedLoadingLayer->init();
    }
    
    return s_SharedLoadingLayer;
}


void LoadingLayer::purgeLoadingLayer(void)
{
    CC_SAFE_RELEASE_NULL(s_SharedLoadingLayer);
}

LoadingLayer::LoadingLayer()
:m_pTips(NULL)
,m_pAnimateLeft(NULL)
,m_pAnimateRight(NULL)
,m_pAnimateCenter(NULL)
,m_pAnimateCenterBegin(NULL)
,m_uWid0(0)
,m_uWid1(0)
,m_bMenuEnabled(false)
,m_pDelegate(NULL)
,m_fPercent(0.f)
{
    
}

LoadingLayer::~LoadingLayer()
{
	CC_SAFE_RELEASE(m_pTips);
	CC_SAFE_RELEASE(m_pAnimateLeft);
	CC_SAFE_RELEASE(m_pAnimateRight);
	CC_SAFE_RELEASE(m_pAnimateCenter);
	CC_SAFE_RELEASE(m_pAnimateCenterBegin);
}

bool LoadingLayer::init()
{
    if(!DLayerModal::init()) return false;
	removeAllChildren();

	CCLayerColor* colorLayer = CCLayerColor::create(ccc4(0,0,0,255),DESIGN_SIZE.width,DESIGN_SIZE.height);
	this->addChild(colorLayer);
	cocos2d::CCNode *__pNode__ = NULL;
	CCB_READER("PL_ui/ccb/pl_common/loading",__pNode__);
	if(__pNode__) addChild(__pNode__, 1, 1);

	m_pAnimateRight->removeFromParent();
	CCClippingNode* pCNode = CCClippingNode::create();
	pCNode->addChild(m_pAnimateRight);
	CCNode *stencil = CCNode::create();
	CCSprite *pTop = CCSprite::create("PL_ui/map/battle_ani_cover_r.png");
	stencil->addChild(pTop);
	pCNode->setStencil(stencil);
	pCNode->setAlphaThreshold(0.5);
	addChild(pCNode);
	pCNode->setPosition(m_pAnimateRight->getPosition());
	m_pAnimateRight->setPosition(CCPointZero);

	m_pAnimateLeft->removeFromParent();
	pCNode = CCClippingNode::create();
	pCNode->addChild(m_pAnimateLeft);
	stencil = CCNode::create();
	CCSprite *pBtm = CCSprite::create("PL_ui/map/battle_ani_cover_l.png");
	stencil->addChild(pBtm);
	pCNode->setStencil(stencil);
	pCNode->setAlphaThreshold(0.5);
	addChild(pCNode);
	pCNode->setPosition(m_pAnimateLeft->getPosition());
	m_pAnimateLeft->setPosition(CCPointZero);

	m_pAnimateCenterBegin->setDelegate(this);
	m_pAnimateCenter->setVisible(false);
    return true;
}

bool LoadingLayer::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"m_pTips",cocos2d::CCLabelTTF* ,m_pTips);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateLeft", UIAnimate *, m_pAnimateLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateRight", UIAnimate *, m_pAnimateRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateCenter", UIAnimate *, m_pAnimateCenter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimateCenterBegin", UIAnimate *, m_pAnimateCenterBegin);
    return false;
}

SEL_MenuHandler LoadingLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackTouch", LoadingLayer::callBackTouch);
	return NULL;
}

void LoadingLayer::onEnter()
{
    DLayerModal::onEnter();
    if(m_pTips)
    {
        m_pTips->setString(DM_GET_NOTICE_MODEL->getTipsInfo()->getDialog()->getCString());
    }
	if (m_uWid0 == 0 || m_uWid1 == 0)
	{
		setBattleWarriors();//如果没有设置wids，自动根据BattleInfo设置 PS：只有PVE设置wids
	}
	//	m_pAnimateCenter->playAtIndex(0);
	m_pAnimateLeft->playAtIndex(0);
	m_pAnimateRight->playAtIndex(0);
	//	m_pAnimateCenter->playAtIndex(0);
	m_pAnimateCenterBegin->playAtIndex(0);
	m_fPercent = 0.f;
}

void LoadingLayer::onExit()
{
	DLayerModal::onExit();
	m_uWid0 = 0;
	m_uWid1 = 0;
	m_pAnimateLeft->stop();
	m_pAnimateRight->stop();
	m_pAnimateCenter->stop();
	m_pAnimateCenterBegin->stop();
	m_bMenuEnabled = false;
	m_pAnimateCenter->setVisible(false);
	m_pAnimateCenterBegin->setVisible(true);
}

void LoadingLayer::setBattleWarriors()
{
	BattleModel *pModel = DM_GET_BATTLE_MODEL;
	battle::BattleInfo *pInfo = pModel->getBattleInfo();
	battle::BattleUnit *pUnitLeft = NULL;
	battle::BattleUnit *pUnitRight = NULL;
	battle::BattleUnit *pUnitBoss = NULL;
	for (unsigned int i = 0; i < pInfo->m_pUnits->count(); i++)
	{
		battle::BattleUnit *pUnit = (battle::BattleUnit *)pInfo->m_pUnits->objectAtIndex(i);
		if(!pUnit->position) continue;
		if (pUnit->m_pForce == pInfo->getForce(battle::kForceLeft))
		{
			if (!pUnitLeft || pUnitLeft->position > pUnit->position)
			{
				pUnitLeft = pUnit;
			}
		}
		else
		{
			if (!pUnitRight || pUnitRight->position > pUnit->position)
			{
				pUnitRight = pUnit;
			}
			if(pUnit->is_boss)//如果是pve需要取为boss,boss的位置可能不是最小位置
			{
				pUnitBoss = pUnit;
			}
		}
	}
	if(pUnitBoss) pUnitRight = pUnitBoss;
	CCAssert(pUnitLeft != NULL && pUnitRight != NULL, "Error battle info");
	unsigned int uWid0 = pUnitLeft != NULL ? pUnitLeft->getAvatarId() : DEFAULT_UNIT;
	unsigned int uWid1 = pUnitRight != NULL ? pUnitRight->getAvatarId() : DEFAULT_UNIT;
	setWarriorIds(uWid0, uWid1);
}

void LoadingLayer::setWarriorIds(unsigned int wid0, unsigned int wid1)
{
	CCSprite* pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(wid1, 2.0);
	pNode->setFlipX(false);
	pNode->setPosition(ccp(100, -600));
	m_pAnimateRight->replaceNodeInBone(pNode, "character");

	pNode = ResourceManager::sharedResourceManager()->getWarriorIcon(wid0, 2.0);
	pNode->setFlipX(true);
	pNode->setPosition(ccp(-100, -400));
	m_pAnimateLeft->replaceNodeInBone(pNode, "character");

	m_uWid0 = wid0;
	m_uWid1 = wid1;
}

void LoadingLayer::callBackTouch(cocos2d::CCObject *pObj)
{
	if (!m_bMenuEnabled)
	{
		return;
	}
	DM_GET_SOUND_MODEL->playCommonEffectSound(SFX_FIGHT, 1);
	if (m_pDelegate)
	{
		m_pDelegate->loadingLayerTouched(this);
	}
	unschedule(schedule_selector(LoadingLayer::updateTouched));
}

void LoadingLayer::setMenuEnabled(bool bEnabled)
{
	m_bMenuEnabled = bEnabled;
	if (bEnabled)
	{
		unschedule(schedule_selector(LoadingLayer::updateTouched));
		scheduleOnce(schedule_selector(LoadingLayer::updateTouched), AUTO_TOUCH_DELAY);
		this->stopAllActions();
		m_pAnimateCenter->playWithName("ani2");
	}
	
}

void LoadingLayer::updateTouched(float dt)
{
	callBackTouch(NULL);
}

void LoadingLayer::setLoadingPercent(float pct)
{
	if (pct > m_fPercent)
	{
		int dt =  (int)(pct * LOADING_TAG_MAX - m_fPercent * LOADING_TAG_MAX);
		if (dt > 0)
		{
			this->stopAllActions();
			this->runAction(CCNumberBy::create(0.1 * dt, m_fPercent * LOADING_TAG_MAX, dt, tween_num_selector(LoadingLayer::showPercentAni)));
			m_fPercent = pct;
		}
	}
	
}

void LoadingLayer::showPercentAni(float num)
{
	m_pAnimateCenter->playAtIndex((int)num);
}

void LoadingLayer::animationComplete(UIAnimate *pAnimate)
{
	if (pAnimate == m_pAnimateCenterBegin)
	{
		m_pAnimateCenter->setVisible(true);
		m_pAnimateCenterBegin->setVisible(false);
		if (m_bMenuEnabled)
		{
			setMenuEnabled(true);
		}
		
	}
}