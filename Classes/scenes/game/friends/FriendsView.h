//
//  FriendsView.h
//  好友主页
//
//  Created by Eci on 14-04-10.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__FriendsView__
#define __QSMY__FriendsView__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../guild/DLoadingTabelView.h"
#include "Friends.h"

class FriendsTable;
class FriendsViewDelegate;

class FriendsView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public DTabViewDelegate
{
public:
	FriendsView();
	~FriendsView();

	static FriendsView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();
	void updateID();
	void updateRequest();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx);

	void setType(FriendsNav eNav);

	void setDelegate(FriendsViewDelegate *pDelegate);
protected:
	FriendsViewDelegate *m_pDelegate;
private:
	void callBackSwitch(CCObject *pSender);
	void callBackPopup(CCObject *pSender);
	void callBackBottom(CCObject *pSender);

	DTabView	*m_pTabView;

	CCNode *m_pNodeContainerFriends;
	CCNode *m_pNodeContainerAdd;
	CCNode *m_pNodeAdd;
	CCNode *m_pNodeFriends;

	CCMenuItem *m_pBtnRemove;
	CCMenuItem *m_pBtnMessage;

	CCMenuItem *m_pBtnRecommend;
	CCMenuItem *m_pBtnApply;
	CCLabelBMFont *m_pBMFID;
	CCLabelBMFont *m_pBMFCount;

	CCNode *m_pNodeNoApply;

	DButton *m_pDBtnBottom;
	CCNode *m_pNodeSearch;
	CCNode *m_pNodeIgnore;

	CCNode *m_pNodeRequestNum;
	CCLabelBMFont *m_pBMFRequestNum;

	FriendsTable *m_pNowTable;
	FriendsTable *m_pAddTable;

	FriendsModel *m_pModel;
	uint8_t m_nTag;

	FriendsNav m_eNav;
	
};

#endif