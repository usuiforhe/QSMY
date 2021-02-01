//
//  TeamChapterCell.h
//  组队章节
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__TeamChapterCell__
#define __QSMY__TeamChapterCell__

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"

class TeamChapterCell;

class TeamChapterCellDelegate
{
public:
	virtual void TeamChapterCellSelected(TeamChapterCell *pCell) = 0;
	virtual void TeamChapterCellViewItem(TeamChapterCell *pCell) = 0;
};

class TeamChapterCell 
	:public cocos2d::extension::CCTableViewCell
	,public cocos2d::extension::CCBMemberVariableAssigner
	,public cocos2d::extension::CCBSelectorResolver
{
	CC_SYNTHESIZE(TeamChapterCellDelegate *, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(friends::ChapterInfo *, m_pData, Data);
public:
	TeamChapterCell();
	~TeamChapterCell();

	static TeamChapterCell* create();
	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	void updateView();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };

	void setData(friends::ChapterInfo *pData);
	void setNew(bool bNew);
private:
	CCNode *m_pNodeBG;
	CCRichLabelTTF *m_pTTFName;
	CCNode *m_pNodeDropContainer;
	CCNodeRGBA *m_pRgbaColor;

	CCNode *m_pNodeFree;
	CCNode *m_pNodePayed;
	CCNode *m_pNodeNew;

	void callBackTouch(CCObject *pSender);
	void callBackView(CCObject *pSender);
};

#endif