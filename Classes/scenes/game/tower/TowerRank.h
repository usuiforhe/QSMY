//
//  TowerRank.h
//  千机楼-排行榜
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TowerRank__
#define __QSMY__TowerRank__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/TowerModel.h"
#include "../pvp/Pvp.h"
#include "Tower.h"
#include "../common/OtherFormationView.h"


USING_NS_CC;
USING_NS_CC_EXT;

class TowerModel;
class TowerRankInfo;

class TowerRank
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public TowerChildDelegate
{
public:
	typedef cocos2d::CCArray TowerRankCellArray;
	TowerRank();
	~TowerRank();
	virtual bool init();
	CREATE_FUNC(TowerRank);
public:
    
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
private:
	virtual void UpdateInfo(){};
	void updateView();
private:
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
private:
	TowerModel*			m_pTowerModel;
	TowerDelegate*		m_pDelegate;
public:
	virtual void SetDelegate(TowerDelegate* pDelegate){	m_pDelegate = pDelegate;}
private:
	void onTouchBack(CCObject*);
	void onRankShow(CCObject*);

	void setRankState();
private:
	CCMenuItemImage*	m_btnBack;
	//CCLabelBMFont*		m_UserRank;
	CCNode*				m_tabViewLayer;
	DTableView*			m_pView;
	TowerRankCellArray*	m_pArrCells;
	cocos2d::extension::CCData *m_pCCData;

	CCMenuItemImage*            m_pRankBtn;
	CCNode*             m_pNowTxt;
	CCNode*             m_pOldTxt;
	CCNode*             m_pNowTitle;
	CCNode*             m_pOldTitle;

	bool                isNowRank;
};

class TowerRankCell :
public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
//,public cocos2d::extension::CCBSelectorResolver
{
public:
    CC_SYNTHESIZE(TowerDelegate*, m_pDelegate, Delegate);
    
    TowerRankCell();
    virtual ~TowerRankCell();
	static TowerRankCell* create(TowerRank *pRank, CCData* pCCData);
public:
    virtual bool init(TowerRank *pRank, CCData* pCCData);
	virtual bool				 onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
	//virtual SEL_MenuHandler		 onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName){return NULL;}
private:
	void onLookUser(CCObject*);
private:
	DButton*	m_lookUserInfo;
    
	CCLabelBMFont*		m_rankNum;
	CCLabelBMFont*		m_userLev;
	CCLabelBMFont*		m_todayTotal;
	CCLabelTTF*			m_userName;
	CCLabelBMFont*		m_allTotal;
	CCNode*				m_userimg;
private:
	TowerModel*			m_pTowerModel;
	TowerRank*			m_pRank;
public:
    void updateCell(const tower::TowerRankInfo *towerRankInfo);
};

#endif /* defined(__QSMY__TowerRank__) */
