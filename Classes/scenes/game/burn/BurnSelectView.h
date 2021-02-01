//BurnSelectview
//炼化英雄（魂魄）选择界面
//
#ifndef __PILI__BurnSelectView__
#define __PILI__BurnSelectView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"
#include "../warrior/WarriorListView.h"
#include "../Game.h"
#include "../warrior/SoulListView.h"

#pragma mark ----------------BurnListSelectTable 弟子选择列表------------------
class BurnListSelectTable
	: public FiltableWarriorList
{
public:
	BurnListSelectTable();
	virtual ~BurnListSelectTable();

	static BurnListSelectTable * create(cocos2d::CCSize size);

	virtual bool init(cocos2d::CCSize size);
	virtual void setSelectedCell(WarriorCell * pCell,WarriorCell * curCell = NULL,bool boo = false);
	virtual void onExit()
	{
		FiltableWarriorList::onExit();
		if(m_cSelecteMore) 
		{
			m_cSelecteMore->removeAllObjects();
		}
	}

	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);

	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
public:
	void isSelectMore(bool boo)
	{
		m_bMoreSelete = boo;
	}
protected:
	CC_SYNTHESIZE_RETAIN(warrior::WarriorCurInfo *, m_pSelectedWarrior, SelectedWarrior);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray* , m_cSelecteMore ,SelecteMore); //英雄多项选择
	bool m_bMoreSelete; //是否可以多选

private:
	void changeSelectList(WarriorCell * pCell,bool boo); //多选更改执行
};

#pragma mark ----------------BurnSelectView 炼化选择弹出------------------
class BurnSelectView
	: public DLayer
	, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	BurnSelectView();

	~BurnSelectView();

	static BurnSelectView * create(WarriorFilterDelegate * pFilter);

	virtual bool init(WarriorFilterDelegate * pFilter);

	virtual void onEnter();

	virtual void onExit();

	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

	inline void setDelegate(WarriorSelectDelegate * pDelegate)
	{
		m_pDelegate = pDelegate;
	}

	void setOrder(bool isReverse)
	{
		m_pSelectTable->setOrder(isReverse);
	}

	void setMoreSelect(bool boo)
	{
		isMoreSelet = boo;
		m_pSelectTable->isSelectMore(boo);
		m_pSoulTable->isSelectMore(boo);
	}

	void setFilter(WarriorFilterDelegate * pFilter);

	void setSelectedWarrior(warrior::WarriorCurInfo * pSelectedWarrior);

protected:
	void onConfirmClick(cocos2d::CCObject * sender);
	void onBackClick(cocos2d::CCObject * sender);

protected:
	BurnListSelectTable * m_pSelectTable;
	SoulSelectTable* m_pSoulTable;
	cocos2d::CCNode * m_pContainer;
	cocos2d::CCNode * m_pBurnTips;
	DButton * m_pConfirmButton;
	DButton * m_pBackButton;
	warrior::WarriorCurInfo * pSelectedWarrior;

	WarriorSelectDelegate * m_pDelegate;
	bool isMoreSelet;
private:
	DTabView                *m_pDTabView;
	CCNode*  m_pTabContainer;
};

#endif //__PILI__BurnSelectView__