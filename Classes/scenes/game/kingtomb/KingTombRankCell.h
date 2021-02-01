//
//  KingTombHistoryCell.h
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_KingTomb_Rank_Cell___
#define __QSMY_KingTomb_Rank_Cell___

#include "AppInclude.h"
#include "components/components.h"
#include "model/KingTombModel.h"

#define RANK_ICON_COUNT	3
class KingTombRankCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	KingTombRankCell();
	~KingTombRankCell();

	static KingTombRankCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	//帮会排行
	void setGuildRankData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank);
	//本帮排行
	void setUserRankData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank);
	void setData(kingTomb::kingTombRankInfo* pInfo , unsigned int rank);
private:
	cocos2d::CCLabelBMFont *m_pLabelRank;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelCount;
	cocos2d::CCLabelBMFont *m_pLabelFight;
	cocos2d::CCLabelTTF    *m_pLabelName;
	cocos2d::CCNode		   *m_pGuildNode;
	cocos2d::CCNode		   *m_pUserNode;
	cocos2d::CCNode		   *m_pRankIcon[RANK_ICON_COUNT];
};


#endif	// __QSMY_KingTomb_Rank_Cell___
