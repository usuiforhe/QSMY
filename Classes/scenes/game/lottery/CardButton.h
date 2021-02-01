//
//  CardButton.h
//  QSMY
//
//  Created by wanghejun on 13-7-30.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__CardButton__
#define __QSMY__CardButton__

#include "AppInclude.h"
#include "model/LotteryModel.h"

class CardButton
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pTimes, Times);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pFree, Free);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pFee, Fee);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pCD, CD);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pPrice, Price);
//    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pNoTimes, NoTimes);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pCDNode, CDNode);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pItemIcon, ItemIcon);
	CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, m_pPointIcon, PointIcon);
    
    CC_SYNTHESIZE_READONLY(card::CardInfo*, m_pCard, Card);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*,m_pFirstNBCard, FirstNBCard);
    
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode *, m_pNodeItem, NodeItem);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont *, m_pBMFOwned, BMFOwned);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode *, m_pItemCost2, ItemCost2);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode *, m_pItemCost3, ItemCost3);
	CC_SYNTHESIZE_READONLY(cocos2d::CCNode *, m_pPointCost, PointCost);
public:
    CREATE_FUNC(CardButton);
    CardButton();
    virtual ~CardButton();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual void onExit();
    virtual void update(float dt);
public:
    void setCard(card::CardInfo* pCard);
private:
    void updateInfo();
private:
    float  m_fWaitTime;
};


class CardButtonLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CardButtonLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CardButton);
};

#endif /* defined(__QSMY__CardButton__) */
