//
//  OtherFormationView.h
//  查看其它玩家阵容（包括机器人）
//
//  Created by cll on 13-6-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__OtherFormationView__
#define __QSMY__OtherFormationView__

#include "AppInclude.h"
#include "model/OtherFormationModel.h"
#include "common.h"

#define EQUIP_BOX_COUNT 4
#define MAX_FATE_COUNT 6

class OtherFormationListView;
class OtherFormationWarriorInfoView;
#pragma mark - FormationView
class OtherFormationView
:public DPopup
,public DPageViewDataSource
,public cocos2d::extension::CCScrollViewDelegate
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
public:
    
    enum OtherFormationType
    {
        kFormationRobot,		//机器人阵型
        kFormationTarget,		//真实玩家阵型
        
    };
    
    CC_SYNTHESIZE_RETAIN(otherFormation::FormationInfo*, m_pFormationInfo, FormationInfo);
    
    OtherFormationView();
    ~OtherFormationView();
    virtual bool init();
    CREATE_FUNC(OtherFormationView);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual float cellSizeForTable(DPageView *view);
    virtual cocos2d::CCNode* cellAtIndex(DPageView *view, unsigned int idx);
    virtual uint32_t numberOfCellsInTableView(DPageView *view);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    
private:
    CC_SYNTHESIZE_READONLY(int8_t, m_iCurrentPage, CurrentPage);
    
    OtherFormationListView              *m_pFormationListView;
    DPageView                           *m_pPageView;
    cocos2d::CCArray                    *m_pWarriorInfoViews;
private:
    void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){};
    void closeCallBack(cocos2d::CCObject *obj);
    
private:
    
    void adjustScrollView(float offset);
    
    void showFormations();
    
    void updateView();
    void reset();
    void updateFormation();
    
    void updateInfo();
    
    int  pageToPositionIdx(const int &page);
    
    int  getPageIndexForWarriorID(const uint32_t &wid);
    
    
    
public:
    void setCurrentPage(uint8_t i);
    void showWarrior(const uint32_t &wid);
private:
    cocos2d::CCLabelBMFont  *m_pAttack;                 //攻
    cocos2d::CCLabelBMFont  *m_pSpeed;                  //速
    cocos2d::CCLabelBMFont  *m_pDefence;                //防
    cocos2d::CCLabelBMFont  *m_pHp;                     //血
    
    cocos2d::CCLabelBMFont  *m_pPrice;                  //身价
    cocos2d::CCLabelBMFont  *m_pTotalPrice;             //总身价
    EquipBox                *m_pEquipBox[EQUIP_BOX_COUNT];
    cocos2d::CCNode         *m_pEquipNone[EQUIP_BOX_COUNT];
    SkillBox                *m_pSkillBox;
    SkillBox                *m_pSkillBoxSP;
	cocos2d::CCLabelTTF     *m_pFate[MAX_FATE_COUNT];   //缘
	cocos2d::CCLabelTTF     *m_pFateHighlight[MAX_FATE_COUNT];   //缘
    cocos2d::CCNode         *m_pWarriorListContainer;   //弟子列表
    cocos2d::CCNode         *m_pWarriorInfoContainer;   //弟子信息容器
    cocos2d::CCNode         *m_pUpLevel;
    
    cocos2d::extension::CCData  *m_pWarriorData;          //弟子信息CCB
// private:
//     cocos2d::ccColor3B         m_fateActive;
//     cocos2d::ccColor3B         m_fateDeActive;
};


#pragma mark - FormationWarriorInfoView
class  OtherFormationWarriorInfoView
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    
public:
    OtherFormationWarriorInfoView();
    ~OtherFormationWarriorInfoView();
    static OtherFormationWarriorInfoView * create(cocos2d::extension::CCData* pData, uint32_t pid);
    virtual bool init(cocos2d::extension::CCData* pData,uint32_t pid);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    
private:
    
    OtherFormationView *m_pDelegate;
    otherFormation::WarriorPropInfo* m_pWarriorInfo;
    uint32_t m_nPid;
    
private:
    cocos2d::CCLabelBMFont  *m_pLevel;              //级别
    cocos2d::CCLabelTTF     *m_pName;               //名字
    cocos2d::CCSprite       *m_pGrade;
    
    cocos2d::CCLabelBMFont  *m_pExp;                //经验
    DProgressBar            *m_pExpBar;             //经验条
    
    WarriorTypeMark         *m_pWarriorTypeMark;    //弟子类型
    cocos2d::CCNode         *m_pContainer;              //弟子容器·
    
    cocos2d::CCMenuItem     *m_pWarriorButton;
    
    cocos2d::CCNode         *m_pDefault;        //默认弟子
    cocos2d::CCNode         *m_pWarrior;        //弟子
    
private:
    void touchWarrior(CCObject *obj);
    
    
public:
    void updateView();
    void setWarriorInfo(otherFormation::WarriorPropInfo*);
    uint32_t getWid();
    uint32_t getPid();
    
    void setDelegate(OtherFormationView *pDelegate);
};


class OtherFormationListViewCell;
#pragma mark - FormationListView
class OtherFormationListView
:public DLayer
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
#pragma mark - - init etc.
public:
    OtherFormationListView();
    ~OtherFormationListView();
    CREATE_FUNC(OtherFormationListView);
    virtual bool init();
    void reset();
    CC_SYNTHESIZE_RETAIN(otherFormation::FormationInfo*, m_pFormationInfo, FormationInfo);
    
#pragma mark - - variable
private:
    CC_SYNTHESIZE(DTableView *, m_pView, View);
    CC_SYNTHESIZE(OtherFormationView*, m_pDelegate, Delegate);
    
    cocos2d::extension::CCData      *m_pCellData;
    
#pragma mark - - CCTableViewDelegate
private:
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
    virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    
#pragma mark - - methods private
public:
    void updateView();
};



#pragma mark - FormationListViewCell
class OtherFormationListViewCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
{
#pragma mark - - init etc.
public:
    OtherFormationListViewCell();
    ~OtherFormationListViewCell();
    virtual bool init(cocos2d::extension::CCData *pData);
    static OtherFormationListViewCell* create(cocos2d::extension::CCData *pData);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
#pragma mark - - variable
private:
    otherFormation::WarriorPropInfo *m_pWarriorInfo;
    
private:
    WarriorHeadSmall          *m_pWarrior;
    cocos2d::CCMenuItem       *m_pSoundItem;
    cocos2d::CCNode           *m_pUnlock;
    cocos2d::CCLabelBMFont    *m_pUnlockLevel;
    cocos2d::CCSprite         *m_pHighLight;
    
#pragma mark - - methods private
private:
    void updateView();
    
#pragma mark - - methods private
public:
    void setHighlight(bool val);
    
    void setUnLockInfo(uint32_t level);
    
    void setWarriorInfo(otherFormation::WarriorPropInfo*);
};

#endif /* defined(__QSMY__OtherFormationView__) */
