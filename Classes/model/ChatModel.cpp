//
//  ChatModel.cpp
//  聊天信息
//
//  Created by mzp on 14-4-23.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "ChatModel.h"
#include "../network/HttpClient.h"
#include "managers/GameManager.h"
#include "GuildModel.h"
#include "managers/DataManager.h"
#include "utils/TimerUtil.h"
#include "json/json.h"
#include "utils/StringUtil.h"

USING_NS_CC;
using namespace chat;

ChatModel::ChatModel()
	: m_pChatInfoMap(NULL)
	, m_pChatInfoBufferMap(NULL)
	, m_nLastTime(0)
	, m_nGuildLastId(0)
	, m_nWorldLastId(0)
	, m_bGuildRoll(true)
	, m_bWorldRoll(true)
	, m_bGuildGetting(false)
	, m_bWorldGetting(false)
	, m_nGuildID(0)
	, m_bCheckNew(false)
	, m_kChatCheckType(chat::kChatCheckTypeAll)
	,m_pChatFormatDic(NULL)
{

}

ChatModel::~ChatModel()
{
	CC_SAFE_RELEASE(m_pChatInfoMap);
	CC_SAFE_RELEASE(m_pChatInfoBufferMap);
	CC_SAFE_RELEASE_NULL(m_pChatFormatDic);
}

bool ChatModel::init()
{
	m_pChatInfoMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pChatInfoMap);
	m_pChatFormatDic = CCDictionary::create();
	CC_SAFE_RETAIN(m_pChatFormatDic);
	loadChatFormatData(CSVDecoder::create(CHAT_FORMAT_CONFIG_PATH)->getData());
	unsigned int chat_buffer_num = D_CONFIG_UINT_FOR_KEY(CHAT_BUFFER_NUM);
	for (unsigned int i = 0; i < chat::kChatChannelCount; ++i)
	{
		CCArray* pChannelInfo = CCArray::createWithCapacity(chat_buffer_num);
		m_pChatInfoMap->setObject(pChannelInfo, (int)i);
	}

	m_pChatInfoBufferMap = CCDictionary::create();
	CC_SAFE_RETAIN(m_pChatInfoBufferMap);
	for (unsigned int i = 0; i < chat::kChatChannelCount; ++i)
	{
		CCArray* pChannelInfo = CCArray::createWithCapacity(chat_buffer_num);
		m_pChatInfoBufferMap->setObject(pChannelInfo, (int)i);
	}
	
	return Model::init();
}

void ChatModel::loadChatFormatData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		chat::ChatFormat *pFormat = ChatFormat::create();
		pFormat->chat_type = (chat::ChatType)atoi(val[i][0].c_str());
		pFormat->format_title = val[i][2].c_str();
		pFormat->format_ct = val[i][3].c_str();
		replace(pFormat->format_ct, "\\n", "\n");
		//replace(pFormat->format_ct, "%", "%%");
		m_pChatFormatDic->setObject(pFormat,pFormat->chat_type);
	}
}

chat::ChatFormat* ChatModel::getChatFormatByType(chat::ChatType kType)
{
	return static_cast<chat::ChatFormat*>(m_pChatFormatDic->objectForKey(kType));
}

void ChatModel::openCheckNew()
{
	m_bCheckNew = true;
	// 初始化帮会信息 帮会开通后开启
	BINDING_ONLY(DM_GET_GUILD_MODEL, this, ChatModel::updateGuildInfo, D_EVENT_GUILD_GET);
	DM_GET_GUILD_MODEL->syncFromServer();
}

void ChatModel::closeCheckNew()
{
	m_bCheckNew = false;
	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::checkNew), this);
}

