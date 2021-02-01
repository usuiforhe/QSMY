//
//  FriendsModel.cpp
//  QSMY
//
//  Created by Eci on 14-4-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendsModel.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include <algorithm>

USING_NS_CC;


static int sortFriendByLevel(const CCObject* p1, const CCObject* p2)
{
	return ((friends::FriendInfo*)p1)->getLevel() > ((friends::FriendInfo*)p2)->getLevel();
};

namespace friends
{
	FriendWarrior::FriendWarrior()
		: m_nWID(0)
		, m_nUpLevel(0)
		, m_nAvatarID(0)
		, m_nSkillLevel(0)
		, m_nFight(0)
	{

	}

	FriendWarrior::~FriendWarrior()
	{

	}

	void FriendWarrior::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nWID, val, "wid");
		DJSON_UINT(m_nUpLevel, val, "up");
		DJSON_UINT(m_nAvatarID, val, "did");
		DJSON_UINT(m_nSkillLevel, val, "slv");
		DJSON_UINT(m_nFight, val, "f");
	}
	uint32_t FriendWarrior::getAvatarID()
	{
		if (m_nAvatarID)
		{
			return D_FIX_DRESS_ID(m_nAvatarID);
		}
		return m_nWID;
	}

	FriendWarrior *FriendWarrior::create(warrior::WarriorCurInfo *pCur)
	{
		if (!pCur)
		{
			return NULL;
		}
		
		FriendWarrior *pRet = new FriendWarrior(); 
		if (pRet && pRet->init(pCur))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

	bool FriendWarrior::init(warrior::WarriorCurInfo *pCur)
	{
		m_nWID = pCur->getWid();
		m_nUpLevel = pCur->getWarriorUpLv();
		m_nAvatarID = pCur->getDressId();
		m_nSkillLevel = pCur->getSlv();
		//m_nFight = 0; 暂时不需要转换
		return init();
	}

	FriendInfo::FriendInfo()
		: m_nPresentTs(0)
		, m_nReceiveTs(0)
		, m_nGiftState(0)
		, m_pWarriorFront(NULL)
		, m_pWarriorBack(NULL)
		, m_bDup(false)
		, m_bFate(false)
		, m_bRanout(false)
		, m_bNew(false)
	{

	}
	FriendInfo::~FriendInfo()
	{
		CC_SAFE_RELEASE(m_pWarriorFront);
		CC_SAFE_RELEASE(m_pWarriorBack);
	}
	void FriendInfo::parse(const Json::Value &val)
	{
		FriendBasic::parse(val);
		DJSON_UINT(m_nPresentTs, val, "sgTs");
		DJSON_UINT(m_nReceiveTs, val, "rgTs");
		DJSON_UINT(m_nAssociateTs, val, "cTs");
		DJSON_UINT(m_nGiftState, val, "gs");
		m_pWarriorFront = FriendWarrior::create();
		m_pWarriorFront->parse(val["fw"]);
		CC_SAFE_RETAIN(m_pWarriorFront);
		m_pWarriorBack = FriendWarrior::create();
		m_pWarriorBack->parse(val["bw"]);
		CC_SAFE_RETAIN(m_pWarriorBack);
		
	}

	FriendWarrior *FriendInfo::getWarriorAtIndex(uint32_t pos)
	{
		if (pos % 2 == 1)
		{
			return m_pWarriorFront;
		}
		return m_pWarriorBack;
	}

	FriendBasic::FriendBasic()
		: m_nFriendID(0)
		, m_sName()
		, m_nLevel(0)
		, m_nLeaderID(0)
		, m_nAvatarID(0)
		, m_bRequested(false)
	{

	}

	FriendBasic::~FriendBasic()
	{

	}

	void FriendBasic::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nFriendID, val, "fid");
		DJSON_STRING(m_sName, val, "name");
		DJSON_UINT(m_nLevel, val, "lv");
		DJSON_UINT(m_nLeaderID, val, "lid");
		DJSON_UINT(m_nAvatarID, val, "ldid");
		DJSON_BOOL(m_bRequested, val, "requested");
	}

	uint32_t FriendBasic::getAvatarID()
	{
		if (m_nAvatarID)
		{
			return D_FIX_DRESS_ID(m_nAvatarID);
		}
		return m_nLeaderID;
	}

	DungeonWarrior::DungeonWarrior()
		: m_nPosition(0)
		, m_nLevel(0)
		, m_nHP(0)
		, m_nMaxHP(0)
		, m_bRevival(false)
		, m_bEnhance(false)
		, m_bRecover(false)
	{

	}
	DungeonWarrior::~DungeonWarrior()
	{

	}
	
	void DungeonWarrior::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nPosition, val, "pos");
		DJSON_UINT(m_nWID, val, "wid");
		DJSON_UINT(m_nAvatarID, val, "did");
		DJSON_UINT(m_nLevel, val, "lv");
		DJSON_UINT(m_nUpLevel, val, "uplv");
		DJSON_UINT(m_nHP, val, "hp");
		DJSON_UINT(m_nMaxHP, val, "maxHp");
	}
	
	const Json::Value DungeonWarrior::getBuffJson()
	{
		Json::Value val(Json::objectValue);
		val["pos"] = m_nPosition;
		if (m_bRevival)
		{
			val["r"] = 1;
		}
		if (m_bEnhance)
		{
			val["e"] = 1;
		}
		if (m_bRecover)
		{
			val["h"] = 1;
		}
		if (m_bRevival || m_bEnhance || m_bRecover)
		{
			return val;
		}
		return Json::Value(Json::nullValue);
	}

	Dungeon::Dungeon()
		: m_nOngoingID(0)
		, m_nOngoingProgress(0)
		, m_nFriendID(0)
		, m_pClearList(NULL)
		, m_pTodayList(NULL)
		, m_nResetCount(0)
		, m_pWarriors(NULL)
		, m_pFriendWarrior(NULL)
		, m_nMaxClear(0)
		, m_vAssistedFriendID()
	{

	}

	Dungeon::~Dungeon()
	{
		CC_SAFE_RELEASE(m_pClearList);
		CC_SAFE_RELEASE(m_pTodayList);
		CC_SAFE_RELEASE(m_pWarriors);
		CC_SAFE_RELEASE(m_pFriendWarrior);
	}
	bool Dungeon::init()
	{
		m_pClearList = CCArray::create();
		CC_SAFE_RETAIN(m_pClearList);
		m_pTodayList = CCArray::create();
		CC_SAFE_RETAIN(m_pTodayList);
		m_pWarriors = CCDictionary::create();
		CC_SAFE_RETAIN(m_pWarriors);
		return ValueObjcet::init();
	}
	
	void Dungeon::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nOngoingID, val, "cd");
		DJSON_UINT(m_nOngoingProgress, val, "cr");
		DJSON_UINT(m_nFriendID, val, "fid");
		DJSON_UINT(m_nResetCount, val, "rt");
		m_pClearList->removeAllObjects();
		for (Json::ValueIterator i = val["cList"].begin(); i != val["cList"].end(); i++)
		{
			Json::Value v = *i;
			m_pClearList->addObject(CCInteger::create(v.asUInt()));
			if (m_nMaxClear < v.asUInt())
			{
				m_nMaxClear = v.asUInt();
			}
			
		}
		m_pTodayList->removeAllObjects();
		for (Json::ValueIterator i = val["tc"].begin(); i != val["tc"].end(); i++)
		{
			Json::Value v = *i;
			m_pTodayList->addObject(CCInteger::create(v.asUInt()));
		}
		m_pWarriors->removeAllObjects();
		for (Json::ValueIterator i = val["warriors"].begin(); i != val["warriors"].end(); i++)
		{
			Json::Value v = *i;
			DungeonWarrior *pWarrior = DungeonWarrior::create();
			pWarrior->parse(v);
			m_pWarriors->setObject(pWarrior, pWarrior->getPosition());
		}
		m_vAssistedFriendID.clear();
		for (Json::ValueIterator i = val["ta"].begin(); i != val["ta"].end(); i++)
		{
			Json::Value v = *i;
			m_vAssistedFriendID.push_back(v.asUInt());
		}
		
	}
	DropInfo::DropInfo()
		: m_nID(0)
		, m_nDropType(0)
		, m_nDropID(0)
	{

	}
	DropInfo::~DropInfo()
	{

	}
	void DropInfo::parse(const CSVDecoder::CSVLine &line)
	{
		m_nID		= atoi(line[0].c_str());
		m_nDropType	= atoi(line[1].c_str());
		m_nDropID	= atoi(line[2].c_str());
	}

	BattleInfo::BattleInfo()
		: m_nID(0)
		, m_nChapterID(0)
		, m_nTurnOrder(0)
		, m_nEffect(0)
		, m_nSceneID(0)
		, m_nLeaderID(0)
	{

	}
	BattleInfo::~BattleInfo()
	{

	}
	void BattleInfo::parse(const CSVDecoder::CSVLine &line)
	{
		m_nID			= atoi(line[0].c_str());
		m_nChapterID	= atoi(line[1].c_str());
		m_nTurnOrder	= atoi(line[2].c_str());
		m_nEffect		= atoi(line[3].c_str());
		m_nSceneID		= atoi(line[4].c_str());
		m_nLeaderID		= atoi(line[5].c_str());
	}

	ChapterInfo::ChapterInfo()
		: m_nID(0)
		, m_sName()
		, m_nTurns(0)
		, m_nFriendPos(0)
		, m_nUnlockChapter(0)
		, m_nRushChapter(0)
		, m_nDropType(0)
		, m_nDropID(0)
		, m_nDropDetailsID(0)
		, m_nSceneID(1)
		, m_pBattleList(NULL)
		, m_pDropList(NULL)
		, m_bUnlock(false)
		, m_bDone(false)
		, m_bRush(false)
	{

	}
	ChapterInfo::~ChapterInfo()
	{
		CC_SAFE_RELEASE(m_pBattleList);
		CC_SAFE_RELEASE(m_pDropList);
	}
	bool ChapterInfo::init()
	{
		m_pBattleList = CCArray::create();
		CC_SAFE_RETAIN(m_pBattleList);
		m_pDropList = CCArray::create();
		CC_SAFE_RETAIN(m_pDropList);
		return ValueObjcet::init();
	}
	void ChapterInfo::parse(const CSVDecoder::CSVLine &line)
	{
		m_nID				= atoi(line[0].c_str());
		m_sName				= line[1];
		m_nTurns			= atoi(line[2].c_str());
		m_nFriendPos		= atoi(line[3].c_str());
		m_nUnlockChapter	= atoi(line[4].c_str());
		m_nRushChapter		= atoi(line[5].c_str());
		m_nDropType			= atoi(line[6].c_str());
		m_nDropID			= atoi(line[7].c_str());
		m_nDropDetailsID	= atoi(line[8].c_str());
		m_nSceneID			= atoi(line[9].c_str());
	}
}


