//
//  DNumberBandit.cpp
//  QSMY
//
//  Created by wanghejun on 13-11-14.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DNumberBandit.h"

USING_NS_CC;

#define CHAR_HEIGHT 78.0
#define DEFAULT_LAP 3
#define DEFAULT_PIC "PL_ui/common/ani_imgNumber.png"
#define DEFAULT_TEXT_COUNT 10

DNumberBandit::DNumberBandit()
:m_pNumber(NULL)
,m_pClippingNode(NULL)
,m_nLap(0)
,m_nLastTarget(0)
,m_nTextCount(DEFAULT_TEXT_COUNT)
,m_fCharHeight(0.f)
,m_fDur(0.f)
{
    
}

DNumberBandit::~DNumberBandit()
{
   
}

DNumberBandit* DNumberBandit::create(unsigned int lap,float char_height,const char* sPath)
{
    DNumberBandit* pRet = new DNumberBandit();
    if(pRet && pRet->init(lap, char_height, sPath))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DNumberBandit::init()
{
    return init(DEFAULT_LAP,CHAR_HEIGHT,DEFAULT_PIC);
}

bool DNumberBandit::init(unsigned int lap, float char_height,const char* sPath)
{
    if(!CCNode::init()) return false;
    m_nLap = lap;
    m_fCharHeight = char_height;
    m_sPath = sPath;
    
    removeAllChildren();
    CCLayerColor* pStencil = CCLayerColor::create(ccc4(255, 255, 255, 255),640,m_fCharHeight);
    m_pClippingNode = CCClippingNode::create(pStencil);
    addChild(m_pClippingNode);
    m_pNumber = CCNode::create();
    m_pNumber->setAnchorPoint(CCPointZero);
    m_pClippingNode->addChild(m_pNumber);
    
    CCSprite* pNode = CCSprite::create(m_sPath.c_str());
    pNode->setAnchorPoint(CCPointZero);
    pNode->setPosition(ccp(0,-m_fCharHeight*m_nTextCount+m_fCharHeight));
    m_pNumber->addChild(pNode);
    
    return true;
}

void DNumberBandit::setToTarget(unsigned char target)
{
    CCAssert(target<m_nTextCount, "");
    m_pNumber->removeAllChildrenWithCleanup(true);
    CCSprite* pNode = CCSprite::create(m_sPath.c_str());
    pNode->setAnchorPoint(CCPointZero);
    pNode->setPosition(ccp(0.f, -m_fCharHeight * (m_nTextCount - target - 1)));
    m_pNumber->setPosition(CCPointZero);
    m_pNumber->addChild(pNode);
    m_nLastTarget = target;
}

void DNumberBandit::start(float delay,unsigned char target)
{
    CCAssert(target<m_nTextCount, "");
    m_pNumber->removeAllChildren();
    for (int i(0); i<=m_nLap; ++i) {
        CCSprite* pNode = CCSprite::create(m_sPath.c_str());
        pNode->setAnchorPoint(CCPointZero);
        pNode->setPosition(ccp(0.f,-m_fCharHeight*m_nTextCount*(i+1)+m_fCharHeight*(m_nLastTarget+1)));
        m_pNumber->addChild(pNode);
    }
    
//    CCLog("DNumberBandit::start target: %d, last target:%d", (int)target, (int)m_nLastTarget);
    
    CCArray* arrayOfActions = CCArray::create();

    float dis = m_fCharHeight*m_nTextCount*m_nLap - m_fCharHeight*3.f-m_fCharHeight*3.f + m_fCharHeight*((int)target - (int)m_nLastTarget);
    float dur = dis/600;
    CCLog("dis:%f, dur:%f.", dis, dur);
    arrayOfActions->addObject(CCDelayTime::create(delay));
    arrayOfActions->addObject(CCEaseIn::create(CCMoveBy::create(0.6f, ccp(0.f,m_fCharHeight*3.f)),2));
    arrayOfActions->addObject(CCMoveBy::create(dur, ccp(0,dis)));
    arrayOfActions->addObject(CCEaseOut::create(CCMoveBy::create(0.6f, ccp(0.f,m_fCharHeight*3.f)),2));
    arrayOfActions->addObject(CCCallFunc::create(this, callfunc_selector(DNumberBandit::animationEndCallBack)));
    
    CCAction* pAction = CCSequence::create(arrayOfActions);
    m_pNumber->runAction(pAction);
    m_fDur = dur+0.6f*2.f;
    m_nLastTarget = target;
}

float DNumberBandit::getDur()
{
    return m_fDur;
}

void DNumberBandit::animationEndCallBack()
{
    setToTarget(m_nLastTarget);
}

