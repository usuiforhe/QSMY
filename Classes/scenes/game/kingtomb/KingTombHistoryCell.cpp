//
//  KingTombHistoryCell.cpp
//  皇陵探秘历史记录
//
//  Created by mzp on 14-8-8.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "KingTombHistoryCell.h"
#include "managers/DataManager.h"
#include "managers/GameManager.h"



KingTombAssistBonusCell::KingTombAssistBonusCell()
	: m_pAssistBonusInfo(NULL)
	, m_pBtnAdd(NULL)
	, m_pContent(NULL)
{

}

KingTombAssistBonusCell::~KingTombAssistBonusCell()
{
	CC_SAFE_RELEASE(m_pBtnAdd);
	CC_SAFE_RELEASE(m_pContent);
}

KingTombAssistBonusCell* KingTombAssistBonusCell::create(cocos2d::extension::CCData* pData)
{
	KingTombAssistBonusCell* pRet = new KingTombAssistBonusCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
    return NULL;
}

bool KingTombAssistBonusCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);

	m_pBtnAdd->setTarget(this, menu_selector(KingTombAssistBonusCell::receiveCallBack));

	return true;
}

bool KingTombAssistBonusCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnAdd", CCMenuItem *, m_pBtnAdd);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent", CCLabelTTF *, m_pContent);

	return false;
}

cocos2d::SEL_MenuHandler KingTombAssistBonusCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void KingTombAssistBonusCell::setAssistBonusInfo(kingTomb::AssistBonusInfo* pInfo)
{
	m_pAssistBonusInfo = pInfo;
	if (pInfo)
	{
		// 宝库名字
		std::string kingTombTypeName = "";
		kingTomb::kingTombTypeInfo* pKingTombTypeInfo = DM_GET_KINGTOMB_MODEL->getTombTypeInfo(pInfo->getTreasureType());
		if (pKingTombTypeInfo)
		{
			kingTombTypeName = pKingTombTypeInfo->getName();
		}

		// 奖励物品信息
		std::string bonusContent = "";
		cocos2d::CCArray* bonusData = pInfo->getBonusData();
		cocos2d::CCObject* obj = NULL;
		if (bonusData)
		{
			CCARRAY_FOREACH(bonusData, obj)
			{
				drop::Drop* pDrop = (drop::Drop*)obj;
				if (bonusContent == "")
				{
					bonusContent = DM_GET_DROP_MODEL->dropContent(pDrop);
				}
				else
				{
					bonusContent += "," + DM_GET_DROP_MODEL->dropContent(pDrop);
				}
			}
		}
		
		m_pContent->setString(D_LOCAL_STRING("KingTomb%sAssist%sBonus%s", pInfo->getAssistName().c_str(), kingTombTypeName.c_str(), bonusContent.c_str()).c_str());
	}
	else
	{
		m_pContent->setString("");
	}
}

void KingTombAssistBonusCell::receiveCallBack(cocos2d::CCObject* sender)
{
	if (m_pAssistBonusInfo)
	{
		HTTP_CLIENT->getAssistBonusDragonValley(m_pAssistBonusInfo->getId());
	}
}






KingTombLogCell::KingTombLogCell()
	: m_pContent(NULL)
{

}

KingTombLogCell::~KingTombLogCell()
{
	CC_SAFE_RELEASE(m_pContent);
}

KingTombLogCell* KingTombLogCell::create(cocos2d::extension::CCData* pData)
{
	KingTombLogCell* pRet = new KingTombLogCell();
	if(pRet && pRet->init(pData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
    return NULL;
}

bool KingTombLogCell::init(cocos2d::extension::CCData* pData)
{
	CCB_READER_DATA_AND_ADDCHILD(pData);

	return true;
}

bool KingTombLogCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContent", CCLabelTTF *, m_pContent);

	return false;
}

cocos2d::SEL_MenuHandler KingTombLogCell::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	return NULL;
}

void KingTombLogCell::setLogInfo(kingTomb::LogInfo* pInfo)
{
	if (pInfo)
	{
		// 宝库名字
		std::string kingTombTypeName = "";
		kingTomb::kingTombTypeInfo* pKingTombTypeInfo = DM_GET_KINGTOMB_MODEL->getTombTypeInfo(pInfo->getTreasureType());
		if (pKingTombTypeInfo)
		{
			kingTombTypeName = pKingTombTypeInfo->getName();
		}

		// 奖励物品信息
		std::string bonusContent = "";
		cocos2d::CCArray* bonusData = pInfo->getBonus();
		cocos2d::CCObject* obj = NULL;
		if (bonusData)
		{
			CCARRAY_FOREACH(bonusData, obj)
			{
				drop::Drop* pDrop = (drop::Drop*)obj;
				if (bonusContent == "")
				{
					bonusContent = DM_GET_DROP_MODEL->dropContent(pDrop);
				}
				else
				{
					bonusContent += "," + DM_GET_DROP_MODEL->dropContent(pDrop);
				}
			}
		}

		// 失去物品信息
		std::string loseContent = "";
		cocos2d::CCArray* loseData = pInfo->getLose();
		if (loseData)
		{
			CCARRAY_FOREACH(loseData, obj)
			{
				drop::Drop* pDrop = (drop::Drop*)obj;
				if (loseContent == "")
				{
					loseContent = DM_GET_DROP_MODEL->dropContent(pDrop);
				}
				else
				{
					loseContent += "," + DM_GET_DROP_MODEL->dropContent(pDrop);
				}
			}
		}

		m_pContent->setString(D_LOCAL_STRING("KingTomb%sBonus%sAnd%sLose%s", kingTombTypeName.c_str(), pInfo->getName().c_str(), bonusContent.c_str(), loseContent.c_str()).c_str());
	}
	else
	{
		m_pContent->setString("");
	}
}