using namespace friends;


FriendsModel::FriendsModel()
	: m_pFriends(NULL)
	, m_pRecommend(NULL)
	, m_pFound(NULL)
	, m_pRequests(NULL)
	, m_pDungeon(NULL)
	, m_bFriendsSynced(false)
	, m_bRecommendSynced(false)
	, m_bRequestsSynced(false)
	, m_pMsgReceiver(NULL)
	, m_pChapters(NULL)
	, m_pSelectedChapter(NULL)
	, m_pRemoveFriends(NULL)
	, m_mBuffItemNum()
	, m_nFriendsLastSynced(0)
	, m_nRequestsCount(0)
	, m_nMsgLastSent(0)
	, m_bInTeam(false)
{
    
}

FriendsModel::~FriendsModel()
{
	CC_SAFE_RELEASE(m_pFriends);
	CC_SAFE_RELEASE(m_pRecommend);
	CC_SAFE_RELEASE(m_pFound);
	CC_SAFE_RELEASE(m_pRequests);
	CC_SAFE_RELEASE(m_pDungeon);
	CC_SAFE_RELEASE(m_pMsgReceiver);
	CC_SAFE_RELEASE(m_pChapters);
	CC_SAFE_RELEASE(m_pSelectedChapter);
	CC_SAFE_RELEASE(m_pRemoveFriends);
}

