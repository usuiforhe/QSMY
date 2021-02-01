//
//  PrizeCell.h
//  QSMY
//
//  Created by 汪燕军 on 13-9-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__PrizeCell__
#define __QSMY__PrizeCell__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PrizeModel.h"


class PrizeDelegate;

class PrizeCell
:public cocos2d::extension::CCTableViewCell
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(PrizeDelegate*, m_pDelegate, Delegate);
public:
    PrizeCell();
    ~PrizeCell();
    
    static PrizeCell * create(cocos2d::extension::CCData * pData);
    
    bool init(cocos2d::extension::CCData * pData);
    
    void setPrizeItem(prize::PrizeItem * pPrizeItem);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;};
    
private:
    void updateView();
    void viewCallBack(cocos2d::CCObject * sender);
    void drawCallBack(cocos2d::CCObject * sender);
    
private:
    cocos2d::CCNode * m_pContainer;
    cocos2d::CCSprite * m_pBgGrade;
    cocos2d::CCSprite * m_pSoulMark;
    cocos2d::CCSprite * m_pDressMark;
    cocos2d::CCLabelTTF * m_pDesc;
    cocos2d::CCLabelTTF * m_pName;
    cocos2d::CCNode * m_pAmountContainer;
    cocos2d::CCLabelBMFont * m_pAmount;
    cocos2d::CCSprite * m_pInProcess;
    cocos2d::CCNode * m_pDrawContainer;
    
    prize::PrizeItem * m_pPrizeItem;
};

#endif /* defined(__QSMY__PrizeCell__) */