bool ChatModel::syncGuild(bool roll)
{
	// 防止同时请求两次相同的信息
	if (m_bGuildGetting)
	{
		return false;
	}

	guild::GuildUser* userInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (!userInfo)
	{
		return false;
	}

	uint32_t gid = userInfo->getGuildID();

	if (m_nGuildID != gid)
	{
		m_nGuildID = gid;
		m_nGuildLastId = 0;
		CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelGuild);
		if (pChannelInfo)
		{
			pChannelInfo->removeAllObjects();
			postNotification(D_EVENT_CHAT_GUILD_UPDATE);
		}
	}

	if (gid == 0)
	{
		Tips_Alert(D_LOCAL_STRING("GuildErrorNoGuild"));
		return false;
	}
	
	m_bGuildRoll = roll;
	m_bGuildGetting = true;
	HTTP_CLIENT->chatGetGuild(gid, m_nGuildLastId + 1, this, callfuncJson_selector(ChatModel::onChatGetGuild));

	return true;
}

bool ChatModel::syncWorld(bool roll)
{
	// 防止同时请求两次相同的信息
	if (m_bWorldGetting)
	{
		return false;
	}

	m_bWorldRoll = roll;
	m_bWorldGetting = true;
	HTTP_CLIENT->chatGetWorld(m_nWorldLastId + 1, this, callfuncJson_selector(ChatModel::onChatGetWorld));

	return true;
}

bool ChatModel::sendGuildMsg(const char* msg)
{
	guild::GuildUser* userInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (!userInfo)
	{
		return false;
	}

	uint32_t gid = userInfo->getGuildID();
	if (gid == 0)
	{
		Tips_Alert(D_LOCAL_STRING("GuildErrorNoGuild"));
		return false;
	}

	// 消息不能为空
	if (strcmp(msg, "") == 0)
	{
		Tips_Alert(D_LOCAL_STRING("ChatSendEmpty"));
		return false;
	}

	// 聊天间隔时间5秒
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	if ((now - m_nLastTime) <= D_CONFIG_UINT_FOR_KEY(CHAT_PUSH_INTERVAL_TIME))
	{
		Tips_Alert(D_LOCAL_STRING("ChatIntervalTime"));
		return false;
	}

	HTTP_CLIENT->chatSendGuild(msg);

	return true;
}

bool ChatModel::sendWorldMsg(const char* msg)
{
	// 检测聊天消耗道具
	if (DM_GET_ITEM_MODEL->getItemNum(ITEM_CHAT_ID) == 0)
	{
		// 提示
		//Tips_Alert(D_LOCAL_STRING("ChatItemNotEnough"));
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS, CCInteger::create(ITEM_CHAT_ID));
		return false;
	}

	// 消息不能为空
	if (strcmp(msg, "") == 0)
	{
		Tips_Alert(D_LOCAL_STRING("ChatSendEmpty"));
		return false;
	}

	// 聊天间隔时间5秒
	unsigned int now = GameManager::sharedGameManager()->getServerTime();
	if ((now - m_nLastTime) <= D_CONFIG_UINT_FOR_KEY(CHAT_PUSH_INTERVAL_TIME))
	{
		Tips_Alert(D_LOCAL_STRING("ChatIntervalTime"));
		return false;
	}

	HTTP_CLIENT->chatSendWorld(msg);

	return true;
}

void ChatModel::checkNew(float dt)
{
	HTTP_CLIENT->chatCheckNew(m_nWorldLastId + 1, m_nGuildLastId + 1, m_nGuildID, this, callfuncJson_selector(ChatModel::onChatCheckNew));
}

cocos2d::CCArray* ChatModel::getChatInfoByChannel(chat::ChatChannel channel)
{
	if (!m_pChatInfoMap)
	{
		return NULL;
	}
	
	return (CCArray*)m_pChatInfoMap->objectForKey((int)channel);
}

void ChatModel::parseSendGuild(const Json::Value& requestData,const Json::Value& responseData)
{
	m_nLastTime = GameManager::sharedGameManager()->getServerTime();

	syncGuild(true);

	postNotification(D_EVENT_CHAT_GUILD_SEND_SUC);
}

std::string getCardWarriorName(Json::Value warriors)
{
	std::string warriorName ="";
	for(uint32_t i = 0 ; i < warriors.size() ; i++)
	{
		uint32_t nWarriorID = warriors[i].asUInt();
		warrior::WarriorBaseInfo *pWarriorCurInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(nWarriorID);
		if(pWarriorCurInfo == NULL) continue;
		if(i == warriors.size()-1)
		{
			//warriorName +=pWarriorCurInfo->getBaseInfo()->getWarriorName();
			warriorName.append(pWarriorCurInfo->getWarriorName().c_str());
		}
		else
		{
			//warriorName +=pWarriorCurInfo->getBaseInfo()->getWarriorName()+",";
			warriorName.append(pWarriorCurInfo->getWarriorName().c_str());
			warriorName.append(",");
		}
	}

	return warriorName;
}

