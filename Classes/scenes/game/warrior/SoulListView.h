//
//  SoulListView.h
//  QSMY
//
//  Created by 汪燕军 on 13-8-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SoulListView__
#define __QSMY__SoulListView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"

class WarriorTypeMark;
class WarriorDelegate;
class SoulCell;
//curCell和boo一起使用，取消或选中，curCell都不能为空（多选时用到）boo(true增加false删除)
struct SoulTableDelegate
{
	virtual void setSelectedCell(SoulCell * pCell,SoulCell * curCell = NULL,bool boo = false) = 0;
};

#pragma mark ----------------SoulCell--------------------

class SoulCell:
public cocos2d::extension::CCTableViewCell,
public cocos2d::extension::CCBMemberVariableAssigner,
public cocos2d::extension::CCBSelectorResolver
{
public:
    SoulCell();
    virtual ~SoulCell();
    
    static SoulCell * create(cocos2d::extension::CCData * pData);
    
    bool init(cocos2d::extension::CCData * pData);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;};
    
	virtual void setSelected(bool selected);

    virtual void setSoul(warrior::SoulInfo * pSoul);
    
    void setDelegate(WarriorDelegate * pDelegate)
    {
        m_pDelegate = pDelegate;
    }

	void setSelectDelegate(SoulTableDelegate * pDelegate)
	{
		m_pSelectDelegate = pDelegate;
	}

	virtual warrior::SoulInfo * getSoul()
	{
		return m_pSoul;
	}
protected:
    void updateView();
    
    void viewCallBack(cocos2d::CCObject * pSender);
    
    void onRecruitClick(cocos2d::CCObject * sender);
    
protected:    
    warrior::SoulInfo * m_pSoul;
    
    cocos2d::CCLabelTTF * m_pName;
    cocos2d::CCNode * m_pAvatorContainer;
    //cocos2d::CCNodeRGBA *m_pRgbaGrade;
	std::vector<cocos2d::CCNode*> m_gradeBgNodes;
    cocos2d::CCLabelBMFont * m_pAmount;
    WarriorTypeMark * m_pWarriorTypeMark;
    
    cocos2d::CCNode * m_pRecruitContainer;
    DButton * m_pRecruitButton;
    cocos2d::CCSprite * m_pNotEnough;
    cocos2d::CCSprite * m_pRecruited;
    
    WarriorDelegate * m_pDelegate;
	SoulTableDelegate * m_pSelectDelegate;
};
#pragma mark ----------------SoulSelectCell 魂魄选择列表单元------------------
class SoulSelectCell
:public SoulCell
{

public:
	static SoulSelectCell * create(cocos2d::extension::CCData * pData);

	SoulSelectCell();
	virtual ~SoulSelectCell();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	void updateView();

	virtual void setSelected(bool selected);

protected:
	void onSelectClick(cocos2d::CCObject * sender);

protected:
	cocos2d::CCMenuItemImage * m_pSelect;
	bool m_bSelected;
};
#pragma mark ----------------SoulListTable--------------------

class SoulListTable:
public DLayer,
public cocos2d::extension::CCTableViewDataSource,
public cocos2d::extension::CCTableViewDelegate,
public SoulTableDelegate
{
public:
    SoulListTable();
    
    virtual ~SoulListTable();
    
    static SoulListTable * create(cocos2d::CCSize size);
    
    bool init(cocos2d::CCSize size);    
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
    
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    virtual void setSelectedCell(SoulCell * pCell,SoulCell * curCell = NULL,bool boo = false);

    void setOrder(bool isReverse)
    {
        m_bIsReverse = isReverse;
    }
    
    void setDelegate(WarriorDelegate * pDelegate)
    {
        m_pDelegate = pDelegate;
    }
    
protected:
    virtual void updateView();

    void onWarriorRecruit(cocos2d::CCObject * pObj);
    
protected:
	
    DTableView * m_pTableView;
    cocos2d::extension::CCData * m_pData;
    cocos2d::CCArray * m_pSoulList;
    WarriorDelegate * m_pDelegate;
    bool m_bIsReverse;    
	/**
     *	@brief	选中的单元
     */
    SoulCell * m_pCell;
};

#pragma mark ----------------SoulSelectTable 多选列表--------------------
class SoulSelectTable
	:public SoulListTable
{
public:
	SoulSelectTable();
	virtual ~SoulSelectTable();
	static SoulSelectTable * create(cocos2d::CCSize size);

	virtual bool init(cocos2d::CCSize size);
	virtual void setSelectedCell(SoulCell * pCell,SoulCell * curCell = NULL,bool boo = false);
	virtual void onEnter();
	virtual void onExit()
	{
		SoulListTable::onExit();
		if(m_cSelecteMore) 
		{
			m_cSelecteMore->removeAllObjects();
		}

	}

	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
public:
	void isSelectMore(bool boo)
	{
		m_bMoreSelete = boo;
	}
protected:
	CC_SYNTHESIZE_RETAIN(warrior::SoulInfo *, m_pSelectedSoul, SelectedSoul);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCArray* , m_cSelecteMore ,SelecteMore); //英雄多项选择
	virtual void updateView();
	bool m_bMoreSelete; //是否可以多选

private:
	void changeSelectList(SoulCell * pCell,bool boo); //多选更改执行
};

#pragma mark ----------------SoulListView--------------------

class SoulListView
: public DLayer
{
public:
    SoulListView()
    :DLayer()
    ,m_pTable(NULL)
    {
        
    };
    
    virtual ~SoulListView()
    {
        
    }
    
    static SoulListView * create(const cocos2d::CCSize & size);
    
    virtual bool init(const cocos2d::CCSize & size);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void setOrder(bool isReverse)
    {
        m_pTable->setOrder(isReverse);
    }
    
    void setDelegate(WarriorDelegate * pDelegate)
    {
        m_pTable->setDelegate(pDelegate);
    }
    
protected:
    SoulListTable * m_pTable;
};

#endif /* defined(__QSMY__SoulListView__) */
