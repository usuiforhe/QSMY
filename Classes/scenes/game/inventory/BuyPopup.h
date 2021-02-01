//
//  BuyPopup.h
//  用户升级弹出框
//
//  Created by fengkerong on 14-07-4.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BuyPopup__
#define __QSMY__BuyPopup__

#include "AppInclude.h"
#include "../Game.h"
#include "components/components.h"
#include "model/UserModel.h"
#include "model/MallModel.h"

class BuyPopup : 
	public DPopup, 
	public cocos2d::extension::CCBMemberVariableAssigner,
	public cocos2d::extension::CCBSelectorResolver
{      
public:
	BuyPopup();
	virtual ~BuyPopup();
	virtual bool init();
	CREATE_FUNC(BuyPopup);
	virtual void closePopup();
	virtual void onBackKey();

	D_POPUP_PRIORITY(DPOPUP_USER_LEVELUP_PRIORITY);

	virtual bool									 onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler				 onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName){return NULL;}
public:

	virtual void onEnter();
	void updateInfo();

	void closeCallBack(cocos2d::CCObject * obj){
		closePopup();
	}

	void changeCallBackReduce(cocos2d::CCObject * obj);
	void changeCallBackAdd(cocos2d::CCObject * obj);
	void changeCallBackAdd10(cocos2d::CCObject * obj);
	void addNum(int num);
	void doBuy(cocos2d::CCObject * obj);
	void buyCallback(const Json::Value & requestData,const Json::Value &responseData);

private:
	cocos2d::CCLabelBMFont*			m_award1Num;			//奖励元宝数	-- 对应用户iPoints
	cocos2d::CCLabelBMFont*			m_award2Num;			//奖励银币数	-- 对应用户iCoins
	cocos2d::CCLabelBMFont*			m_maxPower;				//最大体力
	cocos2d::CCLabelBMFont*			m_unlockWarriorNum;		//最大上阵弟子
	cocos2d::CCLabelBMFont*         m_pLevel;               //等级
	
	cocos2d::CCNode*                m_pNavButton;
	cocos2d::CCNode*                m_pEffectNode;          //效果
	cocos2d::CCNode*                m_pLightEffectNode;     //光效果

	DButton*		                m_pCloseButton;			//确定按钮
	DButton*		                m_pConfirmButton;		//
	DButton*		                m_pCancelButton;		//
	DButton*		                m_pJiahao;				//
	DButton*		                m_pJiajiahao;			//
	DButton*		                m_pJianhao;				//
	cocos2d::CCLabelTTF*			m_pItemName;
	cocos2d::CCLabelTTF*			m_pItemInfo;
	cocos2d::CCLabelBMFont*         m_pCount;
	cocos2d::CCLabelBMFont*         m_pTotalPrice;
	cocos2d::CCNode*                m_pContainer;

	mall::ProductInfo *m_pProductInfo;
	
};
#endif /* defined(__QSMY__BuyPopup__) */
