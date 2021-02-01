//
//  WarriorGradeBg.h
//  PL
//
//  Created by zhanglong on 14-7-7.
//  Copyright (c) 2014Äê thedream. All rights reserved.
//

#ifndef __PL__WarriorGradeBg__
#define __PL__WarriorGradeBg__

#define WARRIOR_GRADE_NUM 4
 
#include "AppInclude.h"
#include "model/WarriorModel.h"

class WarriorGradeBg 
	:public cocos2d::CCNode
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	friend class PictureCollect;
public:
	CREATE_FUNC(WarriorGradeBg);
	WarriorGradeBg();
	~WarriorGradeBg();
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setWarriorGrade(warrior::WarriorGrade grade);
	void reset();
	void hideGradeLabel();
private:
	void updateView();

	warrior::WarriorGrade m_eGrade;
	std::vector<cocos2d::CCNode*>		m_vGradeBgs;
	cocos2d::CCNode* m_pGradeLabel[WARRIOR_GRADE_NUM];
};

class WarriorGradeBgLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorGradeBgLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorGradeBg);
};

#endif