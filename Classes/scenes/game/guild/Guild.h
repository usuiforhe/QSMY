//
//  Guild.h
//  公会视图
//
//  Created by Eci on 13-10-23.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__Guild__
#define __QSMY__Guild__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "GuildView.h"
#include "GuildNoneView.h"
#include "GuildCreateView.h"
#include "GuildJoinView.h"
#include "GuildRankView.h"
#include "GuildManageView.h"
#include "GuildMallView.h"

#include "../inventory/ItemInfoView.h"
#include "../warrior/WarriorInfo.h"
#include "../inventory/InventoryDelegate.h"

#include	"../kingtomb/KingTomb.h"
#include "../guildboss/GuildBoss.h"

class GuildView;
class GuildNoticeView;

enum GuildPageType
{
	kGuildPageNone	= 0,
	kGuildPageGuild	= 1,
	kGuildPageBoss		= 2,
	kGuildPageTomb	= 3,
};
// class GuildDelegate
// {
// public:
// //	virtual void openPopup(CCNode *pPopup) = 0;
// 	virtual void openUpperPopup(CCNode *pPopup) = 0;
// };

class Guild 
	:public DLayer
	,public cocos2d::extension::CCBSelectorResolver
	,public GuildViewDelegate
	,public GuildNoneViewDelegate
	,public GuildCreateViewDelegate
	,public GuildJoinViewDelegate
	,public GuildRankViewDelegate
	,public GuildManageViewDelegate
	,public GuildQuestionViewDelegate
	,public GuildTreeViewDelegate
	,public GuildMallViewDelegate
	,public GuildKingTombDelegate
//	,public GuildDelegate
	,public BackKeyInterface
{
public:
	Guild();
	~Guild();

	static Guild* create(GuildPageType val);
	virtual bool init(GuildPageType val);

	virtual void onEnter();
	virtual void onExit();

	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	//魔界探险 苍龙现 中的弹框 添加于公会的顶层
	virtual void openTopPopup(cocos2d::CCLayer*);

#pragma mark GuildNoneViewDelegate

	virtual void willJoinGuild();
	virtual void willCreateGuild();
	virtual void willClose();

#pragma mark GuildCreateViewDelegate

	virtual void createGuildWithName(const char * pGuildName);

	void createGuildWithName(const Json::Value &requestData,const Json::Value &responseData);

	virtual void closeGuildCreateView();
	
#pragma mark GuildJoinViewDelegate

	virtual void closeGuildSearchView();

#pragma mark GuildViewDelegate

	virtual void willShowRankView();

	virtual void willShowManageView();

	virtual void willShowTreeView();
	virtual void willShowMallView();
	virtual void willShowQuestionView();
	virtual void willShowNoticeView();

#pragma mark GuildRankViewDelegate

	virtual void willCloseRankView();

#pragma mark GuildManageViewDelegate

	virtual void willCloseManageView();

#pragma mark GuildQuestionViewDelegate
	virtual void willCloseQuestionView(uint32_t dropID = 0);

#pragma mark GuildTreeViewDelegate
	virtual void willCloseTreeView();

#pragma mark GuildMallViewDelegate
	virtual void willCloseStoreView() ;
	virtual void showItemInfoView( item::ItemInfo *pItemInfo ) ;
	virtual void showWarriorInfoView( warrior::WarriorBaseInfo *pWarrior ) ;
	virtual void closeInfoView( InventoryType type ) ;


#pragma mark GuildDelegate
	virtual void openUpperPopup(CCNode *popUp);
	virtual void openGuildDetail(CCNode *popUp);
private:

	cocos2d::CCSprite *m_pBackground;
	GuildModel *m_pModel;
	GuildView *m_pGuildView;
	GuildNoneView *m_pNoneView;
	GuildCreateView *m_pCreateView;
	GuildJoinView *m_pJoinView;
	GuildRankView *m_pRankView;
	GuildManageView *m_pManageView ;
	ItemInfoView *m_pItemInfoView ;
	WarriorInfoView *m_pSoulInfoView ;
	cocos2d::CCNode	*m_pCommonLayer;
	cocos2d::CCNode	*m_pPopupLayer;
	cocos2d::CCNode	*m_pPopupLayerUpper;

	KingTomb*	m_pTombView;
	GuildBoss*	m_pBossView;

	void updateView();
	void updateViewAfterQuiting();
	void footerCallBack(cocos2d::CCObject*);
	void showNoGuildError();

private:
	uint32_t m_nGuildID;
	//进入帮会系统时的type
	GuildPageType m_CreatePageType;
	//帮会当前页
	GuildPageType m_CurrentPageType;
protected:
	CC_SYNTHESIZE(NavigationDelegate *, m_pDelegate, Delegate);

public:
	virtual void onBackKey();
};

#endif