bool FriendsModel::init()
{
	m_pDungeon = Dungeon::create();
	CC_SAFE_RETAIN(m_pDungeon);

	m_pChapters = CCArray::create();
	CC_SAFE_RETAIN(m_pChapters);

	loadChapterInfo();

    return Model::init();
}

CCArray *FriendsModel::getFriends(cocos2d::CCArray *pFriends)
{
	if (!pFriends)
	{
		pFriends = CCArray::create();
	}
	else
	{
		pFriends->removeAllObjects();
	}
	if (m_pFriends)
	{
		CCArray *pReceive = CCArray::create();
		CCArray *pReturn = CCArray::create();
		CCArray *pPresent = CCArray::create();
		CCArray *pFormation = CCArray::create();
		CCObject *pObj = NULL;
		CCARRAY_FOREACH(m_pFriends, pObj)
		{
			FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
			GiftState eState = this->getGiftState(pInfo);
			switch (eState)
			{
			case kGiftStateReceive:
				pReceive->addObject(pInfo);
				break;
			case kGiftStateReturn:
				pReturn->addObject(pInfo);
				break;
			case kGiftStatePresent:
				pPresent->addObject(pInfo);
				break;
			case kGiftStateFormation:
				pFormation->addObject(pInfo);
				break;
			default:;
			}
		}
		pFriends->addObjectsFromArray(pReceive);
		pFriends->addObjectsFromArray(pReturn);
		pFriends->addObjectsFromArray(pPresent);
		pFriends->addObjectsFromArray(pFormation);
	}
	return pFriends;
}

