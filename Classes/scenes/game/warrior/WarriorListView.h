//
//  WarriorListView.h
//  弟子选择 弟子列表
//
//  Created by 汪燕军 on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorListView__
#define __QSMY__WarriorListView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/WarriorModel.h"


struct WarriorSelectDelegate
{
    virtual void setSelectedWarrior(warrior::WarriorCurInfo * pWarrior) = 0;
	//数据集合，heroType英雄1（魂魄2）
	virtual void setSelectedWarriorMore(cocos2d::CCArray *pWarrior,uint8_t heroType) = 0;
};

struct WarriorSortDelegate
{
	virtual void setSortList(warrior::WarriorSortType type) = 0;
};

class WarriorCell;


//curCell和boo一起使用，取消或选中，curCell都不能为空（多选时用到）boo(true增加false删除)
struct WarriorListTableDelegate
{
    virtual void setSelectedCell(WarriorCell * pCell,WarriorCell * curCell = NULL,bool boo = false) = 0;
};

class WarriorTypeMark;
class WarriorDelegate;

#pragma mark ----------------WarriorCell 弟子单元基类------------------
class WarriorCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    static WarriorCell * create(cocos2d::extension::CCData * pData);
    
    WarriorCell();
    
    virtual ~WarriorCell();
    
    virtual bool init(cocos2d::extension::CCData * pData);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    
    virtual void setWarrior(warrior::WarriorCurInfo * pWarrior, bool updateView = true);
    
    virtual warrior::WarriorCurInfo * getWarrior()
    {
        return m_pWarrior;
    }
    
    virtual void setDelegate(WarriorListTableDelegate * pDelegate)
    {
        m_pListTableDelegate = pDelegate;
    }
    
    virtual void setSelected(bool selected);
    
protected:
    
    virtual void updateView();
    
protected:
    warrior::WarriorCurInfo * m_pWarrior;
    
    cocos2d::CCLabelTTF * m_pName;
    //cocos2d::CCNodeRGBA * m_pRgbaGrade;
	std::vector<cocos2d::CCNode*> m_gradeBgNodes;

    cocos2d::CCLabelBMFont * m_pLevel;
    cocos2d::CCLabelBMFont * m_pUpLevel;
    cocos2d::CCLabelBMFont * m_pFight;
    cocos2d::CCSprite * m_pInFormation;
    cocos2d::CCNode * m_pAvatorContainer;
	cocos2d::CCLabelBMFont  *m_pZizhiNum;
    //cocos2d::CCSprite * m_pGradeLevel;
    
    WarriorTypeMark * m_pWarriorTypeMark;
    
    WarriorListTableDelegate * m_pListTableDelegate;

private:
	CCNode* getGradeNodeByColorGrade(warrior::WarriorGrade grade);
};


#pragma mark ----------------WarriorListCell 弟子列表单元------------------
class WarriorListCell
:public WarriorCell
{
public:
    static WarriorListCell * create(cocos2d::extension::CCData * pData);
    
    WarriorListCell();
    
    virtual ~WarriorListCell();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    

    
    virtual void setSelected(bool selected);
    
    virtual void setDelegate(WarriorDelegate * pWarriorDelegate)
    {
        m_pWarriorDelegate = pWarriorDelegate;
    }
    
    virtual void setDelegate(WarriorListTableDelegate * pDelegate)
    {
        WarriorCell::setDelegate(pDelegate);
    }
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void setWarrior(warrior::WarriorCurInfo * pWarrior, bool updateView = true);
    void updatePos();
protected:
    void updateView();
    void binding();
    
protected:
    void onTransferClick(cocos2d::CCObject * sender);
    void onDevelopClick(cocos2d::CCObject * sender);
    void onPromoteClick(cocos2d::CCObject * sender);
    void onAvatarClick(cocos2d::CCObject * sender);
    void onExplodeClick(cocos2d::CCObject * sender);
    
protected:
    cocos2d::CCSprite * m_pUpAvailable;
    cocos2d::CCNode * m_pExplodeContainer;
    DButton * m_pStrengthen;
    DButton * m_pDevelop;
    DButton * m_pPromote;
    cocos2d::CCMenuItem * m_pAvatarButton;
    cocos2d::CCMenuItem * m_pExplodeButton;
    cocos2d::CCNode * m_pRoot;
    //cocos2d::CCNode * m_pPromoteContainer;
    WarriorDelegate * m_pWarriorDelegate;
    
    CC_SYNTHESIZE(bool, m_bExploded, Exploded);
};

