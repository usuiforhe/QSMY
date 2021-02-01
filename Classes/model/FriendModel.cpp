//
//  FriendModel.cpp
//  QSMY
//
//  Created by Eci on 14-4-9.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "FriendModel.h"
#include "network/HttpClient.h"
#include "managers/GameManager.h"
#include <algorithm>
//隔多久刷新一次好友列表	15分钟好了
#define FRIEND_LIST_UPDATE_TIME	900

USING_NS_CC;

static int sortFriendList(const CCObject* p1, const CCObject* p2)
{
	partner::FriendInfo* f1 = (partner::FriendInfo*)p1;
	partner::FriendInfo* f2 = (partner::FriendInfo*)p2;
	uint32_t i1 = f1->getOffTime();
	uint32_t i2 = f2->getOffTime();
	return i1 < i2;
};

namespace partner
{
	
	///////////////////////////////////////////FriendBasic

	FriendBasic::FriendBasic()
		: m_nFriendID(0)
		, m_sName()
		, m_nLevel(0)
		, m_nOffTime(0)
		, m_nFight(0)
		, m_bRequested(false)
		, m_bNewRequest(false)
	{
	}
	FriendBasic::~FriendBasic()
	{
	}
	void FriendBasic::parse(const Json::Value &val)
	{
		DJSON_UINT(m_nFriendID, val, "friend_id");
		DJSON_STRING(m_sName, val, "friend_name");
		DJSON_UINT(m_nLevel, val, "friend_lv");
		DJSON_UINT(m_nOffTime, val, "friend_off_time");
		DJSON_UINT(m_nFight, val, "friend_fight");
		DJSON_BOOL(m_bRequested, val, "requested");
	}

	///////////////////////////////////////////FriendInfo
	FriendInfo::FriendInfo()
		: m_nPresentTs(0)
		, m_nReceiveTs(0)
		, m_nGiftState(0)
		, m_nAssociateTs(0)
		, m_bDup(false)
		, m_bFate(false)
		, m_bRanout(false)
		, m_bNew(false)
	{
	}
	FriendInfo::~FriendInfo()
	{
	}
	void FriendInfo::parse(const Json::Value &val)
	{
		FriendBasic::parse(val);
		DJSON_UINT(m_nPresentTs, val, "friend_sgTs");
		DJSON_UINT(m_nReceiveTs, val, "friend_rgTs");
		DJSON_UINT(m_nAssociateTs, val, "friend_cTs");
		DJSON_UINT(m_nGiftState, val, "friend_gs");
	}

}


using namespace partner;


FriendModel::FriendModel()
	: m_pFriend(NULL)
	, m_pRecommend(NULL)
	, m_pRequest(NULL)
	, m_bRecommendSynced(false)
	, m_bFriendSynced(false)
	, m_uFriendSyncedTime(0)
	, m_uRecommendTimes(0)
{
    
}

FriendModel::~FriendModel()
{
	CC_SAFE_RELEASE(m_pFriend);
	CC_SAFE_RELEASE(m_pRecommend);
	CC_SAFE_RELEASE(m_pRequest);
}

bool FriendModel::init()
{
	m_pFriend = CCArray::create();
	CC_SAFE_RETAIN(m_pFriend);

	m_pRecommend = CCArray::create();
	CC_SAFE_RETAIN(m_pRecommend);

	m_pRequest = CCArray::create();
	CC_SAFE_RETAIN(m_pRequest);
	

    return Model::init();
}

void	FriendModel::onEnter()
{
	syncFriend();
	syncRecommend();
}





/************************************************************************/
/* http		请求                                                                    */
/************************************************************************/
//请求好友名单
void FriendModel::syncFriend()
{
	if (m_bFriendSynced && GameManager::sharedGameManager()->getServerTime() - m_uFriendSyncedTime < FRIEND_LIST_UPDATE_TIME)
	{
		//尚未过期
		postNotification(D_EVENT_FRIEND_LIST);
	}
	else
	{
		//从零开始
		HTTP_CLIENT->getFriends();
	}
}

//请求 推荐名单and申请名单
void FriendModel::syncRecommend()
{
	if (m_bRecommendSynced)
	{
		postNotification(D_EVENT_FRIEND_RECOMMEND);
	}
	else
	{
		//从零开始
		HTTP_CLIENT->recommendFriend(0);
	}
}

