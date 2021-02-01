//
//  MailModel.cpp
//  QSMY
//
//  Created by cll on 13-7-17.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "MailModel.h"
#include "../managers/GameManager.h"
#include "json/json.h"
#include "utils/StringUtil.h"
#include "utils/TimerUtil.h"

//#include "DropModel.h"
//#include "MatchModel.h"

USING_NS_CC;
using namespace match;
using namespace mail;
using namespace drop;

MailModel::MailModel()
: m_pMailFormatMap(NULL)
, m_iFriendNewNum(0)
, m_iSystemNewNum(0)
, m_pSysMailList(NULL)
, m_pUserMailList(NULL)
,m_pAllMailList(NULL)
, m_bHasAttachment(false)
,m_pMaillGain(NULL)
,m_pMaillEvent(NULL)
{
    
}

MailModel::~MailModel()
{
	CC_SAFE_RELEASE_NULL(m_pMailFormatMap);
	CC_SAFE_RELEASE_NULL(m_pSysMailList);
	CC_SAFE_RELEASE_NULL(m_pUserMailList);
	CC_SAFE_RELEASE_NULL(m_pAllMailList);
	CC_SAFE_RELEASE_NULL(m_pMaillGain);
	CC_SAFE_RELEASE_NULL(m_pMaillEvent);
    TimerUtil::sharedTimerUtil()->unscheduleSelector(schedule_selector(MailModel::getNewMails), this);
}

bool MailModel::init()
{
	m_pMailFormatMap = MailFormatMap::create();
	CC_SAFE_RETAIN(m_pMailFormatMap);
    
	m_pSysMailList = MailList::create();
	CC_SAFE_RETAIN(m_pSysMailList);
	m_pUserMailList = MailList::create();
	CC_SAFE_RETAIN(m_pUserMailList);
	m_pAllMailList = MailList::create();
	CC_SAFE_RETAIN(m_pAllMailList);
	m_pMaillGain = MailList::create();
	CC_SAFE_RETAIN(m_pMaillGain);
	m_pMaillEvent = MailList::create();
	CC_SAFE_RETAIN(m_pMaillEvent);

	loadMailFormatData(  CSVDecoder::create(MAIL_FORMAT_CONFIG_PATH)->getData());
    
    TimerUtil::sharedTimerUtil()->schedule(schedule_selector(MailModel::getNewMails), this,MAILLIST_REFRESH_INTERVAL);
    
    return Model::init();
}

void MailModel::loadMailFormatData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		Format *pFormat = Format::create();
		pFormat->mail_type = (MailType)atoi(val[i][0].c_str());
		
		pFormat->format_str = val[i][1].c_str();
        replace(pFormat->format_str, "\\n", "\n");
        replace(pFormat->format_str, "%", "%%");
		m_pMailFormatMap->setObject(pFormat, pFormat->mail_type);
        
	}
}


Format * MailModel::getMailFormatByType(MailType mType)
{
	return (Format*)m_pMailFormatMap->objectForKey(mType);
}


static std::string formatJson(const char* src0,const Json::Value& params)
{
    if(params.isNull())
    {
        return src0;
    }else if(params.isString())
    {
        if(params.asString().empty()) return src0; //空字符串为默认值
        std::string src(src0);
        std::string::size_type pos = src.find("%%",0);
        if(pos!=std::string::npos) src.replace(pos,strlen("%%"),"%");
        return CCString::createWithFormat(src.c_str(),params.asString().c_str())->getCString();
    }else if(params.isNumeric())
    {
        std::string src(src0);
        std::string::size_type pos = src.find("%%",0);
        if(pos!=std::string::npos) src.replace(pos,strlen("%%"),"%");
        return CCString::createWithFormat(src.c_str(),params.asInt())->getCString();
    }else if(params.isArray())
    {
        std::string ret(src0);
        for (uint32_t i(0); i<params.size(); ++i) {
            ret = formatJson(ret.c_str(),params[i]);
        }
        return ret;
    }
    else if(params.isObject())
    {
        Json::Value::Members members(params.getMemberNames());
        std::string ret(src0);
        for (Json::Value::Members::reverse_iterator it = members.rbegin(); it!=members.rend(); ++it) {
            const std::string &key =*it;
            ret = formatJson(ret.c_str(), params[key]);
        }
        return ret;
    }
    return src0;
}


