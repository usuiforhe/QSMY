//
//  SkillView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SkillView.h"
#include "BattleDefine.h"
#include "managers/ResourceManager.h"

using namespace battle;
using namespace sp;
USING_NS_CC;

#pragma mark skillView

SkillView::SkillView()
:m_pArmature(NULL)
,m_fDelay(0.0f)
,m_nSkillID(0)
{
    
}
SkillView::~SkillView()
{
    CC_SAFE_RELEASE(m_pArmature);
}

SkillView* SkillView::create(uint32_t skill_id,float delay)
{
    SkillView* ret = new SkillView();
    
    if(ret && ret->init(skill_id,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
    
}

bool SkillView::init(uint32_t skill_id,float delay)
{
	m_nSkillID = skill_id;
    if(!CCNode::init()) return false;
    m_fDelay = delay;
    m_pArmature = Armature::create(DM_GET_BATTLE_MODEL->getSkill(skill_id)->getSkillInfo()->getSkeletonName(),SKILL_SKILL_ARMATURE);
    CC_SAFE_RETAIN(m_pArmature);
   // m_pArmature->getAnimation()->MovementEventSignal.connect(this, &SkillView::onMovementEvent);
    m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(SkillView::onMovementEvent), COMPLETE);
    CCActionInterval* delayTime0 = CCDelayTime::create(delay);
    this->runAction(CCSequence::create(delayTime0,
                                       CCCallFunc::create(this, callfunc_selector(SkillView::startAnimation)),
                                       NULL));
    return true;
}

void SkillView::startAnimation()
{
    m_pArmature->getAnimation()->playByIndex(0);
	addChild(m_pArmature);
	DM_GET_SOUND_MODEL->playSkillSound(m_nSkillID);
}

float SkillView::getDuration()
{
    MovementData *pMovementData = m_pArmature->getAnimation()->getAnimationData()->getMovementByIndex(0);
    return m_fDelay + pMovementData->getDuration() * m_pArmature->getAnimation()->getAnimationSpeed();
}


void SkillView::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
    //if(strcmp(_eventType, COMPLETE)==0)
	//{
        removeFromParent();
	//}
}

//==================================


#pragma mark SkillEffect
SkillEffect::SkillEffect()
:m_pArmature(NULL)
,m_fDelay(0.0f)
,m_nSkillID(0)
{
    
}
SkillEffect::~SkillEffect()
{
    CC_SAFE_RELEASE(m_pArmature);
}

SkillEffect* SkillEffect::create(uint32_t skill_id,float delay)
{
    SkillEffect* ret = new SkillEffect();
    
    if(ret && ret->init(skill_id,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
    
}

bool SkillEffect::init(uint32_t skill_id,float delay)
{
    if(!CCNode::init()) return false;
	m_nSkillID = skill_id;
    m_fDelay = delay;
    m_pArmature = Armature::create(DM_GET_BATTLE_MODEL->getSkill(skill_id)->getSkillInfo()->getSkeletonName(),SKILL_EFFECT_ARMATURE);
    CC_SAFE_RETAIN(m_pArmature);
    //m_pArmature->getAnimation()->MovementEventSignal.connect(this, &SkillEffect::onMovementEvent);
    m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(SkillEffect::onMovementEvent), COMPLETE);
    CCActionInterval* delayTime0 = CCDelayTime::create(delay);
    this->runAction(CCSequence::create(delayTime0,
                                       CCCallFunc::create(this, callfunc_selector(SkillEffect::startAnimation)),
                                       NULL));
    return true;
}

void SkillEffect::startAnimation()
{
    m_pArmature->getAnimation()->playByIndex(0);
	addChild(m_pArmature);
	DM_GET_SOUND_MODEL->playEffectSound(m_nSkillID);
}

float SkillEffect::getDuration()
{
    MovementData *pMovementData = m_pArmature->getAnimation()->getAnimationData()->getMovementByIndex(0);
    return m_fDelay + pMovementData->getDuration() * m_pArmature->getAnimation()->getAnimationSpeed();
}


void SkillEffect::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
    //if(strcmp(_eventType, COMPLETE)==0)
	//{
        removeFromParent();
	//}
}



//==================================

#pragma mark SkillUnitEffect

