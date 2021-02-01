//
//  RechargeItem.h
//  QSMY
//
//  Created by jiangke on 13-12-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__RechargeItem__
#define __QSMY__RechargeItem__

#include "AppInclude.h"
#include "components/components.h"
#include "model/PlatformModel.h"
#include "model/UserModel.h"
#include "Account.h"



class RechargeItem
:public cocos2d::CCNode
,public cocos2d::extension::CCBMemberVariableAssigner
{
    CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);

    
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_PYuanbao, Yuanbao);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pMonthCard, MonthCard);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pNum, Num);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_pInfoTTF, InfoTTF);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pAdditionalTf, AdditionalTf);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pYuanbaoTF, YuanbaoTF);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelBMFont*, m_pEveryDayYuanbaoTf, EveryDayYuanbaoTf);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pInfo, Info);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pAdditionalIco, AdditionalIco);
    CC_SYNTHESIZE_READONLY(cocos2d::CCNode*, m_pContainer, Container);
    
private:
    DButton                     *m_pRechageButton;
    platform::RechargeInfo      *m_pRechargeInfo;
    
public:
    CREATE_FUNC(RechargeItem);
    RechargeItem();
    virtual ~RechargeItem();
    D_INIT_DEFAULT(CCNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);

    void setInfoData(platform::RechargeInfo* m_pInfo);
    
private:
    void updateView();
    void menuCallback(CCObject * obj);
    void reset();
    
};


class RechargeItemLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(RechargeItemLoader, loader);
    
    
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(RechargeItem);
};

#endif /* defined(__QSMY__RechargeItem__) */
