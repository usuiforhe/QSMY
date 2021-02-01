//
//  Friend.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__Friend__
#define __QSMY__Friend__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "FriendListView.h"

class Friend 
	:public DLayer
	,public BackKeyInterface
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public FriendListDelegate
{
public:

	enum FriendMenuType{
		kFriendMenuView = 2,
		kFriendMenuChallenge = 3,
		kFriendMenuDelete = 4
	};

	Friend();
	~Friend();

	static Friend* create();
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void onEnter();
	virtual void onExit();

	virtual void showMenu(unsigned int fid);
	void toggleView(bool val){
		m_pFriendless->setVisible(val);
	}
	void onMenu(cocos2d::CCObject*);

public:
	virtual void onBackKey();

private:
	cocos2d::CCMenuItemImage	*m_pChallenge;
	cocos2d::CCMenuItemImage	*m_pView;
	cocos2d::CCMenuItemImage	*m_pDelete;
	DButton									*m_pBtnClose;
	cocos2d::CCNode						*m_pPopup;

	cocos2d::CCNode	*m_pFocusLayer;
	cocos2d::CCNode	*m_pPopupLayer;
	cocos2d::CCNode	*m_pContainer;
	cocos2d::CCNode	*m_pContainerPos;
	DButton				*m_pCloseButton;
	DButton				*m_pConfirmButton;
	cocos2d::CCNode	*m_pFriendless;

private:
	unsigned int m_uCurrentFriendID;
	DTabView     *m_pDTabView;
	FriendModel *m_pModel;

private:
	void closeCallBack(CCObject* obj);
	void onMenuClose(CCObject*);
	void updateRequest(float dt);
	void callBackRequest(const Json::Value &request, const Json::Value &response);
};

#endif