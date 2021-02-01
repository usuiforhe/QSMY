//
//  FriendsFoundView.h
//  找到好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsFoundView__
#define __QSMY__FriendsFoundView__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "Friends.h"

class FriendsViewDelegate;

class FriendsFoundView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
public:
	FriendsFoundView();
	~FriendsFoundView();

	static FriendsFoundView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
private:
	CCLabelTTF *m_pTTFName;
	CCLabelTTF *m_pTTFTips;
	CCLabelBMFont *m_pBMFLevel;
	CCNode *m_pContainer;
	CCNodeRGBA *m_pRgbaGrade;

	FriendsModel *m_pModel;

	void callBackAdd(CCObject *pSender);
	void callBackClose(CCObject *pSender);

	void callBackAdd(const Json::Value &request, const Json::Value &response);
};

#endif