SkillUnitEffect::SkillUnitEffect()
:m_pArmature(NULL)
,m_fDelay(0.0f)
,unit_effect_id(0)
,m_nSkillID(0)
{
    
}
SkillUnitEffect::~SkillUnitEffect()
{
    CC_SAFE_RELEASE(m_pArmature);
}

SkillUnitEffect* SkillUnitEffect::create(battle::Skill *pSkill,float delay)
{
    SkillUnitEffect* ret = new SkillUnitEffect();
    
    if(ret && ret->init(pSkill,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
    
}

void SkillUnitEffect::setZOrder(int zOrder)
{
    CCNode::setZOrder(zOrder);
    m_pArmature->setZOrder(zOrder);
}

int SkillUnitEffect::getZOrder()
{
    return m_pArmature->getZOrder();
}

bool SkillUnitEffect::init(battle::Skill *pSkill,float delay)
{
    if(!CCNode::init()) return false;
	m_nSkillID = pSkill->getSid();
    m_fDelay = delay;
    m_pArmature = Armature::create(pSkill->getSkillInfo()->getSkeletonName(),SKILL_UNIT_EFFECT_ARMATURE);
    CC_SAFE_RETAIN(m_pArmature);
    unit_effect_id = pSkill->getCurrentUnitEffect();
    //m_pArmature->getAnimation()->MovementEventSignal.connect(this, &SkillUnitEffect::onMovementEvent);
    m_pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(SkillUnitEffect::onMovementEvent), COMPLETE);
    CCActionInterval* delayTime0 = CCDelayTime::create(delay);
    this->runAction(CCSequence::create(delayTime0,
                                       CCCallFunc::create(this, callfunc_selector(SkillUnitEffect::startAnimation)),
                                       NULL));
    return true;
}

void SkillUnitEffect::startAnimation()
{
    m_pArmature->getAnimation()->playByIndex(unit_effect_id);
	addChild(m_pArmature);
	DM_GET_SOUND_MODEL->playUnitEffSound(m_nSkillID, unit_effect_id);
}

float SkillUnitEffect::getDuration()
{
    MovementData *pMovementData = m_pArmature->getAnimation()->getAnimationData()->getMovementByIndex(unit_effect_id);
    return m_fDelay + pMovementData->getDuration() * m_pArmature->getAnimation()->getAnimationSpeed();
}


void SkillUnitEffect::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
    //if(strcmp(_eventType, COMPLETE)==0)
	//{
        removeFromParent();
	//}
}


//==================================
#pragma mark SkillCover

SkillCover::SkillCover()
:m_pLayer(NULL)
,m_pArmature(NULL)
,m_fDelay(0.0f)
{}
SkillCover::~SkillCover(){
    CC_SAFE_RELEASE(m_pLayer);
    CC_SAFE_RELEASE(m_pArmature);
}

