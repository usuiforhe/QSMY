//
//  GuildCell.h
//  公会主页
//
//  Created by Eci on 13-10-28.

//  Copyright (c) 2013年 thedream. All rights reserved.
//


#ifndef __QSMY__GuildCell__
#define __QSMY__GuildCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "../../../managers/GameManager.h"

class GuildCell
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	GuildCell();
	~GuildCell();

	static GuildCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

#pragma mark setter

	void setEnabled(bool isEnabled);

	void setFileName(const char *pFileName);
	void setTime(uint32_t iTime);

private:
	cocos2d::CCLabelBMFont *m_pLabelTime;
	cocos2d::CCLayer *m_pSpriteEnabled;
	cocos2d::CCLayer *m_pSpriteDisabled;

protected:
	std::string m_sFileName;
	bool m_isEnabled;
};


#endif