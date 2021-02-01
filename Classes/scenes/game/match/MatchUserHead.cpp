//
//  MatchUserHead.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchUserHead.h"
#include "managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

MatchUserHead::MatchUserHead()
:m_pContainer(NULL)
,m_pImgWin(NULL)
,m_pImgLose(NULL)
,m_pImgBeted(NULL)
,m_pTfName(NULL)
,m_pTfLevel(NULL)
,m_pPlayerImg(NULL)
,m_pDefaultImg(NULL)
,m_pVsInfo(NULL)
,m_eVS(match::kVSNone)
,m_pBgWin(NULL)
,m_pBgLose(NULL)
{
    
}


MatchUserHead::~MatchUserHead()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pImgWin);
    CC_SAFE_RELEASE(m_pImgLose);
    CC_SAFE_RELEASE(m_pImgBeted);
    CC_SAFE_RELEASE(m_pTfName);
    CC_SAFE_RELEASE(m_pTfLevel);
    CC_SAFE_RELEASE(m_pPlayerImg);
    CC_SAFE_RELEASE(m_pDefaultImg);
	SAFE_RELEASE_UNBINDING_ALL(m_pVsInfo, this);
	CC_SAFE_RELEASE(m_pBgWin);
	CC_SAFE_RELEASE(m_pBgLose);
}

void MatchUserHead::updateView()
{
	match::VSPlayer *pVsPlayer = NULL;//m_pVsInfo ? (m_eVS == match::kVSLeft ? m_pVsInfo->m_pLeft : m_pVsInfo->m_pRight) : NULL;
	if (m_pVsInfo)
	{
		if (m_eVS == match::kVSLeft)
		{
			if (m_pVsInfo->getIsSwitched())
			{
				pVsPlayer = m_pVsInfo->m_pRight;
			}
			else
			{
				pVsPlayer = m_pVsInfo->m_pLeft;
			}
		}
		else
		{
			if (m_pVsInfo->getIsSwitched())
			{
				pVsPlayer = m_pVsInfo->m_pLeft;
			}
			else
			{
				pVsPlayer = m_pVsInfo->m_pRight;
			}
		}
		
	}
	
	m_pPlayerImg->setVisible(pVsPlayer!=NULL && pVsPlayer->visible);
	//m_pDefaultImg->setVisible(pVsPlayer==NULL || !pVsPlayer->visible);
    if(m_pContainer) m_pContainer->removeAllChildren();
	if(pVsPlayer && pVsPlayer->visible)
	{
		//m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForMatch(pVsPlayer->m_pDesp->getAvatarId()));
		m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIconForChangeFormation(pVsPlayer->m_pDesp->getAvatarId()));
		m_pImgWin->setVisible(pVsPlayer->win);
		m_pBgWin->setVisible(pVsPlayer->win);
		m_pImgLose->setVisible(pVsPlayer->lose);
		m_pBgLose->setVisible(pVsPlayer->lose);
		//m_pImgBeted
		m_pTfName->setString(pVsPlayer->m_pDesp->name.c_str());
		m_pTfLevel->setString(CCString::createWithFormat("%d",pVsPlayer->m_pDesp->lv)->getCString());
	}
}

void MatchUserHead::setVSInfo(match::VSInfo* pVsInfo, match::VS vs)
{
	CC_SAFE_RETAIN(pVsInfo);
	SAFE_RELEASE_UNBINDING_ALL(m_pVsInfo, this);
	m_pVsInfo = pVsInfo;
	m_eVS = vs;
    BINDING_EXEC(m_pVsInfo,this,MatchUserHead::updateView,D_EVENT_MATCH_REPLAY);
}

bool MatchUserHead::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgWin",  CCSprite*, this->m_pImgWin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgLose", CCSprite *, this->m_pImgLose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pImgBeted", CCSprite *, this->m_pImgBeted);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfName", CCLabelTTF *, this->m_pTfName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTfLevel", CCLabelBMFont *, this->m_pTfLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPlayerImg", CCNode *, this->m_pPlayerImg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefaultImg", CCSprite *, this->m_pDefaultImg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgWin", CCNode *, this->m_pBgWin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgLose", CCNode *, this->m_pBgLose);
    return false;
}