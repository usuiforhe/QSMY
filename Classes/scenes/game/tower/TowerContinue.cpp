//
//  TowerContinue.cpp
//  千机楼-失败继续
//
//  Created by ldr123 on 13-07-02.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "TowerContinue.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"
#include "model/TowerModel.h"
#include "managers/TipsManager.h"


USING_NS_CC;
USING_NS_CC_EXT;

TowerContinue::TowerContinue()
	:m_MissionName(NULL), 
	m_Icon(NULL), 
	m_HaveIcon(NULL),
	m_Money(NULL),
	m_RebirthCounts(NULL),
	m_CanRebirthCounts(NULL),
	m_VipLev(NULL),
	m_btnContinueCancel(NULL),
	m_btnContinueOk(NULL),
	m_Buff(NULL),
	m_HavePower(NULL),
	m_pPopupLayer(NULL),
	m_attackAdd(NULL),
	m_defenseAdd(NULL),
	m_hpAdd(NULL),
	m_btnReplay(NULL),
	m_pNodeReplay(NULL)
{
	for (int i=0; i<POINT_COUNT; ++i)
	{
		m_point[i] = NULL;
		m_tips_[i] = NULL;
	}
}

TowerContinue::~TowerContinue()
{
	CC_SAFE_RELEASE(m_MissionName);
	CC_SAFE_RELEASE(m_Icon);
	CC_SAFE_RELEASE(m_HaveIcon);
	CC_SAFE_RELEASE(m_Money);
	CC_SAFE_RELEASE(m_RebirthCounts);
	CC_SAFE_RELEASE(m_CanRebirthCounts);
	CC_SAFE_RELEASE(m_VipLev);
	CC_SAFE_RELEASE(m_btnContinueCancel);
	CC_SAFE_RELEASE(m_btnContinueOk);
	CC_SAFE_RELEASE(m_Buff);
	CC_SAFE_RELEASE(m_HavePower);
	CC_SAFE_RELEASE(m_pPopupLayer);
	for (int i=0; i<POINT_COUNT; ++i)
	{
		CC_SAFE_RELEASE(m_point[i]);
		CC_SAFE_RELEASE(m_tips_[i]);
	}
	CC_SAFE_RELEASE(m_defenseAdd);
	CC_SAFE_RELEASE(m_attackAdd);
	CC_SAFE_RELEASE(m_hpAdd);
	CC_SAFE_RELEASE(m_btnReplay);
	CC_SAFE_RELEASE(m_pNodeReplay);
}

bool TowerContinue::init()
{
	if (!DLayer::init())
	{
		return false;
	}

    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_huodong/PL_tianshoufuge/pl_tsfg_lose");

	m_pPopupLayer = CCNode::create();
	CC_SAFE_RETAIN(m_pPopupLayer);

	m_pTowerModel = DM_GET_TOWER_MODEL;

	m_btnReplay->getButton()->setTarget(this, menu_selector(TowerContinue::replayCallBack));
	m_pNodeReplay->setVisible(m_pTowerModel->isCanReplayBattle());

	m_btnContinueCancel->getButton()->setTarget(this,menu_selector(TowerContinue::onContinue));
	m_btnContinueCancel->getButton()->setTag(0);
	m_btnContinueOk->getButton()->setTarget(this,menu_selector(TowerContinue::onContinue));
	m_btnContinueOk->getButton()->setTag(1);
	return true;
}

bool TowerContinue::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_MissionName", CCLabelBMFont *, m_MissionName);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_Icon",				CCNode *,			m_Icon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HaveIcon",			CCNode *,			m_HaveIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_Money",			CCLabelBMFont *,	m_Money);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_RebirthCounts",	CCLabelBMFont *,	m_RebirthCounts);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_CanRebirthCounts",	CCLabelBMFont *,	m_CanRebirthCounts);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_Buff",				CCLabelBMFont *,	m_Buff);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pVipNow",			CCLabelBMFont *,	m_VipLev);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnContinueCancel",DButton *,	m_btnContinueCancel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnContinueOk",	DButton *,	m_btnContinueOk);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_HavePower",		CCLabelBMFont*,		m_HavePower);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point0",			CCMenuItem *,			m_point[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point1",			CCMenuItem *,			m_point[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point2",			CCMenuItem *,			m_point[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point3",			CCMenuItem *,			m_point[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_point4",			CCMenuItem *,			m_point[4]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_0",			CCNode *,			m_tips_[0]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_1",			CCNode *,			m_tips_[1]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_2",			CCNode *,			m_tips_[2]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_3",			CCNode *,			m_tips_[3]);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_tips_4",			CCNode *,			m_tips_[4]);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_attackAdd",		CCLabelBMFont*,		m_attackAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_defenseAdd",		CCLabelBMFont*,		m_defenseAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_hpAdd",		CCLabelBMFont*,		m_hpAdd);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnReplay",	DButton *,	m_btnReplay);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeReplay", CCNode *, m_pNodeReplay);
	return false;
}

