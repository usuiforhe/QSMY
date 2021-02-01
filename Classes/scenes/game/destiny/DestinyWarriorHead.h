//
//  DestinyWarriorHead.h
//  天命选择弟子
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DestinyWarriorHead__
#define __QSMY__DestinyWarriorHead__

#include "model/WarriorModel.h"
#include "AppInclude.h"
#include "../../../components/components.h"

#define D_EVENT_DESTINYWARRIOR_UPDATED "destiny warrior update" //更新数量
#define D_EVENT_DESTINYWARRIOR_ADD "destiny warrior add"

class DestinyWarriorData:public ValueObjcet
{
    CC_SYNTHESIZE(cocos2d::ccColor3B, m_Color, Color);
    CC_SYNTHESIZE(uint32_t, m_nId, ID);
    CC_SYNTHESIZE(warrior::WarriorGrade,color_grade, Grade);
    CC_SYNTHESIZE(bool, m_bIsWarrior,IsWarrior);
    CC_SYNTHESIZE(std::string, m_name, Name);
    CC_SYNTHESIZE_READONLY(uint32_t, m_nCount, Count);
    CC_SYNTHESIZE(uint32_t, m_nAvatarId, AvatarId);
    
public:
    
    void setCount(uint32_t val);
    
    int getAdded(); //获取已经添加
    
    DestinyWarriorData();
    virtual ~DestinyWarriorData();
    
    static DestinyWarriorData* create(warrior::WarriorBaseInfo* pWarrior);
    virtual bool init(warrior::WarriorBaseInfo* pWarrior);
    
    static DestinyWarriorData* create(warrior::WarriorCurInfo* pWarrior);
    virtual bool init(warrior::WarriorCurInfo* pWarrior);
    
    static DestinyWarriorData* create(warrior::SoulInfo* pSoul);
    virtual bool init(warrior::SoulInfo* pSoul);
    
    void add();
    int getRemain();
    
    void fixAdd(int num);
    
private:
    WarriorModel    *m_pWarriorModel;
    int        m_nAdd;
};


class DestinyWarriorHead
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
{

    CC_SYNTHESIZE_READONLY(DestinyWarriorData*, m_pData, Data);
public:
    CREATE_FUNC(DestinyWarriorHead);
    DestinyWarriorHead();
    virtual ~DestinyWarriorHead();
    D_INIT_DEFAULT(DLayer);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void setData(DestinyWarriorData* data);
    
    virtual bool dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches = NULL);
    virtual void dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    virtual void update(float dt);
private:
    void updateView();
private:
    cocos2d::extension::CCScale9Sprite  *m_pGrade;
    cocos2d::CCNode                     *m_pContainer;
    cocos2d::extension::CCScale9Sprite  *m_pBorderHighLight;
    cocos2d::extension::CCScale9Sprite  *m_pBorder;
    cocos2d::CCNode                     *m_pStars;
    
    cocos2d::CCLabelBMFont              *m_pCount;
    cocos2d::CCSprite                   *m_pSoulMark;
    cocos2d::CCSprite                   *m_pWarriorMark;
    cocos2d::CCLabelTTF                 *m_pName;
};


class DestinyWarriorHeadLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(DestinyWarriorHeadLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(DestinyWarriorHead);
};

#endif /* defined(__QSMY__DestinyWarriorHead__) */
