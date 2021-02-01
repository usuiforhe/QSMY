//
//  VipView.h
//  商城VIP列表
//
//  Created by cll on 13-6-9.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__VIPVIEW__
#define __QSMY__VIPVIEW__

#include "AppInclude.h"
#include "components/components.h"
#include "managers/DataManager.h"

#define VIP_BUTTON_COUNT 15
#define VIP_LV_FIX 8

class AccountDelegate;

class VipView
:public DLayer
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCBSelectorResolver
{
    
    CC_SYNTHESIZE(AccountDelegate*, m_pDelegate, Delegate);
    
public:
	VipView();
	~VipView();
    
	CREATE_FUNC(VipView);

    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}

    virtual void onEnter();
    
private:
	void closeCallBack(cocos2d::CCObject * obj);
    void backCallBack(cocos2d::CCObject * obj);
    void viewCallBack(cocos2d::CCObject * obj);
    void onVipSelectCallBack(cocos2d::CCObject * obj);
    void buyVipBag(cocos2d::CCObject* obj);
    void buyCallback(const Json::Value & requestData,const Json::Value &responseData);
    void updateView();
    void updateInfo();

    user::VipInfo* getSelVipInfo();
private:
    int32_t  m_selectIndex;		//当前选中的vip等级
	cocos2d::CCArray *m_pVipInfos;
    user::UserInfo   *m_pUserInfo;
	bool m_firstEnter;
private:
    
    cocos2d::CCNode             *m_pContainer;
    cocos2d::CCLabelTTF         *m_pTitle;
    cocos2d::CCLabelTTF         *m_pContent;
    DButton                     *m_pBuy;
	DButton                     *m_pRechargeButton;
	DButton                     *m_pCloseButton;
    cocos2d::CCNode             *m_pCanBuy;
    cocos2d::CCNode             *m_pHave;
    cocos2d::CCMenuItemImage	*m_pVipAll[VIP_BUTTON_COUNT];
	cocos2d::CCMenuItemImage	*m_pVip[VIP_LV_FIX + 1];

    cocos2d::CCLabelBMFont      *m_pPrice;
    cocos2d::CCLabelBMFont      *m_pPriceNow;
    cocos2d::CCLabelBMFont      *m_pVipNow;
    cocos2d::CCLabelBMFont      *m_pVipSelected;
	cocos2d::CCNode					 *m_pExpInfoNode;
	cocos2d::CCLabelTTF			 *m_pExpInfo;
	cocos2d::CCLabelTTF			 *m_pExpCoin;
	cocos2d::CCLabelTTF			 *m_pExpLevel;
	cocos2d::CCLabelTTF      *m_pExp;
	DProgressBar					     *m_pProgress;

	cocos2d::extension::CCScrollView		*m_pScrollView;
	cocos2d::CCLayer								*m_pNoTouchLayer;
	cocos2d::CCNode								*m_pFixNode;
	cocos2d::CCNode								*m_pScrollNode;
	cocos2d::CCNode								*m_pButtonsNode;
};




#endif /* defined(__QSMY__VIPVIEW__) */
