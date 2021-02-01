//
//  WarriorHead.h
//  PL
//
//  Created by zhanglong on 14-7-7.
//  Copyright (c) 2014Äê thedream. All rights reserved.
//

#ifndef __PL__WarriorHead__
#define __PL__WarriorHead__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "WarriorGradeBg.h"

class WarriorHead 
	:public WarriorGradeBg
{
	CC_SYNTHESIZE_READONLY(CCNode *, m_pIconSprite, IconSprite);
public:
	CREATE_FUNC(WarriorHead);
	WarriorHead();
	~WarriorHead();
	D_INIT_DEFAULT(CCNode);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void setWarriorBaseInfo(warrior::WarriorBaseInfo *pBase);
	void setNum(unsigned int uNum);
	void setIsSoul(bool isSoul);
private:
	void updateView();

	warrior::WarriorBaseInfo *m_pBase;
	cocos2d::CCLabelTTF *m_pItemName;
	cocos2d::CCNode *m_pSoulMark;
	cocos2d::CCLabelBMFont *m_pBMFNum;
};

class WarriorHeadLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorHeadLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorHead);
};

#endif