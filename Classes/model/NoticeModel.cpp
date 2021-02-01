//
//  NoticeModel.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "NoticeModel.h"
#include "LotteryModel.h"
#include "../managers/DataManager.h"
#include "../managers/GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace notice;

cocos2d::CCString * TipsInfo::getDialog()
{
    if(m_pDialogList->count() == 0)
    {
        return NULL;
    }
    
    return (cocos2d::CCString *)m_pDialogList->objectAtIndex(rand() % m_pDialogList->count());
}

void NoticeTemplateInfo::loadLine(const CSVDecoder::CSVLine &line)
{
    m_kNoticeType = (NoticeType)atoi(line[0].c_str());
    m_sTextTemplate = line[1].c_str();
	m_isGlobal = atoi(line[2].c_str());
}


static const char* getWarriorName(warrior::WarriorBaseInfo* pInfo)
{
    cocos2d::ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(pInfo->getWarriorId());
    return  CCString::createWithFormat("[color=FF%02x%02x%02x]\\[%s\\][/color]",color.r,color.g,color.b,pInfo->getWarriorName().c_str())->getCString();

}

static const char* getEquipName(equip::EquipBaseInfo * pInfo)
{
    cocos2d::ccColor3B color = DM_GET_EQUIP_MODEL->getEquipColorByGrade(pInfo->getEquipGrade());
    return  CCString::createWithFormat("[color=FF%02x%02x%02x]\\[%s\\][/color]",color.r,color.g,color.b,pInfo->getEquipName().c_str())->getCString();
}

static const char* getSkillname(skill::Skill *pInfo)
{
    cocos2d::ccColor3B color = DM_GET_SKILL_MODEL->getSkillColor(pInfo->grade);
    return  CCString::createWithFormat("[color=FF%02x%02x%02x]\\[%s\\][/color]",color.r,color.g,color.b,pInfo->getSkillName())->getCString();
}

cocos2d::CCString * NoticeTemplateInfo::formatContent(const char * playerName, const Json::Value &data)
{
    switch (m_kNoticeType) {
        case kNoticeSkillEnhance:
        {
            uint32_t skillId = data["sid"].asUInt();
            uint32_t lv = data["lv"].asUInt();
            
            skill::Skill * pSkill = DM_GET_SKILL_MODEL->getSkill(skillId);
            if(pSkill)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, getSkillname(pSkill), lv, NULL);
            }
            break;
        }            
        case kNoticeWarriorUp:
        {
            uint32_t wid = data["wid"].asUInt();
            uint32_t up = data["up"].asUInt();
            
            warrior::WarriorBaseInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
            if(pWarrior)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, getWarriorName(pWarrior), up, NULL);
            }
            
            break;
        }            
        case kNoticeEquipEnhance:
        {
            uint32_t eid = data["eid"].asUInt();
            uint32_t lv = data["lv"].asUInt();
            equip::EquipBaseInfo * pEquip = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(eid);
            if(pEquip)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, getEquipName(pEquip), lv, NULL);
            }
            
            break;
        }            
        case kNoticeOpenBox:
        {
            uint32_t eid = data["eid"].asUInt();
            uint32_t itemId = data["itemId"].asUInt();
            item::ItemBase * pItem = DM_GET_ITEM_MODEL->getItemBaseByID(itemId);
            equip::EquipBaseInfo * pEquip = DM_GET_EQUIP_MODEL->getEquipBaseInfoByID(eid);
            if(pItem && pEquip)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, pItem->getItemName().c_str(), getEquipName(pEquip), NULL);
            }
            break;
        }
        case kNoticeDrawCard:
        {
            uint32_t wid = data["wid"].asUInt();
            
            std::string from = D_LOCAL_STRING(CCString::createWithFormat("Card_%d",data["from"].asUInt())->getCString());
            
            warrior::WarriorBaseInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
            if(pWarrior)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, from.c_str(), getWarriorName(pWarrior), NULL);
            }
            break;
        }
        case kNoticeSignUp:
        {
            uint32_t times = data["times"].asUInt();
            const Json::Value & drop = data["drop"];
            const char * dropName = getDropName(drop);
            if(dropName != NULL)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, times, dropName, NULL);
            }
            break;
        }
        case kNoticeWarriorRecruit:
        {
            uint32_t wid = data.asUInt();
            warrior::WarriorBaseInfo * pWarrior = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(wid);
            if(pWarrior)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, getWarriorName(pWarrior), NULL);
            }
            break;
        }
        case kNoticeAchieveDestiny:
        {
            uint32_t id = data["id"].asUInt();
            //uint32_t lv = data["lv"].asUInt();
            
            destiny::DestinyInfo * pDestiny = DM_GET_DESTINY_MODEL->getDestinyInfoByID(id);
            if(pDestiny)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, pDestiny->getName().c_str(), NULL);
            }
            
            break;
        }
        case kNoticeGetPrestigeReward:
        {
            uint32_t cid = data["cid"].asUInt();
            const Json::Value & drop = data["drop"];
            
            const char * dropName = getDropName(drop);
            
            pve::ChapterInfo * pChapter = DM_GET_PVE_MODEL->getChapterInfo(cid);
            if(pChapter)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, pChapter->chapter_name.c_str(), dropName, NULL);
            }
            
            break;
        }
        case kNoticeMammon:
        {
            uint32_t num = data.asUInt();
            return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, num, NULL);
            break;
        }
        case kNoticeExchange:
        {
            const Json::Value & drop = data["drop"];
            uint32_t contr = data["contr"].asUInt();
            
            const char * dropName = getDropName(drop);
            
            if(dropName != NULL)
            {
                return CCString::createWithFormat(m_sTextTemplate.c_str(), playerName, dropName, contr, NULL);
            }
            
            break;
        }
        default:
            return CCString::create(m_sTextTemplate);
            break;
    }
    
    return NULL;
}

