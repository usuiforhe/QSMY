//
//  MatchView.h
//  比赛结果列表
//
//  Created by cll on 13-4-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__MatchView__
#define __QSMY__MatchView__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../model/MatchModel.h"

class MatchDelegate;
class MatchViewCell;

class MatchView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
	MatchView();
	~MatchView();
    
	static MatchView * create(MatchDelegate *pDelegate);

	virtual bool init(MatchDelegate *pDelegate);

	virtual void onEnter();
	virtual void onExit();
    
    
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;}
    
private:

	void updateMatchInfo();
	void awardCallback(cocos2d::CCObject *sender);
	void replayCallback(cocos2d::CCObject *sender);
    void signCallBack(cocos2d::CCObject*obj);
    void update(float dt);
private:
    cocos2d::CCPoint            m_Pos;
    
    
	MatchDelegate               *m_pDelegate;
	MatchModel                  *m_pMatchModel;
	MatchModel::MatchInfoVec    *m_pMatchInfoVec;
    
    DTableView                  *m_pListView;
    cocos2d::CCMenuItem         *m_pSignMenu;
    cocos2d::extension::CCData  *m_pCCData; //条目ccb 信息
    cocos2d::CCNode             *m_pContainer;
    
    cocos2d::CCLabelBMFont              *m_pTimeLabel;
    cocos2d::CCNode                     *m_pTime;
	cocos2d::CCNode						*m_pVipLabel;

	cocos2d::CCNode				*m_pNodeNoMatch;

	cocos2d::CCSprite						*m_pCompete;
	cocos2d::CCSprite						*m_pCompeted;
	cocos2d::CCMenuItemImage	*m_pCompeteMenu;
    
private:
    float               m_fTimer;
	uint32_t			m_nLastIndex;
	uint32_t			m_nWatchingIndex;
};




class MatchViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{

public:
    
	MatchViewCell();
    ~MatchViewCell();

	static MatchViewCell* create(cocos2d::extension::CCData* pData);
	virtual bool init(cocos2d::extension::CCData* pData);
	void setMatchInfo(match::MatchInfo* pMatchInfo);
    void setRound(uint32_t index);
	void setWatched(bool isWatched);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

private:
    
	void updateCellView();
    
public:
    cocos2d::CCMenuItem     *m_pMenuItemReplay; //回放
    cocos2d::CCMenuItem     *m_pMenuItemAward;  //领奖
	//DButton     *m_pButton;  //button
	DButton						*m_pBtnAward;  
	DButton						*m_pBtnWatch;
    
private:
    cocos2d::CCLabelBMFont  *m_pTfLevel;        //级别
    cocos2d::CCLabelTTF     *m_pTfName;         //名字
    cocos2d::CCSprite       *m_pImgLose;        //负
    cocos2d::CCSprite       *m_pImgWin;         //胜
    cocos2d::CCSprite       *m_pAward;          //领奖
	cocos2d::CCSprite       *m_pReplay;         //回放
	cocos2d::CCSprite       *m_pLive;         //回放
    cocos2d::CCNode         *m_pHeadContainer;  //头像
    cocos2d::CCLabelBMFont  *m_pRound;          //轮数

	cocos2d::CCNode         *m_pNodeAward;  
	cocos2d::CCNode         *m_pNodeWatch;  
	

private:
    
	match::MatchInfo* m_pMatchInfo;		//比赛信息

};



#endif /* defined(__QSMY__MatchView__) */
