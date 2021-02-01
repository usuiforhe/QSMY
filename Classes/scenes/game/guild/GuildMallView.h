///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GuildMallView.h
// QSMY
// 门派商店
//
// Created by jin.xia on 14-07-04.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __QSMY__GuildMallView__
#define __QSMY__GuildMallView__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"
#include "../inventory/InventoryDelegate.h"
#include "../Game.h"

#include "GuildHead.h"


class GuildMallViewDelegate
:public InventoryDelegate
{
public:
    virtual void willCloseStoreView() = 0 ;
    virtual void showItemInfoView( item::ItemInfo *pItemInfo ) = 0 ;
    virtual void showWarriorInfoView( warrior::WarriorBaseInfo *pWarrior ) = 0 ;
};


class GuildMallView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
    CC_SYNTHESIZE( GuildMallViewDelegate *, m_pDelegate, Delegate ) ;

public:
    GuildMallView();
    ~GuildMallView();
    static GuildMallView *create() ;
    virtual bool init() ;

    virtual void onEnter() ;
    virtual void onExit() ;
    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, 
        const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;

    // CCTableViewDataSource
    virtual uint32_t numberOfCellsInTableView( CCTableView *table ) ;
    virtual CCSize tableCellSizeForIndex( CCTableView *table, unsigned int idx ) ;
    virtual CCTableViewCell* tableCellAtIndex( CCTableView *table, uint32_t idx ) ;

    // CCTableViewDelegate
    virtual void tableCellTouched( CCTableView* table, CCTableViewCell* cell ) ;
    virtual void scrollViewDidScroll( cocos2d::extension::CCScrollView* view ) {} ;
    virtual void scrollViewDidZoom( cocos2d::extension::CCScrollView* view ) {} ;

private:
    GuildModel *m_pGuildModel ;
	bool m_isExpShown ;

    DTableView *m_pListView ;

    //GuildHead *m_pGuildHead ;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelPosition;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelExp;
	DProgressBar* m_pExpBar;				/*经验条*/
	cocos2d::CCLabelBMFont *m_pLabelNextExp;

    cocos2d::CCMenuItem *m_pBackBtn ;
    DButton *m_pRefreshBtn ;
    cocos2d::CCNode *m_pContainer ;
    cocos2d::CCLabelBMFont *m_pCrtContributeTF ;
    cocos2d::CCLabelBMFont *m_pTotalContributeTF ;
    cocos2d::CCLabelBMFont *m_pRemainTimeTF ;
    cocos2d::CCLabelBMFont *m_pRemainCountTF ;
	cocos2d::CCLabelBMFont *m_pPoint ;
    void updateView() ;
    void updateTree() ;
    void update( float dt ) ;
    void backHandler( cocos2d::CCObject *sender ) ;
    void refreshHandler( cocos2d::CCObject *sender ) ;
};

#endif