void ChatModel::parseGuildInfo(const Json::Value& requestData,const Json::Value& responseData)
{
	m_bGuildGetting = false;

	// 消息进来加入缓存
	CCArray* pChannelInfoBuffer = (CCArray*)m_pChatInfoBufferMap->objectForKey((int)chat::kChatChannelGuild);
	if (!pChannelInfoBuffer)
	{
		return;
	}
	
	const Json::Value& chatMsg = responseData["chatMsg"];
	if (chatMsg.empty())
	{
		return;
	}
	
	for (Json::Value::iterator iter(chatMsg.begin()); iter != chatMsg.end(); ++iter)
	{
		chat::ChatInfo* pInfo = chat::ChatInfo::create();
		DJSON_UINT(pInfo->uid, (*iter), "uid");
		DJSON_UINT(pInfo->id, (*iter), "id");
		DJSON_STRING(pInfo->name, (*iter), "name");
		DJSON_UINT(pInfo->ts, (*iter), "ts");
		if(pInfo->uid !=0)
		{
			DJSON_STRING(pInfo->msg, (*iter), "msg");
		}
		else
		{
			int type = 0;
			const Json::Value& msgObj = (*iter)["msg"];
			DJSON_UINT(type,msgObj,"id");
			ChatType curType = static_cast<ChatType>(type);
			ChatFormat* pFormat = getChatFormatByType(curType);
			pInfo->name = pFormat->format_title;
			pInfo->msg = pFormat->format_ct;
			std::string name ="";
			std::string guildName ="";
			Json::Value warriors = Json::arrayValue;
			switch(curType)
			{
			case kChatTypeDrawCardOne:
			case kChatTypeDrawCardTen:
				DJSON_STRING(name,msgObj,"name");
				warriors = msgObj["wids"];
				//replace(pInfo->msg, "%%", "%");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),getCardWarriorName(warriors).c_str())->getCString();
				break;
			case kChatTypeCreateGuild:
				DJSON_STRING(name,msgObj,"name");
				DJSON_STRING(guildName,msgObj,"guildName");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),guildName.c_str())->getCString();
				break;
			case kChatTypeChallengeBoss:
				DJSON_STRING(name,msgObj,"name");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str())->getCString();
				break;
			case kChatTypeTower:
				DJSON_STRING(name,msgObj,"name");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str())->getCString();
				break;
			case kChatTypeDragonValleyPur:
			case kChatTypeDragonValleyOra:
				DJSON_STRING(name,msgObj,"name");
				DJSON_STRING(guildName,msgObj,"guildName");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),guildName.c_str())->getCString();
				break;
			}		
		}

		m_nGuildLastId = pInfo->id;
		pChannelInfoBuffer->addObject(pInfo);
	}

	if (m_bGuildRoll)
	{
		// 滚动显示消息
		updateGuildMsg(0.0f);

		if (pChannelInfoBuffer->count() > 0)
		{
			TimerUtil::sharedTimerUtil()->schedule(schedule_selector(ChatModel::updateGuildMsg), this, D_CONFIG_FLOAT_FOR_KEY(CHAT_SHOW_INTERVAL_TIME));
		}
	}
	else
	{
		CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelGuild);
		if (pChannelInfo)
		{
			pChannelInfo->addObjectsFromArray(pChannelInfoBuffer);
			pChannelInfoBuffer->removeAllObjects();
			filterGuildUser();
			checkNumber(chat::kChatChannelGuild);
			postNotification(D_EVENT_CHAT_GUILD_UPDATE);
		}
	}
}

void ChatModel::parseSendWorld(const Json::Value& requestData,const Json::Value& responseData)
{
	m_nLastTime = GameManager::sharedGameManager()->getServerTime();

	syncWorld(true);

	postNotification(D_EVENT_CHAT_WORLD_SEND_SUC);
}

