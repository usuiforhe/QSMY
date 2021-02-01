//
//  Friends.h
//  好友
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__Friends__
#define __QSMY__Friends__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"


enum FriendsNav
{
	kFriendsNavNoPop = -1,
	kFriendsNavHome= 0,
	kFriendsNavRemove = 1,
	kFriendsNavMessage = 2,
	kFriendsNavSearch = 3,
	kFriendsNavSend = 4,
	kFriendsNavRemoveConfirm = 5,
	kFriendsNavFullError = 6,
	kFriendsNavNow = 7,
	kFriendsNavAdd = 8,
	kFriendsNavFound = 9,
};

class FriendsViewDelegate
{
public:
	virtual void FriendsNavigateTo(FriendsNav nav) = 0;
protected:
private:
};

class FriendsView;

class Friends 
	:public DLayer
	,public BackKeyInterface
	,public FriendsViewDelegate
{
public:
	Friends();
	~Friends();

	static Friends* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual void FriendsNavigateTo(FriendsNav nav);

private:
	cocos2d::CCNode	*m_pFocusLayer;
	cocos2d::CCNode	*m_pPopupLayer;
	FriendsView *m_pFriendsView;
	FriendsModel *m_pModel;
protected:
	FriendsNav m_eShowingNav;
	FriendsNav m_ePopingNav;
public:
	virtual void onBackKey();
private:
	void updateRequest(float dt);
	void callBackRequest(const Json::Value &request, const Json::Value &response);
};

#endif