//拼接邮件内容
void MailModel::formatMailContent(mail::MailInfo * pMail,const Json::Value& params)
{

	Format *pFormat = getMailFormatByType(pMail->mail_type);
	drop::Drop * pDrop = NULL;
	CCObject * obj = NULL;
    
	if(pFormat)
	{
        std::string content = pMail->content;
        pMail->content = pFormat->format_str;
        //from
        if(!pMail->send_user.empty())
        {
            std::string::size_type pos = pMail->content.find("%%",0);
            if(pos!=std::string::npos) pMail->content.replace(pos,strlen("%%"),"%");
            pMail->content = CCString::createWithFormat(pMail->content.c_str(),pMail->send_user.c_str())->getCString();
        }
                
        std::vector<std::string> award;
		//奖励内容
		CCARRAY_FOREACH(pMail->arAtt,obj)
		{
			pDrop = (drop::Drop *)obj;
			
            
			switch(pDrop->getType())
			{
                case drop::kDropTypeItem:
                case drop::kDropTypePkg:
                    award.push_back(CCString::createWithFormat("%s%s",pDrop->getItem()->getBaseInfo()->getItemName().c_str(),D_LOCAL_STRING("%dGetNum",pDrop->getItem()->getNum()).c_str())->getCString());
                    break;
                case drop::kDropTypeEquip:
                    award.push_back(CCString::createWithFormat("%s%s",pDrop->getEquip()->getBaseInfo()->getEquipName().c_str(),D_LOCAL_STRING("%dGetNum",1).c_str())->getCString());
                    break;
                case drop::kDropTypeWarrior:
                    award.push_back(CCString::createWithFormat("%s",pDrop->getWarrior()->getBaseInfo()->getWarriorName().c_str())->getCString());
                    break;
                case drop::kDropTypeSoul:
                    award.push_back(CCString::createWithFormat("%s%s",D_LOCAL_STRING("%sSoul",pDrop->getSoul()->getBaseInfo()->getWarriorName().c_str()).c_str(),D_LOCAL_STRING("%dGetNum",pDrop->getSoul()->getNum()).c_str())->getCString());
                    break;
                case drop::kDropTypeCoin:
                    award.push_back(CCString::createWithFormat("%d%s",pDrop->getCoin(),D_LOCAL_STRING("Coin").c_str())->getCString());
                    break;
                case drop::kDropTypePoint:
                    award.push_back(CCString::createWithFormat("%d%s",pDrop->getPoint(),D_LOCAL_STRING("Point").c_str())->getCString());
                    break;
                case drop::kDropTypeAvatar:
                    award.push_back(CCString::createWithFormat("%s%s",pDrop->getAvatarInfo()->getBaseInfo()->getName().c_str(),D_LOCAL_STRING("%dGetNum",1).c_str())->getCString());
                    break;
                case drop::kDropTypeNone:
                case drop::kDropTypePower:
                case drop::kDropTypeVip:
                default:
                    break;
			}
		}
        
		//邮件内容
        switch (pMail->mail_type) {
            case kMailTypeMatchBetAward:
            {
                std::string matchString = "";
                if (params["finalNum"].asUInt() > (uint32_t)match::k8)
                {
                    matchString = CCString::createWithFormat("%sVS%s(%s)",params["user1"].asString().c_str(),params["user2"].asString().c_str(), D_LOCAL_STRING("%dFinalNum%d",params["finalNum"].asUInt(),params["finalNum"].asUInt()/2).c_str())->getCString();
                }
                else if (params["finalNum"].asUInt() == (uint32_t)match::k8)
                {
                    matchString = CCString::createWithFormat("%sVS%s(%s)",params["user1"].asString().c_str(),params["user2"].asString().c_str(), D_LOCAL_STRING("QualFinal").c_str())->getCString();
                }
                else if (params["finalNum"].asUInt() == (uint32_t)match::k4)
                {
                    matchString = CCString::createWithFormat("%sVS%s(%s)",params["user1"].asString().c_str(),params["user2"].asString().c_str(), D_LOCAL_STRING("SemiFinal").c_str())->getCString();
                }
                else
                    matchString = CCString::createWithFormat("%sVS%s(%s)",params["user1"].asString().c_str(),params["user2"].asString().c_str(), D_LOCAL_STRING("Final").c_str())->getCString();
                
                replace(pMail->content, "%%", "%");
                pMail->content = CCString::createWithFormat(pMail->content.c_str(),matchString.c_str(),join(award,", ").c_str())->getCString();
                
                break;
            }
            //case kMailTypeTowerRankAward:
            //{
            //    replace(pMail->content, "%%", "%");  
            //    pMail->content = CCString::createWithFormat(pMail->content.c_str(),params["towerrank"].asUInt(),join(award,", ").c_str())->getCString();
            //    break;
            //}
            //case kMailTypePvp:
            //{
            //    replace(pMail->content, "%%", "%");
            //    pMail->content = CCString::createWithFormat(pMail->content.c_str(),params.asString().c_str(),join(award,", ").c_str())->getCString();
            //    break;
            //}
            case kMailTypeSoulHunter:
            {
                replace(pMail->content, "%%", "%");
                warrior::WarriorBaseInfo* pInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(params["wid"].asUInt());
                pMail->content = CCString::createWithFormat(pMail->content.c_str(),pInfo->getWarriorName().c_str(),params["attacker"].asString().c_str(),join(award,", ").c_str())->getCString();
                break;
            }
			case kMailTypeGuildPosition:
			{
				replace(pMail->content, "%%", "%");
				//CCLog("%s",pMail->content.c_str());
				std::string sPosName = DM_GET_GUILD_MODEL->getPositionByType(guild::GuildPosition(params["new_pos"].asUInt()));
				pMail->content = CCString::createWithFormat(pMail->content.c_str(),sPosName.c_str(),join(award,", ").c_str())->getCString();
				break;
			}
			case kMailTypeFromUser:
			{
				pMail->sender_id = params["send_id"].asUInt();

				if(!content.empty())
				{
					std::string::size_type pos = pMail->content.find("%%",0);
					if(pos!=std::string::npos) pMail->content.replace(pos,strlen("%%"),"%");
					pMail->content = CCString::createWithFormat(pMail->content.c_str(),content.c_str())->getCString();
				}
				replace(pMail->content, "%%", "%");
				pMail->content = CCString::createWithFormat(pMail->content.c_str(),join(award,", ").c_str())->getCString();
				break;
			}
			case kMailTypeTeam:
			{
				replace(pMail->content, "%%", "%");
				uint32_t dungeonId = params["dungeonId"].asUInt();
				friends::ChapterInfo *pInfo = DM_GET_FRIENDS_MODEL->getChapterByID(dungeonId);
				std::string sName = "";
				if (pInfo != NULL)
				{
					sName = pInfo->getName();
				}
				
				pMail->content = CCString::createWithFormat(pMail->content.c_str(), sName.c_str(),join(award,", ").c_str())->getCString();
				break;
			}
			case kMailTypeRechargeGain:
			{
				replace(pMail->content, "%%", "%");
				if(params.isMember("lv"))
				{
					uint32_t lv = params["lv"].asUInt();

					pMail->content = CCString::createWithFormat(pMail->content.c_str(),lv)->getCString();
				}
				break;
			}
			case kMailTypeLimiteHero:
			{
				replace(pMail->content, "%%", "%");
				if(params.isMember("rank"))
				{
					uint32_t rank = params["rank"].asUInt();

					pMail->content = CCString::createWithFormat(pMail->content.c_str(),rank)->getCString();
				}
				
				break;
			}
            case kMailTypeWonderBoss:
            {
                replace(pMail->content, "%%", "%");
                if (params.isMember("name") && params.isMember("bossId") && params.isMember("rank"))
                {
                    warrior::WarriorBaseInfo* pInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(params["bossId"].asUInt());
                    pMail->content = CCString::createWithFormat(pMail->content.c_str(), params["name"].asCString(), pInfo->getWarriorName().c_str(), params["rank"].asUInt())->getCString();
                }
                break;
            }
            case kMailTypeWonderRank:
            {
                replace(pMail->content, "%%", "%");
                if(params.isMember("rank"))
				{
					uint32_t rank = params["rank"].asUInt();
                    
					pMail->content = CCString::createWithFormat(pMail->content.c_str(),rank)->getCString();
				}
                break;
            }
			case kMailTypeTotalRechargeAward:
			{
				replace(pMail->content, "%%", "%");
				break;
			}
            default:
                pMail->content = formatJson(pMail->content.c_str(),params);
                
                
                if(!content.empty())
                {
                    std::string::size_type pos = pMail->content.find("%%",0);
                    if(pos!=std::string::npos) pMail->content.replace(pos,strlen("%%"),"%");
                    pMail->content = CCString::createWithFormat(pMail->content.c_str(),content.c_str())->getCString();
                }
                
                replace(pMail->content, "%%", "%");
                pMail->content = CCString::createWithFormat(pMail->content.c_str(),join(award,", ").c_str())->getCString();
                break;
        }
	}
}


