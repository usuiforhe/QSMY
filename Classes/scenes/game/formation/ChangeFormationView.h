//
//  ChangeFormationView.h
//  更换阵容
//
//  Created by wanghejun on 13-3-29.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ChangeFormationView__
#define __QSMY__ChangeFormationView__

#include "AppInclude.h"
#include "Formation.h"
#include "../common/common.h"

#pragma mark - ChangeFormationView
class ChangeFormationListView;
class ChangeFormationItem;
class ChangeFormationListViewCell;

class ChangeFormationView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{
  
    
#pragma mark - - init etc.
public:
    //阵容变换状态
    enum ChangeStatus
    {
        kChangeStatusFalse,         //不在编辑阵容
        kChangeStatusFromTop,       //从上面开始选
        kChangeStatusFromBottom     //从下面开始选
    };
    ChangeFormationView();
    ~ChangeFormationView();
    virtual bool init();
    CREATE_FUNC(ChangeFormationView);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual void onEnter();
    virtual void onExit();
	
#pragma mark - - methods public
public:    
	CC_SYNTHESIZE(warrior::WarriorCurInfo *, m_pWarriorInfo, WarriorInfo);
	CC_SYNTHESIZE_READONLY(uint32_t, m_nSelectedWarriorId, SelectedWarriorId);

	void change(uint32_t posId1,uint32_t posId2);
	void join(uint32_t wid,uint32_t posId);

	void touchChangeFormationItem(ChangeFormationItem *pItem);
	void touchChangeListViewCell(ChangeFormationListViewCell *pCell);
    void touchMoveBeganFormationItem(ChangeFormationItem *pItem);
	void touchMoveEndFormationItem(ChangeFormationItem *pItem);

	void showFormationSelectView(formation::PositionInfo* pPos);

#pragma mark - - variable
private:
	CC_SYNTHESIZE(FormationDelegate*, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(ChangeStatus, m_bChangeStatus, ChangeStatus);
    ChangeFormationListView					*m_pChangeFormationListView;
    FormationModel									*m_pFormationModel;
    WarriorModel										*m_pWarriorModel;
    formation::PositionInfo						*m_pSelectedPos;
    bool														m_bFromEmpty; //从空位点击
	cocos2d::CCLabelBMFont					*m_pFight;             //总战斗力
	cocos2d::CCNode								*m_pWarriorListContainer;   //弟子列表
	ChangeFormationItem							*m_pWarriors[FORMATION_POS_MAX];

	cocos2d::CCNode								*m_pNoneWarrior;        //没有弟子
	DButton												*m_pLotteryButton;      //去抽卡
	cocos2d::CCMenuItemImage				*m_pBackButton;

#pragma mark - - methods private
private:

    void navToLottery(cocos2d::CCObject* obj);
	void navToFormation(cocos2d::CCObject* obj);
    void updateView();    
    void setChangeState(ChangeStatus changeState);
    void reset();
    void onCallBack(const Json::Value&,const Json::Value&);
};

#pragma mark ----- ChangeFormationItem
class ChangeFormationItem
:public cocos2d::CCLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE_READONLY(WarriorHeadMid*, m_pWarrior, Warrior);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pSelected, Selected);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCanChange, Change);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchMenu, TouchMenu);
    CC_SYNTHESIZE_READONLY(WarriorTypeMark*, m_pWarriorTypeMark, WarriorTypeMark);
    CC_SYNTHESIZE_READONLY(cocos2d::CCRect, m_visibleRect, _VisibleRect);
#pragma mark - - init etc.
public:
    ChangeFormationItem();
    ~ChangeFormationItem();
    CREATE_FUNC(ChangeFormationItem);
    virtual bool init();
	virtual void onEnter();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void changeFlipActionState();
    void runFlipAction();
    void flipActionDone();
    
    void setMaskVisible(bool visible);
