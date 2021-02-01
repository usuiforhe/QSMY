//
//  ConfigModel.cpp
//  QSMY
//
//  Created by Eci on 13-12-28.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "ConfigModel.h"
USING_NS_CC;

ConfigModel::ConfigModel()
	:m_pDictConfig(NULL)
{
    
}

ConfigModel::~ConfigModel()
{
	CC_SAFE_RELEASE(m_pDictConfig);
}

bool ConfigModel::init()
{
	CCLOG("ConfigModel::init");
	m_pDictConfig = CCDictionary::create();
	CC_SAFE_RETAIN(m_pDictConfig);
	loadGameConfig(  CSVDecoder::create(GAME_CONFIG_PATH)->getData());
    return Model::init();
}

void ConfigModel::loadGameConfig(const CSVDecoder::CSV &val)
{
	for (uint32_t i=0; i<val.size(); ++i)
	{
		m_pDictConfig->setObject(ccs(val[i][1]), val[i][0]);
	}
}

CCString *ConfigModel::getConfigByKey(const char *pKey)
{
	CCString *pConfig = static_cast<CCString *>(m_pDictConfig->objectForKey(pKey));
	if (pConfig)
	{
		return pConfig;
	}

#ifdef QSMY_DEBUG
	CCLOG("Error Config For Key : %s", pKey);
	CCAssert(0, "Error Config For Key");
#endif
	return CCString::create("");
}

void ConfigModel::setConfigByKey(const char *pKey, const char *pConfig)
{
    m_pDictConfig->setObject(ccs(pConfig), pKey);
}