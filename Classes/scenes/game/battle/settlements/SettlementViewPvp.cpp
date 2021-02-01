//
//  SettlementViewPvp.cpp
//  QSMY
//
//  Created by wanghejun on 13-8-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SettlementViewPvp.h"
#include "../../Game.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"


USING_NS_CC;
using namespace battle;

SettlementViewPvp::SettlementViewPvp()
:m_pInfo(NULL)
,m_pDelegate(NULL)
,m_pReplay(NULL)
,m_pExp(NULL)
,m_pGold(NULL)
,m_pLose(NULL)
,m_pWin(NULL)
,m_pEffectNode(NULL)
,m_pUpgrade(NULL)
,m_pWarriors(NULL)
,m_pMapName(NULL)
,m_pMapEffectNode(NULL)
,m_pClose(NULL)
,m_pNameLeft(NULL)
,m_pNameRight(NULL)
,m_pFightLeft(NULL)
,m_pFightRight(NULL)
,m_pCoin(NULL)
,m_pScore(NULL)
,m_pEndu(NULL)
,m_pButtonLeft1(NULL)
,m_pButtonLeft2(NULL)
,m_pButtonRight1(NULL)
,m_pButtonRight2(NULL)
{
	
}

SettlementViewPvp::~SettlementViewPvp()
{
    CC_SAFE_RELEASE(m_pInfo);
    
    
    CC_SAFE_RELEASE(m_pReplay);
    CC_SAFE_RELEASE(m_pExp);
    CC_SAFE_RELEASE(m_pGold);
    CC_SAFE_RELEASE(m_pLose);
    CC_SAFE_RELEASE(m_pWin);
    CC_SAFE_RELEASE(m_pEffectNode);
    CC_SAFE_RELEASE(m_pUpgrade);
    CC_SAFE_RELEASE(m_pWarriors);
    CC_SAFE_RELEASE(m_pMapName);
    CC_SAFE_RELEASE(m_pMapEffectNode);
	CC_SAFE_RELEASE(m_pClose);

	CC_SAFE_RELEASE(m_pNameLeft);
	CC_SAFE_RELEASE(m_pNameRight);
	CC_SAFE_RELEASE(m_pFightLeft);
	CC_SAFE_RELEASE(m_pFightRight);
	CC_SAFE_RELEASE(m_pCoin);
	CC_SAFE_RELEASE(m_pScore);
	CC_SAFE_RELEASE(m_pEndu);
	CC_SAFE_RELEASE(m_pButtonLeft1);
	CC_SAFE_RELEASE(m_pButtonLeft2);
	CC_SAFE_RELEASE(m_pButtonRight1);
	CC_SAFE_RELEASE(m_pButtonRight2);
}


bool SettlementViewPvp::init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo)
{
    CCAssert(delegate!=NULL && dynamic_cast<settlement::SettlementInfoPvp*>(pInfo)!=NULL, "");
    if(!DLayer::init()) return false;
    CC_SAFE_RELEASE(m_pInfo);
    CC_SAFE_RETAIN(pInfo);
    m_pInfo = dynamic_cast<settlement::SettlementInfoPvp*>(pInfo);
    m_pDelegate =delegate;

	
    
    //CCB_READER_AND_ADDCHILD("ui/ccb/pve/pveSettle");
	CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_lunjian/PL_lunjian_jiesuan");

	m_pWin->setVisible(m_pInfo->m_bWin == true);
	m_pLose->setVisible(m_pInfo->m_bWin == false);

	m_pNameLeft->setString(m_pInfo->m_sSelfName.c_str());
	m_pNameRight->setString(m_pInfo->m_sTargetName.c_str());
	m_pFightLeft->setString(CCString::createWithFormat("%d" , m_pInfo->m_iSelfFight)->getCString());
	m_pFightRight->setString(CCString::createWithFormat("%d" , m_pInfo->m_iTargetFight)->getCString());
	//m_pCoin->setString(CCString::createWithFormat("%d" , m_pInfo->m_iRewardyb)->getCString());
	//m_pScore->setString(CCString::createWithFormat("%d" , m_pInfo->m_iPoint)->getCString());
	m_pEndu->setString(CCString::createWithFormat("%d" , m_pInfo->m_iEndurance)->getCString());
	m_pButtonLeft1->getButton()->setTarget(this, menu_selector(SettlementViewPvp::replayBattle));
	m_pButtonLeft2->getButton()->setTarget(this, menu_selector(SettlementViewPvp::replayBattle));

	m_pButtonRight1->getButton()->setTarget(this, menu_selector(SettlementViewPvp::viewOtherFormation));
	m_pButtonRight2->getButton()->setTarget(this, menu_selector(SettlementViewPvp::viewOtherFormation));
	if(m_pInfo->m_uTargetID == 0)
	{
		//机器人，置灰
		m_pButtonRight1->getButton()->setEnabled(false);
		m_pButtonRight2->getButton()->setEnabled(false);
	}

	//if(m_pInfo->m_bWin == true)
	//{
		//胜利的话 搞出一些动画

	CCNumberTo * pAction1 = CCNumberTo::create(1.5f,0.0f,m_pInfo->m_iRewardyb,tween_num_selector(SettlementViewPvp::numberAction1));
	CCNumberTo * pAction2 = CCNumberTo::create(1.5f,0.0f,m_pInfo->m_iPoint,tween_num_selector(SettlementViewPvp::numberAction2));
	runAction(pAction1);
	runAction(pAction2);
	//}

    return true;
}