CCArray *FriendsModel::getRecommend(cocos2d::CCArray *pRecommend)
{
	if (!pRecommend)
	{
		pRecommend = CCArray::create();
	}
	else
	{
		pRecommend->removeAllObjects();
	}
	if (m_pRecommend)
	{
		if (m_pFriends)
		{
			CCObject *pObj = NULL;
			CCARRAY_FOREACH(m_pRecommend, pObj)
			{
				FriendBasic *pBasic = static_cast<FriendBasic *>(pObj);
				FriendInfo *pFound = NULL;
				CCARRAY_FOREACH(m_pFriends, pObj)
				{
					FriendInfo *pFriend = static_cast<FriendInfo *>(pObj);
					if (pFriend->m_nFriendID == pBasic->m_nFriendID)
					{
						pFound = pFriend;
						break;
					}
				}
				if (!pFound)
				{
					pRecommend->addObject(pBasic);
				}
			}
		}
		else
		{
			pRecommend->addObjectsFromArray(m_pRecommend);
		}
	}
	return pRecommend;
}

CCArray *FriendsModel::getRequests(cocos2d::CCArray *pRequests)
{
	if (!pRequests)
	{
		pRequests = CCArray::create();
	}
	else
	{
		pRequests->removeAllObjects();
	}
	if (m_pRequests)
	{
		pRequests->addObjectsFromArray(m_pRequests);
	}
	return pRequests;
}

void FriendsModel::parseFriends(const Json::Value &val)
{
	if (!m_pFriends)
	{
		m_pFriends = CCArray::create();
		CC_SAFE_RETAIN(m_pFriends);
	}
	else
	{
		m_pFriends->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		Json::Value v = *i;
		FriendInfo *pInfo = FriendInfo::create();
		pInfo->parse(v);
		m_pFriends->addObject(pInfo);
	}

	std::sort(m_pFriends->data->arr, m_pFriends->data->arr + m_pFriends->data->num, sortFriendByLevel);

	if (m_bFriendsSynced)
	{
		m_nFriendsLastSynced = GameManager::sharedGameManager()->getServerTime();
	}
	m_bFriendsSynced = true;
	
	postNotification(D_EVENT_FRIENDS_GET);
}

void FriendsModel::parseRecommend(const Json::Value &val)
{
	if (!m_pRecommend)
	{
		m_pRecommend = CCArray::create();
		CC_SAFE_RETAIN(m_pRecommend);
	}
	else
	{
		m_pRecommend->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		Json::Value v = *i;
		FriendBasic *pInfo = FriendBasic::create();
		pInfo->parse(v);
		m_pRecommend->addObject(pInfo);
	}
	
	m_bRecommendSynced = true;
	postNotification(D_EVENT_FRIENDS_RECOMMEND);
}

void FriendsModel::parseFound(const Json::Value &val)
{
	if (!m_pFound)
	{
		m_pFound = FriendBasic::create();
		CC_SAFE_RETAIN(m_pFound);
	}
	m_pFound->parse(val);
}

void FriendsModel::parseRequests(const Json::Value &val)
{
	if (!m_pRequests)
	{
		m_pRequests = CCArray::create();
		CC_SAFE_RETAIN(m_pRequests);
	}
	else
	{
		m_pRequests->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		Json::Value v = *i;
		FriendBasic *pInfo = FriendBasic::create();
		pInfo->parse(v);
		m_pRequests->addObject(pInfo);
	}
	m_bRequestsSynced = true;
	if (m_nRequestsCount != m_pRequests->count())
	{
		m_nRequestsCount = m_pRequests->count();
		postNotification(D_EVENT_FRIENDS_NEW_REQUEST);
	}
	
	postNotification(D_EVENT_FRIENDS_REQUESTS);
}