MailInfo* MailModel::getMailByID(uint32_t id)
{
    
	CCObject *obj = NULL;
	MailInfo * pMail = NULL;
    
	CCArray *pList = CCArray::create();
	//pList->addObjectsFromArray(m_pUserMailList);
	pList->addObjectsFromArray(m_pSysMailList);

	CCARRAY_FOREACH(pList, obj)
	{
		pMail = (MailInfo*) obj;
        
		if(pMail->id == id)
		{
			return pMail;
		}
	}
    
	return NULL;
}


void MailModel::parseMails(const Json::Value &val)
{
	m_pUserMailList->removeAllObjects();
	m_pSysMailList->removeAllObjects();
	m_pAllMailList->removeAllObjects();
	m_pMaillGain->removeAllObjects();
	m_pMaillEvent->removeAllObjects();
	m_bHasAttachment = false;
	const Json::Value &normalMail = val["mails"];
	const Json::Value &friendMail = val["friendMails"];
	for (uint32_t i = 0; i<normalMail.size(); i++)
	{
		MailInfo *pMail = parseMail(normalMail[i]);
		if (pMail->checkHasAtt() && !pMail->checkAttAccepted())
		{
			m_bHasAttachment = true;
		}

		pMail->retain();
		addMailInorder(pMail, m_pSysMailList);
		addMailInorder(pMail, m_pAllMailList);
	}

	for (uint32_t i = 0; i<friendMail.size(); i++)
	{
		MailInfo *pMail = parseMail(friendMail[i]);
		pMail->mail_type = kMailTypeFromFriend;

		pMail->retain();
		addMailInorder(pMail, m_pUserMailList);
		addMailInorder(pMail, m_pAllMailList);
	}
	
	for(uint32_t i=0;i<m_pAllMailList->count();i++)
	{
		MailInfo *pMail = static_cast<MailInfo*>(m_pAllMailList->objectAtIndex(i));
		if(pMail->checkHasAtt())
		{
			addMailInorder(pMail, m_pMaillGain);
		}
		else
		{
			addMailInorder(pMail, m_pMaillEvent);
		}
	}

	isSynced = true;
    
	postNotification(D_EVENT_MAILLIST_UPDATE);
}