#pragma mark - - variable
private:
	void changeZOrder(int order);
 
    int  m_nIndex;
    int  m_nWid;
    bool m_bEmpty;
    bool m_bLock;
    bool m_bChangeState;
	bool m_touchMoved;
    bool m_bRunningFlipAction;
    
	cocos2d::CCRect m_touchRect;
	cocos2d::CCPoint m_touchBeganPoint;
	cocos2d::CCPoint m_originalPosition;
    
    FormationModel      *m_pFormationModel;
    WarriorModel        *m_pWarriorModel;
	
    CCNode*     m_pFlipNode;
	CCNode*		m_pNoWarriorNode;
    CCSprite*   m_pNoWarriorBg;
    CCSprite*   m_pMask;
	CCNode*		m_pGradeNode;
	CCNode*		m_pWarriorInfoNode;
	CCNode*		m_pChangeButtonNode;
	CCLabelTTF* m_pLockedLabel;

	CCSprite*	m_pGradeOrange;
	CCSprite*	m_pGradePurple;
	CCSprite*	m_pGradeBlue;
	CCSprite*	m_pGradeGreen;

	CCSprite*	m_pTitleChange;
	CCSprite*	m_pTitleUp;

	CCNode*		m_pHeadContainer;
	CCLabelBMFont*	m_pLevel;
	CCLabelTTF*			m_pName;
	CCMenuItemImage* m_pChangeButton;
    
    CC_SYNTHESIZE(ChangeFormationView*, m_pDelegate, Delegate);
    
#pragma mark - - methods private
private:
    void changeButtonCallBack(CCObject *obj);
	void setWarriorInfoVisible(bool visible);
#pragma mark - - methods public
public:
    int  getWarriorID();
    
    bool isHaveWarrior();
    
    bool getEmpty();
    void setEmpty(const bool &empty);
    
    bool getLock();
    void setLock(const bool &lock);
    
    int  getIndex();
    void setIndex(const int &index);
    
    bool getChangeState();
    void setChangeState(const bool &changeState);
    
    void setSelected(bool value);
    
    void updateView();
	void updateWarriorInfo(warrior::WarriorCurInfo * pWarriorInfo, uint8_t iPosId = -1);
};

class ChangeFormationItemLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ChangeFormationItemLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ChangeFormationItem);
};

#pragma mark - ChangeFormationListView
class ChangeFormationListView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
    
#pragma mark - - init etc.
public:
    ChangeFormationListView();
    ~ChangeFormationListView();
    virtual bool init();
    CREATE_FUNC(ChangeFormationListView);
    
    virtual void onEnter();
    virtual void onExit();
    
#pragma mark - - variable
private:
    WarriorModel                    *m_pWarriorModel;
    FormationModel                  *m_pFormationModel;
    DTableView                      *m_pView;
    WarriorModel::WarriorCurInfoVec *m_pWarriors;
    cocos2d::extension::CCData      *m_pCellData;
    
    CC_SYNTHESIZE(ChangeFormationView*, m_pDelegate, Delegate);
    
#pragma mark - - CCTableViewDelegate
private:
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
    virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
#pragma mark - - methods private
private:
    void updateWarriorList();
    void onFormationJoined();
    
#pragma mark - - methods public
public:
    void updateView();
    void resetChangeState();
    void setChangeState(const bool &changeState);
};

#pragma mark - - ChangeFormationListViewCell
class ChangeFormationListViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
#pragma mark - - init etc.
public:
    ChangeFormationListViewCell();
    ~ChangeFormationListViewCell();
    virtual bool init(cocos2d::extension::CCData* pData);
    static ChangeFormationListViewCell* create(cocos2d::extension::CCData* pData);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
#pragma mark - - methods private
private:
    void updateView();
    
#pragma mark - - methods public
public:
    int  getWarriorID();
    void setWarriorInfo(warrior::WarriorCurInfo*);
    void setChangeState(bool changeState);
    void setSelected(bool value);
    
#pragma mark - - variable
private:
    warrior::WarriorCurInfo* m_pWarriorInfo;
    bool m_bChangeState;
    
private:
    
    cocos2d::CCLabelBMFont                  *m_pPrice;
    WarriorTypeMark                         *m_pWarriorTypeMark;
    cocos2d::CCNode                         *m_pSelected;
    cocos2d::CCNode                         *m_pCanChange;
    cocos2d::CCNode                         *m_pStars;
    cocos2d::CCNode                         *m_pUpLevel;
    cocos2d::CCLabelTTF                     *m_pName;
    cocos2d::CCLabelBMFont                  *m_pLevel;
//    cocos2d::extension::CCScale9Sprite      *m_pBorderHighLight;
//    cocos2d::extension::CCScale9Sprite      *m_pBorder;
    cocos2d::CCNode                         *m_pContainer;
    cocos2d::extension::CCScale9Sprite      *m_pGrade;
    
    
};

#endif /* defined(__QSMY__ChangeFormationView__) */