void ChatModel::parseWorldInfo(const Json::Value& requestData,const Json::Value& responseData)
{
	m_bWorldGetting = false;

	// 消息进来加入缓存
	CCArray* pChannelInfoBuffer = (CCArray*)m_pChatInfoBufferMap->objectForKey((int)chat::kChatChannelWorld);
	if (!pChannelInfoBuffer)
	{
		return;
	}

	const Json::Value& chatMsg = responseData["chatMsg"];
	if (chatMsg.empty())
	{
		return;
	}

	for (Json::Value::iterator iter(chatMsg.begin()); iter != chatMsg.end(); ++iter)
	{
		chat::ChatInfo* pInfo = chat::ChatInfo::create();
		DJSON_UINT(pInfo->id, (*iter), "id");
		DJSON_UINT(pInfo->uid, (*iter), "uid");
		DJSON_STRING(pInfo->name, (*iter), "name");
		DJSON_UINT(pInfo->ts, (*iter), "ts");
		if(pInfo->uid !=0)
		{
			DJSON_STRING(pInfo->msg, (*iter), "msg");
		}
		else
		{
			int type = 0;
			const Json::Value& msgObj = (*iter)["msg"];
			DJSON_UINT(type,msgObj,"id");
			ChatType curType = static_cast<ChatType>(type);
			ChatFormat* pFormat = getChatFormatByType(curType);
			pInfo->name = pFormat->format_title;
			pInfo->msg = pFormat->format_ct;
			std::string name ="";
			std::string guildName ="";
			Json::Value warriors = Json::arrayValue;
			switch(curType)
			{
			case kChatTypeDrawCardOne:
			case kChatTypeDrawCardTen:
				DJSON_STRING(name,msgObj,"name");
				warriors = msgObj["wids"];
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),getCardWarriorName(warriors).c_str())->getCString();
				break;
			case kChatTypeCreateGuild:
				DJSON_STRING(name,msgObj,"name");
				DJSON_STRING(guildName,msgObj,"guildName");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),guildName.c_str())->getCString();
				break;
			case kChatTypeChallengeBoss:
				DJSON_STRING(name,msgObj,"name");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str())->getCString();
				break;
			case kChatTypeTower:
				DJSON_STRING(name,msgObj,"name");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str())->getCString();
				break;
			case kChatTypeDragonValleyPur:
			case kChatTypeDragonValleyOra:
				DJSON_STRING(name,msgObj,"name");
				DJSON_STRING(guildName,msgObj,"guildName");
				pInfo->msg = CCString::createWithFormat(pInfo->msg.c_str(),name.c_str(),guildName.c_str())->getCString();
				break;
			}		
		}

		m_nWorldLastId = pInfo->id;
		pChannelInfoBuffer->addObject(pInfo);
	}

	if (m_bWorldRoll)
	{
		// 滚动显示消息
		updateWorldMsg(0.0f);

		if (pChannelInfoBuffer->count() > 0)
		{
			TimerUtil::sharedTimerUtil()->schedule(schedule_selector(ChatModel::updateWorldMsg), this, D_CONFIG_FLOAT_FOR_KEY(CHAT_SHOW_INTERVAL_TIME));
		}
	}
	else
	{
		CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelWorld);
		if (pChannelInfo)
		{
			pChannelInfo->addObjectsFromArray(pChannelInfoBuffer);
			pChannelInfoBuffer->removeAllObjects();
			checkNumber(chat::kChatChannelWorld);
			postNotification(D_EVENT_CHAT_WORLD_UPDATE);
		}
	}
}

void ChatModel::onChatGetGuild(const Json::Value& requestData,const Json::Value& responseData)
{
	postNotification(D_EVENT_CHAT_GUILD_GET_SUC);
}

void ChatModel::onChatGetWorld(const Json::Value& requestData,const Json::Value& responseData)
{
	postNotification(D_EVENT_CHAT_WORLD_GET_SUC);
}