void SettlementViewPvp::numberAction1(float val)
{
	m_pCoin->setString(CCString::createWithFormat("+%.1f" , val)->getCString());
	if(m_pInfo->m_iRewardyb == int(val)) m_pCoin->setString(D_CSTRING_FROM_INT(int(val)));
}
void SettlementViewPvp::numberAction2(float val)
{
	m_pScore->setString(CCString::createWithFormat("+%.1f" , val)->getCString());
	if(m_pInfo->m_iPoint == int(val)) m_pScore->setString(D_CSTRING_FROM_INT(int(val)));
}

bool SettlementViewPvp::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{   
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReplay", DButton*, this->m_pReplay);    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLose", CCNode*, this->m_pLose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWin", CCNode*, this->m_pWin);   
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGold", CCLabelBMFont*, this->m_pGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pExp", CCLabelBMFont*, this->m_pExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode*, this->m_pEffectNode);  
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pUpgrade", CCNode*, this->m_pUpgrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriors", CCNode*, this->m_pWarriors);    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapName", CCLabelTTF*, this->m_pMapName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pMapEffectNode", CCNode*, this->m_pMapEffectNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pClose", CCMenuItem*, this->m_pClose);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameLeft", CCLabelTTF*, this->m_pNameLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameRight", CCLabelTTF*, this->m_pNameRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFightLeft", CCLabelBMFont*, this->m_pFightLeft);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFightRight", CCLabelBMFont*, this->m_pFightRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCoin", CCLabelBMFont*, this->m_pCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pScore", CCLabelBMFont*, this->m_pScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEndu", CCLabelBMFont*, this->m_pEndu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonLeft1", DButton*, this->m_pButtonLeft1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonLeft2", DButton*, this->m_pButtonLeft2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRight1", DButton*, this->m_pButtonRight1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pButtonRight2", DButton*, this->m_pButtonRight2);
    return false;
}
SEL_MenuHandler SettlementViewPvp::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "cardCallBack", SettlementViewPvp::cardCallBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", SettlementViewPvp::quitBattle);
	return NULL;
}

void SettlementViewPvp::viewOtherFormation(CCObject*)
{
	if(m_pDelegate && m_pInfo) m_pDelegate->showOtherFormation(m_pInfo->m_uTargetID);
}

void SettlementViewPvp::replayBattle(CCObject*)
{
    if(m_pDelegate) m_pDelegate->replayBattle();
}

void SettlementViewPvp::gotoEnhanceEquip(CCObject*)
{
    if(m_pDelegate) m_pDelegate->quitBattle();
    Game::sharedGame()->NavigateTo(navigation::kNavEquip);
}

void SettlementViewPvp::goToFormationView(CCObject*)
{
	Game::sharedGame()->NavigateTo(navigation::kNavFormation);
	this->quitBattle(NULL);
}

void SettlementViewPvp::goToMallView(CCObject*)
{
	Game::sharedGame()->NavigateTo(navigation::kNavMallLottery);
	this->quitBattle(NULL);
}


void SettlementViewPvp::quitBattle(CCObject*)
{
    //NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_DROPS_SHOW,cocos2d::CCInteger::create(m_pInfo->dropId));
    //HTTP_CLIENT->getUserInfo(); //同步用户信息
	if(m_pDelegate) m_pDelegate->quitBattle();
}

void SettlementViewPvp::callBackUpgrade(CCObject *sender)
{
	/*CCMenuItem *pItem = (CCMenuItem *)sender;
	pve::PveFailureInfo *pInfo = (pve::PveFailureInfo *)pItem->getUserObject();

	Game::sharedGame()->NavigateTo((navigation::NavigationType)pInfo->uScene,false,pInfo->uIndex>0?CCInteger::create(pInfo->uIndex):NULL);
	this->quitBattle(NULL);*/
}

void SettlementViewPvp::closeView()
{
	this->quitBattle(NULL);
}

void SettlementViewPvp::cardCallBack(CCObject* sender)
{
	CCNode* menu = dynamic_cast<CCNode*>(sender);
	int tag = menu->getTag();
	if(tag == 4){
		goToMallView(NULL);
	}
	else{
		goToFormationView(NULL);
	}

}