MailInfo* MailModel::parseMail(const Json::Value &val, MailInfo *pMail)
{
	if(pMail == NULL)
	{
		pMail = MailInfo::create();
	}
    
	uint8_t mail_type = 0;
	DJSON_UINT(pMail->id,val,"id");
    DJSON_UINT(mail_type,val,"mail_type");
	pMail->mail_type = (mail::MailType)mail_type;
	DJSON_CSTRING(pMail->title,val,"title");
	DJSON_CSTRING(pMail->content,val,"content");
	DJSON_BOOL(pMail->att_flag,val,"att_flag");
	DJSON_BOOL(pMail->is_read,val,"is_read");
	DJSON_CSTRING(pMail->send_user,val,"send_user");
	DJSON_UINT(pMail->sender_id, val, "send_id");
	DJSON_UINT(pMail->mail_ts, val, "mail_ts");
    
    for (uint32_t i=0; i<val["arAtt"].size();i++)
	{
		Drop* pDrop = DM_GET_DROP_MODEL->parseDrop(val["arAtt"][i]);
		if(pDrop) pMail->arAtt->addObject(pDrop);
	}
    
	formatMailContent(pMail,val["params"]);
    
	return pMail;
}


void MailModel::addMailInorder(MailInfo * pMail, MailList * pList)
{
	CCAssert(pList != NULL, "Mail list error");
    MailInfo * pMailInfo = NULL;
    uint32_t i = 0;
    
    for (; i<pList->count(); ++i)
    {
        
        pMailInfo = (MailInfo *)pList->objectAtIndex(i);
        
		//先按有无附件排序，再按时间先后排序
        if (!pMail->checkAttAccepted() && pMailInfo->checkAttAccepted())
        {
            break;
        }
		if (pMail->checkAttAccepted() == pMailInfo->checkAttAccepted() && pMail->id > pMailInfo->id)
		{
			break;
		}
    }
    
    pList->insertObject(pMail,i);
}