SkillCover* SkillCover::create(ActionSkill *pAction,float delay)
{
    SkillCover* ret;
    switch (pAction->m_pCoactors->size()) {
        case 1:
            ret = new SkillCoverTwo();
            break;
        /*case 2:
            ret = new SkillCoverThree();
            break;
        case 3:
            ret = new SkillCoverFour();
            break;*/
        default:
            ret = new SkillCoverOne();
            break;
    }
    
    if(ret && ret->init(pAction,delay))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

void SkillCover::show()
{
    setVisible(true);
    if(m_pArmature) m_pArmature->getAnimation()->playByIndex(0,-1,-1,true);
}

void SkillCover::finish()
{
    runAction(CCSequence::create(CCDelayTime::create(getDuration()),
                                 CCCallFunc::create(this, callfunc_selector(SkillCover::removeFromParent)),
                                 NULL));
}

bool SkillCover::init(battle::ActionSkill *pAction,float delay)
{
    if(!CCNode::init()) return false;
    m_fDelay = delay;
    battle::Skill *pSkill = DM_GET_BATTLE_MODEL->getSkill(pAction->skill_id);
    
    skillName = pSkill->getSkillInfo()->getSkillName();
    mSkillColor = DM_GET_SKILL_MODEL->getSkillColorConfig(pSkill->getSkillInfo()->grade);
    m_pLayer = CCLayerColor::create(ccc4(0,0,0,255*0.5));
    m_pLayer->ignoreAnchorPointForPosition(false);
    m_pLayer->setPosition(D_DESIGN_POINT_CENTER);
    CC_SAFE_RETAIN(m_pLayer);
    addChild(m_pLayer);
    setVisible(false);
    runAction(CCSequence::create(CCDelayTime::create(delay)
                                 ,CCCallFunc::create(this, callfunc_selector(SkillCover::show))
                                 ,NULL));
    return true;
}


void SkillCover::hideCover()
{
    //if(m_pArmature) m_pArmature->getAnimation()->playByIndex(1,-1,-1,true);
}

#pragma mark skill name

void SkillCover::showSkillName()
{
	if(skillName.compare("-1") == 0) return ;
    unsigned short* utf16String = cc_utf8_to_utf16(skillName.c_str());
    
    int stringLen = utf16String ? cc_wcslen(utf16String) : 0;
    
    /*float firstDelay = m_fDelay +0.3f;
    
    CCSpawn * pTTFAction = NULL;
    
    float scale1 = 0.6f*2;
    float scale2 = 1.1f*2;
    float scale3 = 1.0f*2;
    
    float tf_height = 0;
    
    for (int i =0; i<stringLen; ++i)
    {
        const char * str = cc_utf16_to_utf8(utf16String+i,1,NULL,NULL);
        CCLabelBMFont* ttf = CCLabelBMFont::create(str, D_FONT_SKILL_NAME);
        pTTFAction = CCSpawn::create(CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f),
                                                        CCScaleTo::create(0.16f, scale2),
                                                        CCScaleTo::create(0.1f, scale3),
                                                        NULL),
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f),
                                                        CCFadeIn::create(0.16f),
                                                        NULL),
                                     
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f+0.26f+0.8f),
                                                        CCScaleTo::create(0.1f, scale2),
                                                        CCScaleTo::create(0.16f, 0.0f),
                                                        NULL),
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f+0.26f+0.1f+0.8f),
                                                        CCFadeOut::create(0.16f),
                                                        NULL),
                                        NULL);
        CC_SAFE_DELETE_ARRAY(str);

        CCSprite* pFontChar = (CCSprite*)ttf->getChildByTag(0);
        DSprite* _tf = DSprite::createWithTexture(pFontChar->getTexture(), pFontChar->getTextureRect());

        addChild(_tf);
        
        _tf->setPosition(ccp(500,700-tf_height));
        tf_height+=ttf->getContentSize().height*scale3;
        _tf->setScale(scale1);
        _tf->setOpacity(0.0f);
        _tf->setColor(mSkillColor->top, mSkillColor->bottom);
        _tf->runAction(pTTFAction);
    }*/
	for (int i =0; i<stringLen; ++i)
	{
#ifndef QSMY_DEBUG  //最多支持7个技能字
		if(i > 6) break;
#endif
		const char * str = cc_utf16_to_utf8(utf16String+i,1,NULL,NULL);
		CCLabelBMFont* ttf = CCLabelBMFont::create(str, D_FONT_SUPPER_SKILL_NAME);
		CCSprite* pFontChar = (CCSprite*)ttf->getChildByTag(0);
		DSprite* pCharSprite = DSprite::createWithTexture(pFontChar->getTexture(), pFontChar->getTextureRect());
		pCharSprite->setColor(mSkillColor->top, mSkillColor->bottom);
		pCharSprite->setScale(1.5f);
		m_pArmature->getBone(CCString::createWithFormat("name%d", i+1)->getCString())->replaceDisplayAt(pCharSprite, 0);
		CC_SAFE_DELETE_ARRAY(str);
	}
    CC_SAFE_DELETE_ARRAY(utf16String);
    
}

bool SkillCoverOne::init(battle::ActionSkill *pAction,float delay)
{
    if(!SkillCover::init(pAction,delay)) return false;
    
    //CCSprite* pBg = CCSprite::create("battle/skill/common/skillcoverbg.png");
	uint8_t pForceType = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->m_pForce->id;
    //m_pArmature = Armature::create(pForceType == kForceRight?SKILL_COVER_HEROLEFT_SK:SKILL_COVER_SK,"ani",sp::ZORDER);
	m_pArmature = Armature::create(SKILL_COVER_SK,"ani",sp::ZORDER);
    m_pArmature->setPosition(D_DESIGN_POINT_CENTER);
	battle::Skill *pSkill = DM_GET_BATTLE_MODEL->getSkill(pAction->skill_id);
	CCSprite* pNode1 = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId());
	CCSprite* pNode2 = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId());
	CCSprite* pNode3 = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId());
	//pNode->setScale(2.f);
	/*if(pForceType == kForceLeft)
	{
	pNode->setFlipX(true);
	}*/
	
	m_pArmature->getBone("hero1_1")->replaceDisplayAt(pNode1, 0);
	m_pArmature->getBone("hero1_2")->replaceDisplayAt(pNode2, 0);
	m_pArmature->getBone("hero1_3")->replaceDisplayAt(pNode3, 0);
    //m_pArmature->getBone("bg")->replaceDisplayAt(pBg, 0);
    addChild(m_pArmature);
    CC_SAFE_RETAIN(m_pArmature);
    showSkillName();
    finish();
   /* runAction(CCSequence::create(CCDelayTime::create(getDuration()-0.5f),
                                 CCCallFunc::create(this, callfunc_selector(SkillCover::hideCover)),
                                 NULL));*/
    return true;
}

