//
//  EffectView.cpp
//  QSMY
//
//  Created by wanghejun on 13-5-25.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "EffectView.h"
#include "BattleDefine.h"

USING_NS_CC;
using namespace sp;



UnitEffect::UnitEffect()
:m_pArmature(NULL)
,m_fDelay(0.0f)
,m_sEffectName()
{
    
}
UnitEffect::~UnitEffect()
{
    CC_SAFE_RELEASE(m_pArmature);
}


UnitEffect* UnitEffect::create(battle::ResultDamage* pResultDamage,float delay)
{
    UnitEffect* ret = new UnitEffect();
    
    if(ret && ret->init(pResultDamage,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
    
}

void UnitEffect::setZOrder(int zOrder)
{
    CCNode::setZOrder(zOrder);
    m_pArmature->setZOrder(zOrder);
}

int UnitEffect::getZOrder()
{
    return m_pArmature->getZOrder();
}

bool UnitEffect::init(battle::ResultDamage* pResultDamage,float delay)
{
    if(!CCNode::init()) return false;

	m_nWarriorID = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pResultDamage->actor)->sid;
    m_fDelay = delay;
    if(pResultDamage->is_block) m_sEffectName = EFFECT_PARRY_ARMATURE;
    else if(pResultDamage->is_critical) m_sEffectName = EFFECT_CRIT_ARMATURE;
    else m_sEffectName = EFFECT_ORDINARY_ARMATURE;
    
	m_pArmature = Armature::create(EFFECT_SK,m_sEffectName.c_str());

    CC_SAFE_RETAIN(m_pArmature);
    //m_pArmature->getAnimation()->MovementEventSignal.connect(this, &UnitEffect::onMovementEvent);
    
    m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitEffect::onMovementEvent), COMPLETE);
    
    CCActionInterval* delayTime0 = CCDelayTime::create(delay);
    this->runAction(CCSequence::create(delayTime0,
                                       CCCallFunc::create(this, callfunc_selector(UnitEffect::startAnimation)),
                                       NULL));
    return true;
}

void UnitEffect::startAnimation()
{
    m_pArmature->getAnimation()->playByIndex(0);
    addChild(m_pArmature);

	std::string sid = EFFECT_SK;//CCString::createWithFormat("%u", m_nWarriorID)->getCString();
	DM_GET_SOUND_MODEL->playEffectSound(sid, 0, m_sEffectName.c_str());
}

float UnitEffect::getDuration()
{
    MovementData *pMovementData = m_pArmature->getAnimation()->getAnimationData()->getMovementByIndex(0);
    return m_fDelay + pMovementData->getDuration() * m_pArmature->getAnimation()->getAnimationSpeed();
}


void UnitEffect::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
   // std::string typesStr = _eventType;
    
    //CCLOG("SkillType:%s , id:%s",_eventType,_movementID);
	//if (typesStr.compare(COMPLETE) == 0)
	//{
        removeFromParent();
	//}
}
