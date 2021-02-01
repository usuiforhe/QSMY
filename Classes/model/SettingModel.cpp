//
//  SettingModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-13.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SettingModel.h"

USING_NS_CC;
using namespace settings;

SettingModel::SettingModel()
:m_pSfxStatus(false)
,m_pMusicStatus(false)
,m_pCCUserDefault(NULL)
,m_pGuides(NULL)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_enableCDKey = false;
#else
    m_enableCDKey = true;
#endif
}
SettingModel::~SettingModel()
{
    CC_SAFE_RELEASE(m_pGuides);
}

bool SettingModel::init()
{
    m_pCCUserDefault = CCUserDefault::sharedUserDefault();
    m_pSfxStatus = m_pCCUserDefault->getBoolForKey(KEY_SFX_STATUS,true);
    m_pMusicStatus = m_pCCUserDefault->getBoolForKey(KEY_MUSIC_STATUS,true);
    
    m_pGuides = CCArray::create();
    CC_SAFE_RETAIN(m_pGuides);
    loadData();
    return Model::init();
}

void SettingModel::loadData()
{
    parseGuideList( CSVDecoder::create(GUIDE_CONFIG_PATH)->getData());
}

void SettingModel::parseGuideList(const CSVDecoder::CSV &val)
{
    for (uint32_t i=0; i<val.size(); ++i)
	{
        GuideInfo* pItem= GuideInfo::create();
		pItem->m_Title = val[i][0].c_str();
        pItem->m_Info = val[i][1];
        m_pGuides->addObject(pItem);
	}
}

void SettingModel::setSfxStatus(bool val)
{
    m_pSfxStatus = val;
    m_pCCUserDefault->setBoolForKey(KEY_SFX_STATUS, val);
    m_pCCUserDefault->flush();
    postNotification(D_EVENT_SETTING_SFX_STATUS_UPDATED);
}

void SettingModel::setMusicStatus(bool val)
{
    m_pMusicStatus = val;
    m_pCCUserDefault->setBoolForKey(KEY_MUSIC_STATUS, val);
    m_pCCUserDefault->flush();
    postNotification(D_EVENT_SETTING_MUSIC_STATUS_UPDATED);
}
