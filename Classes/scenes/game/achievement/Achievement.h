//
//  Achievement.h
//  成就页面
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__Achievement__
#define __QSMY__Achievement__

#define ACHIEVE_TAB_COUNT	4

#include "AppInclude.h"
#include "components/components.h"
#include "../Game.h"
#include "managers/DataManager.h"
#include "AchievementCell.h"
#include "../common/common.h"

class Achievement
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public DTabViewDelegate
,public AchievementDelegate
{
public:
    
    Achievement();
    virtual ~Achievement();
	CREATE_FUNC(Achievement);
	virtual bool init();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {};
	virtual void scrollViewDidScroll(CCScrollView* view) {};
	virtual void scrollViewDidZoom(CCScrollView* view) {};

	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void SelectTabAtIndex(DTabView *tabView, uint32_t idx);

	virtual void onEnter();
	virtual void onExit();

	virtual void onView(unsigned int id , drop::DropType type);
private:
    DButton	*m_pCloseButton;

	void closeCallBack(CCObject* obj);

	CCNode	*m_pCell;

	CCNode								*m_pTableContainer;
	CCNode								*m_pNodeContainer;
	cocos2d::CCLabelBMFont	*m_pBMFPercent;
	DProgressBar						*m_pPBPercent;
	ItemBox								*m_pItemBox;
	DButton								*m_pBtnGet;
	cocos2d::CCLabelBMFont	*m_pBMFScore;

	DTableView *m_pTableView[ACHIEVE_TAB_COUNT];

	cocos2d::CCArray *m_pArr[ACHIEVE_TAB_COUNT];

	cocos2d::CCMenuItem *m_pBtn[ACHIEVE_TAB_COUNT];
	cocos2d::CCLabelBMFont *m_pTabLabel[ACHIEVE_TAB_COUNT];

	void tabCallBack(CCObject *pObj);

	DTabView *m_pTabView;

	AchievementModel *m_pModel;
	uint8_t m_currentTag;

	//更新界面
	void updateView();
	//更新积分奖励
	void updateAward();
	void getScoreAward(CCObject*);
};


class AchievementEffect : public DEffectNode
{
public:
	AchievementEffect();
	~AchievementEffect();

	static AchievementEffect* create();
	virtual bool init();

private:
	sp::Armature* m_pFlash;
private:
	void animateEnd(sp::Armature * target, const char* event,const char* data);
	void showFlash();
};

#endif /* defined(__QSMY__Achievement__) */
