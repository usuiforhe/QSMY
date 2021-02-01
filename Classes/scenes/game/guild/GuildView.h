//
//  GuildView.h
//  公会主页
//
//  Created by Eci on 13-10-21.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildView__
#define __QSMY__GuildView__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildQuestionView.h"
#include "GuildTreeView.h"

class GuildViewDelegate
{
public:
	virtual void willShowRankView() = 0;
	virtual void willShowManageView() = 0;
	virtual void willShowTreeView() = 0;
	virtual void willShowMallView() = 0;
	virtual void willShowQuestionView() = 0;
	virtual void willShowNoticeView() = 0;
protected:
private:
};


class GuildView 
	:public DLayer
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
	,public DMarqueeViewDataSouce
	,public cocos2d::extension::CCTableViewDataSource
	,public cocos2d::extension::CCTableViewDelegate
//	,public GuildQuestionViewDelegate
//	,public GuildTreeViewDelegate
{
public:
	//入口类型
	enum GUILD_MENU_TYPES
	{
		kGuildSign = 0,
		kGuildTree = 1,
		kGuildQuestion = 2,
		kGuildShop = 3 
	};

public:
	GuildView();
	~GuildView();

	static GuildView* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual void update(float delta);
	void updateQuestion(float dt);

	void updateView();

	void updateViewWithDurOfPB(float duration); // for progress bar
	void updateNoticeView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {return NULL;};
#pragma mark DMarqueeViewDataSouce
	virtual cocos2d::CCString * getMarqueeText();

#pragma mark CCTableViewDataSource
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);

#pragma mark CCTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};

// #pragma mark GuildQuestionViewDelegate
// 	virtual void willCloseQuestionView();
// 
// #pragma mark GuildTreeViewDelegate
// 	virtual void willCloseTreeView();
protected:
	CC_SYNTHESIZE(GuildViewDelegate *, m_pDelegate, Delegate);
private:
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelPosition;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelExp;
	DProgressBar* m_pExpBar;				/*经验条*/
	cocos2d::CCLabelBMFont *m_pLabelNextExp;

	//cocos2d::CCLabelTTF *m_pLabelNotice0;
	//cocos2d::CCLabelTTF *m_pLabelNotice1;
	//cocos2d::CCLabelTTF *m_pLabelNotice2;

	cocos2d::CCRichLabelTTF *m_pRichLabelNotice0;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice1;
	cocos2d::CCRichLabelTTF *m_pRichLabelNotice2;

	cocos2d::CCRichLabelTTF *m_pLabelAnnouncement;

	DMarqueeView *m_pMarquee;

	cocos2d::CCMenuItem *m_pButtonEditAnnouncement;

	cocos2d::CCMenuItem *m_pButtonManage;
	cocos2d::CCMenuItem *m_pButtonRank;
	cocos2d::CCMenuItem *m_pBtnShop ;

	GuildModel *m_pModel;
    cocos2d::CCNode *m_pNodeRequestNum;
	cocos2d::CCNode *m_pContainer;
	DTableView *m_pListView;

// 	GuildQuestionView *m_pQuestionView;
// 	GuildTreeView *m_pTreeView;
	float m_fTime;
	float m_fQuetionTime;

#pragma callback function
	void callBackEditAnnouncement(CCObject *sender);
	void callBackManage(CCObject *sender);
	void callBackRank(CCObject *sender);
	void callBackShop(CCObject *sender);
	void pageCallBack(cocos2d::CCObject *sender);

	void callBackSign(const Json::Value &requestData,const Json::Value &responseData);

	void callBackShowExp(sp::Armature * target, const char* event,const char* data);

};

#endif