//申请好友
void FriendModel::doApply(unsigned int fid)
{
	HTTP_CLIENT->addFriend(fid , this,callfuncJson_selector(FriendModel::onApply));
}






/************************************************************************/
/* http		响应                                                                     */
/************************************************************************/

//赠送好友耐力响应
void FriendModel::onSendGift(const Json::Value &requestData,const Json::Value &responseData)
{
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_GIFTSEND).c_str());
	unsigned int fid = requestData["fid"].asUInt();

	FriendInfo *pInfo = NULL;	
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriend, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if(pInfo->getFriendID() == fid)
		{
			//更新时间 发送消息
			pInfo->m_nPresentTs = GameManager::sharedGameManager()->getServerTime();
			pInfo->postNotification(D_EVENT_FRIEND_INFO_REFRESH);
			break;
		}
	}
}

void FriendModel::onReceiveAndSendGiftAll()
{
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_GIFTSENDGET).c_str());
	uint32_t uNow = GameManager::sharedGameManager()->getServerTime();
	uint32_t uStart = TimeUtil::getStartTimeOfToday(uNow);
	//处理回赠的列表
	//CCArray* pGiftList = NULL;
	//getGiftList(pGiftList);
	CCArray* pGiftList = getGiftList(NULL);
	if(pGiftList)
	{
		CCObject *pObj = NULL;
		CCARRAY_FOREACH(pGiftList, pObj)
		{
			FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
			//更新时间和状态
			pInfo->m_nGiftState = 1;
			//if(pInfo->m_nReceiveTs < uStart) pInfo->m_nReceiveTs = uNow;
			if(pInfo->m_nPresentTs < uStart) pInfo->m_nPresentTs = uNow;
		}
	}
	postNotification(D_EVENT_FRIEND_GIFT_LIST);
}

//是否有新的好友请求
void FriendModel::requestsCountGot(const Json::Value &responseData)
{
	//更新申请数
	unsigned int count = 0;
	DJSON_UINT(count, responseData, "new");
	if(count > 0 && count != m_pRequest->count() && m_bRecommendSynced)
	{
		//有新的请求
		//重新请求一遍
		m_bRecommendSynced = false;
		syncRecommend();
	}

	//更新好友数
	unsigned int friendnum = 0;
	DJSON_UINT(friendnum, responseData, "friend");
	if(friendnum > m_pFriend->count() && m_bFriendSynced)
	{
		//有新的好友
		m_bFriendSynced = false;
		syncFriend();
	}
}


void FriendModel::removeRequest(uint32_t nFriendID)
{
	int idx = 0;
	FriendBasic *pBasic = NULL;
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pRequest, pObj)
	{
		pBasic = static_cast<FriendBasic *>(pObj);
		if (pBasic->m_nFriendID == nFriendID)
		{
			m_pRequest->removeObjectAtIndex(idx);
			break;
		}
		idx++;
	}
	//m_nRequestsCount--;
	//postNotification(D_EVENT_FRIENDS_NEW_REQUEST);
	postNotification(D_EVENT_FRIEND_RECOMMEND);
}

void FriendModel::addFriend(const Json::Value &val)
{
	FriendInfo *pAdd = FriendInfo::create();
	pAdd->parse(val);
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriend, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (pInfo->m_nFriendID == pAdd->m_nFriendID)
		{
			//新加的好友已经有了 退出
			return;
		}
	}
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_ADDSUCCESS).c_str());
	m_pFriend->addObject(pAdd);
	postNotification(D_EVENT_FRIEND_LIST);	

	//从推荐列表里删除 如果有的话
	CCObject *pObjRecommend = NULL;
	int idx = 0;
	CCARRAY_FOREACH(m_pRecommend, pObjRecommend)
	{
		FriendBasic *pInfo = static_cast<FriendBasic *>(pObjRecommend);
		if (pInfo->m_nFriendID == pAdd->m_nFriendID)
		{
			m_pRecommend->removeObjectAtIndex(idx);
			postNotification(D_EVENT_FRIEND_RECOMMEND);
			break;
		}
		idx++;
	}
}

