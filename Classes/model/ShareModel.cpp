//
//  ShareModel.cpp
//  QSMY
//
//  Created by heven on 14-4-29.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "ShareModel.h"
#include "managers/DataManager.h"
#include "managers/PlatformManager.h"

USING_NS_CC;
using namespace share;

ShareModel::ShareModel()
:m_pArrayForShare(NULL)
,m_nShareTimes(0)
{
    
}

ShareModel::~ShareModel()
{
    CC_SAFE_RELEASE(m_pArrayForShare);
}


bool ShareModel::init()
{
    m_pArrayForShare = CCArray::create();
	CC_SAFE_RETAIN(m_pArrayForShare);
    
    loadData();
    return Model::init();
}


void ShareModel::loadData()
{
    loadShareData( CSVDecoder::create(SHARE_CONFIG_PATH)->getData());
}

void ShareModel::loadShareData(const CSVDecoder::CSV &val)
{
	m_pArrayForShare->removeAllObjects();
	for (uint32_t i = 0; i < val.size(); i++)
	{
		share::ShareInfo* shareInfo = share::ShareInfo::create();
		shareInfo->m_sKey = val[i][0];
		shareInfo->m_sAppKey = val[i][1];
		shareInfo->m_sIconPath = val[i][2];
        shareInfo->m_sShareUrl = val[i][3];
		m_pArrayForShare->addObject(shareInfo);
	}
	
}

share::ShareInfo* ShareModel::getShareInfoByKey(const char* key)
{
	CCObject* obj = NULL;
	share::ShareInfo* pShareInfo = NULL;
	CCARRAY_FOREACH(m_pArrayForShare, obj)
	{
		pShareInfo = (share::ShareInfo*)obj;
		if (pShareInfo && pShareInfo->m_sKey == key)
		{
			return pShareInfo;
		}
	}
    
	return NULL;
}

uint32_t ShareModel::getShareTimes()
{
    return m_nShareTimes;
}

void ShareModel::setShareTimes(uint32_t val)
{
    m_nShareTimes = val;
    postNotification(D_EVENT_SHARE_TIMES_UPDATE);
}

bool ShareModel::isCanReward()
{
   if(strcmp(DM_GET_PLATFORM_MODEL->getConfig().getUser().c_str(), TAIWANG_GOOGLE_LABLE) == 0)
   {
       return false;
   }
    
    return m_nShareTimes < D_CONFIG_UINT_FOR_KEY(MAX_SHARE_TIMES);
}



