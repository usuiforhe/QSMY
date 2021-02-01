//
//  PvpView.h
//  论剑可挑战列表
//
//  Created by cll on 13-4-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpView__
#define __QSMY__PvpView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/PvpModel.h"

struct PvpDelegate;
class PvpViewCell;
class PvpBossView;
class PvpRobotView;

#pragma mark ------------PvpView--------------
class PvpView:
public DLayer,
public cocos2d::extension::CCTableViewDataSource,
public cocos2d::extension::CCTableViewDelegate,
public cocos2d::extension::CCBMemberVariableAssigner
{    
public:
	PvpView();

	~PvpView();
    
	CREATE_FUNC(PvpView);

    virtual bool init();
    
	virtual void onEnter();
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	void setDelegate(PvpDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
	void setSlideEnabled(bool val){
		doSlide = val;
	}
private:
	void updateView();

private:
    DTableView          *m_pListView;
	PvpDelegate         *m_pDelegate;
    
// 	PvpBossView         *m_pBoss;		//东皇太一
// 	PvpRobotView        *m_pRobot;    //试炼木人
//     cocos2d::CCNode     *m_pListContainer;
    
	cocos2d::CCNode	*m_pContainer;

    cocos2d::extension::CCData * m_pData;
	cocos2d::CCNode	*m_pCell;
    
	cocos2d::CCArray    *m_pTargets;
	cocos2d::CCDictionary      * m_pRanks;

	CCLabelBMFont *m_pBMFRank;
	CCLabelBMFont *m_pBMFPvpPoints;
	CCLabelBMFont *m_pTTFCountDown;

	void update(float dt);

	CCSprite *m_pCloud;
	//是否滑动
	bool doSlide;
};

#endif /* defined(__QSMY__PvpView__) */