const char * NoticeTemplateInfo::getDropName(const Json::Value & drop)
{
    drop::Drop * pDrop = DM_GET_DROP_MODEL->parseDrop(drop);
    if(!pDrop)
    {
        return NULL;
    }
    switch(pDrop->getType())
    {            
        case drop::kDropTypeItem:
        case drop::kDropTypePkg:
            if(pDrop->getItem() && pDrop->getItem()->getBaseInfo())
            {
                return pDrop->getItem()->getBaseInfo()->getItemName().c_str();
            }
            break;
        case drop::kDropTypeEquip:
            if(pDrop->getEquip() && pDrop->getEquip()->getBaseInfo())
            {
                return getEquipName(pDrop->getEquip()->getBaseInfo());
            }
            break;
        case drop::kDropTypeWarrior:
            if(pDrop->getWarrior() && pDrop->getWarrior()->getBaseInfo())
            {
                return getWarriorName(pDrop->getWarrior()->getBaseInfo());
            }
            break;
        case drop::kDropTypeSoul:
            if(pDrop->getSoul() && pDrop->getSoul()->getBaseInfo())
            {
                return getWarriorName(pDrop->getSoul()->getBaseInfo());
            }
            break;
        case drop::kDropTypeCoin:
            return DM_GET_ITEM_MODEL->getItemBaseByID(COIN_ID)->getItemName().c_str();
            break;
        case drop::kDropTypePoint:
            return DM_GET_ITEM_MODEL->getItemBaseByID(POINT_ID)->getItemName().c_str();
            break;
        case drop::kDropTypeVip:
        {
            user::VipInfo * pVipInfo = DM_GET_USER_MODEL->getVipInfoByVipLevel(pDrop->getVip());
            if(pVipInfo)
            {
                item::ItemBase * pItem = DM_GET_ITEM_MODEL->getItemBaseByID(pVipInfo->getVipItemId());
                return pItem->getItemName().c_str();
            }
            break;
        }
        case drop::kDropTypePower:
            return DM_GET_ITEM_MODEL->getItemBaseByID(POWER_ID)->getItemName().c_str();
            break;
        case drop::kDropTypeAvatar:
            return pDrop->getAvatarInfo()->getBaseInfo()->getName().c_str();
            break;
        case drop::kDropTypeNone:
        default:
            return NULL;
    }
    
    return NULL;
}

void NoticeInfo::parse(NoticeModel * pNoticeModel, const Json::Value &data, bool exceptSelf)
{
    m_kNoticeType = (NoticeType)data["type"].asUInt();
    NoticeTemplateInfo * pTemplate = pNoticeModel->getTemplate(m_kNoticeType);
    if(pTemplate && pTemplate->getIsGlobal())
    {
        const char * playerName(NULL);
        DJSON_CSTRING(playerName, data, "name");
        
        if(exceptSelf && playerName == DM_GET_USER_MODEL->getUserInfo()->getUserName())
        {
            return;
        }
        
        CC_SAFE_RELEASE(m_pText);
        m_pText = pTemplate->formatContent(playerName, data["args"]);
        CC_SAFE_RETAIN(m_pText);
    }
}

NoticeModel::NoticeModel()
: Model()
, m_pTipsInfo(NULL)
, m_pNoticeTemplates(NULL)
, m_pNoticeMap(NULL)
, m_pSyncTimeMap(NULL)
{
    
}

