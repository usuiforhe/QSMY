//
//  TeamFriends.h
//  组队好友选择
//
//  Created by Eci on 14-04-12.

//  Copyright (c) 2014年 thedream. All rights reserved.
//


#define UNSELECT -1

#include "TeamFriends.h"
#include "../../../network/HttpClient.h"
#include "../../../managers/GameManager.h"
#include "TeamFriendsCell.h"
#include <algorithm>


static int sortFrontWarriorWithFightAndFate(const CCObject* p1, const CCObject* p2)
{
	friends::FriendInfo *pFriend1 = (friends::FriendInfo*)p1;
	friends::FriendInfo *pFriend2 = (friends::FriendInfo*)p2;
	friends::FriendWarrior *p1Warrior = pFriend1->getWarriorFront();
	friends::FriendWarrior *p2Warrior = pFriend2->getWarriorFront();
	
	if (pFriend1->getFate() == pFriend2->getFate())
	{
		return p1Warrior->getFight() > p2Warrior->getFight();
	}
	else
	{
		return pFriend1->getFate();
	}
}


static int sortBackWarriorWithFightAndFate(const CCObject* p1, const CCObject* p2)
{
	friends::FriendInfo *pFriend1 = (friends::FriendInfo*)p1;
	friends::FriendInfo *pFriend2 = (friends::FriendInfo*)p2;
	friends::FriendWarrior *p1Warrior = pFriend1->getWarriorBack();
	friends::FriendWarrior *p2Warrior = pFriend2->getWarriorBack();

	if (pFriend1->getFate() == pFriend2->getFate())
	{
		return p1Warrior->getFight() > p2Warrior->getFight();
	}
	else
	{
		return pFriend1->getFate();
	}
}

TeamFriends::TeamFriends()
	: m_pNodeContainer(NULL)
	, m_pBtnRule(NULL)
	, m_pDBtnCancel(NULL)
	, m_pCell(NULL)
	, m_pDelegate(NULL)
	, m_pModel(NULL)
	, m_pList(NULL)
	, m_pFateWarriors(NULL)
	, m_pFormationWarriors(NULL)
{
}

TeamFriends::~TeamFriends()
{
	CC_SAFE_RELEASE(m_pNodeContainer);
	CC_SAFE_RELEASE(m_pBtnRule);
	CC_SAFE_RELEASE(m_pDBtnCancel);
	CC_SAFE_RELEASE(m_pCell);
	CC_SAFE_RELEASE(m_pList);
	CC_SAFE_RELEASE(m_pFateWarriors);
	CC_SAFE_RELEASE(m_pFormationWarriors);
}

