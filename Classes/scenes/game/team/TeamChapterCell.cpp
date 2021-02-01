//
//  TeamChapterCell.h
//  组队章节
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "TeamChapterCell.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
using namespace sp;

TeamChapterCell::TeamChapterCell()
	: m_pNodeBG(NULL)
	, m_pTTFName(NULL)
	, m_pNodeDropContainer(NULL)
	, m_pRgbaColor(NULL)
	, m_pNodeFree(NULL)
	, m_pNodePayed(NULL)
	, m_pNodeNew(NULL)
	, m_pDelegate(NULL)
	, m_pData(NULL)
{
}

TeamChapterCell::~TeamChapterCell()
{
	CC_SAFE_RELEASE(m_pNodeBG);
	CC_SAFE_RELEASE(m_pTTFName);
	CC_SAFE_RELEASE(m_pNodeDropContainer);
	CC_SAFE_RELEASE(m_pRgbaColor);
	CC_SAFE_RELEASE(m_pNodeFree);
	CC_SAFE_RELEASE(m_pNodePayed);
	CC_SAFE_RELEASE(m_pNodeNew);
	CC_SAFE_RELEASE(m_pData);
}

TeamChapterCell* TeamChapterCell::create()
{
	TeamChapterCell *pRet = new TeamChapterCell();
	if (pRet && pRet->init())
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool TeamChapterCell::init()
{
	if(!CCTableViewCell::init())
	{
		return false;
	}

	CCB_READER_AND_ADDCHILD("ui/ccb/team/teamChapterCell");
	return true;
}

void TeamChapterCell::onEnter()
{
	CCTableViewCell::onEnter();
}

void TeamChapterCell::onExit()
{
	CCTableViewCell::onExit();
}

bool TeamChapterCell::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeBG", CCNode *, m_pNodeBG);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTTFName", CCRichLabelTTF *, m_pTTFName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeDropContainer", CCNode *, m_pNodeDropContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRgbaColor", CCNodeRGBA *, m_pRgbaColor);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeFree", CCNode *, m_pNodeFree);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodePayed", CCNode *, m_pNodePayed);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeNew", CCNode *, m_pNodeNew);
	return false;
}



SEL_MenuHandler TeamChapterCell::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackTouch", TeamChapterCell::callBackTouch);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "callBackView", TeamChapterCell::callBackView);
	return NULL;
}

void TeamChapterCell::callBackTouch(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamChapterCellSelected(this);
	}
}

void TeamChapterCell::callBackView(CCObject *pSender)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamChapterCellViewItem(this);
	}
}

void TeamChapterCell::setData(friends::ChapterInfo *pData)
{
	if (!pData)
	{
		return;
	}
	CC_SAFE_RETAIN(pData);
	CC_SAFE_RELEASE(m_pData);
	m_pData = pData;
	m_pTTFName->setString(pData->getName().c_str());
	m_pNodeDropContainer->removeAllChildren();
	switch (pData->getDropType())
	{
	case drop::kDropTypeItem:
	case drop::kDropTypePkg:
		{
			m_pNodeDropContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon50P(pData->getDropID()));

			item::ItemBase *pBase = DM_GET_ITEM_MODEL->getItemBaseByID(pData->getDropID());
			bool showGrade = pBase->isNeedShowGradeColor();
			if (showGrade)
			{
				cocos2d::ccColor3B color = DM_GET_ITEM_MODEL->getItemColorByGrade(pBase->getItemGrade());
				m_pRgbaColor->setColor(color);
			}
			else
			{
				m_pRgbaColor->setColor(ccc3(255,255,255));
			}
			break;
		}
	case drop::kDropTypeEquip:
		{
			m_pNodeDropContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon50P(pData->getDropID()));

			equip::EquipBaseInfo *pBase = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(pData->getDropID());
			bool showGrade = pBase->isNeedShowGradeColor();
			if (showGrade)
			{
				cocos2d::ccColor3B color = DM_GET_EQUIP_MODEL->getEquipColorByGrade(pBase->getEquipGrade());
				m_pRgbaColor->setColor(color);
			}
			else
			{
				m_pRgbaColor->setColor(ccc3(255,255,255));
			}
			break;
		}
	case drop::kDropTypeSoul:
	case drop::kDropTypeWarrior:
		{
			m_pNodeDropContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pData->getDropID()));


			warrior::WarriorBaseInfo *pBase = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pData->getDropID());
			cocos2d::ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByGrade(pBase->getRealGrade());
			m_pRgbaColor->setColor(color);
			break;
		}
	case drop::kDropTypeCoin:
	case drop::kDropTypePoint:
	case drop::kDropTypeVip:
	case drop::kDropTypePower:
	case drop::kDropTypeAvatar:
	default:;
	}
	m_pNodeBG->removeAllChildren();
	m_pNodeBG->addChild(ResourceManager::sharedResourceManager()->getSceneThumbIconForTeam(pData->getSceneID()));

	m_pNodePayed->setVisible(m_pData->getIsDone());
	m_pNodeFree->setVisible(!m_pNodePayed->isVisible());
}

void TeamChapterCell::setNew(bool bNew)
{
	m_pNodeNew->setVisible(bNew);
}