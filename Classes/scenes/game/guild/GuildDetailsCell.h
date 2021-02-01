//
//  GuildDetailsCell.h
//  公会详情
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildDetailsCell__
#define __QSMY__GuildDetailsCell__

#define  DETAILS_CELL_SIZE        CCSizeMake(640, 100)

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

class GuildDetailsCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildDetailsCell();
	~GuildDetailsCell();

	static GuildDetailsCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter

	void setData(ValueObjcet *pData);

protected:
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelKnighthood;

	cocos2d::CCNode *m_pBossMark;
	cocos2d::CCNode *m_pViceMark;
};


#endif