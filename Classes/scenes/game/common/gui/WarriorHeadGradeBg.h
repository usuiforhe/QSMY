//
//  WarriorHeadGradeBg.h
//  PL
//
//  Created by chenshengfei on 14-7-31.
//  Copyright (c) 2014Äê thedream. All rights reserved.
//

#ifndef __PL__WarriorHeadGradeBg__
#define __PL__WarriorHeadGradeBg__

#include "AppInclude.h"
#include "model/WarriorModel.h"

class WarriorHeadGradeBg 
	:public cocos2d::CCNode
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	CREATE_FUNC(WarriorHeadGradeBg);
	WarriorHeadGradeBg();
	~WarriorHeadGradeBg();
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setWarriorGrade(warrior::WarriorGrade grade);
	void updateView();
private:
	
	warrior::WarriorGrade m_eGrade;
	std::vector<cocos2d::CCNode*>		m_vGradeBgs;
};

class WarriorHeadGradeBgLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorHeadGradeBgLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorHeadGradeBg);
};

#endif