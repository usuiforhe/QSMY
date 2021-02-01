//
//  WarriorHeadSmall.h
//  弟子小头像公共
//
//  Created by wanghejun on 13-7-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__WarriorHeadSmall__
#define __QSMY__WarriorHeadSmall__

#include "AppInclude.h"
#include "model/WarriorModel.h"
#include "WarriorHeadGradeBg.h"

class WarriorHeadSmall
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    //CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, HeadContainer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBorderHighLight, BorderHighLight);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pBorder, Border);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pStars, Stars);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pTfLevel, TfLevel);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pLevelBg, LevelBg);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pLevel, Level);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pHead, Head);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pDefault, Default);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pUpLevel, UpLevel);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pHas, Has);          //已经拥有 用在技能界面
    CC_SYNTHESIZE(cocos2d::CCSprite*, m_pNotInBattle, NotInBattle);
	CC_SYNTHESIZE(cocos2d::CCSprite*, m_pTouchToChange, TouchToChange);

	CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pMenuItem, MenuItem);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAddBurn, AddBurn);
	CC_SYNTHESIZE_READONLY(WarriorHeadGradeBg*,m_pGrade,Grade);
    
public:
    CREATE_FUNC(WarriorHeadSmall);
    WarriorHeadSmall();
    virtual ~WarriorHeadSmall();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    void resetWarriorInfo();
    void setWarriorInfo(warrior::WarriorCurInfo* pWarriorInfo);
	//炼化专用方法
	void setWarriorInfoOnBurn(warrior::WarriorCurInfo* pWarriorInfo);
    void setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo);
    void setWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo,uint32_t avtarId);
private:
    void updateWarriorInfo(warrior::WarriorBaseInfo* pWarriorInfo);
};


class WarriorHeadSmallLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(WarriorHeadSmallLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(WarriorHeadSmall);
};

#endif /* defined(__QSMY__WarriorHeadSmall__) */
