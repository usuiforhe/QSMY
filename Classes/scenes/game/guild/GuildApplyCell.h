//
//  GuildApplyCell.h
//  公会申请
//
//  Created by Eci on 13-10-25.

//  Copyright (c) 2013年 thedream. All rights reserved.
//




#ifndef __QSMY__GuildApplyCell__
#define __QSMY__GuildApplyCell__


#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

#define  APPLY_CELL_SIZE        CCSizeMake(640, 131)
#define  APPLY_CELL_SIZE_EXPAND        CCSizeMake(640, 174)

class GuildApplyCellDelegate
{
public:
	virtual void willDealApply(uint32_t applyID, int actoin) = 0;
};


class GuildApplyCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildApplyCell();
	~GuildApplyCell();

	static GuildApplyCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter

	void setData(ValueObjcet *pData);
	void setExpand(bool isExpanded);

	CC_SYNTHESIZE(GuildApplyCellDelegate *, m_pDelegate, Delegate);

private:
	cocos2d::CCNode *m_pContainer;

	cocos2d::CCNode *m_pNodeExpand;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelBMFont *m_pLabelFight;
	cocos2d::CCNode *m_pAdmin;

	DButton *m_pButtonAgree;
	DButton *m_pButtonRefuse;

	guild::GuildApplication *m_pData;

#pragma mark callback function

	void callBackAgree(cocos2d::CCObject *sender);
	void callBackRefuse(cocos2d::CCObject *sender);

protected:
	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*,m_pSelectButton,SelectButton);
};


#endif