//
//  DestinyWarriorHead.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-19.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyWarriorHead.h"
#include "managers/ResourceManager.h"
#include "managers/DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;



DestinyWarriorData::DestinyWarriorData()
:m_nId(0)
,m_Color(ccc3(0, 0, 0))
,color_grade()
,m_bIsWarrior(false)
,m_name("")
,m_nCount(0)
,m_pWarriorModel(NULL)
,m_nAdd(0)
{
    
}

DestinyWarriorData::~DestinyWarriorData()
{
    
}

DestinyWarriorData* DestinyWarriorData::create(warrior::WarriorBaseInfo* pWarrior)
{
    DestinyWarriorData* pRet = new DestinyWarriorData();
    if(pRet && pRet->init(pWarrior))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool DestinyWarriorData::init(warrior::WarriorBaseInfo* pW)
{
    if(!ValueObjcet::init()) return false;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    this->setColor(m_pWarriorModel->getWarriorColorByWid(pW->getWarriorId()));
    this->setID(pW->getWarriorId());
    this->setAvatarId(pW->getResId());
    this->setGrade(pW->getColorGrade());
    this->setIsWarrior(true);
    this->setName(pW->getWarriorName().c_str());
    this->setCount(0);
    return true;
}


DestinyWarriorData* DestinyWarriorData::create(warrior::WarriorCurInfo* pWarrior)
{
    DestinyWarriorData* pRet = new DestinyWarriorData();
    if(pRet && pRet->init(pWarrior))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool DestinyWarriorData::init(warrior::WarriorCurInfo* pW)
{
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    init(pW->getBaseInfo());
    this->setAvatarId(pW->getAvatarId());
    this->setCount(1);
    return true;
}


DestinyWarriorData* DestinyWarriorData::create(warrior::SoulInfo* pSoul)
{
    DestinyWarriorData* pRet = new DestinyWarriorData();
    if(pRet && pRet->init(pSoul))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool DestinyWarriorData::init(warrior::SoulInfo* pS)
{
    if(!ValueObjcet::init()) return false;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    this->setColor(m_pWarriorModel->getWarriorColorByWid(pS->getBaseInfo()->getWarriorId()));
    this->setID(pS->getBaseInfo()->getWarriorId());
    this->setAvatarId(pS->getBaseInfo()->getResId());
    this->setGrade(pS->getBaseInfo()->getColorGrade());
    this->setIsWarrior(false);
    this->setName(pS->getBaseInfo()->getWarriorName().c_str());
    this->setCount(pS->getNum());
    return true;
}

int DestinyWarriorData::getRemain()
{
    return m_nCount-m_nAdd;
}
void DestinyWarriorData::add()
{
    if(getRemain()>0)
    {
        if(m_bIsWarrior && DM_GET_FORMATION_MODEL->isWarriorBattle(m_nId))
        {
            Tips_Alert(D_LOCAL_STRING("DestinyEnrolledUnitError").c_str());
            return;
        }
        m_nAdd++;
        postNotification(D_EVENT_DESTINYWARRIOR_ADD);
    }
}

void DestinyWarriorData::fixAdd(int num)
{
    m_nAdd = num;
    postNotification(D_EVENT_DESTINYWARRIOR_UPDATED);
}

void DestinyWarriorData::setCount(uint32_t val)
{
    m_nCount = val;
    postNotification(D_EVENT_DESTINYWARRIOR_UPDATED);
}

int DestinyWarriorData::getAdded()
{
    return m_nAdd;
}


#pragma mark DestinyWarriorHead

#define ACTION_TAG 0

DestinyWarriorHead::DestinyWarriorHead()
:m_pContainer(NULL)
,m_pGrade(NULL)
,m_pBorderHighLight(NULL)
,m_pBorder(NULL)
,m_pStars(NULL)
,m_pSoulMark(NULL)
,m_pWarriorMark(NULL)
,m_pCount(NULL)
,m_pName(NULL)
,m_pData(NULL)
{
    
}


DestinyWarriorHead::~DestinyWarriorHead()
{
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pBorderHighLight);
    CC_SAFE_RELEASE(m_pBorder);
    CC_SAFE_RELEASE(m_pStars);
    CC_SAFE_RELEASE(m_pSoulMark);
    CC_SAFE_RELEASE(m_pWarriorMark);
    CC_SAFE_RELEASE(m_pCount);
    CC_SAFE_RELEASE(m_pName);
    SAFE_RELEASE_UNBINDING_ALL(m_pData, this);
}

bool DestinyWarriorHead::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade",  CCScale9Sprite*, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode*, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorderHighLight", CCScale9Sprite *, this->m_pBorderHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBorder", CCScale9Sprite*, this->m_pBorder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pStars", CCNode*, this->m_pStars);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSoulMark", CCSprite*, this->m_pSoulMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorMark", CCSprite*, this->m_pWarriorMark);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCount", CCLabelBMFont*, this->m_pCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF*, this->m_pName);
    return false;
}



void DestinyWarriorHead::setData(DestinyWarriorData* pData)
{
    if(m_pData == pData) return;
    SAFE_RELEASE_UNBINDING_ALL(m_pData, this);
    CC_SAFE_RETAIN(pData);
    m_pData = pData;
    BINDING_EXEC(m_pData, this, DestinyWarriorHead::updateView, D_EVENT_DESTINYWARRIOR_UPDATED);
    BINDING_EXEC(m_pData, this, DestinyWarriorHead::updateView, D_EVENT_DESTINYWARRIOR_ADD);
}

void DestinyWarriorHead::updateView()
{
    this->m_pGrade->setColor(m_pData->getColor());
    this->m_pContainer->removeAllChildren();
    this->m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForFormation(m_pData->getAvatarId()));
    this->m_pBorderHighLight->setVisible(m_pData->getGrade()>warrior::kWarGradeBlue);
    this->m_pBorder->setVisible(m_pData->getGrade()<=warrior::kWarGradeBlue);
    this->m_pStars->setVisible(m_pData->getGrade()>warrior::kWarGradePurple);
    this->m_pSoulMark->setVisible(!m_pData->getIsWarrior());
    this->m_pWarriorMark->setVisible(m_pData->getIsWarrior());
    this->m_pName->setString(m_pData->getName().c_str());
    this->m_pCount->setString(CCString::createWithFormat("%d",m_pData->getRemain())->getCString());
}


bool DestinyWarriorHead::dTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent,bool *bSwallowsTouches)
{
    CCPoint pt = pTouch->getLocation();
    CCRect rcBoundingBox = CCRectMake( 0, 0, getContentSize().width, getContentSize().height );
    rcBoundingBox = CCRectApplyAffineTransform( rcBoundingBox, nodeToWorldTransform());
    if(!rcBoundingBox.containsPoint( pt ) )
    {
        return false;
    }
    schedule(schedule_selector(DestinyWarriorHead::update), 0.05f, kCCRepeatForever, 0.5f);
    return true;
}

void DestinyWarriorHead::update(float dt)
{
    m_pData->add();
}

void DestinyWarriorHead::dTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    DLayer::dTouchEnded(pTouch, pEvent);
    unschedule(schedule_selector(DestinyWarriorHead::update));
    update(0.f);
}

void DestinyWarriorHead::dTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    DLayer::dTouchCancelled(pTouch,pEvent);
    unschedule(schedule_selector(DestinyWarriorHead::update));
}