SEL_MenuHandler TowerContinue::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onContinue", TowerContinue::onContinue);

	return NULL;
}

void TowerContinue::onContinue(CCObject *sender)
{
	CCNode *pNode = (CCNode*)sender;
	if (pNode)
	{
		//0放弃  1复活
		int nTag = pNode->getTag();
		if (nTag == 0)
		{
			HTTP_CLIENT->towerEnd();
			if (m_pDelegate)
			{
				m_pDelegate->NavigateTo(tower::kNavFailed);
			}
		}
		else
		{
			tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
			TowerModel::TowerContinueArr* pContinueArr = m_pTowerModel->GetTowerContinueArr();
			user::UserInfo *pUserInfo = DM_GET_USER_MODEL->getUserInfo();

			if (pInfo && pContinueArr && pUserInfo)
			{
				uint16_t NeedCount = pInfo->_continue + 1;
				CCObject* objC = NULL;
				uint16_t MaxConinueCnt = 0;
				tower::TowerContinueData* pContinueData = NULL;
				CCARRAY_FOREACH(pContinueArr, objC)
				{
					tower::TowerContinueData* pTmp = (tower::TowerContinueData*)objC;
					if (pTmp->uContinueCount == NeedCount)
					{
						pContinueData = pTmp;
					}

					if (pTmp->uVipLev == pUserInfo->getVipLevel())
					{
						if (MaxConinueCnt < pTmp->uContinueCount)
						{
							MaxConinueCnt = pTmp->uContinueCount;
						}
					}
				}

				if (!pContinueData && pContinueArr->count()>0)
				{
					pContinueData = (tower::TowerContinueData*)(pContinueArr->lastObject());
					MaxConinueCnt = pContinueData->uContinueCount;
				}

				if (pInfo->_continue < MaxConinueCnt)
				{
                    uint32_t needCoins = m_pTowerModel->getNeedCoins(pContinueData);
					if (needCoins > 0)
					{
						if (needCoins > pUserInfo->getCoins())
						{
							NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_COIN);
							return;
						}
					}
					else
					{
						if (pContinueData->uNeedPoint > pUserInfo->getPoints())
						{
							NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_POINT);
							return;
						}
					}

					HTTP_CLIENT->towerGoon(this, callfuncJson_selector(TowerContinue::ContinueResult));

					return;
				}
                
                if(pUserInfo->getVipLevel()>=DM_GET_USER_MODEL->getMaxVipLevel())
                {
                    Tips_Warning(D_LOCAL_STRING("CantNotRebirth").c_str());
                    return;
                }

				std::string strTips = D_LOCAL_STRING("RebirthNumIsOk%d", pUserInfo->getVipLevel()+1).c_str();
                NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_VIPLV,CCStringMake(strTips));
			}
		}
	}
}


void TowerContinue::ContinueResult(const Json::Value &requestData, const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;

	if (m_pDelegate)
	{
		m_pDelegate->NavigateTo(tower::kNavDifficulty);
	}
}

void TowerContinue::NotificationUpdate()
{
	UpdateInfo();
}

