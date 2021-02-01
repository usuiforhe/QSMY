//
//  MatchBetMenu.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-10.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MatchBetMenu.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

MatchBetMenu::MatchBetMenu()
:m_pRewardMenuItem(NULL)
,m_pFailed(NULL)
,m_pBeted(NULL)
,m_pBetMenuItem(NULL)
,m_pBetInfo(NULL)
,m_pVSInfo(NULL)
,m_pRewarded(NULL)
,m_pBgMenuItem(NULL)
,m_pViewMenuItem(NULL)
{
    
}

MatchBetMenu::~MatchBetMenu()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pRewardMenuItem);
    CC_SAFE_RELEASE(m_pBetMenuItem);
    CC_SAFE_RELEASE(m_pFailed);
    CC_SAFE_RELEASE(m_pBeted);
    CC_SAFE_RELEASE(m_pRewarded);
    CC_SAFE_RELEASE(m_pBgMenuItem);
    SAFE_RELEASE_UNBINDING_ALL(m_pBetInfo, this);
    SAFE_RELEASE_UNBINDING_ALL(m_pVSInfo,this);
    CC_SAFE_RELEASE(m_pViewMenuItem);
}

void MatchBetMenu::setVSInfo(match::VSInfo *pVSInfo)
{
    SAFE_RELEASE_UNBINDING_ALL(m_pVSInfo,this);
    CC_SAFE_RETAIN(pVSInfo);
    m_pVSInfo = pVSInfo;
    
    if(pVSInfo==NULL) //比赛未开始 还没知道比赛队伍信息
    {
        m_pBeted->setVisible(false);
        m_pFailed->setVisible(false);
        m_pRewarded->setVisible(false);
        m_pRewardMenuItem->setVisible(false);
        m_pBetMenuItem->setVisible(true);
        m_pBetMenuItem->setEnabled(false);
        m_pBgMenuItem->setVisible(false);
        m_pViewMenuItem->setVisible(false);
        
    }else
    {
        match::BetInfo* pBetInfo = m_pModel->getBetInfo(pVSInfo->finalNum, pVSInfo->mid);
        
        SAFE_RELEASE_UNBINDING_ALL(m_pBetInfo, this);
        CC_SAFE_RETAIN(pBetInfo);
        m_pBetInfo = pBetInfo;
		BINDING_ONLY(m_pBetInfo, this, MatchBetMenu::updateView, D_EVENT_MATCH_UPDATE_BETINFO);
		BINDING_ONLY(m_pVSInfo, this, MatchBetMenu::updateView, D_EVENT_MATCH_REPLAY);
        updateView();
    }
}

bool MatchBetMenu::init()
{
    if(!CCNode::init()) return false;
    
    m_pModel = DM_GET_MATCH_MODEL;
    
    return true;
}


void MatchBetMenu::updateView()
{
    if(m_pBetInfo)  //如果已经竞猜
    {
        
        m_pBetMenuItem->setVisible(false);
        m_pViewMenuItem->setVisible(false);
        
        m_pRewardMenuItem->setVisible(false);
        m_pBgMenuItem->setVisible(false);
        
        m_pBeted->setVisible(false);
        m_pFailed->setVisible(false);
        m_pRewarded->setVisible(false);
        
        switch ((match::BetResult)m_pBetInfo->result)
        {
            case match::kBetResultNone: //未出结果
                m_pBgMenuItem->setVisible(true);
                m_pBgMenuItem->setEnabled(true);
                m_pBeted->setVisible(true);
                break;
            case match::kBetResultWin: //中
                if(m_pBetInfo->status)  //已领
                {
                    m_pBgMenuItem->setVisible(true);
                    m_pBgMenuItem->setEnabled(true);
                    m_pRewarded->setVisible(true);
                }else                   //未领
                {
                    m_pRewardMenuItem->setVisible(true);
                }
                break;
            case match::kBetResultFailed: //未中
                m_pBgMenuItem->setVisible(true);
                m_pBgMenuItem->setEnabled(true);
                m_pFailed->setVisible(true);
                break;
            default:
                break;
        }
        
    }
    else
    {
        if(m_pVSInfo->winUser == match::kVSNone) //如果比赛没完成 并且没有竞猜
        {
            m_pBeted->setVisible(false);
            m_pFailed->setVisible(false);
            m_pRewarded->setVisible(false);
            
            m_pRewardMenuItem->setVisible(false);
            m_pBetMenuItem->setVisible(true);
            m_pBetMenuItem->setEnabled(true);
            m_pBgMenuItem->setVisible(false);
            m_pViewMenuItem->setVisible(false);
        }else                                   //如果已完成 并且没有竞猜
        {
            
            m_pViewMenuItem->setVisible(true);
            m_pViewMenuItem->setEnabled(true);
            m_pBeted->setVisible(false);
            m_pFailed->setVisible(false);
            m_pRewarded->setVisible(false);
            m_pRewardMenuItem->setVisible(false);
            m_pBetMenuItem->setVisible(false);
            m_pBgMenuItem->setVisible(false);
        }
    }
	if (m_pVSInfo->isWatched() == false && m_pVSInfo->winUser != match::kVSNone)
	{
		m_pViewMenuItem->setVisible(true);
		m_pViewMenuItem->setEnabled(true);
		m_pFailed->setVisible(false);
		m_pRewarded->setVisible(false);
		m_pRewardMenuItem->setVisible(false);
	}
	if (m_pVSInfo->m_pLeft->visible == false || m_pVSInfo->m_pRight->visible == false)
	{

		m_pBeted->setVisible(false);
		m_pFailed->setVisible(false);
		m_pRewarded->setVisible(false);
		m_pRewardMenuItem->setVisible(false);
		m_pBetMenuItem->setVisible(true);
		m_pBetMenuItem->setEnabled(false);
		m_pBgMenuItem->setVisible(false);
		m_pViewMenuItem->setVisible(false);
	}
	
}

bool MatchBetMenu::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardMenuItem",  CCMenuItemImage*, this->m_pRewardMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pViewMenuItem",  CCMenuItemImage*, this->m_pViewMenuItem);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBetMenuItem", CCMenuItemImage *, this->m_pBetMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBgMenuItem", CCMenuItemImage *, this->m_pBgMenuItem);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFailed", CCSprite *, this->m_pFailed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBeted", CCSprite *, this->m_pBeted);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewarded", CCSprite *, this->m_pRewarded);
    
    return false;
}