void FriendsModel::parseDungeon(const Json::Value &val)
{
	m_pDungeon->parse(val);
	uint32_t cid = m_pDungeon->m_nOngoingID;
	uint32_t fid = m_pDungeon->m_nFriendID;
	CCObject *pObj = NULL;
	if (cid == 0)
	{
		setSelectedChapter(NULL);
	}
	else
	{
		CCARRAY_FOREACH(m_pChapters, pObj)
		{
			ChapterInfo *pInfo = static_cast<ChapterInfo *>(pObj);
			if (pInfo->m_nID == cid)
			{
				setSelectedChapter(pInfo);
				break;
			}

		}
	}
	if (fid > 0)
	{
		if (m_pDungeon)
		{
			if (std::find(m_pDungeon->m_vAssistedFriendID.begin(), m_pDungeon->m_vAssistedFriendID.end(), fid) == m_pDungeon->m_vAssistedFriendID.end())
			{
				m_pDungeon->m_vAssistedFriendID.push_back(fid);
			}
		}
		
// 		CCARRAY_FOREACH(m_pFriends, pObj)
// 		{
// 			FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
// 			if (pInfo->m_nFriendID == fid)
// 			{
// 				pInfo->m_nAssistTs = GameManager::sharedGameManager()->getServerTime();
// 				break;
// 			}
// 			
// 		}
	}
	else
	{
		CC_SAFE_RELEASE(m_pDungeon->m_pFriendWarrior);
		m_pDungeon->m_pFriendWarrior = NULL;
	}
	
	if (m_pDungeon->m_nMaxClear > 0)
	{
		CCObject *pObj = NULL;
		CCARRAY_FOREACH(m_pChapters, pObj)
		{
			ChapterInfo *pChapter = static_cast<ChapterInfo *>(pObj);
			if (pChapter->m_nID <= m_pDungeon->m_nMaxClear + 1)
			{
				pChapter->m_bUnlock = true;
			}
			if (pChapter->m_nRushChapter <= m_pDungeon->m_nMaxClear)
			{
				pChapter->m_bRush = true;
			}
			if (pChapter->m_nRushChapter == 17)
			{
				CCLOG("");
			}
			
		}
	}
	else
	{
		ChapterInfo *pChapter = static_cast<ChapterInfo *>(m_pChapters->objectAtIndex(0));
		pChapter->m_bUnlock = true;
	}
	

	CCARRAY_FOREACH(m_pDungeon->m_pTodayList, pObj)
	{
		CCInteger *pCID = static_cast<CCInteger *>(pObj);
		if (pCID->getValue() == 1)
		{
			CCLOG("");
		}
		CCObject *pChapterObj = NULL;
		CCARRAY_FOREACH(m_pChapters, pChapterObj)
		{
			ChapterInfo *pChapter = static_cast<ChapterInfo *>(pChapterObj);

			if (pCID->getValue() == pChapter->m_nID)
			{
				
				pChapter->m_bDone = true;
				break;
			}
		}
	}
	
	m_pDungeon->isSynced = true;
	postNotification(D_EVENT_FRIENDS_DUNGEON);
}

void FriendsModel::parseDungeonRush(const Json::Value &val)
{
	uint32_t fid = m_pDungeon->getFriendID();

	//CCObject *pObj = NULL;
	if (fid > 0)
	{
		if (m_pDungeon)
		{
			if (std::find(m_pDungeon->m_vAssistedFriendID.begin(), m_pDungeon->m_vAssistedFriendID.end(), fid) == m_pDungeon->m_vAssistedFriendID.end())
			{
				m_pDungeon->m_vAssistedFriendID.push_back(fid);
			}
		}
// 		CCARRAY_FOREACH(m_pFriends, pObj)
// 		{
// 			FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
// 			if (pInfo->m_nFriendID == fid)
// 			{
// 				pInfo->m_nAssistTs = GameManager::sharedGameManager()->getServerTime();
// 				break;
// 			}
// 
// 		}
	}
	parseDungeon(val);
}

uint32_t FriendsModel::getFriendsCount()
{
	if (!m_pFriends)
	{
		return 0;
	}
	return m_pFriends->count();
}

void FriendsModel::syncFriends()
{
	if (m_bFriendsSynced)
	{
		postNotification(D_EVENT_FRIENDS_GET);
	}
	else
	{
		HTTP_CLIENT->getFriends();
	}
}

void FriendsModel::syncRecommend()
{
	if (m_bRecommendSynced)
	{
		postNotification(D_EVENT_FRIENDS_RECOMMEND);
	}
	else
	{
		HTTP_CLIENT->recommendFriend();
	}
}

void FriendsModel::syncRequests()
{
	if (m_bRequestsSynced)
	{
		postNotification(D_EVENT_FRIENDS_REQUESTS);
	}
	else
	{
		HTTP_CLIENT->getFriendRequests();
	}
}

void FriendsModel::syncDungeon()
{
	if (m_pDungeon->isSynced)
	{
		postNotification(D_EVENT_FRIENDS_DUNGEON);
	}
	else
	{
		HTTP_CLIENT->FriendDungeon();
	}
}

GiftState FriendsModel::getGiftState(friends::FriendInfo *pInfo)
{
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	uint32_t uStart = TimeUtil::getStartTimeOfToday(uNow);
	if (pInfo->getGiftState() == 0 && pInfo->getReceiveTs() > uStart)
	{
		return kGiftStateReceive;
	}
	else if (pInfo->getPresentTs() < uStart)
	{
		if (pInfo->getReceiveTs() > uStart)
		{
			return kGiftStateReturn;
		}
		else
		{
			return kGiftStatePresent;
		}
	}
	return kGiftStateFormation;
}