TeamFriends* TeamFriends::create()
{
	TeamFriends *pRet = new TeamFriends();
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

bool TeamFriends::init()
{
	if(!DLayer::init())
	{
		return false;
	}


	CCB_READER_AND_ADDCHILD("ui/ccb/team/teamFriendsList");


	cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("WarriorTypeMark", WarriorTypeMarkLoader::loader());
	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(pNodeLoaderLibrary);
	m_pCell = ___ccbReader__->readNodeGraphFromFile("ui/ccb/team/teamFriendsListCell", this);
	___ccbReader__->release();
	CC_SAFE_RETAIN(m_pCell);

	m_pTableView = DTableView::create(this, m_pNodeContainer->getContentSize());
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	m_pNodeContainer->addChild(m_pTableView);

	m_pDBtnCancel->getButton()->setTarget(this, menu_selector(TeamFriends::callBackCancel));
	m_pBtnRule->setTarget(this, menu_selector(TeamFriends::callBackRule));

	m_pModel = DM_GET_FRIENDS_MODEL;

	m_pList = CCArray::create();
	CC_SAFE_RETAIN(m_pList);

	m_pFateWarriors = CCArray::create();
	CC_SAFE_RETAIN(m_pFateWarriors);

	m_pFormationWarriors = CCArray::create();
	CC_SAFE_RETAIN(m_pFormationWarriors);

	uint32_t nEmptyIdx = m_pModel->getFriendIndex();

	FormationModel *pFormation = DM_GET_FORMATION_MODEL;
	for (uint8_t i = 0; i < FORMATION_POS_MAX - 1; ++i)
	{
		uint8_t idx = i;
		if (nEmptyIdx <= idx)
		{
			idx += 1;
		}
		formation::PositionInfo *pPosInfo = pFormation->getUnlockedPositionInfoByIndex(idx);
		if (NULL == pPosInfo)
		{
			continue;
		}
		m_pFormationWarriors->addObject(CCInteger::create(pPosInfo->iWarriorId));
		warrior::WarriorCurInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(pPosInfo->iWarriorId);
		if (pWarrior)
		{
			CCArray *warriorFate = CCArray::create();
			DM_GET_WARRIOR_MODEL->getWarriorFatesByWID(pWarrior->getWid(), warriorFate);

			CCObject *fateObj = NULL;
			CCARRAY_FOREACH(warriorFate, fateObj)
			{
				warrior::FateInfo *pFateInfo = static_cast<warrior::FateInfo *>(fateObj);
				CCInteger *member_id = NULL;
				CCObject *obj = NULL;

				CCInteger *absentID = NULL;

				CCARRAY_FOREACH(pFateInfo->member_ids, obj)
				{
					member_id = (CCInteger*)obj; 
					if (!DM_GET_FORMATION_MODEL->isWarriorBattle(member_id->getValue()))
					{
						if (absentID)
						{
							absentID = NULL;
							break;
						}
						else
						{
							absentID = member_id;
						}
					}
				}
				if (absentID)
				{
					CCLOG("%d", absentID->getValue());
					m_pFateWarriors->addObject(absentID);
				}
			}
		}
	}

	return true;
}

void TeamFriends::onEnter()
{
	BINDING_ONLY(m_pModel, this, TeamFriends::updateView, D_EVENT_FRIENDS_GET);
	m_pModel->syncFriends();
	DLayer::onEnter();
}

void TeamFriends::updateView()
{
	m_pModel->getFriends(m_pList);

	CCObject *pObj = NULL;
	CCArray *pArrAvailable = CCArray::create();
	CCArray *pArrDup = CCArray::create();
	CCArray *pArrRanout = CCArray::create();
	CCArray *pArrNew = CCArray::create();

	CCARRAY_FOREACH(m_pList, pObj)
	{
		friends::FriendInfo *pFriend = static_cast<friends::FriendInfo *>(pObj);
		pFriend->setFate(false);
		CCObject *pID = NULL;
		CCARRAY_FOREACH(m_pFateWarriors, pID)
		{
			CCInteger *pWID = static_cast<CCInteger *>(pID);
			if (pWID->getValue() == pFriend->getWarriorAtIndex(m_pModel->getFriendIndex())->getWID())
			{
				pFriend->setFate(true);
				break;
			}
		}
		pFriend->setDup(false);
		CCARRAY_FOREACH(m_pFormationWarriors, pID)
		{
			CCInteger *pWID = static_cast<CCInteger *>(pID);
			if (pWID->getValue() == pFriend->getWarriorAtIndex(m_pModel->getFriendIndex())->getWID())
			{
				pFriend->setDup(true);
				break;
			}
		}
		pFriend->setRanout(m_pModel->isAssistToday(pFriend));
//		pFriend->setNew(m_pModel->isAssociateToday(pFriend));
		if (pFriend->getRanout())
		{
			if (!pFriend->getFate() || !pArrRanout->count())
			{
				pArrRanout->addObject(pFriend);
			}
			else
			{
				pArrRanout->insertObject(pFriend, 0);
			}
		}
		else if (pFriend->getNew())
		{
			if (!pFriend->getFate() || !pArrNew->count())
			{
				pArrNew->addObject(pFriend);
			}
			else
			{
				pArrNew->insertObject(pFriend, 0);
			}
		}
		else if (pFriend->getDup())
		{
			pArrDup->addObject(pFriend);
		}
		else
		{
			pArrAvailable->addObject(pFriend);
		}
	}
	if (m_pModel->getFriendIndex() % 2 == 0)
	{
		std::sort(pArrAvailable->data->arr, pArrAvailable->data->arr + pArrAvailable->data->num, sortBackWarriorWithFightAndFate);
	}
	else
	{
		std::sort(pArrAvailable->data->arr, pArrAvailable->data->arr + pArrAvailable->data->num, sortFrontWarriorWithFightAndFate);
	}
	m_pList->removeAllObjects();
	m_pList->addObjectsFromArray(pArrAvailable);
	m_pList->addObjectsFromArray(pArrDup);
	m_pList->addObjectsFromArray(pArrNew);
	m_pList->addObjectsFromArray(pArrRanout);
	m_pTableView->reloadData();
}

void TeamFriends::onExit()
{
	UNBINDING_ALL(m_pModel, this);
	DLayer::onExit();
}

bool TeamFriends::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNodeContainer", CCNode *, m_pNodeContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBtnRule", CCMenuItem *, m_pBtnRule);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDBtnCancel", DButton *, m_pDBtnCancel);
	return false;
}

CCTableViewCell* TeamFriends::tableCellAtIndex(CCTableView *table, uint32_t idx)
{
	TeamFriendsCell *pCell = (TeamFriendsCell *)table->dequeueCell();
	if (!pCell)
	{
		pCell = TeamFriendsCell::create();
		pCell->setDelegate(this);
	}
	friends::FriendInfo *pFriend = (friends::FriendInfo *)m_pList->objectAtIndex(idx);
	pCell->setData(pFriend);
	return pCell;
}

uint32_t TeamFriends::numberOfCellsInTableView(CCTableView *table)
{
	if (!m_pList)
	{
		return 0;
	}
	return m_pList->count();
}


CCSize TeamFriends::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_pCell->getContentSize();
}

void TeamFriends::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

void TeamFriends::TeamFriendsCellSelected(TeamFriendsCell *pCell)
{
	m_pModel->selectFriend(pCell->getData());
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavMake);
	}
}

void TeamFriends::callBackCancel(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavMake);
	}
}

void TeamFriends::TeamFriendsCellView(TeamFriendsCell *pCell)
{
	// TeamWarrior removed
}

void TeamFriends::callBackRule(CCObject *pObj)
{
	if (m_pDelegate)
	{
		m_pDelegate->TeamNavigateTo(kTeamNavRule);
	}
}