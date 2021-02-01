//
//  SigninCell.cpp
//  QSMY
//
//  Created by Eci on 14-7-17.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "SigninCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

SigninCell::SigninCell()
	: m_pNodeDouble(NULL)
	, m_pNodeSignin(NULL)
	, m_pNodeSigned(NULL)
	, m_pNodeTomorrow(NULL)
	, m_pNodeTodaySigned(NULL)
	, m_pDelegate(NULL)
	, m_pLight(NULL)
	, m_pTodayText(NULL)
	, m_pTodayImg(NULL)
{
    D_RESET_C_ARRAY(m_pNodeDay, MAX_DAY);
	D_RESET_C_ARRAY(m_pItem, ITEM_COUNT);
}

SigninCell::~SigninCell()
{
	CC_SAFE_RELEASE(m_pNodeDouble);
	CC_SAFE_RELEASE(m_pNodeSignin);
	CC_SAFE_RELEASE(m_pNodeSigned);
	CC_SAFE_RELEASE(m_pNodeTomorrow);
	CC_SAFE_RELEASE(m_pNodeTodaySigned);
	CC_SAFE_RELEASE(m_pLight);
	CC_SAFE_RELEASE(m_pTodayText);
	CC_SAFE_RELEASE(m_pTodayImg);
	D_SAFE_RELEASE_C_ARRAY(m_pNodeDay, MAX_DAY);
	D_SAFE_RELEASE_C_ARRAY(m_pItem, ITEM_COUNT);
}

bool SigninCell::init()
{
    if(!CCTableViewCell::init()) return false;
    
	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("ItemBox", ItemBoxLoader::loader());
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorHeadGradeBg", WarriorHeadGradeBgLoader::loader());
	CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_qiandao/PL_qiandao_cell", pNodeLoaderLibrary);

    return true;
}

bool SigninCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDouble", CCNode *, m_pNodeDouble);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSignin", CCNode *, m_pNodeSignin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeSigned", CCNode *, m_pNodeSigned);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTomorrow", CCNode *, m_pNodeTomorrow);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeTodaySigned", CCNode *, m_pNodeTodaySigned);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLight", CCNode *, m_pLight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayText", CCNode *, m_pTodayText);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTodayImg", CCNode *, m_pTodayImg);
	D_CCB_ASSIGN_ARRAY_FROM_ONE("m_pNodeDay", CCNode *, m_pNodeDay, MAX_DAY);
	D_CCB_ASSIGN_ARRAY("m_pItem", ItemBox *, m_pItem, ITEM_COUNT);
    return false;
}

SEL_MenuHandler SigninCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackSignin", SigninCell::callBackSignin);
	return NULL;
}

void SigninCell::setData(signin::SigninAward *pData)
{
	for (int i = 0; i < MAX_DAY; i++)
	{
		m_pNodeDay[i]->setVisible(pData->getDay() == i + 1);
	}
	m_pNodeDouble->setVisible(pData->getIsDouble() > 0);
	m_pItem[0]->updateSigninItem(pData , 0);
	m_pItem[1]->updateSigninItem(pData , 1);
}

void SigninCell::setSigned(bool isSigned)
{
	m_pNodeSignin->setVisible(!isSigned);
	m_pNodeSigned->setVisible(isSigned);
}

void SigninCell::callBackSignin(CCObject *pObj)
{
	if (m_pNodeSigned->isVisible() || DM_GET_SIGNIN_MODEL->isSignedToday())
	{
		return;
	}
	if (m_pDelegate)
	{
		m_pDelegate->doSignin(this);
	}
	
}

void SigninCell::setTomorrow(bool tomorrow)
{
	m_pNodeTomorrow->setVisible(tomorrow);
}

void SigninCell::setTodaySigned(bool todaySigned)
{
	m_pNodeTodaySigned->setVisible(todaySigned);
	m_pLight->setVisible(todaySigned);
	if(todaySigned){
		m_pNodeSigned->setVisible(false);
		m_pNodeSignin->setVisible(false);
	}
	
}

void SigninCell::setClear()
{
	m_pNodeTodaySigned->setVisible(false);
	m_pNodeSigned->setVisible(false);
	m_pNodeSignin->setVisible(false);
}

void SigninCell::play()
{
	//先修改显示隐藏
	m_pNodeTodaySigned->setVisible(true);
	m_pLight->setVisible(true);
	m_pNodeSigned->setVisible(false);
	m_pNodeSignin->setVisible(false);

	//签到动画 带回调
	//CCActionInterval* orbit = CCOrbitCamera::create(1.2,1,0,0,360,0,0);
	CCScaleBy* scaleto = CCScaleBy::create(0.15f , 2);
	m_pTodayImg->runAction(CCFadeIn::create(1.0f));	
	m_pTodayText->runAction(CCSequence::create(CCFadeIn::create(1.0f) , scaleto , scaleto->reverse()  ,CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(SigninCell::playEnd)), NULL));
}

void SigninCell::playEnd()
{
	m_pDelegate->signinCallBack();
}