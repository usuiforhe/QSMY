//
//  PvpExchangeView.h
//  兑换奖品列表
//
//  Created by cll on 13-4-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PvpExchangeView__
#define __QSMY__PvpExchangeView__

#define PvpExGradeMax	4

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/PvpModel.h"

struct PvpDelegate;
class PvpExchangeViewCell;

#pragma mark -------------PvpExchangeView-------------
class PvpExchangeView
:public DLayer,
public cocos2d::extension::CCTableViewDataSource,
public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
    PvpExchangeView();

	virtual ~PvpExchangeView();
    
	CREATE_FUNC(PvpExchangeView);

    
    virtual bool init();

	virtual void onEnter();
	virtual void onExit();
    
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
    

	virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
private:
	void updateView();

private:
	//刷新消耗元宝提示
	bool m_bTipsShown;
	//弹框确认
	void onMessageBoxComfirm(MessageType,DMessageBox*);
	PvpModel::AwardArray * m_pRealAwards;

    DTableView *m_pListView;
	PvpDelegate *m_pDelegate;
    
    cocos2d::extension::CCData* m_pData;
	cocos2d::CCNode	*m_pCell;

	cocos2d::CCNode *m_pContainer;
	cocos2d::CCLabelBMFont *m_pBMFPvpPoints;
	cocos2d::CCLabelBMFont *m_pBMFPvpNeedPoints;
	cocos2d::CCLabelBMFont *m_pLevel;

	cocos2d::CCNode *m_pUpBox;
	cocos2d::CCNode *m_pMaxLvlImg;

	DButton *m_pDBtnRefresh;
	cocos2d::CCLabelBMFont *m_pBMFCD;
	cocos2d::CCLabelBMFont *m_pBMFItemCount;

	void refreshCallBack(CCObject *pObj);

	void update(float dt);
};

#pragma mark -------------PvpExchangeViewCell-------------
class PvpExchangeViewCell:
public cocos2d::extension::CCTableViewCell,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCBSelectorResolver
{

public:
	PvpExchangeViewCell():
    m_pDelegate(NULL),
    m_pAwardInfo(NULL),
    m_pContainer(NULL),
	m_pWarriorContainer(NULL),
    m_pExchangeContainer(NULL),
    m_pName(NULL),
    m_pNum(NULL),
    m_pDesc(NULL),
    m_pButton(NULL),
    m_pViewButton(NULL),
    m_pFontExchange(NULL),
    m_pFontDraw(NULL)
	,m_pBMFNeedPoints(NULL)
	,m_pData(NULL)
	,m_pIconSoul(NULL)
	,m_pIconPiece(NULL)
	,m_pTxtRank(NULL)
	,m_pTxtPoint(NULL)
	,m_pAmountNode(NULL)
	,m_pNeed(NULL)
	,m_pHave(NULL)
    {
		D_RESET_C_ARRAY(m_pNodeGrade, PvpExGradeMax);
    };
    
	~PvpExchangeViewCell();
    
	static PvpExchangeViewCell* create(cocos2d::extension::CCData*);
    
	virtual bool init(cocos2d::extension::CCData*);
    
	void setDelegate(PvpDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) { return NULL; };
    
//	void setAwardInfo(pvp::AwardInfo* pAwardInfo);

	void setData(pvp::ChMallItem *pData);
	//弹框确认
	void onMessageBoxComfirm(MessageType,DMessageBox*);
private:
	void menuCallback(cocos2d::CCObject *sender);
    void viewCallback(cocos2d::CCObject *sender);
	void updateView();

private:
	PvpDelegate *m_pDelegate;
	pvp::AwardInfo *m_pAwardInfo;
  
	pvp::ChMallItem *m_pData;

    cocos2d::CCNode * m_pContainer;
	cocos2d::CCNode * m_pWarriorContainer;
    cocos2d::CCNode * m_pExchangeContainer;
    cocos2d::CCLabelTTF * m_pName;
    cocos2d::CCLabelBMFont * m_pNum;
    cocos2d::CCLabelTTF * m_pDesc;
    DButton * m_pButton;
    cocos2d::CCMenuItem * m_pViewButton;
    cocos2d::CCSprite * m_pFontExchange;
    cocos2d::CCSprite * m_pFontDraw;

	cocos2d::CCLabelBMFont	*m_pBMFNeedPoints;

	cocos2d::CCNode *m_pNodeGrade[PvpExGradeMax];

	cocos2d::CCNode * m_pIconSoul;
	cocos2d::CCNode * m_pIconPiece;

	cocos2d::CCNode * m_pTxtRank;
	cocos2d::CCNode * m_pTxtPoint;

	cocos2d::CCNode * m_pAmountNode;
	cocos2d::CCLabelBMFont	*m_pNeed;
	cocos2d::CCLabelBMFont	*m_pHave;
};



#endif /* defined(__QSMY__PvpExchangeView__) */
