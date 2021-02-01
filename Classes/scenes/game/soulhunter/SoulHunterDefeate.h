//
// SoulHunterDefeate.h
//  狩魂战斗失败界面
//
//  Created by liuxiaogang on 13-10-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulHunterDefeate__
#define __QSMY__SoulHunterDefeate__

#include "AppInclude.h"
#include "components/components.h"
#include "model/SoulHunterModel.h"
#include "managers/DataManager.h"


class SoulHunterDefeate
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	SoulHunterDefeate();
	~SoulHunterDefeate();
	virtual bool init();
	static SoulHunterDefeate*  create();

	virtual void onEnter();
	virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void againCallBack(cocos2d::CCObject* sender);
	void closeCallBack(cocos2d::CCObject* sender);
    
private:
	DButton						*m_pBackButton;
	DButton						*m_pAgainButton;
};

#endif /* defined(__QSMY__SoulHunterDefeate__) */