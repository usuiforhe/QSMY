//
//  DestinyWarriorListCell.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyWarriorListCell.h"
#include "components/components.h"
#include "../../../managers/ResourceManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

DestinyWarriorListCell::DestinyWarriorListCell()
:m_pWarriorHead(NULL)
{
    
}

DestinyWarriorListCell::~DestinyWarriorListCell()
{
    CC_SAFE_RELEASE(m_pWarriorHead);
}

bool DestinyWarriorListCell::init(cocos2d::extension::CCData *pData)
{
    if(!CCTableViewCell::init()) return false;
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorHead", DestinyWarriorHeadLoader::loader());
    DCCBReader * pReader = new DCCBReader(pNodeLoaderLibrary);
    cocos2d::CCNode *pNode= pReader->readNodeGraphFromData(pData, this, CCDirector::sharedDirector()->getWinSize());
    pReader->release();
    
    if(pNode) addChild(pNode);
    
    m_pWarriorHead = (DestinyWarriorHead*)pNode;
    CC_SAFE_RETAIN(m_pWarriorHead);
    
    return true;
}

DestinyWarriorListCell * DestinyWarriorListCell::create(cocos2d::extension::CCData *pData)
{
    DestinyWarriorListCell* pRet = new DestinyWarriorListCell();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


