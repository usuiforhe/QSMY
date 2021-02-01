//
//  DestinyInfoView.h
//  天命详情
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyInfoView__
#define __QSMY__DestinyInfoView__

#include "AppInclude.h"
#include "Destiny.h"
#include "managers/DataManager.h"
#include "DestinyWarriorIcon.h"
#include "DragonBone.h"


class DestinyInfoView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE(DestinyDelegate*, m_DestinyDelegate, Delegate);
    CC_SYNTHESIZE_READONLY(destiny::DestinyInfo*, m_pDestinyInfo, DestinyInfo);
    
public:
    DestinyInfoView();
    virtual ~DestinyInfoView();
    virtual bool init();
    virtual bool initView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    virtual void onEnter();
    virtual void onExit();
    
    void setDestinyInfo(destiny::DestinyInfo* pDestinyInfo);
    
    void closeCallBack(cocos2d::CCObject* sender);
private:
    void _updateDestinyInfo();
    void _updateWarrior();
protected:
    virtual void updateDestinyInfo();
    virtual void updateWarrior();
protected:
    cocos2d::CCLabelBMFont  *m_pProperty;
    cocos2d::CCLabelBMFont  *m_pLevel;
    cocos2d::CCLabelTTF     *m_pName;
    cocos2d::CCNode         *m_pHighLight;
    cocos2d::CCNode         *m_pFullEffect;
    cocos2d::CCSprite       *m_pGrade;
    cocos2d::CCNode         *m_pContainer;
    DProgressBar            *m_pProgressBar;
    
    DestinyModel            *m_pDestinyModel;
    WarriorModel            *m_pWarriorModel;
    FormationModel          *m_pFormationModel;
};


class DestinyInfoViewAny
:public DestinyInfoView
,public cocos2d::extension::CCTableViewDataSource
,public cocos2d::extension::CCTableViewDelegate
{
    
public:
    DestinyInfoViewAny();
    virtual ~DestinyInfoViewAny();
    CREATE_FUNC(DestinyInfoViewAny);
    virtual bool initView();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    virtual void onExit();
protected:
    //virtual void updateWarrior();
    virtual void updateDestinyInfo();
    void updateView(cocos2d::CCObject* sender);
private:
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
    
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual void tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx);
	virtual uint32_t numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
private:
    void resetCallBack(cocos2d::CCObject* sender);
    void submitCallBack(cocos2d::CCObject* sender);
    void improveCallBack(const Json::Value &requestData, const Json::Value &responseData);
private:
    void clearDestinyWarriorDatas();
private:
    CCNode                      *m_pListContainer;              //弟子列表容器
    DTableView                  *m_pWarriorList;                //弟子列表组件
    cocos2d::CCMenuItemImage    *m_pGiveup;                     //放弃按钮
    cocos2d::CCMenuItemImage    *m_pSubmit;                     //提交按钮
private:
    cocos2d::extension::CCData  *m_pData;                       //弟子条目ui数据
    cocos2d::CCArray            *m_pDestinyWarriorDatas;        //所有弟子魂魄数据
    destiny::DestinyInfo        *m_pTempInfo;                   //天命信息
    sp::Armature                *m_pCompleteEffect;             //升级动画
    uint16_t                    m_nCurrentLv;                   //当前天命等级
};


class DestinyInfoViewNoAward:public DestinyInfoView
{
public:
    DestinyInfoViewNoAward();
    virtual ~DestinyInfoViewNoAward();
    CREATE_FUNC(DestinyInfoViewNoAward);
    virtual bool initView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
protected:   
    virtual void updateDestinyInfo();
    virtual void updateWarrior();
    virtual void updateSlots();
protected:
    void touchSlot(cocos2d::CCObject* sender);
    void initSlots();
protected:
    DestinyWarriorIconItem *m_pSlots[MAX_SLOTS];
};




class DestinyInfoViewAward:public DestinyInfoViewNoAward
{
public:
    DestinyInfoViewAward();
    virtual ~DestinyInfoViewAward();
    CREATE_FUNC(DestinyInfoViewAward);
    virtual bool init();
    virtual bool initView();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    virtual void onEnter();
    
protected:
    virtual void updateDestinyInfo();
private:
    void reward(CCObject*obj);
    void viewInfo(CCObject*obj);
    void updateAward();
private:
    cocos2d::CCMenuItem                     *m_pViewInfoMenuItem;
    cocos2d::CCMenuItem                     *m_pRewardMenuItem;
    cocos2d::CCNode                         *m_pRewardEffect;
    cocos2d::CCLabelTTF                     *m_pAwardName;
    cocos2d::CCNode                         *m_pAwardContainer;
    cocos2d::extension::CCScale9Sprite      *m_pAwardGrade;
    cocos2d::CCLabelBMFont                  *m_pAwardCount;
    cocos2d::CCSprite                       *m_pRewardDaily;
private:
    ItemModel                               *m_pItemModel;
    EquipModel                              *m_pEquipModel;
};


#endif /* defined(__QSMY__DestinyInfoView__) */
