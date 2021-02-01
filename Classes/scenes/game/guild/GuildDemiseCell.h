//
//  GuildDemiseCell.h
//  公会让位
//
//  Created by Eci on 13-10-26.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildDemiseCell__
#define __QSMY__GuildDemiseCell__

#define  DEMISE_CELL_SIZE        CCSizeMake(581, 100)

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

class GuildDemiseCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildDemiseCell();
	~GuildDemiseCell();

	static GuildDemiseCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter

	void setSelected(bool isSelected);

	void setData(guild::GuildUserBasics *pData);
private:
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelTTF *m_pLabelKngihthood;
	cocos2d::CCLabelTTF *m_pLabelName;
	DButton *m_pButtonSelected;
protected:
	guild::GuildUserBasics *m_pData;
};


#endif