void TowerContinue::UpdateInfo()
{
	tower::TowerInfo *pInfo = m_pTowerModel->GetTowerInfo();
	if (!pInfo)
	{
		return;
	}


	TowerModel::TowerContinueArr* pContinueArr = m_pTowerModel->GetTowerContinueArr();
	if (!pContinueArr)
	{
		return;
	}

	user::UserInfo *pUserInfo = DM_GET_USER_MODEL->getUserInfo();
	if (!pUserInfo)
	{
		return;
	}

	uint16_t NeedCount = pInfo->_continue + 1;
	CCObject* objC = NULL;
	uint16_t MaxConinueCnt = 0;
	tower::TowerContinueData* pContinueData = NULL;
	CCARRAY_FOREACH(pContinueArr, objC)
	{
		tower::TowerContinueData* pTmp = (tower::TowerContinueData*)objC;
		if (pTmp->uContinueCount == NeedCount)
		{
			pContinueData = pTmp;
		}

		if (pTmp->uVipLev == pUserInfo->getVipLevel())
		{
			if (MaxConinueCnt < pTmp->uContinueCount)
			{
				MaxConinueCnt = pTmp->uContinueCount;
			}
		}
	}

	if (!pContinueData && pContinueArr->count()>0)
	{
		pContinueData = (tower::TowerContinueData*)(pContinueArr->lastObject());
		MaxConinueCnt = pContinueData->uContinueCount;
	}

	if (!pContinueData)
	{
		return;
	}

	m_Icon->removeAllChildren();
    uint32_t needCoins = m_pTowerModel->getNeedCoins(pContinueData);
	if (needCoins > 0)
	{
		m_Icon->addChild(CCSprite::create("PL_ui/all/all_tongbi.png"));
		m_HaveIcon->addChild(CCSprite::create("PL_ui/all/all_tongbi.png"));
		m_Money->setString(CCString::createWithFormat("%d",needCoins)->getCString());
		m_HavePower->setString(CCString::createWithFormat("%d", pUserInfo->getCoins())->getCString());
	}
	else
	{
		m_Icon->addChild(CCSprite::create("PL_ui/all/all_yuanbao.png"));
		m_HaveIcon->addChild(CCSprite::create("PL_ui/all/all_yuanbao.png"));
		m_Money->setString(CCString::createWithFormat("%d",pContinueData->uNeedPoint)->getCString());		
		m_HavePower->setString(CCString::createWithFormat("%d", pUserInfo->getPoints())->getCString());
	}

	int nResultCnt = MaxConinueCnt-pInfo->_continue;
	if (nResultCnt < 0)
	{
		nResultCnt = 0;
	}
	m_Buff->setString(CCString::createWithFormat("+%d%%",pContinueData->uGetBuff)->getCString());
	m_RebirthCounts->setString(CCString::createWithFormat("%d",nResultCnt)->getCString());
	m_CanRebirthCounts->setString(CCString::createWithFormat("%d",MaxConinueCnt)->getCString());
	m_VipLev->setString(CCString::createWithFormat("%d",pUserInfo->getVipLevel())->getCString());
	m_MissionName->setString(CCString::createWithFormat("%d",pInfo->mid)->getCString());
	int nPos = pInfo->mid%POINT_COUNT;
	for (int i=0; i<POINT_COUNT; ++i)
	{
		m_point[i]->setEnabled(false);
		if (i == nPos)
		{
			//m_point[i]->setVisible(true);
			m_point[i]->selected();
		}
		else
		{
			//m_point[i]->setVisible(false);
			m_point[i]->unselected();
		}
	}

	if(pInfo->mid % POINT_COUNT > 0)
	{
		for (int i=0; i<POINT_COUNT; ++i)
		{
			int nTmp = ((int)(pInfo->mid/POINT_COUNT)*POINT_COUNT+1+i)%3;
			m_tips_[i]->setVisible(nTmp == 0);
		}
	}
	else
	{
		for (int i=0; i<POINT_COUNT; ++i)
		{
			int nTmp = ((int)(pInfo->mid/POINT_COUNT - 1)*POINT_COUNT+1+i)%3;
			m_tips_[i]->setVisible(nTmp == 0);
		}
	}
	tower::TowerAdd *pAddInfo = m_pTowerModel->GetTowerAdd();
	m_attackAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fAtt * 100))->getCString());
	m_defenseAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fDef * 100))->getCString());
	m_hpAdd->setString(CCString::createWithFormat("%d%%", (int)(pAddInfo->fHp * 100))->getCString());
}

void TowerContinue::onEnter()
{
	DLayer::onEnter();
	BINDING_EXEC(m_pTowerModel, this, TowerContinue::NotificationUpdate, D_EVENT_TOWER_INFO_UPDATE);
}

void TowerContinue::onExit()
{
	UNBINDING_ALL(m_pTowerModel, this);
	DLayer::onExit();
}

void TowerContinue::replayCallBack(CCObject *sender)
{
	m_pTowerModel->replayBattle();
}