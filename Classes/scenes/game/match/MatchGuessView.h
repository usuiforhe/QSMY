//
//  MatchGuessView.h
//  淘汰赛比赛对阵-竞猜列表
//
//  Created by cll on 13-4-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchGuessView__
#define __QSMY__MatchGuessView__

#include "AppInclude.h"
#include "../../../managers/DataManager.h"
#include "../../../components/components.h"
#include "MatchUserHead.h"
#include "MatchBetMenu.h"

class MatchDelegate;

class MatchGuessListView
:public DLayer
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCTableViewDelegate
{

public:
	MatchGuessListView();
	virtual ~MatchGuessListView();

	static MatchGuessListView * create(MatchDelegate *pDelegate);

	virtual bool init(MatchDelegate *pDelegate);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){}
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}

	virtual void onEnter();
    virtual void onExit();

	void menuCallBack(CCObject * obj);
    
private:
    void updateView();
    
private:
    cocos2d::CCLabelBMFont* m_pTfTimes;
    cocos2d::CCLabelBMFont* m_pTfNum;
    cocos2d::CCLabelBMFont* m_pTfTime8;
    cocos2d::CCLabelBMFont* m_pTfTime4;
    cocos2d::CCLabelBMFont* m_pTfTime2;

	cocos2d::CCNode*	m_pContainer;
	cocos2d::CCNode*	m_pNode8_4;
	cocos2d::CCNode*	m_pNode4_2;
	cocos2d::CCNode*	m_pNode2_1;
    
private:
	DSlideView   *m_pSlideView;
	MatchDelegate *m_pDelegate;
	MatchModel *m_pMatchModel;
    int32_t    m_nCurrentPage;
    bool isFirstEnter;

	cocos2d::CCLabelBMFont      *m_pTfItemHas;
};



class MatchGuessView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{
public:
	MatchGuessView();
 	virtual ~MatchGuessView();

	static MatchGuessView *create(match::MatchFinalCat finalNum);

    virtual bool init(match::MatchFinalCat finalNum);

	virtual void onEnter();
	virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
	void setDelegate(MatchDelegate* pDelegate)
	{
		m_pDelegate = pDelegate;
	}
    
protected:

    void updateVS();
	virtual void updateVSInfo();
    
    void updateBet();
	virtual void updateBetInfo();
    
	void updateMatchGuessList();
    
    
    void reset();
    
    void betCallBack(cocos2d::CCObject*);
//    void rewardCallBack(cocos2d::CCObject*);
    
protected:
    
	match::MatchFinalCat    m_finalNum;
	MatchModel              *m_pMatchModel;
	MatchDelegate           *m_pDelegate;
	MatchModel::VSInfoVec   *m_VSInfoVec;
    
    cocos2d::CCArray* m_pVsList;
    cocos2d::CCArray* m_pBetList;

};


class MatchGuessView8:public MatchGuessView
{
    
public:
    MatchGuessView8();
    virtual ~MatchGuessView8();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual bool init(match::MatchFinalCat finalNum);

private:
    MatchUserHead   *m_pVs0;
    MatchUserHead   *m_pVs1;
    MatchUserHead   *m_pVs2;
    MatchUserHead   *m_pVs3;
    MatchUserHead   *m_pVs4;
    MatchUserHead   *m_pVs5;
    MatchUserHead   *m_pVs6;
    MatchUserHead   *m_pVs7;
    
    MatchBetMenu    *m_pBet0;
    MatchBetMenu    *m_pBet1;
    MatchBetMenu    *m_pBet2;
    MatchBetMenu    *m_pBet3;
};

class MatchGuessView4:public MatchGuessView
{
    
public:
    MatchGuessView4();
    virtual ~MatchGuessView4();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual bool init(match::MatchFinalCat finalNum);
private:
    MatchUserHead   *m_pVs0;
    MatchUserHead   *m_pVs1;
    MatchUserHead   *m_pVs2;
    MatchUserHead   *m_pVs3;
    
    MatchBetMenu    *m_pBet0;
    MatchBetMenu    *m_pBet1;

};

class MatchGuessView2:public MatchGuessView
{
    
public:
    MatchGuessView2();
    virtual ~MatchGuessView2();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual bool init(match::MatchFinalCat finalNum);
private:
    MatchUserHead   *m_pVs0;
    MatchUserHead   *m_pVs1;
    
    MatchBetMenu    *m_pBet0;

};



#endif /* defined(__QSMY__MatchGuessView__) */