NoticeModel::~NoticeModel()
{
    CC_SAFE_RELEASE(m_pTipsInfo);
    CC_SAFE_RELEASE(m_pNoticeTemplates);
    CC_SAFE_RELEASE(m_pNoticeMap);
    CC_SAFE_RELEASE(m_pSyncTimeMap);
}

bool NoticeModel::init()
{
    m_pTipsInfo = TipsInfo::create();
    m_pTipsInfo->retain();
    
    m_pNoticeTemplates = CCDictionary::create();
    m_pNoticeTemplates->retain();
    
    m_pNoticeMap = CCDictionary::create();
    m_pNoticeMap->retain();
    m_pSyncTimeMap = CCDictionary::create();
    m_pSyncTimeMap->retain();
    
    parseTipsInfoData( CSVDecoder::create(TIPS_INFO_CONFIG_PATH)->getData());
    parseNoticeTemplateData( CSVDecoder::create(NOTICE_TEMPLATE_CONFIG_PATH)->getData());
        
    return Model::init();
}

void NoticeModel::parseNotice(NoticeListType listType, const Json::Value &responseData)
{
    cocos2d::CCArray * pList = (cocos2d::CCArray *)m_pNoticeMap->objectForKey((int) listType);
    if(pList == NULL)
    {
        pList = CCArray::create();
        m_pNoticeMap->setObject(pList, (int)listType);
    }
    
    NoticeInfo * pNotice = NoticeInfo::create();
    pNotice->parse(this, responseData, false);
    if(pNotice->getText() != NULL)
    {
        pList->addObject(pNotice);
    }
}

void NoticeModel::parseNotices(NoticeListType listType, const Json::Value &responseData)
{    
    cocos2d::CCArray * pList = (cocos2d::CCArray *)m_pNoticeMap->objectForKey((int) listType);
    if(pList == NULL)
    {
        pList = CCArray::create();
        m_pNoticeMap->setObject(pList, (int)listType);
    }
    
    pList->removeAllObjects();
    for(Json::Value::iterator iter(responseData.begin()); iter != responseData.end(); ++iter)
    {
        NoticeInfo * pNotice = NoticeInfo::create();
        pNotice->parse(this, *iter,false);
        if(pNotice->getText() != NULL)
        {
            pList->addObject(pNotice);
        }
    }
}

notice::NoticeTemplateInfo * NoticeModel::getTemplate(notice::NoticeType type)
{
    return dynamic_cast<notice::NoticeTemplateInfo *>(m_pNoticeTemplates->objectForKey((int)type));
}

cocos2d::CCString * NoticeModel::getNextNoticeText(notice::NoticeListType listType)
{
    cocos2d::CCArray * pList = dynamic_cast<cocos2d::CCArray *>(m_pNoticeMap->objectForKey((int) listType));
    
    if(pList == NULL)
    {
        pList = CCArray::create();
        m_pNoticeMap->setObject(pList, (int)listType);
    }

    CCString * pText(NULL);
    if(pList->count() > 0)
    {
        NoticeInfo * pNotice = (NoticeInfo *)pList->lastObject();
        if(pNotice->getText() != NULL)
        {
            pText = CCString::create(pNotice->getText()->getCString());
        }
        pList->removeLastObject();
    }
    CCInteger * syncTime = dynamic_cast<CCInteger *>(m_pSyncTimeMap->objectForKey((int)listType));
    if(syncTime == NULL)
    {
        syncTime = CCInteger::create(0);
        m_pSyncTimeMap->setObject(syncTime, (int)listType);
    }
    if(pList->count() == 0 && syncTime->getValue() + D_CONFIG_UINT_FOR_KEY(NOTICE_MIN_SYNC_INTERVAL) < GameManager::sharedGameManager()->getServerTime())
    {
        HTTP_CLIENT->getNotices((uint32_t)listType, static_cast<uint32_t>(syncTime->getValue()));
        m_pSyncTimeMap->setObject(CCInteger::create(GameManager::sharedGameManager()->getServerTime()), (int)listType);
    }
    
    return pText;
}

void NoticeModel::parseTipsInfoData(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
        m_pTipsInfo->addDialog(cocos2d::CCString::create(val[i][1]));
	}
}

void NoticeModel::parseNoticeTemplateData(const CSVDecoder::CSV &val)
{
    CSVDecoder::CSV csv = val;
    m_pNoticeTemplates->removeAllObjects();
    for(CSVDecoder::CSV::iterator iter(csv.begin()); iter != csv.end(); ++iter)
    {
        NoticeTemplateInfo * pTemplate = NoticeTemplateInfo::create();
        pTemplate->loadLine(*iter);
        
        m_pNoticeTemplates->setObject(pTemplate, (int)(pTemplate->getNoticeType()));
    }
}
