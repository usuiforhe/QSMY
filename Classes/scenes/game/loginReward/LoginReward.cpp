//
//  LoginReward.h
//  登陆奖励
//
//  Created by ldr123 on 13-07-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "LoginReward.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/LoginRewardModel.h"
#include "model/DropModel.h"


#include "managers/ResourceManager.h"
#include "managers/TipsManager.h"


USING_NS_CC;
using namespace sp;

LoginRewardNode::LoginRewardNode():m_pBackground(NULL), m_pText(NULL){}
LoginRewardNode::~LoginRewardNode()
{
	CC_SAFE_RELEASE(m_pBackground);
	CC_SAFE_RELEASE(m_pText);
}

LoginRewardNode* LoginRewardNode::create(const char *pSpriteFile, const char *pNum)
{
	LoginRewardNode* pRet = new LoginRewardNode();
	if (pRet && pRet->init(pSpriteFile, pNum))
	{
		pRet->autorelease();
		return pRet;
	}

	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool LoginRewardNode::init(const char *pSpriteFile, const char *pNum)
{
	m_pBackground = CCSprite::create(pSpriteFile);
	m_pBackground->setAnchorPoint(ccp(0.f, 0.f));
	CC_SAFE_RETAIN(m_pBackground);
	addChild(m_pBackground);

	m_pText = CCLabelBMFont::create(pNum, D_FONT_WARRIOR_VALUE);
	m_pText->setAnchorPoint(ccp(0.5f, 0.f));
	m_pText->setPosition(ccp(70.f, 0.f));
	CC_SAFE_RETAIN(m_pText);
	addChild(m_pText);

	return CCNode::init();
}

LoginReward::LoginReward()
{
	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		m_Background[i]		= NULL;
		m_Continer[i]		= NULL;
		m_ContinerTitle[i]	= NULL;
		m_btnReward[i]		= NULL;
		m_pOneArmBandit[i]	= NULL;
        m_pDiffs[i]          = NULL;
		m_pEffectNode[i]	= NULL;
	}
	m_TouchBack = NULL;
}

LoginReward::~LoginReward()
{
	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		CC_SAFE_RELEASE(m_Background[i]);
		CC_SAFE_RELEASE(m_Continer[i]);
		CC_SAFE_RELEASE(m_ContinerTitle[i]);
		CC_SAFE_RELEASE(m_btnReward[i]);
		CC_SAFE_RELEASE(m_pOneArmBandit[i]);
        CC_SAFE_RELEASE(m_pDiffs[i]);
		CC_SAFE_RELEASE(m_pEffectNode[i]);
	}

	CC_SAFE_RELEASE(m_TouchBack);
}

bool LoginReward::init()
{
	if (!DPopup::init(true))
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("ui/ccb/LoginReward");

	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		m_bGetting[i]		= false;
		m_pOneArmBandit[i]	= DOneArmBandit::create(LoginRewardNode::create("ui/loginReward/loginReward/default.png", ""), this, m_Continer[i]->getContentSize());
		CC_SAFE_RETAIN(m_pOneArmBandit[i]);
		m_pOneArmBandit[i]->setTag(i);
	}

	UpdateInfo();

	m_TouchBack->setVisible(false);

	return true;
}