void MailModel::setAllMailsReaded()
{
    cocos2d::CCObject * obj;
    mail::MailInfo * pMailInfo;
    
    uint32_t old_newMailCount = m_iSystemNewNum + m_inormalAttNum;
    
    CCARRAY_FOREACH(m_pSysMailList,obj)
    {
        pMailInfo = (mail::MailInfo *)obj;
        bool isNew = pMailInfo->isNew();
        pMailInfo->is_read = true;
        pMailInfo->postNotification(D_EVENT_MAIL_UPDATE);
		
		/*if(isNew && pMailInfo->checkHasAtt() && m_inormalAttNum>0)
		{
			--m_inormalAttNum;
		}*/
        if(isNew && !pMailInfo->checkHasAtt() && m_iSystemNewNum > 0)
        {
            --m_iSystemNewNum;
        }
    }
    
    if(old_newMailCount != m_iSystemNewNum + m_inormalAttNum)
    {
        postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
    }
};

void MailModel::setAllFriendMailsReaded()
{
	cocos2d::CCObject * obj;
	mail::MailInfo * pMailInfo;

	uint32_t old_newMailCount = m_iFriendNewNum;

	CCARRAY_FOREACH(m_pUserMailList,obj)
	{
		pMailInfo = (mail::MailInfo *)obj;
		bool isNew = pMailInfo->isNew();
		pMailInfo->is_read = true;
		pMailInfo->postNotification(D_EVENT_MAIL_UPDATE);
		if(!pMailInfo->isNew() && isNew && m_iFriendNewNum > 0)
		{
			--m_iFriendNewNum;
		}
	}

	if(old_newMailCount != m_iFriendNewNum)
	{
		postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
	}
};

//设置附件已接收
void MailModel::setMailAttAccepted(mail::MailInfo* pMail)
{
    bool isNew = pMail->isNew();
    pMail->att_flag = false;	//可理解为已领取附件
    pMail->is_read = true;		//标识为已读
    pMail->postNotification(D_EVENT_MAIL_UPDATE);
    if(!pMail->isNew() && isNew && pMail->checkHasAtt())
    {
        --m_inormalAttNum;
        postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
    }
    
};
//一键领取 重新获取附件邮件列表
void MailModel::setAllMailAttAccepted(const Json::Value &val)
{
	//CCObject *obj = NULL;
	//MailInfo * pMail = NULL;
	//CCArray *pList = CCArray::create();
	//pList->addObjectsFromArray(m_pUserMailList);
	//pList->addObjectsFromArray(m_pSysMailList);
	//CCARRAY_FOREACH(pList, obj)
	//{
	//	pMail = (MailInfo*) obj;
	//	if(pMail->att_flag)
	//	{
	//		//如果有附件 设置为已读，就代表领取过了
	//		pMail->is_read = true;
	//		pMail->att_flag = false;
	//		//--m_iSystemNewNum;
	//		--m_inormalAttNum;
	//		//发送通知
	//		pMail->postNotification(D_EVENT_MAIL_UPDATE);
	//	}
	//}

	m_pSysMailList->removeAllObjects();
	m_pAllMailList->removeAllObjects();
	m_pMaillGain->removeAllObjects();
	m_pMaillEvent->removeAllObjects();
	m_bHasAttachment = false;
	const Json::Value &normalMail = val["mails"];
	for (uint32_t i = 0; i<normalMail.size(); i++)
	{
		MailInfo *pMail = parseMail(normalMail[i]);
		if (pMail->checkHasAtt() && !pMail->checkAttAccepted())
		{
			m_bHasAttachment = true;
		}

		pMail->retain();
		addMailInorder(pMail, m_pSysMailList);
		addMailInorder(pMail, m_pAllMailList);
	}

	if(val.isMember("friendMails"))
	{
		const Json::Value &friendMail = val["friendMails"];
		m_pUserMailList->removeAllObjects();

		for (uint32_t i = 0; i<friendMail.size(); i++)
		{
			MailInfo *pMail = parseMail(friendMail[i]);
			pMail->mail_type = kMailTypeFromFriend;

			pMail->retain();
			addMailInorder(pMail, m_pUserMailList);
			addMailInorder(pMail, m_pAllMailList);
		}
	}

	for(uint32_t i=0;i<m_pAllMailList->count();i++)
	{
		MailInfo *pMail = static_cast<MailInfo*>(m_pAllMailList->objectAtIndex(i));
		if(pMail->checkHasAtt())
		{
			addMailInorder(pMail, m_pMaillGain);
		}
		else
		{
			addMailInorder(pMail, m_pMaillEvent);
		}
	}

	m_inormalAttNum = 0;
	postNotification(D_EVENT_MAILLIST_UPDATE);
	postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
}

