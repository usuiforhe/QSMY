//
//  SoulHunterWarriorView.h
//  狩魂单个弟子视图
//
//  Created by liuxiaogang on 13-10-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterWarriorView__
#define __QSMY__SoulHunterWarriorView__

#include "AppInclude.h"
#include "model/SoulHunterModel.h"
#include "model/WarriorModel.h"


class SoulHunterWarriorView
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pBgGrade, BgGrade);							//背景框 高品质弟子
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pSoulNumBg, SoulNumBg);						//背景框 魂魄剩余数量
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAvatorContainer, AvatorContainer);				//头像容器
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);								//弟子名
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pSoulNum, SoulNum);						//魂魄剩余数量
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pProtectLeftTime, ProtectLeftTime);		//剩余保护时间
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pProtectOn, ProtectOn);							//保护状态 
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pSnatchMenuItem, SnatchMenuItem);			//抢夺按钮  
public:
    CREATE_FUNC(SoulHunterWarriorView);
    SoulHunterWarriorView();
    virtual ~SoulHunterWarriorView();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setData(warrior::WarriorCurInfo * pWarrior);
	void updateView();

	virtual void onEnter();
    virtual void onExit();
private:
	warrior::WarriorCurInfo					*m_pWarriorInfo;
	cocos2d::CCNode *m_pEffectNode;
};


class SoulHunterWarriorViewLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SoulHunterWarriorViewLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SoulHunterWarriorView);
};

#endif /* defined(__QSMY__SoulHunterWarriorView__) */