float SkillCoverOne::getDuration()
{
    return m_fDelay+2.f;
}


bool SkillCoverTwo::init(battle::ActionSkill *pAction,float delay)
{
    if(!SkillCover::init(pAction,delay)) return false;
    /*
    CCSprite* pTop = CCSprite::create("battle/skill/common/skillcoverbg.png");
    CCSprite* pBottom = CCSprite::create("battle/skill/common/skillcoverbg.png");
    
    m_pArmature = Armature::create(SKILL_COVER_SK,"2p",sp::ZORDER);
    m_pArmature->setPosition(D_DESIGN_POINT_CENTER);
    CCSprite* pNode = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId());
    m_pArmature->getBone("character0")->replaceDisplayAt(pNode, 0);
    
    pNode = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->m_pCoactors->at(0))->getAvatarId());
    m_pArmature->getBone("character1")->replaceDisplayAt(pNode, 0);
    
    
    m_pArmature->getBone("bgtop")->replaceDisplayAt(pTop, 0);
    m_pArmature->getBone("bgbottom")->replaceDisplayAt(pBottom, 0);
    
    
    CCClippingNode* pCNode = CCClippingNode::create();
    pCNode->addChild(m_pArmature);
    
    CCNode *stencil = CCNode::create();
    pTop = CCSprite::create("battle/skill/common/skillcoverbg.png");
    pTop->setScaleY(-1.f);
    pTop->setAnchorPoint(CCPointZero);
    pTop->setPosition(ccp(-52.f,1047.f));
    stencil->addChild(pTop);
    
    pTop = CCSprite::create("battle/skill/common/skillcoverbg.png");
    pTop->setScaleX(-1.f);
    pTop->setAnchorPoint(CCPointZero);
    pTop->setPosition(ccp(680.f,-90.f));
    stencil->addChild(pTop);
    
    pCNode->setStencil(stencil);
    pCNode->setAlphaThreshold(0.5);
    
    addChild(pCNode);*/
	m_pArmature = Armature::create(SKILL_COVER_2P_SK,"ani",sp::ZORDER);
	m_pArmature->setPosition(D_DESIGN_POINT_CENTER);
	
	uint32_t hero1 = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId();
	/*uint8_t coActor1 = pAction->m_pCoactors->at(0);
	skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pAction->skill_id);
	const battle::BattleUnit *pUnit = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor);
	if((!coActor1 || pSkill->wid2 != coActor1) && pSkill->wid1 == pUnit->sid)
	{
	CCLOG("skillId:%d, actor:%d, skill--wid1:%d, wid2:%d, wid3:%d", pAction->skill_id, pAction->actor, pSkill->wid1, pSkill->wid2, pSkill->wid3);
	coActor1 = pSkill->wid2;
	}
	uint32_t hero2 = 0;
	const battle::BattleUnit *pCoUnit1 = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(coActor1);
	if(!pCoUnit1)
	{
	hero2 = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(coActor1)->getAvatarId();
	} else 
	{
	hero2 = pCoUnit1->getAvatarId();
	}*/
	uint32_t hero2 = DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->m_pCoactors->at(0))->getAvatarId();
	
	if(hero1 == 32)//如果是嗜杀者，特殊处理，只能放在后面，避免挡住前面的英雄
	{
		uint32_t temp = hero1;
		hero1 = hero2;
		hero2 = temp;
	}
	
	CCSprite* pNode1_1 = ResourceManager::sharedResourceManager()->getUnitImage(hero1);
	CCSprite* pNode1_2 = ResourceManager::sharedResourceManager()->getUnitImage(hero1);
	CCSprite* pNode1_3 = ResourceManager::sharedResourceManager()->getUnitImage(hero1);

	m_pArmature->getBone("hero1_1")->replaceDisplayAt(pNode1_1, 0);
	m_pArmature->getBone("hero1_2")->replaceDisplayAt(pNode1_2, 0);
	m_pArmature->getBone("hero1_3")->replaceDisplayAt(pNode1_3, 0);

	CCSprite* pNode2_1 = ResourceManager::sharedResourceManager()->getUnitImage(hero2);
	CCSprite* pNode2_2 = ResourceManager::sharedResourceManager()->getUnitImage(hero2);
	CCSprite* pNode2_3 = ResourceManager::sharedResourceManager()->getUnitImage(hero2);
	m_pArmature->getBone("hero2_1")->replaceDisplayAt(pNode2_1, 0);
	m_pArmature->getBone("hero2_2")->replaceDisplayAt(pNode2_2, 0);
	m_pArmature->getBone("hero2_3")->replaceDisplayAt(pNode2_3, 0);

    CC_SAFE_RETAIN(m_pArmature);
    
	addChild(m_pArmature);
    showSkillName();
    finish();
    /*runAction(CCSequence::create(CCDelayTime::create(getDuration()-0.5f),
                                 CCCallFunc::create(this, callfunc_selector(SkillCover::hideCover)),
                                 NULL));*/
    return true;
}

