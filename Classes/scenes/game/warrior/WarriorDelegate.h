//
//  WarriorDelegate.h
//  QSMY
//
//  Created by wanghejun on 13-8-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorDelegate__
#define __QSMY__WarriorDelegate__

#include "model/WarriorModel.h"


enum WarriorViewType
{
    kWarriorNone,
	kWarriorInfo,           //弟子详情
	kSoulInfo,              //魂魄
	kPromotionView,         //突破
	kTransForceView,        //传功
	kDevelopView,           //培养
	kTransSettleView,       //传功结算
	kDevSettleView,         //培养结算
	kRecruitSettleView,     //招募结算
    kSkillInfoView,         //技能界面
    kSkillUpgradeView,      //技能升级界面
	kFormationSelectView,   //选弟子上阵界面
    kUpgradeView,           //英雄强化
	kDressView,           //换装
};


class WarriorDelegate
{
public:
    WarriorDelegate()
    :m_pWarrior(NULL)
    ,m_pParam(NULL)
    ,m_cType(kWarriorNone)
    ,m_lastType(kWarriorNone)
    {
        
    }
    
    virtual ~WarriorDelegate()
    {
        CC_SAFE_RELEASE(m_pWarrior);
        CC_SAFE_RELEASE(m_pParam);
    }
    
	virtual void closeInfoView(WarriorViewType type=kWarriorNone)
    {
        m_lastType = kWarriorNone;
        m_cType = kWarriorNone;
        setWarrior(NULL);
        setParam(NULL);
    }
    
	virtual void showInfoView(WarriorViewType type, warrior::WarriorCurInfo * pWarrior,cocos2d::CCObject* param = NULL)
    {
        m_lastType = m_cType;
        m_cType = type;
        setWarrior(pWarrior);
        setParam(param);
    }
    
protected:
    
    void setWarrior(warrior::WarriorCurInfo* pInfo)
    {
        CC_SAFE_RETAIN(pInfo);
        CC_SAFE_RELEASE(m_pWarrior);
        m_pWarrior = pInfo;
    }
    
    void setParam(cocos2d::CCObject * pParam)
    {
        CC_SAFE_RETAIN(pParam);
        CC_SAFE_RELEASE(m_pParam);
        m_pParam = pParam;
    }
    
    warrior::WarriorCurInfo * m_pWarrior;
    cocos2d::CCObject *       m_pParam;
    WarriorViewType           m_cType;
    WarriorViewType           m_lastType;
};

#endif /* defined(__QSMY__WarriorDelegate__) */
