//
//  TowerInteral.h
//  千机楼积分排行
//
//  Created by mzp on 14-8-19.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#ifndef __QSMY_Tower_Interal__
#define __QSMY_Tower_Interal__

#include "AppInclude.h"
#include "components/components.h"
#include "Tower.h"

class TowerInteral 
	: public DLayer
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCTableViewDataSource
	, public cocos2d::extension::CCTableViewDelegate
	, public TowerChildDelegate
{
public:
	TowerInteral();
	~TowerInteral();
	virtual bool init();
	CREATE_FUNC(TowerInteral);

	virtual void onEnter();
	virtual void onExit();

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	virtual void SetDelegate(TowerDelegate* pDelegate){	m_pDelegate = pDelegate;}

private:
	void onTouchBack(CCObject* sender);
	void NotificationUpdate();

private:
	TowerModel* m_pTowerModel;
	TowerDelegate* m_pDelegate;
	cocos2d::extension::CCData* m_pCCData;
	CCArray* m_pRewardList;
	DTableView*	m_pView;

	CCNode*	m_tabViewLayer;
	CCLabelBMFont* m_pNum;
};


class TowerInteralCell 
	: public cocos2d::extension::CCTableViewCell
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCBSelectorResolver
{
public:
	TowerInteralCell();
	virtual ~TowerInteralCell();
	static TowerInteralCell* create(CCData* pCCData);

	virtual bool init(CCData* pCCData);

	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}

	void setTowerExchangeInfo(tower::TowerExchangeInfo* pTowerExchangeInfo);

	void setDelegate(TowerDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}

private:
	void menuCallback(cocos2d::CCObject* sender);
	void viewCallback(cocos2d::CCObject* sender);
	void updateView();

private:
	TowerDelegate* m_pDelegate;
	tower::TowerExchangeInfo* m_pTowerExchangeInfo;
	CCNode* m_pContainer;
	CCLabelTTF* m_pName;
	CCLabelBMFont* m_pNum;
	DButton* m_pButton;
	CCNode* m_pImgDress;
	CCNode* m_pImgFrag;
	CCNode* m_pImgHunpo;
	CCLabelBMFont* m_pInteral;

	CCNode* m_pGradeOrange;
	CCNode* m_pGradeGreen;
	CCNode* m_pGradeBlue;
	CCNode* m_pGradePurple;
};


#endif	// __QSMY_Tower_Interal__