float SkillCoverTwo::getDuration()
{
    return m_fDelay+3.6f;
}

bool SkillCoverThree::init(battle::ActionSkill *pAction,float delay)
{
    if(!SkillCover::init(pAction,delay)) return false;
    
    m_pArmature = Armature::create(SKILL_COVER_SK,"3p",sp::ZORDER);
    m_pArmature->setPosition(D_DESIGN_POINT_CENTER);
    CCSprite* pNode = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->getAvatarId());
    m_pArmature->getBone("character0")->replaceDisplayAt(pNode, 0);
    
	uint8_t coActor1 = pAction->m_pCoactors->at(0);
	uint8_t coActor2 = pAction->m_pCoactors->at(1);
	/*skill::Skill *pSkill = DM_GET_SKILL_MODEL->getSkill(pAction->skill_id);
	if((!coActor1 || !coActor2) && pSkill->wid1 == DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(pAction->actor)->sid)
	{
		CCLOG("skillId:%d, actor:%d, skill--wid1:%d, wid2:%d, wid3:%d", pAction->skill_id, pAction->actor, pSkill->wid1, pSkill->wid2, pSkill->wid3);
		coActor1 = pSkill->wid2;
		coActor2 = pSkill->wid3;
	}*/

    pNode = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(coActor1)->getAvatarId());
    m_pArmature->getBone("character1")->replaceDisplayAt(pNode, 0);
    
    pNode = ResourceManager::sharedResourceManager()->getUnitImage(DM_GET_BATTLE_MODEL->getBattleInfo()->getUnit(coActor2)->getAvatarId());
    m_pArmature->getBone("character2")->replaceDisplayAt(pNode, 0);
    
    addChild(m_pArmature);
    CC_SAFE_RETAIN(m_pArmature);
    
    
    showSkillName();
    finish();
    runAction(CCSequence::create(CCDelayTime::create(getDuration()-1.0f),
                                 CCCallFunc::create(this, callfunc_selector(SkillCover::hideCover)),
                                 NULL));
    return true;
}