bool LoginReward::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_firstDayBackground",		CCNode *,			m_Background[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_secondDayBackground",		CCNode *,			m_Background[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_thridDayBackground",		CCNode *,			m_Background[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_firstDayContiner",			CCNode *,			m_Continer[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_secondDayContiner",		CCNode *,			m_Continer[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_thirdDayContiner",			CCNode *,			m_Continer[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_firstDayContinerTitle",	CCNode *,			m_ContinerTitle[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_secondDayContinerTitle",	CCNode *,			m_ContinerTitle[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_thridDayContinerTitle",	CCNode *,			m_ContinerTitle[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_firstDayReward",			CCMenuItemImage *,	m_btnReward[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_secondDayReward",			CCMenuItemImage *,	m_btnReward[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_thirdDayReward",			CCMenuItemImage *,	m_btnReward[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_TouchBack",				CCNode *,	m_TouchBack);

	D_CCB_ASSIGN_ARRAY("m_pEffectNode", CCNode *, m_pEffectNode, MAX_LOGIN_REWARD_DAYS);
	
	return false;
}

SEL_MenuHandler LoginReward::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onGetReward", LoginReward::onGetReward);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", LoginReward::onTouchBack);

	return NULL;
}

void LoginReward::UpdateInfo()
{
	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		m_Background[i]->removeAllChildren();
		m_ContinerTitle[i]->removeAllChildren();
		m_Continer[i]->removeAllChildren();
		m_Continer[i]->addChild(m_pOneArmBandit[i]);
	}

	const std::string strTitleH[] = 
	{
		"ui/loginReward/font_fristday_h.png",
		"ui/loginReward/font_secondday_h.png",
		"ui/loginReward/font_thridday_h.png",
	};

	const std::string strTitleL[] = 
	{
		"ui/loginReward/font_fristday_l.png",
		"ui/loginReward/font_secondday_l.png",
		"ui/loginReward/font_thridday_l.png",
	};

	LoginRewardModel *pModel = DM_GET_LOGIN_REWARD_MODEL;
	if (pModel)
	{
		loginreward::LoginRewardDays tmpDays;
		pModel->GetLoginAwardDays(tmpDays);
		for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
		{
			if (tmpDays[i])
			{
                m_bComplete[i] = false;
				m_btnReward[i]->setEnabled(true);
				m_Background[i]->addChild(CCSprite::create("ui/loginReward/bg_OneArmBandit_h.png"));
				m_ContinerTitle[i]->addChild(CCSprite::create(strTitleH[i].c_str()));
				Armature* pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("loginreward_n",sp::ZORDER);
				pArmature->setTag('n');
				m_Background[i]->addChild(pArmature);
			}
			else
			{
                m_bComplete[i] = true;
				m_btnReward[i]->setEnabled(false);
				m_Background[i]->addChild(CCSprite::create("ui/loginReward/OneArmBandit_l.png"));
				m_ContinerTitle[i]->addChild(CCSprite::create(strTitleL[i].c_str()));
			}
		}
	}
}

void LoginReward::onStart(cocos2d::CCNode *pNode)
{
	int nTag = pNode->getTag();
	m_Background[nTag]->getChildByTag('n')->setVisible(false);
	Armature *pArmature =  ResourceManager::sharedResourceManager()->getUIAnimate("loginreward_h",sp::ZORDER);
	pArmature->setTag('h');
	m_Background[nTag]->addChild(pArmature);
}

void LoginReward::onComplete(cocos2d::CCNode *pNode)
{
	int nTag = pNode->getTag();

	m_Background[nTag]->removeChildByTag('h');
	m_Background[nTag]->getChildByTag('n')->setVisible(true);

	Armature *pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("loginreward_o",sp::ZORDER);
	m_pEffectNode[nTag]->addChild(pArmature);

    m_bComplete[nTag]   = true;
	m_bGetting[nTag]    = false;
    if(m_pDiffs[nTag]) UserInfoView::sharedUserInfoView()->removeDiff(m_pDiffs[nTag]);
	for (int i=0; i<MAX_LOGIN_REWARD_DAYS; ++i)
	{
		if (!m_bComplete[i])
		{
			return;
		}
	}

	m_TouchBack->setVisible(true);
}

void LoginReward::onGetReward(CCObject* pObj)//tag 1,2,3
{
	CCNode *pNode = (CCNode*)pObj;
	if (pNode)
	{
		int nTag = pNode->getTag();
		if (nTag<1 || nTag>MAX_LOGIN_REWARD_DAYS)
		{
			return ;
		}

		if (m_bGetting[nTag-1])
		{
			return;
		}


		m_bGetting[nTag-1] = true;
        m_btnReward[nTag-1]->setEnabled(false);

		HTTP_CLIENT->getLoginReward(nTag, this, callfuncJson_selector(LoginReward::GetRewardResult));
	}
}

void LoginReward::GetRewardResult(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    int32_t nSelected = requestData["id"].asInt();
	if (nSelected>0 && nSelected<=MAX_LOGIN_REWARD_DAYS)
	{
		const Json::Value &dropVal = responseData["data"]["drop"];
		uint16_t nType = 0;
		uint32_t nValue = 0;
        
        user::UserInfoDiff* pDiff = NULL;

		DJSON_UINT(nType, dropVal, "type");
		if (nType == drop::kDropTypeCoin) //金币
		{
			DJSON_UINT(nValue, dropVal, "coin");
            pDiff = user::UserInfoDiff::create();
            pDiff->iCoins = nValue;
            UserInfoView::sharedUserInfoView()->addDiff(pDiff);
		}
		else if (nType == drop::kDropTypePoint) //点券
		{
			DJSON_UINT(nValue, dropVal, "point");
            pDiff = user::UserInfoDiff::create();
            pDiff->iPoints = nValue;
            UserInfoView::sharedUserInfoView()->addDiff(pDiff);
		}
        
        m_pDiffs[nSelected-1] = pDiff;
        CC_SAFE_RETAIN(pDiff);
        
		loginreward::LoginRewardResult tmpResult;
		DM_GET_LOGIN_REWARD_MODEL->getLoginAwardResult(nSelected, nType, nValue, DM_GET_USER_MODEL->getUserInfo()->getUserLv(), tmpResult);
		if (tmpResult.uTrueID == 0)
		{
			//如果正确道具是0，说明是异常情况，直接return
			onComplete(m_pOneArmBandit[nSelected-1]);
			return;
		}

        
		cocos2d::CCNode *pFake[FAKE_NUM];
		for (int i=0; i<FAKE_NUM; ++i)
		{
			pFake[i] = LoginRewardNode::create(
													CCString::createWithFormat("ui/loginReward/loginReward/%d.png",tmpResult.uFakeID[i])->getCString(), 
													CCString::createWithFormat("%d",tmpResult.uFakeValue[i])->getCString()
												);
		}
		m_pOneArmBandit[nSelected-1]->setData(
													LoginRewardNode::create(
																				CCString::createWithFormat("ui/loginReward/loginReward/%d.png",tmpResult.uTrueID)->getCString(),
																				CCString::createWithFormat("%d",tmpResult.uTrueValue)->getCString()
																				),
													(cocos2d::CCNode **)pFake
												);
		
        m_pOneArmBandit[nSelected-1]->start();
	}
}

void LoginReward::onTouchBack(cocos2d::CCObject* p)
{
    if (m_TouchBack->isVisible()) {
        closePopup();
    }
}

void LoginReward::onBackKey()
{
	if (!isCompleted())
	{
		return;
	}
	closePopup();
}