void FriendModel::parseFriend(const Json::Value &val)
{
	if (!m_pFriend)
	{
		m_pFriend = CCArray::create();
		CC_SAFE_RETAIN(m_pFriend);
	}
	else
	{
		m_pFriend->removeAllObjects();
	}
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		Json::Value v = *i;
		FriendInfo *pInfo = FriendInfo::create();
		//FriendBasic *pInfo = FriendBasic::create();
		pInfo->parse(v);
		m_pFriend->addObject(pInfo);
	}

	//std::sort(m_pFriends->data->arr, m_pFriends->data->arr + m_pFriends->data->num, sortFriendByLevel);

	/*if (m_bFriendsSynced)
	{
		m_nFriendsLastSynced = GameManager::sharedGameManager()->getServerTime();
	}*/
	m_bFriendSynced = true;
	m_uFriendSyncedTime = GameManager::sharedGameManager()->getServerTime();
	postNotification(D_EVENT_FRIEND_LIST);
}

void FriendModel::parseRequest(const Json::Value &val)
{
	if (!m_pRequest)
	{
		m_pRequest = CCArray::create();
		CC_SAFE_RETAIN(m_pRequest);
	}
	//直接清空吧
	m_pRequest->removeAllObjects();
	//读取
	for (Json::ValueIterator i = val.begin(); i != val.end(); i++)
	{
		//解析
		Json::Value v = *i;
		FriendBasic *pInfo = FriendBasic::create();
		pInfo->parse(v);
		//新的好友申请，不是推荐
		pInfo->m_bNewRequest = true;

		m_pRequest->addObject(pInfo);		
	}
}

//处理推荐页面（推荐和申请）
void FriendModel::parseRecommend(const Json::Value &val)
{
	//处理申请
	parseRequest(val["friendRequests"]);

	//处理推荐
	if (!m_pRecommend)
	{
		m_pRecommend = CCArray::create();
		CC_SAFE_RETAIN(m_pRecommend);
	}

	//清除
	m_pRecommend->removeAllObjects();
	//推荐计数
	m_uRecommendTimes++;

	for (Json::ValueIterator i = val["recommends"].begin(); i != val["recommends"].end(); i++)
	{
		//解析
		Json::Value v = *i;
		FriendBasic *pInfo = FriendBasic::create();
		pInfo->parse(v);

		//去重 if needed
		//bool bExist = false;
		//CCObject *pObj = NULL;
		//CCARRAY_FOREACH(m_pRecommend, pObj)
		//{
		//	FriendBasic *pBasic = static_cast<FriendBasic *>(pObj);
		//	if (pBasic->m_nFriendID == pInfo->m_nFriendID)
		//	{
		//		bExist = true;
		//		break;
		//	}
		//}
		////未重复 添加
		//if(!bExist) m_pRecommend->addObject(pInfo);
		
		m_pRecommend->addObject(pInfo);
	}

	m_bRecommendSynced = true;
	postNotification(D_EVENT_FRIEND_RECOMMEND);
}

//好友申请后回调
void FriendModel::onApply(const Json::Value &requestData,const Json::Value &responseData)
{
	if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_APPLYSUCCESS).c_str());

	uint32_t fid = requestData["fid"].asUInt();
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pRecommend, pObj)
	{
		FriendBasic *pBasic = static_cast<FriendBasic *>(pObj);
		if (pBasic->m_nFriendID == fid)
		{
			pBasic->m_bRequested = true;
			pBasic->postNotification(D_EVENT_FRIEND_BASIC_REFRESH);
			break;
		}
	}
}

void FriendModel::onReceiveGift(const Json::Value &requestData)
{
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_GIFTGET).c_str());
	uint32_t friendID = requestData["fid"].asUInt();

	//DropModel *pModel = DM_GET_DROP_MODEL;
	//cocos2d::CCArray *pAwardList = pModel->getDropArray(requestData[UUID].asUInt());
	//CCObject *pDrop = NULL;
	//if(pAwardList != NULL && pAwardList->count() != 0)
	//{
	//	pDrop = pAwardList->objectAtIndex(0);
	//}

	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriend, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (friendID == pInfo->m_nFriendID)
		{
			//更新时间和状态
			pInfo->m_nGiftState = 1;
			//pInfo->m_nReceiveTs = GameManager::sharedGameManager()->getServerTime();
			postNotification(D_EVENT_FRIEND_GIFT_LIST);
			//pInfo->postNotification(D_EVENT_FRIENDS_STATUS_REFRESH, pDrop);
			break;
		}
	}
}

