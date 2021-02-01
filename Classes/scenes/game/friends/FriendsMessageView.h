//
//  FriendsMessageView.h
//  编辑留言
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsMessageView__
#define __QSMY__FriendsMessageView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../settings/UserFeedback.h"

class FriendsViewDelegate;


class FriendsMessageView 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public EditBoxDelegate
{
	CC_SYNTHESIZE(FriendsViewDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE(uint32_t, m_nUserID, UserID);
	CC_SYNTHESIZE(mail::MailType, m_eType, Type);
public:
	FriendsMessageView();
	~FriendsMessageView();

	static FriendsMessageView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
	virtual void editBoxReturn(EditBox* editBox);

	void setName(std::string sName);
private:
	CCNode *m_pContainer;
	CCLabelTTF *m_pTTFName;
	MyTextFieldTTF *m_pTextField;

#pragma mark callback function
	void callBackEnter(CCObject *sender);
	void callBackClose(CCObject *sender);

	void callBackSend(const Json::Value &request, const Json::Value &response);
};

#endif