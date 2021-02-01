//
//  KingTombHistory.h
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_KingTomb_History___
#define __QSMY_KingTomb_History___


#include "AppInclude.h"
#include "components/components.h"


class KingTombModel;

class KingTombHistory 
	: public DPopup
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCTableViewDataSource
{
public:
	KingTombHistory();
	virtual ~KingTombHistory();

	CREATE_FUNC(KingTombHistory);

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
	void receiveCallBack(cocos2d::CCObject* sender);
	void cancelCallBack(cocos2d::CCObject* sender);

	void updateReward();
	void updateLog();

private:
	KingTombModel* m_pKingTombModel;
	cocos2d::extension::CCData* m_pLogData;
	cocos2d::extension::CCData* m_pRewardData;
	cocos2d::CCArray* m_pAssistBonusList;
	cocos2d::CCArray* m_pLogList;

	cocos2d::CCMenuItem* m_pBtnReward;
	cocos2d::CCMenuItem* m_pBtnLog;
	cocos2d::CCSprite* m_pNodeNewReward;
	cocos2d::CCSprite* m_pNodeNewLog;
	cocos2d::CCNode* m_pContainer;
	cocos2d::CCNode* m_pBtnNodeReward;
	cocos2d::CCNode* m_pBtnNodeLog;
	DButton* m_pBtnCancel;
	DButton* m_pBtnReceive;
	DButton* m_pBtnCancel1;
	DButton* m_pCloseButton;

	DTableView* m_pTvReward;
	DTableView* m_pTvLog;
};



#endif	// __QSMY_KingTomb_History___