//删除好友
void FriendModel::removeFriends(const Json::Value &requestData)
{
	Tips_Alert(D_LOCAL_STRING(FRIEND_TXT_DELETESUCCESS).c_str());
	uint32_t friendID = requestData["fid"].asUInt();
	int idx = 0;
	CCObject *pObj = NULL;
	CCARRAY_FOREACH(m_pFriend, pObj)
	{
		FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
		if (friendID == pInfo->m_nFriendID)
		{
			m_pFriend->removeObjectAtIndex(idx);
			postNotification(D_EVENT_FRIEND_LIST);
			break;
		}
		idx++;
	}
}


void FriendModel::clearRequest()
{
	//清空申请
	m_pRequest->removeAllObjects();
	postNotification(D_EVENT_FRIEND_RECOMMEND);
}


/************************************************************************/
/* 返回数据                                                                     */
/************************************************************************/

CCArray *FriendModel::getFriendList(cocos2d::CCArray *pList)
{
	//返回好友列表
	if (!pList) pList = CCArray::create();
	else pList->removeAllObjects();
	if(m_pFriend) pList->addObjectsFromArray(m_pFriend);
	//排序
	std::sort(pList->data->arr, pList->data->arr + pList->data->num, sortFriendList);
	return pList;
}

CCArray *FriendModel::getRecommend(cocos2d::CCArray *pRecommend)
{
	if (!pRecommend)
	{
		pRecommend = CCArray::create();
	}
	else
	{
		pRecommend->removeAllObjects();
	}

	//加申请列表
	if(m_pRequest)
	{
		pRecommend->addObjectsFromArray(m_pRequest);
	}

	//加推荐列表
	if(m_pRecommend)
	{
		if(m_pFriend)
		{
			CCObject *pObj = NULL;
			CCARRAY_FOREACH(m_pRecommend, pObj)
			{
				FriendBasic *pBasic = static_cast<FriendBasic *>(pObj);
				FriendInfo *pFound = NULL;
				CCARRAY_FOREACH(m_pFriend, pObj)
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

CCArray *FriendModel::getGiftList(cocos2d::CCArray *pList)
{
	//返回好友列表
	if (!pList) pList = CCArray::create();
	else pList->removeAllObjects();

	if(m_pFriend)
	{
		CCObject *pObj = NULL;
		CCARRAY_FOREACH(m_pFriend, pObj)
		{
			FriendInfo *pInfo = static_cast<FriendInfo *>(pObj);
			GiftState state = getGiftState(pInfo);
			if (state == kGiftStateReceive)
			{
				pList->addObject(pInfo);
			}
		}
	}

	return pList;
}

/************************************************************************/
/* 其他相关逻辑                                                                      */
/************************************************************************/

partner::GiftState FriendModel::getGiftState(partner::FriendInfo *pInfo)
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
	//现在只是用这三种状态
	return kGiftStatePresent;
	//return kGiftStateFormation;
}

const char* FriendModel::getStateString(unsigned int offtime)
{
	unsigned int secOfDay = 86400;
	std::string pStateString;
	unsigned int stNow = GameManager::sharedGameManager()->getServerTime();
	unsigned int stToday = TimeUtil::getStartTimeOfToday(stNow);
	int delta = stToday - (stNow - offtime);

	if(offtime < 3600){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_ONLINE);
	}
	else if(delta < 0){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_TODAY);
	}
	else if(delta < secOfDay){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_ONEDAY);
	}
	else if(delta < secOfDay*2){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_TWODAY);
	}
	else if(delta < secOfDay*3){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_THREEDAY);
	}
	else if(delta < secOfDay*7){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_WEEK);
	}
	else if(delta < secOfDay*14){
		pStateString = D_LOCAL_STRING(FRIEND_STATE_TWOWEEK);
	}
	else{
		pStateString = D_LOCAL_STRING(FRIEND_STATE_LONG);
	}
	return CCString::create(pStateString)->getCString();
}