void FriendsModel::selectFriend(friends::FriendInfo *pInfo)
{
	if (!pInfo)
	{
		m_pDungeon->m_nFriendID = 0;
		CC_SAFE_RELEASE(m_pDungeon->m_pFriendWarrior);
		m_pDungeon->m_pFriendWarrior = NULL;
	}
	else
	{
		m_pDungeon->m_nFriendID = pInfo->m_nFriendID;
		friends::FriendWarrior *pWarrior = pInfo->getWarriorAtIndex(getFriendIndex());
		CC_SAFE_RETAIN(pWarrior);
		CC_SAFE_RELEASE(m_pDungeon->m_pFriendWarrior);
		m_pDungeon->m_pFriendWarrior = pWarrior;
	}
	postNotification(D_EVENT_FRIENDS_DUNGEON);
}

void FriendsModel::friendGo()
{
	if (!m_pDungeon->m_pFriendWarrior)
	{
		return;
	}
	HTTP_CLIENT->goFriendDungeonBattle(m_pSelectedChapter->m_nID, m_pDungeon->m_nFriendID,m_pDungeon->m_pFriendWarrior->m_nWID, m_pDungeon->getWarriors());
}

void FriendsModel::friendGoAndResetIfNeeded()
{
	if (!m_pDungeon->m_pFriendWarrior)
	{
		return;
	}
	if (m_pSelectedChapter->m_bDone)
	{
		int rest = getResetRest();
		if (rest == 0)
		{

			return;
		}
	}
	if (m_pSelectedChapter->m_bDone)
	{
		HTTP_CLIENT->resetFriendDungeon(m_pSelectedChapter->m_nID);
	}
	else
	{
		if (m_pSelectedChapter->m_bRush)
		{
			HTTP_CLIENT->rushFriendDungeonBattle(m_pSelectedChapter->m_nID, m_pDungeon->m_nFriendID);
		}
		else
		{
			HTTP_CLIENT->goFriendDungeonBattle(m_pSelectedChapter->m_nID, m_pDungeon->m_nFriendID,m_pDungeon->m_pFriendWarrior->m_nWID, m_pDungeon->getWarriors());
		}
	}
}

uint32_t FriendsModel::getFriendIndex()
{
	// 2=>0
	// 3=>1
	// 6=>2
	// 7=>3
	// 10=>4
	// 11=>5
	// 14=>6
	return m_pSelectedChapter->m_nFriendPos / 2 - 1 + m_pSelectedChapter->m_nFriendPos % 2;
}

void FriendsModel::friendMessage()
{
	if (!m_pMsgReceiver)
	{
		return;
	}
}

bool FriendsModel::isAssistToday(friends::FriendInfo *pInfo)
{
	if (NULL == m_pDungeon)
	{
		return false;
	}
	
	return std::find(m_pDungeon->m_vAssistedFriendID.begin(), m_pDungeon->m_vAssistedFriendID.end(), pInfo->getFriendID()) != m_pDungeon->m_vAssistedFriendID.end();
}

bool FriendsModel::isAssociateToday(friends::FriendInfo *pInfo)
{
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	uint32_t uStart = TimeUtil::getStartTimeOfToday(uNow);
	return pInfo->m_nAssociateTs > uStart;
}

void FriendsModel::loadDropInfo(const CSVDecoder::CSV &val, CCArray *pArr)
{
	if (pArr)
	{
		pArr->removeAllObjects();

		for (uint32_t i = 0; i < val.size(); ++i)
		{
			DropInfo *pInfo = DropInfo::create();
			pInfo->parse(val[i]);
			pArr->addObject(pInfo);
		}
	}
	
}

void FriendsModel::loadBattleInfo(const CSVDecoder::CSV &val, CCArray *pArr)
{
	if (pArr)
	{
		pArr->removeAllObjects();

		for (uint32_t i = 0; i < val.size(); ++i)
		{
			BattleInfo *pInfo = BattleInfo::create();
			pInfo->parse(val[i]);
			pArr->addObject(pInfo);
		}
	}

}

void FriendsModel::loadChapterInfo()
{
	m_pChapters->removeAllObjects();
	CCArray *pDropArr = CCArray::create();
	CCArray *pBattleArr = CCArray::create();
	loadDropInfo( CSVDecoder::create(TEAM_DROP_CONFIG_PATH)->getData(), pDropArr);
	loadBattleInfo( CSVDecoder::create(TEAM_BATTLE_CONFIG_PATH)->getData(), pBattleArr);
	const CSVDecoder::CSV val =  CSVDecoder::create(TEAM_CHAPTER_CONFIG_PATH)->getData();

	for (uint32_t i = 0; i < val.size(); ++i)
	{
		ChapterInfo *pInfo = ChapterInfo::create();
		pInfo->parse(val[i]);
		for (int i = 0; i < pDropArr->count(); i++)
		{
			DropInfo *pDrop = static_cast<DropInfo *>(pDropArr->objectAtIndex(i));
			if (pInfo->m_nDropDetailsID == pDrop->getID())
			{
				pInfo->m_pDropList->addObject(pDrop);
			}
		}
		for (int i = 0; i < pBattleArr->count(); i++)
		{
			BattleInfo *pBattle = static_cast<BattleInfo *>(pBattleArr->objectAtIndex(i));
			if (pInfo->m_nID == pBattle->getChapterID())
			{
				pInfo->m_pBattleList->addObject(pBattle);
			}
		}
		m_pChapters->addObject(pInfo);
	}
}

