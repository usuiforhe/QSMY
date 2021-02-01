//
//  ItemBox.h
//  QSMY
//
//  Created by wanghejun on 13-8-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__ItemBox__
#define __QSMY__ItemBox__

#include "AppInclude.h"
#include "WarriorHeadGradeBg.h"
#include "model/DropModel.h"
#include "model/SigninModel.h"

class ItemBox
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCNodeRGBA*, m_pGrade, Grade);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, Container);
    CC_SYNTHESIZE_READONLY(cocos2d::CCMenuItem*, m_pTouchEvent, TouchEvent);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pName, Name);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pNameBg, NameBg);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pCount, Count);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pPieceIcon, PieceIcon);
	CC_SYNTHESIZE_READONLY(WarriorHeadGradeBg*,m_pGradeBg,GradeBg);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pHunpo,Hunpo);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAmountNode,AmountNode);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAddBurn, AddBurn);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pEmpty, Empty);
public:
    CREATE_FUNC(ItemBox);
    ItemBox();
    virtual ~ItemBox();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	void updateDropItem(drop::Drop *pDropItem);
	//签到专用
	void updateSigninItem(signin::SigninAward *pData , unsigned int idx);
	//炼化专用
	void updateBurnItem(drop::Drop *pDropItem);
	//寻宝专用
	void updateTombItem(drop::Drop *pDropItem , int count = -1);
protected:
	CC_SYNTHESIZE(bool,m_bBurnData,BurnData); //炼化是否有数据
	CC_SYNTHESIZE(uint8_t,m_bHeroType,HeroType); //1英雄2魂魄
	CC_SYNTHESIZE(unsigned int,m_bCount,ItemCount); //数量供显示
};


class ItemBoxLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ItemBoxLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ItemBox);
};

#endif /* defined(__QSMY__ItemBox__) */
