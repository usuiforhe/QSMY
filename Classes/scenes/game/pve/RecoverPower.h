//
//  RecoverPower.h
//  购买体力
//
//  Created by wanghejun on 13-9-4.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__RecoverPower__
#define __QSMY__RecoverPower__

#include "AppInclude.h"
#include "../common/common.h"
#include "model/UserModel.h"
#include "model/MallModel.h"

class PveDelegate;
class RecoverPower
:public cocos2d::CCLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    CC_SYNTHESIZE(PveDelegate*, m_Delegate, Delegate);
public:
    RecoverPower();
    virtual ~RecoverPower();
    CREATE_FUNC(RecoverPower);
    
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    cocos2d::CCLabelTTF       *m_pTodayCurrent;
    //cocos2d::CCLabelBMFont    *m_pVip;
    cocos2d::CCLabelTTF       *m_pTodayTotal0;
    //cocos2d::CCLabelBMFont    *m_pTodayTotal1;
    
    cocos2d::CCLabelBMFont    *m_pCount;
    
    cocos2d::CCLabelTTF       *m_pCost;
    cocos2d::CCLabelBMFont    *m_pPower;
    
    DButton                     *m_pCloseButton;
    DButton                     *m_pCannelButton;
    DButton                     *m_pSubmitButton;
    DButton                     *m_pSubButton;
    DButton                     *m_pAddButton;
    
    cocos2d::CCMenuItem         *m_pSubMinButton;
    DButton                     *m_pAddMaxButton;
    
    //cocos2d::CCNode             *m_pVipNode;
private:
    UserModel                   *m_pUserModel;
    MallModel                   *m_pMallModel;
    mall::ProductInfo           *m_pProductInfo;
    user::UserInfo              *m_pUserInfo;
private:
    uint32_t                    m_nCount;
private:
    void closeCallBack(cocos2d::CCObject*);
    
    void addCallBack(cocos2d::CCObject*);
    void subCallBack(cocos2d::CCObject*);
    
    void addMaxCallBack(cocos2d::CCObject*);
    void subMinCallBack(cocos2d::CCObject*);
    
    void submitCallBack(cocos2d::CCObject*);
    
    void buyCallBack(const Json::Value &requestData, const Json::Value &responseData);
    
    void updateView();
    void updateInfo();
};

#endif /* defined(__QSMY__RecoverPower__) */