void FriendsModel::getChapters(CCArray *pArr)
{
	if (pArr)
	{
		pArr->removeAllObjects();
		pArr->addObjectsFromArray(m_pChapters);
	}
}

void FriendsModel::removeRequest(uint32_t nFriendID)
{
	int idx = 0;
	FriendBasic *pBasic = NULL;
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pRequests, pObj)
	{
		pBasic = static_cast<FriendBasic *>(pObj);
		if (pBasic->m_nFriendID == nFriendID)
		{
			m_pRequests->removeObjectAtIndex(idx);
			break;
		}
		idx++;
	}
	m_nRequestsCount--;
	postNotification(D_EVENT_FRIENDS_NEW_REQUEST);
	postNotification(D_EVENT_FRIENDS_REQUESTS);
}

void FriendsModel::addFriend(const Json::Value &val)
{
	FriendInfo *pAdd = FriendInfo::create();
	pAdd->parse(val);
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (pInfo->m_nFriendID == pAdd->m_nFriendID)
		{
			return;
		}
		
	}
	m_pFriends->addObject(pAdd);
	postNotification(D_EVENT_FRIENDS_GET);
}

void FriendsModel::removeFriendsServer()
{
	//if (m_pRemoveFriends)
	//{
	//	HTTP_CLIENT->deleteFriend(m_pRemoveFriends);
	//}
}

void FriendsModel::removeFriends()
{
	CCArray *restFriends = CCArray::create();
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		CCObject *pDel = NULL;
		int delIdx = 0;
		CCARRAY_FOREACH(m_pRemoveFriends, pDel)
		{
			FriendInfo *pDelInfo = static_cast<FriendInfo *>(pDel);
			if (pDelInfo->m_nFriendID == pInfo->m_nFriendID)
			{
				break;
			}
			
			delIdx++;
		}
		if (delIdx != m_pRemoveFriends->count())
		{
			m_pRemoveFriends->removeObjectAtIndex(delIdx);
			continue;
		}
		restFriends->addObject(pInfo);
	}
	m_pFriends->removeAllObjects();
	m_pFriends->addObjectsFromArray(restFriends);

	postNotification(D_EVENT_FRIENDS_GET);
}


void FriendsModel::clearRequest()
{
	m_pRequests->removeAllObjects();
	m_nRequestsCount = 0;
	postNotification(D_EVENT_FRIENDS_NEW_REQUEST);
	postNotification(D_EVENT_FRIENDS_REQUESTS);
}

void FriendsModel::requestSent(const Json::Value &request)
{
	uint32_t friendID = request["targetId"].asUInt();
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pRecommend, pObj)
	{
		FriendBasic *pBasic = static_cast<FriendBasic *>(pObj);
		if (pBasic->m_nFriendID == friendID)
		{
			pBasic->m_bRequested = true;
			pBasic->postNotification(D_EVENT_FRIENDS_BASIC_REFRESH);
			break;
		}
	}
}

void FriendsModel::setSelectedChapter(friends::ChapterInfo *pChapter)
{
	CC_SAFE_RETAIN(pChapter);
	CC_SAFE_RELEASE(m_pSelectedChapter);
	if (pChapter == NULL)
	{
		m_pLastChapter = m_pSelectedChapter;
	}
	
	m_pSelectedChapter = pChapter;
	if (m_pSelectedChapter)
	{
		cocos2d::CCDictElement *pEle = NULL;
		CCDictionary *pWarriors = m_pDungeon->m_pWarriors;
		CCDICT_FOREACH(pWarriors, pEle)
		{
			DungeonWarrior *pWarrior = static_cast<DungeonWarrior *>(pEle->getObject());
			if (pWarrior->m_nPosition == m_pSelectedChapter->m_nFriendPos)
			{
				CC_SAFE_RETAIN(pWarrior);
				CC_SAFE_RELEASE(m_pDungeon->m_pFriendWarrior);
				m_pDungeon->m_pFriendWarrior = pWarrior;
				break;
			}

		}
	}
}

void FriendsModel::resetDungeon(const Json::Value &requestData)
{
	uint32_t cid = requestData["id"].asUInt();
	CCAssert(m_pSelectedChapter->m_nID == cid, "chapter id mismatch");
	m_pSelectedChapter->m_bDone = false;
	friendGoAndResetIfNeeded();
}

