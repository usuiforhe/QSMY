//
//  GuildTreeCell.h
//  公会天工树
//
//  Created by Eci on 13-10-31.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildTreeCell__
#define __QSMY__GuildTreeCell__


#define  TREE_CELL_SIZE        CCSizeMake(640, 108)

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"


class GuildTreeCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildTreeCell();
	~GuildTreeCell();

	static GuildTreeCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	//侠道王道切换背景显示
	void setType(int val)
	{
		bg_xiadao->setVisible(val == 1);
		bg_wangdao->setVisible(val != 1);
	}
#pragma mark setter
	void setData(ValueObjcet *pData);
	void setEnabled(bool isEnabled);
private:
	cocos2d::CCNode *m_pNodeEnabled;
	cocos2d::CCLabelBMFont *m_pLabelLevel;
	cocos2d::CCLabelTTF *m_pLabelName;
	cocos2d::CCLabelTTF *m_pLabelKnighthood;

	cocos2d::CCNode *m_pActive;
	cocos2d::CCNode *m_pNodeActive;
	cocos2d::CCNode *m_pNodeRequire;

	cocos2d::CCNode *bg_xiadao;
	cocos2d::CCNode *bg_wangdao;

	GuildModel *m_pModel;

	guild::GuildTreeSkill *m_pData;
};


#endif