float SkillCoverThree::getDuration()
{
    return m_fDelay+3.0f;
}

bool SkillCoverFour::init(battle::ActionSkill *pAction,float delay)
{
    if(!SkillCover::init(pAction,delay)) return false;
    return true;
}

float SkillCoverFour::getDuration()
{
    return 0.0f;
}


SkillCoverUnit* SkillCoverUnit::create(ActionSkill *pAction,float delay,bool flipX)
{
    SkillCoverUnit* ret = new SkillCoverUnit();
    if(ret && ret->init(pAction,delay,flipX))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool SkillCoverUnit::init(battle::ActionSkill *pAction,float delay,bool flipX)
{
    if(!SkillCover::init(pAction,delay)) return false;
    m_bFlipX = flipX;
    
    m_pArmature = Armature::create(SKILL_COVER_UNIT_SK,SKILL_COVER_UNIT);
    m_pArmature->setPosition(ccp(0,0));
    addChild(m_pArmature);
    CC_SAFE_RETAIN(m_pArmature);
    
    m_pLayer->removeFromParent();
    showSkillName();
    finish();
    runAction(CCSequence::create(CCDelayTime::create(getDuration()-0.3f),
                                 CCCallFunc::create(this, callfunc_selector(SkillCover::hideCover)),
                                 NULL));
    if(m_bFlipX) setScaleX(-1.f);
    else setScaleX(1.f);
    return true;
}

#define SKILL_BG_HEIGHT 130

void SkillCoverUnit::showSkillName()
{
	if(skillName.compare("-1") == 0) return ;
    unsigned short* utf16String = cc_utf8_to_utf16(skillName.c_str());
    
    int stringLen = utf16String ? cc_wcslen(utf16String) : 0;
    
    float firstDelay = m_fDelay +0.3f;
    
    CCSpawn * pTTFAction = NULL;
    
    float scale1 = 0.2f*2;
    float scale2 = 0.8f*2;
    float scale3 = 0.6f*2;
    
    float tf_height = 0;
    
    float scale4 = 0.9f;// 调整字间距
    
    CCNode* pContainer = CCNode::create();
    addChild(pContainer);
    for (int i = 0; i<stringLen; ++i)
    {
        const char * str = cc_utf16_to_utf8(utf16String+i,1,NULL,NULL);
        CCLabelBMFont* ttf = CCLabelBMFont::create(str, D_FONT_SKILL_NAME);
        pTTFAction = CCSpawn::create(CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f),
                                                        CCScaleTo::create(0.16f, scale2),
                                                        CCScaleTo::create(0.1f, scale3),
                                                        NULL),
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f),
                                                        CCFadeIn::create(0.16f),
                                                        NULL),
                                     
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f+0.26f+0.8f),
                                                        CCScaleTo::create(0.1f, scale2),
                                                        CCScaleTo::create(0.16f, 0.0f),
                                                        NULL),
                                     CCSequence::create(CCDelayTime::create(firstDelay+i*0.1f+0.26f+0.1f+0.8f),
                                                        CCFadeOut::create(0.16f),
                                                        NULL),
                                     NULL);
        CC_SAFE_DELETE_ARRAY(str);
        
        CCSprite* pFontChar = (CCSprite*)ttf->getChildByTag(0);
        DSprite* _tf = DSprite::createWithTexture(pFontChar->getTexture(), pFontChar->getTextureRect());
        
        pContainer->addChild(_tf);
        
        _tf->setPosition(ccp(90,164-tf_height));
        
        tf_height+=(stringLen==2 && i==0)?_tf->getContentSize().height*scale3*2.f*scale4:_tf->getContentSize().height*scale3*scale4; //2个字的技能第二个字空2倍距离
        
        _tf->setScale(scale1);
        _tf->setOpacity(0.0f);
        _tf->setFlipX(m_bFlipX);
        _tf->setColor(mSkillColor->top, mSkillColor->bottom);
        _tf->runAction(pTTFAction);
    }
    
    pContainer->setPosition(ccp(0.f,(tf_height-SKILL_BG_HEIGHT)*.5f));
    
    CC_SAFE_DELETE_ARRAY(utf16String);
}


float SkillCoverUnit::getDuration()
{
    return m_fDelay+2.0f;
}

