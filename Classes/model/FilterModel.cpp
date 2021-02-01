//
//  FilterModel.cpp
//  屏蔽字
//
//  Created by mzp on 14-4-24.
//  Copyright (c) 2014年 thedream. All rights reserved.
//
#include "FilterModel.h"


#define FILTER_REPLACE_STRING	"**"

USING_NS_CC;


FilterModel::FilterModel()
	: m_pFilterVec(NULL)
{

}

FilterModel::~FilterModel()
{
	CC_SAFE_RELEASE(m_pFilterVec);
}

bool FilterModel::init()
{
	m_pFilterVec = CCArray::createWithCapacity(1000);
	CC_SAFE_RETAIN(m_pFilterVec);

	parseFilterData( CSVDecoder::create(FILTER_CONFIG_PATH)->getData());

	return Model::init();
}

std::string FilterModel::filter(const char* str)
{
	std::string source = str;
	source.length();

	CCObject* obj = NULL;
	CCString* pFilter = NULL;
	CCARRAY_FOREACH(m_pFilterVec, obj)
	{
		pFilter = (CCString*)obj;
		if (pFilter && pFilter->length() > 0)
		{
			std::string::size_type index = std::string::npos;
			while ((index = source.find(pFilter->getCString())) != std::string::npos)
			{
				source.replace(index, pFilter->length(), FILTER_REPLACE_STRING);
			}
		}
	}

	return source;
}

void FilterModel::parseFilterData(const CSVDecoder::CSV& val)
{
	for (uint32_t i = 0; i < val.size(); ++i)
	{
		m_pFilterVec->addObject(CCString::create(val[i][0].c_str()));
	}
}
