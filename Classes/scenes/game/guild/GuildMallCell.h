///////////////////////////////////////////////////////////////////////////////////////////////////
//  
// GuildMallCell.h
// QSMY
// 门派商店列表格
//
// Created by jin.xia on 14-07-04.
// Copyright (c) 2013-2014 thedream. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __QSMY__GuildMallCell__
#define __QSMY__GuildMallCell__

#define  GUILD_STORE_TREE_CELL_SIZE        CCSizeMake( 640, 140 )

#include "AppInclude.h"
#include "components/components.h"

#include "GuildMallView.h"


class GuildMallCell
:public CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE( GuildMallViewDelegate *, m_pDelegate, Delegate ) ;

public:
	GuildMallCell() ;
	~GuildMallCell() ;
	static GuildMallCell *create() ;
	virtual bool init() ;
	virtual void onEnter() ;
	virtual void onExit() ;

    virtual bool onAssignCCBMemberVariable( cocos2d::CCObject* pTarget, 
        const char *pMemberVariableName, cocos2d::CCNode *pNode ) ;

	void setData( guild::GuildMallGoods *pData, int idx = 0 ) ;

private:
	GuildModel *m_pGuildModel ;
	guild::GuildMallGoods *m_pMallGoods ;
	drop::Drop *m_pDrop ;
	//兑换格子编号 - 确定兑换对象
	int m_iCellId ;
	int m_iIndex ;

	cocos2d::CCNode *m_pLockNode ;
	cocos2d::CCLabelBMFont *m_pUnlockLevelTF ;
	cocos2d::CCNode *m_pUnlockNode ;
	cocos2d::CCNode *m_pContainer ;
	cocos2d::CCNode *m_pHeroContainer ;
	cocos2d::CCLabelTTF *m_pNameTF ;
	cocos2d::CCLabelBMFont *m_pNumTF ;
	cocos2d::CCLabelBMFont *m_pContributeTF ;
	cocos2d::CCSprite *m_pDressMark ;
	cocos2d::CCSprite *m_pSoulMark ;
	cocos2d::CCSprite *m_pExchangeSp ;
	cocos2d::CCSprite *m_pExchangedSp ;
	DButton *m_pExchangeBtn ;
	cocos2d::CCMenuItem *m_pViewBtn ;
	cocos2d::CCNode *m_pPieceIcon ;
	cocos2d::CCNode *m_pSoulIcon ;

	void setExchanged( bool isExchanged ) ;
	void showInfoHandler( cocos2d::CCObject *sender ) ;
	void requestExchange( cocos2d::CCObject *sender ) ;
	void responseExchange( const Json::Value & requestData, const Json::Value &responseData ) ;
};

#endif