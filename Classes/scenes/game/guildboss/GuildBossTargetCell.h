//
//  GuildBossTargetCell.h
//  公会主页
//
//  Created by Eci on 14-01-27.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildBossTargetCell__
#define __QSMY__GuildBossTargetCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class GuildBossTargetCell;

class GuildBossTargetCellDelegate
{
public:
	virtual void GuildBossTargetSelect(GuildBossTargetCell *pNode) = 0;
	virtual void callBackSubmit(CCObject *pSender) = 0;
};

class GuildBossTargetCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
	GuildBossTargetCell();
	~GuildBossTargetCell();

	static GuildBossTargetCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	void callBackCheck(CCObject *pSender);

	void setSelected(bool selected);

	void setData(guildboss::GuildBossBasic *pData);
	void setRank(uint32_t iRank);
	void setLastWeek(bool bLastWeek);
protected:
	CC_SYNTHESIZE_READONLY(guildboss::GuildBossBasic *, m_pData, Data);
	CC_SYNTHESIZE(GuildBossTargetCellDelegate *, m_pDelegate, Delegate);
private:
	DButton *m_pDBtnCheck;
	cocos2d::CCMenuItemImage	*m_pMenuWar;
	cocos2d::CCLabelTTF *m_pTTFName;
	cocos2d::CCLabelBMFont *m_pBMFFight;
	cocos2d::CCLabelBMFont *m_pBMFStar;
	cocos2d::CCLabelBMFont *m_pBMFRank;
	CCNode *m_pNodeSelf;
	CCNode *m_pNodeTarget;
	CCNode *m_pNodeLastWeek;
	CCNode *m_pNodeThisWeek;
	GuildBossModel *m_pModel;
};

#endif