void ChatModel::onChatCheckNew(const Json::Value& requestData,const Json::Value& responseData)
{
	unsigned int type = responseData["data"]["chatNew"].asUInt();
	m_kChatCheckType = (chat::ChatCheckType)type;
	NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_CHAT_CHECK_NEW, CCInteger::create(type));
	// 如果没有新消息，继续检测
	if (m_bCheckNew && type == chat::kChatCheckTypeNone)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::checkNew), this);
		TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(ChatModel::checkNew), this, D_CONFIG_FLOAT_FOR_KEY(CHAT_CHECK_INTERVAL_TIME));
	}
}

void ChatModel::updateGuildMsg(float dt)
{
	CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelGuild);
	CCArray* pChannelInfoBuffer = (CCArray*)m_pChatInfoBufferMap->objectForKey((int)chat::kChatChannelGuild);
	if (!pChannelInfo || !pChannelInfoBuffer)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::updateGuildMsg), this);
		return;
	}

	if (pChannelInfoBuffer->count() > 0)
	{
		pChannelInfo->addObject(pChannelInfoBuffer->objectAtIndex(0));
		pChannelInfoBuffer->removeObjectAtIndex(0);

		filterGuildUser();
		checkNumber(chat::kChatChannelGuild);
		postNotification(D_EVENT_CHAT_GUILD_UPDATE);
	}

	if (pChannelInfoBuffer->count() == 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::updateGuildMsg), this);
	}
}

void ChatModel::updateWorldMsg(float dt)
{
	CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelWorld);
	CCArray* pChannelInfoBuffer = (CCArray*)m_pChatInfoBufferMap->objectForKey((int)chat::kChatChannelWorld);
	if (!pChannelInfo || !pChannelInfoBuffer)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::updateWorldMsg), this);
		return;
	}

	if (pChannelInfoBuffer->count() > 0)
	{
		pChannelInfo->addObject(pChannelInfoBuffer->objectAtIndex(0));
		pChannelInfoBuffer->removeObjectAtIndex(0);

		checkNumber(chat::kChatChannelWorld);
		postNotification(D_EVENT_CHAT_WORLD_UPDATE);
	}

	if (pChannelInfoBuffer->count() == 0)
	{
		TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::updateWorldMsg), this);
	}
}

void ChatModel::filterGuildUser()
{
	CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)chat::kChatChannelGuild);
	if (!pChannelInfo)
	{
		return;
	}

	CCArray* pRemove = CCArray::create();

	CCObject* obj = NULL;
	chat::ChatInfo* pChatInfo = NULL;
	CCARRAY_FOREACH(pChannelInfo, obj)
	{
		pChatInfo = (chat::ChatInfo*)obj;
		if (pChatInfo)
		{
			if (!DM_GET_GUILD_MODEL->getGuildUserBasicsByUserId(pChatInfo->getUid()))
			{
				pRemove->addObject(obj);
			}
		}
	}

	pChannelInfo->removeObjectsInArray(pRemove);
}

void ChatModel::checkNumber(chat::ChatChannel channel)
{
	CCArray* pChannelInfo = (CCArray*)m_pChatInfoMap->objectForKey((int)channel);
	if (!pChannelInfo)
	{
		return;
	}

	unsigned int chat_buffer_num = D_CONFIG_UINT_FOR_KEY(CHAT_BUFFER_NUM);
	while (pChannelInfo->count() > chat_buffer_num)
	{
		pChannelInfo->removeObjectAtIndex(0);
	}
}

void ChatModel::updateGuildInfo()
{
	UNBINDING_ALL(DM_GET_GUILD_MODEL, this);

	guild::GuildUser* userInfo = DM_GET_GUILD_MODEL->getSelfInfo();
	if (!userInfo)
	{
		return;
	}

	uint32_t gid = userInfo->getGuildID();

	if (m_nGuildID != gid)
	{
		m_nGuildID = gid;
		m_nGuildLastId = 0;
	}

	TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(ChatModel::checkNew), this);
	TimerUtil::sharedTimerUtil()->scheduleOnce(schedule_selector(ChatModel::checkNew), this, D_CONFIG_FLOAT_FOR_KEY(CHAT_CHECK_INTERVAL_TIME));
}
