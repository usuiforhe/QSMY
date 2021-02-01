//
//  AnnounceModel.cpp
//  QSMY
//
//  Created by 汪燕军 on 13-9-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "AnnounceModel.h"
#include "../utils/StringUtil.h"
#include "loaders/ImageLoader.h"
#include "../utils/TimeUtil.h"
#include "../model/UserModel.h"
#include "managers/GameManager.h"
#include "../utils/StringUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace announce;


#define IMG_SIZE CCSizeMake(512, 150);
#define NONE_IMG_SIZE CCSizeMake(512, 60);
#define TIME_DAY_SECOND	86400


#pragma mark - AnnounceInfo -

AnnounceInfo::AnnounceInfo()
: ValueObjcet()
, m_sTitle("")
, m_sContent("")
, m_nMinDays(0)
, m_nMaxDays(0)
, m_sImg("")
, m_sUrl("")
, m_kNavType(navigation::kNavNIL)
, m_nNavTag(0)
, m_bNetworkImg(false)
, m_nAnnounceType(0)
{
    
}

AnnounceInfo::~AnnounceInfo()
{
    
}


void AnnounceInfo::parse(const Json::Value &data)
{
    DJSON_STRING(m_sTitle, data, "title");
    DJSON_STRING(m_sContent, data, "content");
    DJSON_STRING(m_sImg, data, "img");
    uint32_t moduleId(0);
    DJSON_UINT(moduleId, data, "module");
    DJSON_STRING(m_sUrl, data, "url");
	DJSON_UINT(m_nAnnounceType,data,"type");
    if(moduleId > 0)
    {
        m_kNavType = (navigation::NavigationType)(moduleId / 100);
        m_nNavTag = moduleId % 100;
    }

	m_bNetworkImg = true;
}


bool AnnounceInfo::hasPic()
{
    return !m_sImg.empty();
}

cocos2d::CCNode* AnnounceInfo::getPic()
{
    if (hasPic())
    {
		if (m_bNetworkImg)
		{
			return ImageLoader::create(m_sImg.c_str());
		}
		else
		{
			return CCSprite::create(m_sImg.c_str());
		}
    }
    return CCNode::create();
}

cocos2d::CCSize AnnounceInfo::getPicSize()
{
    if(hasPic())
    {
        return IMG_SIZE;
    }
    return NONE_IMG_SIZE;
}


bool AnnounceInfo::isNeedNav()
{
   return m_kNavType != navigation::kNavNIL || !m_sUrl.empty();
}

#pragma mark - AnnounceModel -

AnnounceModel::AnnounceModel()
: Model()
, m_pAnnounceList(NULL)
, m_pAnnounceLocalList(NULL)
,m_pAnnounceNewList(NULL)
,isLoginShow(false)
{
    
}

AnnounceModel::~AnnounceModel()
{
    CC_SAFE_RELEASE(m_pAnnounceList);
	CC_SAFE_RELEASE(m_pAnnounceLocalList);
	CC_SAFE_RELEASE(m_pAnnounceNewList);
}

bool AnnounceModel::init()
{
    m_pAnnounceList = CCArray::create();
    m_pAnnounceList->retain();

	m_pAnnounceLocalList = CCArray::create();
	m_pAnnounceLocalList->retain();

	m_pAnnounceNewList = CCArray::create();
	CC_SAFE_RETAIN(m_pAnnounceNewList);

	parseAnnounceData(  CSVDecoder::create(ANNOUNCE_CONFIG_PATH)->getData());
    
    return Model::init();
}

void AnnounceModel::parseAnnounces(const Json::Value &responseData)
{
	m_pAnnounceList->removeAllObjects();
	m_pAnnounceNewList->removeAllObjects();
    for (Json::Value::iterator iter = responseData.begin(); iter != responseData.end(); ++iter)
    {
        AnnounceInfo * pAnnounce = AnnounceInfo::create();
        pAnnounce->parse(*iter);
        
		if(pAnnounce->getAnnounceType() == 0)
		{
			m_pAnnounceNewList->addObject(pAnnounce);
		}
		else
		{
			m_pAnnounceList->addObject(pAnnounce);
		}
		
    }
    
    isSynced = true;

	addLocalAnnounceList();
}

void AnnounceModel::parseAnnounceData(const CSVDecoder::CSV& val)
{
	m_pAnnounceLocalList->removeAllObjects();

	for (uint32_t i = 0; i< val.size(); ++i)
	{
		AnnounceInfo* pAnnounceInfo = AnnounceInfo::create();
		pAnnounceInfo->m_sTitle = val[i][1].c_str();
		pAnnounceInfo->m_sContent = val[i][2].c_str();
		pAnnounceInfo->m_nMinDays = atoi(val[i][3].c_str());
		pAnnounceInfo->m_nMaxDays = atoi(val[i][4].c_str());
		replace(pAnnounceInfo->m_sContent, "\\n", "\n");
		pAnnounceInfo->m_sImg = val[i][5].c_str();
		pAnnounceInfo->m_kNavType = (navigation::NavigationType)atoi(val[i][6].c_str());
		pAnnounceInfo->m_nNavTag = atoi(val[i][7].c_str());
		pAnnounceInfo->m_sUrl = val[i][8].c_str();
		pAnnounceInfo->m_bNetworkImg = false;

		m_pAnnounceLocalList->addObject(pAnnounceInfo);
	}
}

void AnnounceModel::addLocalAnnounceList()
{
	CCObject* obj = NULL;
	AnnounceInfo* pAInfo = NULL;
	CCARRAY_FOREACH(m_pAnnounceLocalList, obj)
	{
		pAInfo = (AnnounceInfo*)obj;
		if (pAInfo)
		{
			if (pAInfo->m_nMinDays > 1 || pAInfo->m_nMaxDays > 0)
			{
				unsigned int reg_ts = DM_GET_USER_MODEL->getUserInfo()->getRegTs();
				unsigned int now_ts = GameManager::sharedGameManager()->getServerTime();
				unsigned int day = (TimeUtil::getTimeToday(now_ts) - TimeUtil::getTimeToday(reg_ts)) / TIME_DAY_SECOND + 1;
				if ( (day < pAInfo->m_nMinDays || day > pAInfo->m_nMaxDays)
                    && reg_ts !=0 )
				{
					continue;
				}
			}

			m_pAnnounceList->addObject(obj);
		}
	}
}