//确认竞技场积分邮件（不使用）
void MailModel::readSystemMail(MailInfo* pMailInfo)
{
	//更新本邮件的状态
	pMailInfo->is_read = true;
	pMailInfo->postNotification(D_EVENT_MAIL_UPDATE);
	//更新邮件系统数量
	--m_iSystemNewNum;
	if(m_iSystemNewNum < 0) m_iSystemNewNum = 0;
	postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
}

void MailModel::setNewMailsNum(uint32_t sys, uint32_t friendNum,uint32_t normalAtt)
{
	if(m_iSystemNewNum == sys && m_iFriendNewNum == friendNum && m_inormalAttNum == normalAtt) return;
	m_iSystemNewNum = sys;
	m_iFriendNewNum = friendNum;
	m_inormalAttNum = normalAtt;
	isSynced = false;
//    postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
    postNotification(D_EVENT_MAIL_NEW_COUNT_SYNC);
}


void MailModel::getNewMails(float dt)
{
    HTTP_CLIENT->getNewMails(NULL,NULL,true,false); //悄悄的
}

void MailModel::mailDeleted(const Json::Value &val)
{
	const Json::Value &normalMail = val["mailId"];
	uint32_t mailId = 0;
	int idx = 0;

	for (int i=0; i<normalMail.size();i++)
	{
		mailId = normalMail[i].asUInt();
		MailInfo *pInfo = getMailByID(mailId);
		if(pInfo == NULL) return;
		/*if (pInfo->is_read == false && m_iSystemNewNum>0)
		{
			--m_iSystemNewNum;
		}*/
		//更新本邮件
		pInfo->is_read = true;
		pInfo->postNotification(D_EVENT_MAIL_UPDATE);
	}
	//此方法只对事件类邮件有效,成功返回后全部设为已读
	m_iSystemNewNum = 0;
	postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
}

void MailModel::friendMailDeleted(const Json::Value &val)
{
	uint32_t mailID = val["mailId"].asUInt();
	CCObject *pObj = NULL;
	int idx = 0;
	CCARRAY_FOREACH(m_pUserMailList, pObj)
	{
		MailInfo *pInfo = static_cast<MailInfo *>(pObj);
		if (pInfo->id == mailID)
		{
			m_pAllMailList->removeObject(pInfo);
			m_pUserMailList->removeObjectAtIndex(idx);
			if (pInfo->is_read == false && m_iFriendNewNum > 0)
			{
				--m_iFriendNewNum;
				postNotification(D_EVENT_MAIL_NEW_COUNT_UPDATE);
			}
			
			break;
		}
		idx++;
	}
	postNotification(D_EVENT_MAILLIST_UPDATE);
}

uint32_t MailModel::getNewMailsNum()
{
	return m_iFriendNewNum + m_iSystemNewNum + m_inormalAttNum;
}

void MailModel::deleteEventMail()
{
	CCObject* obj;
	CCArray* newMails = CCArray::create();
	MailInfo* pInfo(NULL);
	CCARRAY_FOREACH(m_pMaillEvent,obj)
	{
		pInfo = static_cast<MailInfo*>(obj);
		if(!pInfo->is_read)
		{
			newMails->addObject(pInfo);
		}
	}

	if(newMails->count()<=0) return;

	CCArray* pMails = CCArray::create();
	CCARRAY_FOREACH(newMails,obj)
	{
		pInfo = static_cast<MailInfo*>(obj);
		pMails->addObject(CCInteger::create(pInfo->id));
	}
	HTTP_CLIENT->deleteMail(pMails);
}