bool FriendsModel::isFriend(uint32_t freindID)
{
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (pInfo->getFriendID() == freindID)
		{
			return true;
		}
	}
	return false;
}

bool FriendsModel::isFriendsFull()
{
	int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
	if (nVipLv == 0)
	{
		nVipLv = 1;
	}

	user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
	uint32_t friendMax = pCurInfo->getFriendsMax();
	return m_pFriends->count() >= friendMax;
}

uint32_t FriendsModel::getResetRest()
{
	int nVipLv = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
	if (nVipLv == 0)
	{
		nVipLv = 1;
	}

	user::VipInfo *pCurInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(nVipLv);
	return pCurInfo->getTeamResetMax() - m_pDungeon->getResetCount();
}

void FriendsModel::refreshFriendsIfNeeded()
{
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	if (uNow > m_nFriendsLastSynced + D_CONFIG_UINT_FOR_KEY(FRIENDS_REFRESH_CD))
	{
		HTTP_CLIENT->getFriends();
	}
	else
	{
		Tips_Alert(D_LOCAL_STRING("FriendRefreshCD%u", m_nFriendsLastSynced + D_CONFIG_UINT_FOR_KEY(FRIENDS_REFRESH_CD) - uNow).c_str());
	}
}

void FriendsModel::giftReceived(const Json::Value &requestData)
{
	uint32_t friendID = requestData["friendId"].asUInt();

	DropModel *pModel = DM_GET_DROP_MODEL;
	cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asUInt());
	CCObject *pDrop = NULL;
	if(pAwardList != NULL && pAwardList->count() != 0)
	{
		pDrop = pAwardList->objectAtIndex(0);
	}

	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (friendID == pInfo->m_nFriendID)
		{
			pInfo->m_nGiftState = 1;
			pInfo->postNotification(D_EVENT_FRIENDS_STATUS_REFRESH, pDrop);
			break;
		}
	}
}

void FriendsModel::giftSent(const Json::Value &requestData)
{
	uint32_t friendID = requestData["friendId"].asUInt();
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (friendID == pInfo->m_nFriendID)
		{
			pInfo->m_nPresentTs = GameManager::sharedGameManager()->getServerTime();
			pInfo->postNotification(D_EVENT_FRIENDS_STATUS_REFRESH);
			break;
		}
	}
}

void FriendsModel::requestsCountGot(const Json::Value &responseData)
{
	uint32_t oldCount = m_nRequestsCount;
	DJSON_UINT(m_nRequestsCount, responseData, "new");
	if (oldCount != m_nRequestsCount)
	{
		m_bRequestsSynced = false;
		postNotification(D_EVENT_FRIENDS_NEW_REQUEST);
	}
}

friends::ChapterInfo *FriendsModel::getChapterByID(uint32_t cid)
{
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pChapters, pObj)
	{
		ChapterInfo *pChapter = static_cast<ChapterInfo *>(pObj);
		if (pChapter->m_nID == cid)
		{
			return pChapter;
		}
	}
	return NULL;
}

void FriendsModel::friendGiftFull(const Json::Value &requestData)
{
	uint32_t friendID = requestData["friendId"].asUInt();
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriends, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (pInfo->m_nFriendID == friendID)
		{
			pInfo->m_nPresentTs = GameManager::sharedGameManager()->getServerTime();
			pInfo->postNotification(D_EVENT_FRIENDS_STATUS_REFRESH);
		}
	}
}

void FriendsModel::msgSent()
{
	m_nMsgLastSent = GameManager::sharedGameManager()->getServerTime();
}

uint32_t FriendsModel::getMsgCD()
{
	uint32_t cd = D_CONFIG_UINT_FOR_KEY(FRIENDS_MESSAGE_CD);
	uint32_t now = GameManager::sharedGameManager()->getServerTime();
	if (m_nMsgLastSent + cd < now)
	{
		return 0;
	}
	else
	{
		return m_nMsgLastSent + cd - now;
	}
}

const std::string FriendsModel::getBattleName()
{
	if (!m_bInTeam)
	{
		return "";
	}
	ChapterInfo *pChapter = m_pSelectedChapter != NULL ? m_pSelectedChapter : m_pLastChapter;
	uint32_t progress = m_pSelectedChapter != NULL ? m_pDungeon->m_nOngoingProgress - 1 : m_pLastChapter->m_pBattleList->count() - 1;
	friends::BattleInfo *pBattle = static_cast<friends::BattleInfo *>(pChapter->m_pBattleList->objectAtIndex(progress));
	warrior::WarriorBaseInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(pBattle->getLeaderID());
	if (pWarrior)
	{
		return pWarrior->getWarriorName();
	}
	
	return "";
}