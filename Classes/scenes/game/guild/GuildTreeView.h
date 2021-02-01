//
//  GuildTreeView.h
//  公会天工树
//
//  Created by Eci on 13-10-29.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildTreeView__
#define __QSMY__GuildTreeView__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildHead.h"
#include "effect/GuildExpEffect.h"

class GuildTreeViewDelegate
{
public:
	virtual void willCloseTreeView() = 0;
};


class GuildTreeView
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
{
public:
	GuildTreeView();
	~GuildTreeView();

	static GuildTreeView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

protected:
	CC_SYNTHESIZE(GuildTreeViewDelegate *, m_pDelegate, Delegate);
private:
	GuildModel *m_pModel;
	CCNode *m_pContainer;
	cocos2d::CCMenuItem *m_pButtonCancel;
	cocos2d::CCNode *m_pNodeOverlord;
	cocos2d::CCNode *m_pNodeChivalry;

	cocos2d::CCMenuItem *m_pButtonOverlord;
	cocos2d::CCMenuItem *m_pButtonChivalry;

	cocos2d::CCLabelBMFont *m_pLabelRest;
	cocos2d::CCLabelBMFont *m_pLabelOverlord;
	cocos2d::CCLabelBMFont *m_pLabelChivalry;
	cocos2d::CCLabelBMFont *m_pLabelCoinsCount;
	cocos2d::CCLabelBMFont *m_pLabelItemsRest;
	cocos2d::CCLabelBMFont *m_pLabelTimesRest;


	DButton *m_pButtonItem1;
	DButton *m_pButtonItem10;
	DButton *m_pButtonCoins;

	//GuildHead *m_pGuildHead;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelPosition;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelExp;
	DProgressBar* m_pExpBar;				/*经验条*/
	cocos2d::CCLabelBMFont *m_pLabelNextExp;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice0;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice1;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice2;

	DTableView    *m_pListView;


	uint32_t m_iType;

	bool m_isExpShown;

	void setType(int iType);

#pragma mark callback function
	void callBackCancel(CCObject *sender);
	void callBackType(CCObject *sender);
	void callBackUpgrade(CCObject *sender);
	void callBackUpgrade(const Json::Value &requestData,const Json::Value &responseData);
	void callBackShowExp(sp::Armature * target, const char* event,const char* data);
	
	void updateView();
	void updateTree();
	void updateNotice();
};

#endif