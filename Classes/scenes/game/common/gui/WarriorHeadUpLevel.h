//
//  WarriorHeadUpLevel.h
//  弟子小头像 带进阶等级信息，名字在头像右边
//
//  Created by zhanglong on 14-7-10.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __PL__WarriorHeadUpLevel__H__
#define __PL__WarriorHeadUpLevel__H__

#include "AppInclude.h"
#include "model/WarriorModel.h"

class WarriorHeadSmall;

class WarriorHeadUpLevel
	: public cocos2d::CCNode
	,public cocos2d::extension::CCBMemberVariableAssigner
{
	CC_SYNTHESIZE_READONLY(WarriorHeadSmall*, m_pWarriorHeadSmall, _WarriorHeadSmall);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pUpLevel, UpLevel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
public:
	CREATE_FUNC(WarriorHeadUpLevel);
	WarriorHeadUpLevel();
	virtual ~WarriorHeadUpLevel();
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setWarriorInfo(warrior::WarriorBaseInfo* pWarriorBaseInfo);
	void setWarriorInfo(warrior::WarriorCurInfo* pWarriorInfo);
	void setNotInBattleTipVisible(bool visible);
	void setHeadSprite(cocos2d::CCSprite* pHead);
private:
	void resetWarriorInfo();
	void updateWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo);

};

class WarriorHeadUpLevelLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorHeadUpLevelLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorHeadUpLevel);
};


#endif