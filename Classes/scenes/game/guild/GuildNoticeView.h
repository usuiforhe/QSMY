//
//  GuildNoticeView.h
//  公会公告
//
//  Created by Eci on 13-10-22.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildNoticeView__
#define __QSMY__GuildNoticeView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../settings/UserFeedback.h"




class GuildNoticeView 
	:public DPopup
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public EditBoxDelegate
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
	,public cocos2d::extension::DEditBoxDelegate
#endif*/
{
public:
	GuildNoticeView();
	~GuildNoticeView();

	static GuildNoticeView* create();
	virtual bool init();

	virtual void onEnter();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
// #pragma mark DEditBoxDelegate
//     virtual void editBoxReturn(DEditBox* editBox);
// #endif
	virtual void editBoxReturn(EditBox* editBox);
private:
	DButton *m_pEnter;
	DButton *m_pCancel;
	DButton *m_pClose;
	cocos2d::CCNode *m_pContainer;
	MyTextFieldTTF              *m_pTextField;

	cocos2d::CCNode *m_pNodeToHide;

#pragma mark callback function
	void callBackEnter(CCObject *sender);
	void callBackClose(CCObject *sender);
	void sendCallBack(const Json::Value &,const Json::Value &);
};

#endif