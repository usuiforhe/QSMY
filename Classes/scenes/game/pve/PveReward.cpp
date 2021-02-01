//
//  PveReward.cpp
//  pve奖励
//
//  Created by ldr123 on 13-7-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "PveReward.h"
#include "AppEventType.h"
#include "../user/UserInfo.h"
#include "network/HttpClient.h"
#include "../Game.h"
#include "model/UserModel.h"
#include "managers/TipsManager.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"


#include "../Game.h"

USING_NS_CC;
USING_NS_CC_EXT;

PveReward::PveReward()
:m_ChapterName(NULL)
,m_pLblPlan(NULL)
,m_pDelegate(NULL)
,m_pChapterInfo(NULL)
{
    D_RESET_C_ARRAY(m_itemIcon, PRES_AWARD_COUNT);
    D_RESET_C_ARRAY(m_btnGet, PRES_AWARD_COUNT);
	D_RESET_C_ARRAY(m_pTxtGet, PRES_AWARD_COUNT);
	D_RESET_C_ARRAY(m_pTxtGot, PRES_AWARD_COUNT);
}

PveReward::~PveReward()
{
    CC_SAFE_RELEASE(m_ChapterName);
    CC_SAFE_RELEASE(m_pLblPlan);
    D_SAFE_RELEASE_C_ARRAY(m_itemIcon, PRES_AWARD_COUNT);
    D_SAFE_RELEASE_C_ARRAY(m_btnGet, PRES_AWARD_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pTxtGet, PRES_AWARD_COUNT);
	D_SAFE_RELEASE_C_ARRAY(m_pTxtGot, PRES_AWARD_COUNT);
    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
}

bool PveReward::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("RewardItemBox", RewardItemBoxLoader::loader());
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/pve/PveReward", pNodeLoaderLibrary);
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_shengwangjiangli/PL_shengwangjiangli", pNodeLoaderLibrary);
    
    for (uint8_t i = 0; i<PRES_AWARD_COUNT; ++i)
    {
        m_btnGet[i]->getButton()->setTag(i + 1);
        m_btnGet[i]->getButton()->setTarget(this, menu_selector(PveReward::onTouchGet));
    }
    
    return true;
}


bool PveReward::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_ChapterName",CCLabelTTF *,	m_ChapterName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLblPlan",	CCLabelTTF *,	m_pLblPlan);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_itemIcon0",	RewardItemBox *,	m_itemIcon[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_itemIcon1",	RewardItemBox *,	m_itemIcon[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_itemIcon2",	RewardItemBox *,	m_itemIcon[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnGet0",	DButton *,	m_btnGet[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnGet1",	DButton *,	m_btnGet[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnGet2",	DButton *,	m_btnGet[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGet0" ,	CCNode *,	m_pTxtGet[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGet1" ,	CCNode *,	m_pTxtGet[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGet2" ,	CCNode *,	m_pTxtGet[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGot0" ,	CCNode *,	m_pTxtGot[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGot1" ,	CCNode *,	m_pTxtGot[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTxtGot2" ,	CCNode *,	m_pTxtGot[2]);
    
	return false;
}

SEL_MenuHandler PveReward::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchBack", PveReward::onTouchBack);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTouchGet", PveReward::onTouchGet);
    
	return NULL;
}

void PveReward::onTouchBack(cocos2d::CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->onTouchPveRewardCancel(m_pChapterInfo->getCid());
	}
}

void PveReward::onTouchGet(cocos2d::CCObject *pObj)
{
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pObj);
    //uint8_t viplv = pItem->getTag();
	//修改為讀表再傳vip值
	pve::PrestigeReward* pPrestigeReward = m_pChapterInfo->getAwardByIndex(pItem->getTag() - 1);
	uint8_t viplv = pPrestigeReward->prestige_reward_need_viplv;
    if(viplv>DM_GET_USER_MODEL->getUserInfo()->getVipLevel())
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV);
        return;
    }
	if (m_pDelegate)
	{
		m_pDelegate->onTouchPveRewardOk(m_pChapterInfo->getCid(),viplv);
	}
}

void PveReward::UpdateInfo(pve::ChapterInfo* pChapterInfo, PveRewardDelegate* pDelegate)
{
    if(m_pChapterInfo==pChapterInfo) return;
	CC_SAFE_RETAIN(pChapterInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pChapterInfo, this);
    m_pChapterInfo = pChapterInfo;
	m_pDelegate = pDelegate;
    BINDING_EXEC(m_pChapterInfo, this, PveReward::updateInfo,D_EVENT_PVE_CHAPTER_UPDATED);
    
    
}

void PveReward::updateInfo()
{
    if(m_pChapterInfo==NULL)
    {
        for (uint8_t i = 0; i<PRES_AWARD_COUNT; ++i)
        {
            m_btnGet[i]->setVisible(false);
            m_itemIcon[i]->setVisible(false);
            m_ChapterName->setString("");
            m_pLblPlan->setString("");
        }
        return;
    }
    
    for (uint8_t i = 0; i<PRES_AWARD_COUNT; ++i)
    {
        m_btnGet[i]->setVisible(true);
        m_itemIcon[i]->setVisible(true);
        pve::PrestigeReward* pInfo = m_pChapterInfo->getAwardByIndex(i);
        m_btnGet[i]->setTag(pInfo->prestige_reward_need_viplv);
        //m_btnGet[i]->setEnabled(m_pChapterInfo->isPresAwardCanGet(pInfo->prestige_reward_need_viplv));
        if(m_pChapterInfo->isPresAwardCanGet(pInfo->prestige_reward_need_viplv))
        {
            m_btnGet[i]->getButton()->setEnabled(true);
        }else
        {
            m_btnGet[i]->getButton()->setEnabled(!m_pChapterInfo->isPresAwardIsGotten(pInfo->prestige_reward_need_viplv)
                                                 &&m_pChapterInfo->pres>=m_pChapterInfo->getMaxPrestige()
                                                 && pInfo->prestige_reward_need_viplv>DM_GET_USER_MODEL->getUserInfo()->getVipLevel());
        }
        m_itemIcon[i]->setInfo(m_pChapterInfo, pInfo->prestige_reward_need_viplv);
		m_pTxtGet[i]->setVisible(!m_pChapterInfo->isPresAwardIsGotten(pInfo->prestige_reward_need_viplv));
		m_pTxtGot[i]->setVisible(!m_pTxtGet[i]->isVisible());

		
    }
    
    m_ChapterName->setString(m_pChapterInfo->chapter_name.c_str());
    m_pLblPlan->setString(CCString::createWithFormat("%d/%d",m_pChapterInfo->pres, m_pChapterInfo->getMaxPrestige())->getCString());
    
}



