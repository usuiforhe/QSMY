//
//  BuffEffect.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-20.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BuffView.h"
#include "../../../managers/GameManager.h"


using namespace sp;
USING_NS_CC;

//==================================

BuffEffect* BuffEffect::create(uint32_t buff_id,const char* action_name,float delay)
{
    BuffEffect* ret = new BuffEffect();
    
    if(ret && ret->init(buff_id,action_name,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
    
}

void BuffEffect::setZOrder(int zOrder)
{
    m_pArmature->setZOrder(zOrder);
}

int BuffEffect::getZOrder()
{
    return m_pArmature->getZOrder();
}

bool BuffEffect::init(uint32_t buff_id,const char* action_name,float delay)
{
    if(!CCNode::init()) return false;
	m_nBuffID = buff_id;
    m_fDelay = delay;
    m_pArmature = Armature::create(DM_GET_BUFF_MODEL->getBuff(buff_id)->getSkeletonName(),action_name);
    CC_SAFE_RETAIN(m_pArmature);
    //m_pArmature->getAnimation()->MovementEventSignal.connect(this, &BuffEffect::onMovementEvent);
    m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BuffEffect::onMovementEvent), COMPLETE);
    CCActionInterval* delayTime0 = CCDelayTime::create(delay);
    this->runAction(CCSequence::create(delayTime0,
                                       CCCallFunc::create(this, callfunc_selector(BuffEffect::startAnimation)),
                                       NULL));
    return true;
}

void BuffEffect::startAnimation()
{
    m_pArmature->getAnimation()->playByIndex(0);
	addChild(m_pArmature);
	DM_GET_SOUND_MODEL->playBuffSound(m_nBuffID);
}

void BuffEffect::stopAnimation()
{
	removeFromParent();
}

float BuffEffect::getDuration()
{
    MovementData *pMovementData = m_pArmature->getAnimation()->getAnimationData()->getMovementByIndex(0);
    return m_fDelay + pMovementData->getDuration() * m_pArmature->getAnimation()->getAnimationSpeed();
}


void BuffEffect::onMovementEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID)
{
   // std::string typesStr = _eventType;
    
    //CCLOG("SkillType:%s , id:%s",_eventType,_movementID);
//	if (typesStr.compare(COMPLETE) == 0)
//	{
        removeFromParent();
	//}
}