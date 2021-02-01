//
//  BuffModel.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BuffModel.h"

USING_NS_CC;
using namespace buff;

BuffModel::BuffModel()
:m_Buffs(NULL)
{
    
}

BuffModel::~BuffModel()
{
    CC_SAFE_RELEASE_NULL(m_Buffs);
}

bool BuffModel::init()
{
    m_Buffs = BuffMap::create();
    CC_SAFE_RETAIN(m_Buffs);
    loadData();
    return Model::init();
}

void BuffModel::loadData()
{
    parseInfo(  CSVDecoder::create(BUFF_CONFIG_PATH)->getData());
}

void BuffModel::parseInfo(const CSVDecoder::CSV &val)
{
    m_Buffs->setObject(Buff::create(), DEFAULT_BUFF);
    for (uint32_t i=0; i<val.size(); ++i)
    {
        Buff *pBuff = Buff::create();
        pBuff->sid = atoi(val[i][0].c_str());
        pBuff->m_nDuration = D_MILLISECOND_TO_SECOND(atoi(val[i][1].c_str()));
        m_Buffs->setObject(pBuff, pBuff->sid);
    }
}