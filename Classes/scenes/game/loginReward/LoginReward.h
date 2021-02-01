//
//  LoginReward.h
//  登陆奖励
//
//  Created by ldr123 on 13-07-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__LoginReward__
#define __QSMY__LoginReward__

#include "AppInclude.h"
#include "../../../components/components.h"
#include "../../../components/OneArmBandit/DOneArmBandit.h"
#include "json/json.h"
#include "../../../model/LoginRewardModel.h"
#include "../user/UserInfo.h"

class LoginRewardNode : public cocos2d::CCNode
{
public:
	LoginRewardNode();
	~LoginRewardNode();
public:
	virtual bool init(const char *pSpriteFile, const char *pNum);
	static LoginRewardNode* create(const char *pSpriteFile, const char *pNum);
private:
	cocos2d::CCSprite*		m_pBackground;
	cocos2d::CCLabelBMFont*	m_pText;
};

class LoginReward : 
						public DPopup, 
						public cocos2d::extension::CCBMemberVariableAssigner,
						public cocos2d::extension::CCBSelectorResolver,
						public DOneArmBanditDelegate
{      
public:
	LoginReward();
	virtual ~LoginReward();
	virtual bool init();
	CREATE_FUNC(LoginReward);

	virtual bool									 onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
	virtual cocos2d::SEL_MenuHandler				 onResolveCCBCCMenuItemSelector(cocos2d::CCObject* pTarget, const char* pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject* pTarget, const char* pSelectorName){return NULL;}
public:
	void UpdateInfo();

	virtual void onStart(cocos2d::CCNode *pNode);
	virtual void onComplete(cocos2d::CCNode *pNode);
	void onGetReward(cocos2d::CCObject* p);//tag 1,2,3
	void onTouchBack(cocos2d::CCObject* p);

	void GetRewardResult(const Json::Value &requestData, const Json::Value &responseData);
private:
	cocos2d::CCNode*			m_Background[MAX_LOGIN_REWARD_DAYS];
	cocos2d::CCNode*			m_Continer[MAX_LOGIN_REWARD_DAYS];
	cocos2d::CCNode*			m_ContinerTitle[MAX_LOGIN_REWARD_DAYS];
	cocos2d::CCMenuItemImage*	m_btnReward[MAX_LOGIN_REWARD_DAYS];
	cocos2d::CCNode*	m_TouchBack;
	DOneArmBandit*				m_pOneArmBandit[MAX_LOGIN_REWARD_DAYS];
    user::UserInfoDiff*         m_pDiffs[MAX_LOGIN_REWARD_DAYS];
	cocos2d::CCNode*			m_pEffectNode[MAX_LOGIN_REWARD_DAYS];
private:
	bool						m_bGetting[MAX_LOGIN_REWARD_DAYS];
    bool                        m_bComplete[MAX_LOGIN_REWARD_DAYS];
public:
	bool isCompleted() {if (m_TouchBack) return m_TouchBack->isVisible(); return true;}

	void onBackKey();
};
#endif /* defined(__QSMY__LoginReward__) */