#pragma mark ----------------WarriorListCellSelect 弟子选择列表单元------------------
class WarriorListCellSelect
:public WarriorCell
,public cocos2d::extension::CCBSelectorResolver
{
public:
    static WarriorListCellSelect * create(cocos2d::extension::CCData * pData);
    
    WarriorListCellSelect();
    
    virtual ~WarriorListCellSelect();
    
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

struct WarriorFilterDelegate
: public cocos2d::CCObject
{
    WarriorFilterDelegate():cocos2d::CCObject(){}
    virtual ~WarriorFilterDelegate(){}
    virtual bool operator()(warrior::WarriorCurInfo * pWarrior) = 0;
};

#pragma mark ----------------FiltableWarriorList 可筛选的弟子列表------------------
class FiltableWarriorList
: public DLayer
, public cocos2d::extension::CCTableViewDataSource
, public cocos2d::extension::CCTableViewDelegate
, public WarriorListTableDelegate
,public WarriorSortDelegate
{
public:
    FiltableWarriorList();
    virtual ~FiltableWarriorList();
    
    virtual bool init(cocos2d::CCSize size);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx) = 0;
    
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view){};
    
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    
    /**
     *	@brief	设置排序
     *
     *	@param 	isReverse 	是否反序
     */
    void setOrder(bool isReverse)

    {
        m_bIsReverse = isReverse;
    }
	/**
     *	@brief	设置排序
	 * 根据类型不同排序
	 */
	virtual void setSortList(warrior::WarriorSortType type);
    
    /**
     *	@brief	设置过滤器
     *
     *	@param 	pFilter 	过滤器
     */
    void setFilter(WarriorFilterDelegate * pFilter);
   
    /**
     *	@brief	设置选中单元
     *
     *	@param 	pCell 	选中单元
     */
    virtual void setSelectedCell(WarriorCell * pCell,WarriorCell * curCell = NULL,bool boo = false);
    
protected:
    void updateView();
    
protected:
    DTableView * m_pTableView;
    
    /**
     *	@brief	弟子过滤器
     */
    WarriorFilterDelegate * m_pFilter;

    /**
     *	@brief	CCB数据
     */
    cocos2d::extension::CCData * m_pData;
    
    /**
     *	@brief	弟子列表
     */
    cocos2d::CCArray * m_pWarriorList;
    
    /**
     *	@brief	选中的单元
     */
    WarriorCell * m_pCell;

    /**
     *	@brief	是否反序
     */
    bool m_bIsReverse;

};

#pragma mark ----------------WarriorListTable 弟子列表------------------
class WarriorListTable
: public FiltableWarriorList
{
public:
    WarriorListTable();
    virtual ~WarriorListTable();
    
    static WarriorListTable * create(cocos2d::CCSize size);
    
    virtual bool init(cocos2d::CCSize size);
    
   virtual void setSelectedCell(WarriorCell * pCell,WarriorCell * curCell = NULL,bool boo = false);
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
        
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
        
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    virtual void setDelegate(WarriorDelegate * pWarriorDelegate)
    {
        m_pWarriorDelegate = pWarriorDelegate;
    }
        
protected:
    warrior::WarriorCurInfo * m_pExplodedWarrior;
    WarriorDelegate * m_pWarriorDelegate;
};

#pragma mark ----------------WarriorListSelectTable 弟子选择列表------------------

class WarriorListSelectTable
: public FiltableWarriorList
{
public:
    WarriorListSelectTable();
    virtual ~WarriorListSelectTable();
    
    static WarriorListSelectTable * create(cocos2d::CCSize size);
    
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
};

#pragma mark ----------------WarriorListView 弟子列表----------------------
class WarriorListView
: public DLayer
,public WarriorSortDelegate
{
public:
    WarriorListView()
    :DLayer()
    ,m_pTable(NULL)
    {
        
    };
    
    virtual ~WarriorListView()
    {
        
    }
    
    static WarriorListView * create(WarriorDelegate * pWarriorDelegate, WarriorFilterDelegate * pFilter, const cocos2d::CCSize & size);
    
    virtual bool init(WarriorDelegate * pWarriorDelegate,WarriorFilterDelegate * pFilter, const cocos2d::CCSize & size);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void setOrder(bool isReverse)
    {
        m_pTable->setOrder(isReverse);
    }

	virtual void setSortList(warrior::WarriorSortType type);
    
protected:
    WarriorListTable * m_pTable;
};

#pragma mark ----------------WarriorSelectPopup 弟子选择弹出------------------

class WarriorSelectPopup
: public DLayer
, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    WarriorSelectPopup();
    
    ~WarriorSelectPopup();
    
    static WarriorSelectPopup * create(WarriorFilterDelegate * pFilter);
    
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
	}
    
    void setFilter(WarriorFilterDelegate * pFilter);
    
    void setSelectedWarrior(warrior::WarriorCurInfo * pSelectedWarrior);
    
protected:
    void onConfirmClick(cocos2d::CCObject * sender);
    void onBackClick(cocos2d::CCObject * sender);
    
protected:
    WarriorListSelectTable * m_pSelectTable;
    
    cocos2d::CCNode * m_pContainer;
    DButton * m_pConfirmButton;
    DButton * m_pBackButton;
    warrior::WarriorCurInfo * pSelectedWarrior;
    
    WarriorSelectDelegate * m_pDelegate;
	bool isMoreSelet;
};

#endif /* defined(__QSMY__WarriorListView__) */
