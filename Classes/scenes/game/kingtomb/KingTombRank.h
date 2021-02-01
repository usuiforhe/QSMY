//
//  KingTombRank.h
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_KingTombRank___
#define __QSMY_KingTombRank___


#include "AppInclude.h"
#include "components/components.h"


class KingTombModel;

class KingTombRank 
	: public DPopup
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCTableViewDataSource
{
public:
	KingTombRank();
	virtual ~KingTombRank();

	CREATE_FUNC(KingTombRank);

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView* table, uint32_t idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView* table);
	virtual CCSize tableCellSizeForIndex(CCTableView* table, unsigned int idx);

private:
	void rewardCallBack(cocos2d::CCObject* sender);
	void logCallBack(cocos2d::CCObject* sender);
	void cancelCallBack(cocos2d::CCObject* sender);
	void updateView();

private:
	KingTombModel* m_pKingTombModel;
	cocos2d::extension::CCData* m_pCellData;
	cocos2d::CCArray* m_pUserRankList;
	cocos2d::CCArray* m_pGuildRankList;

	cocos2d::CCMenuItem* m_pBtnReward;
	cocos2d::CCMenuItem* m_pBtnLog;
	cocos2d::CCNode* m_pContainer;
	DButton* m_pBtnCancel;
	DButton* m_pCloseButton;

	DTableView* m_pTvUser;
	DTableView* m_pTvGuild;
};



#endif	// __QSMY_KingTombRank___