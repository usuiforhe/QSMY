//
//  UnitView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "UnitView.h"
#include "BuffView.h"
#include "SkillView.h"
#include "EffectView.h"
#include "BattleDefine.h"

using namespace sp;
using namespace battle;
using namespace warrior;

USING_NS_CC;

static const cocos2d::CCPoint D_FORMATION_POS[14] =
{
	ccp(-150,160),    //一号位
	ccp(-250,160),    //二号位
	ccp(-150,75),   //三号位
	ccp(-250,75),    //四号位
	ccp(-150,-10),   //五号位
	ccp(-250,-10),    //六号位
	ccp(-150,-95),   //七号位
	ccp(-250,-95),    //八号位
	ccp(-150,-180),   //九号位
	ccp(-250,-180),    //十号位
	ccp(-150,-265),   //十一号位
	ccp(-250,-265),    //十二号位
	ccp(-150,-350),    //十三号位
	ccp(-250,-350)    //十四号位
};

static const cocos2d::CCPoint D_FORMATION_OFFSET_POS[8] =
{
	ccp(0,0),   //占一格
	ccp(-50,0),   //占两格
	ccp(-50,0),   //占三格
	ccp(-50,0),   //占四格
	ccp(-50,0),   //占五格
	ccp(-50,0),   //占六格
	ccp(-50,0),   //占七格
	ccp(-50,0)   //占八格
};

const char *UnitView::ATTACK = "attack";
const char *UnitView::STAND = "stand";
const char *UnitView::HARD_STRAIGHT = "hard straight";
const char *UnitView::DEATH = "death";
const char *UnitView::SKILL = "skill";
const char *UnitView::PARRY = "parry";
const char *UnitView::DODGE = "dodge";
const char *UnitView::VERTIGO = "vertigo";

const char *UnitView::CONJURE_BEGIN    = "begin";
const char *UnitView::CONJURE          = "conjure";
const char *UnitView::CONJURE_END      = "end";


UnitView::UnitView()
:m_pUnit(NULL)
,m_ani(NULL)
,m_pName(NULL)
,m_iHp(0)
,m_pHp(NULL)
,m_pBossMark(NULL)
,m_bDead(false)
,isVertigo(false)
,m_pBuffArray(NULL)
{
    
}
UnitView::~UnitView()
{
    CC_SAFE_RELEASE(m_pHp);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pBossMark);
	CC_SAFE_RELEASE_NULL(m_pBuffArray);
    CCLOG("~UnitView %p",this);
}

UnitView* UnitView::create(BattleUnit *pUnit)
{
    UnitView * pview = new UnitView();
    if( pview && pview->init(pUnit))
    {
        pview->autorelease();
        return pview;
    }
    CC_SAFE_DELETE(pview);
    return NULL;
}


bool UnitView::init(BattleUnit *pUnit)
{
    if(!CCNode::init()) return false;
    if(!Notification::init()) return false;
    m_pUnit = pUnit;
    initView();
    setHp(m_pUnit->hp,0);
	m_pBuffArray = CCArray::create();
	m_pBuffArray->retain();
    return true;
}

void UnitView::initView()
{
    m_pHp = DProgressBar::create("battle/ui/bg_hp.png", "battle/ui/bar_hp.png", "battle/ui/mask_hp.png");
    m_pHp->setPosition(ccp(0,-10));
    addChild(m_pHp);
    m_pHp->retain();
    
    if(m_pUnit->is_boss)
    {
        this->setScale(1.0f);
        m_pBossMark = ResourceManager::sharedResourceManager()->getUIAnimate("bosshalo");
        CC_SAFE_RETAIN(m_pBossMark);
        addChild(m_pBossMark,-2);
        CCSprite* pBossMark = CCSprite::create("battle/ui/bosshpmark.png");
        pBossMark->setAnchorPoint(CCPointZero);
        pBossMark->setPosition(ccp(-6,-8));
        m_pHp->addChild(pBossMark);
    }else
    {
        this->setScale(0.8f);
    }
    
    
    Armature *armature;
	uint32_t uAvatarId = m_pUnit->getAvatarId();
	
	if(m_pUnit->m_pForce->id==(int)kForceRight)
	{
		warrior::WarriorBaseInfo* baseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(uAvatarId);
		uAvatarId = baseInfo && baseInfo->getFlipRes() > 0 ? baseInfo->getFlipRes() : uAvatarId;
	}
	

    armature = Armature::create(UNIT_SK_NAME(uAvatarId),UNIT_ARMATURE);
    //armature->getAnimation()->MovementEventSignal.connect(this, &UnitView::onMovementEvent);
    armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onMovementEvent), START);
	armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onMovementEvent), COMPLETE);
	armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onMovementEvent), LOOP_COMPLETE);
    
    
    if(m_pUnit->hp==0) //处理上来就已经死掉的单位
    {
        m_bDead = true;
        armature->getAnimation()->setAnimationScale(0.00001f);
        armature->getAnimation()->play(DEATH);
    }else
    {
        armature->getAnimation()->play(STAND);
    }
    
    m_ani = armature;
    
    CCPoint pos = positionToPoint(m_pUnit->position, 1);
    
    
    if(m_pUnit->m_pForce->id==(int)kForceLeft)
    {
        armature->setScaleX(1.0f);
        pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(pos.x,pos.y));
    }else
    {
		armature->setScaleX(-1.0f);
        pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(-pos.x,pos.y));
    }
    
    setPosition(pos);
	PL_MOVE_WITH_FLEXIBLE_HEIGHT(this, 88.f);    
    //zOrder = -pos.y;
    //setZOrder(zOrder);
    
    addChild(armature,-1);
    
    if(!m_pUnit->is_show) hideUnit();
}

void UnitView::hideUnit()
{
    m_ani->setVisible(false);
    m_pHp->setVisible(false);
    if(m_pBossMark) m_pBossMark->setVisible(false);
}

void UnitView::showUnit()
{
    m_ani->setVisible(true);
    m_pHp->setVisible(true);
    if(m_pBossMark) m_pBossMark->setVisible(true);
}

void UnitView::setHp(int value,float delay)
{
    m_iHp = MIN(MAX(value, 0), m_pUnit->hp);
    if(D_FLOAT_EQUALS(delay, 0.f))
    {
        setHp(CCInteger::create(m_iHp));
    }else
    {
        CCSequence *pSetHpDelay = CCSequence::create(CCDelayTime::create(delay),
                                                     CCCallFuncO::create(this, callfuncO_selector(UnitView::setHp),CCInteger::create(m_iHp)),
                                                     NULL);
        runAction(pSetHpDelay);
    }
    
}

void UnitView::setHp(CCObject *obj)
{
    float percent = (float)((CCInteger*)obj)->getValue()/m_pUnit->max_hp;
    m_pHp->setPercent(percent,0.3f);
    postNotification(NOTIFY_HP_CHANGED);
}

int UnitView::getHp()
{
    return m_iHp;
}

float UnitView::playSkillCover(ActionSkill *pAction,float delay)
{
	if(m_pUnit->sid == 0) return delay;
    SkillCover* pSkillCover = SkillCoverUnit::create(pAction,delay,m_pUnit->m_pForce->id==(int)kForceRight);
    addChild(pSkillCover,0);
    return pSkillCover->getDuration();
}

float UnitView::playAction(const char* name,float delay,bool forceChange,float fixeRate)
{
    if(isDead()) return delay;
    CCArray* arr = CCArray::create(CCString::create(name),CCBool::create(forceChange),NULL);
    if(D_FLOAT_EQUALS(delay, 0.f))
    {
        playAction(arr);
    }else
    {
        
        CCSequence* m_pActionDelay = CCSequence::create(CCDelayTime::create(delay),
                                                        CCCallFuncO::create(this, callfuncO_selector(UnitView::playAction),arr),
                                                        NULL);
        runAction(m_pActionDelay);
    }
    return delay + getActionDuration(name)*fixeRate; //系数调整动画节奏
}

bool UnitView::isActiveAction(const char * name)
{
    return strcmp(name, ATTACK)==0
    || strcmp(name, CONJURE_BEGIN)==0;
//    || strcmp(name, PARRY)==0
//    || strcmp(name, DODGE)==0;
}

void UnitView::playAction(cocos2d::CCObject *obj)
{
    
    CCArray* arr = (CCArray*)obj;
	CCString* name = (CCString*)arr->objectAtIndex(0);
    CCBool * force = (CCBool*)arr->objectAtIndex(1);
    if(strcmp(name->getCString(),DEATH)==0) 
	{
			stopAllActions();
			removeBuffEffect();
	}
    if(strcmp(name->getCString(),VERTIGO)==0) isVertigo = true;
    if(isActiveAction(name->getCString())) isVertigo = false;
    if(strcmp(name->getCString(), ATTACK)==0
       || strcmp(name->getCString(), CONJURE_BEGIN)==0)
    {
        showName();
    }
    
    
    MovementData *pMovementData = m_ani->getAnimation()->getAnimationData()->getMovement(name->getCString());
    if(pMovementData)
    {
        m_ani->getAnimation()->play(name->getCString(),-1,-1,force->getValue());

		DM_GET_SOUND_MODEL->playUnitSound(m_pUnit->sid, 0, name->getCString());

        return;
    }
    CCLOG("找不到动作%s",name->getCString());
}

void UnitView::showName()
{
	if(m_pUnit->sid == 0) return;
    if(m_pName==NULL)
    {
        m_pName = ResourceManager::sharedResourceManager()->getUIAnimate("unit_name",ZORDER);
		/*char* pFontFile = NULL;
		WarriorBaseInfo* pBaseInfo = DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pUnit->sid);
		switch(pBaseInfo->getColorGrade())
		{
		case kWarGradeGreen:
			pFontFile = D_FONT_GREEN_HERO_NAME; break;
		case kWarGradeBlue:
			pFontFile = D_FONT_BLUE_HERO_NAME; break;
		case kWarGradePurple:
			pFontFile = D_FONT_PURPLE_HERO_NAME; break;
		case kWarGradeRed:
			pFontFile = D_FONT_ORANGE_HERO_NAME; break;

		default:
			pFontFile = D_FONT_DEFAULT;break;
		}
		
	    cocos2d::CCLabelBMFont* pNameFont = cocos2d::CCLabelBMFont::create(pBaseInfo->getWarriorName().c_str(), pFontFile);
		CCSprite* pLabelSprite = (CCSprite*)pNameFont->getChildByTag(0);
		DSprite* pNameSprite = DSprite::createWithTexture(pLabelSprite->getTexture(), pLabelSprite->getTextureRect());
		m_pName->getBone("name")->replaceDisplayAt(pNameSprite, 0);*/
        SPContainer *pSPContainer =  SPContainer::create(m_pName->getRenderType());
        cocos2d::ccColor3B color = DM_GET_WARRIOR_MODEL->getWarriorColorByWid(m_pUnit->sid);
#ifndef _WIN32
        CCRichLabelTTF * pLabel = CCRichLabelTTF::create(CCString::createWithFormat("[color=ff%02x%02x%02x][b]%s[/b][/color]",color.r,color.g,color.b,DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pUnit->sid)->getWarriorName().c_str())->getCString(), D_FONT_DEFAULT,D_FONT_SIZE_12);
#else
		CCRichLabelTTF * pLabel = CCRichLabelTTF::create(CCString::createWithFormat("%s",DM_GET_WARRIOR_MODEL->getWarriorBaseByWID(m_pUnit->sid)->getWarriorName())->getCString(), D_FONT_DEFAULT,D_FONT_SIZE_12);
#endif
		//pLabel->enableStroke(ccc3(50, 50, 50), 0.8f);
        int c = 0xff<<24 | color.r<<16 | color.g<<8 | color.b;
        pLabel->enableShadow(CCSizeMake(0,0), c, 2);
        pSPContainer->addChild(pLabel);
        m_pName->getBone("name")->replaceDisplayAt(pSPContainer, 0);
        CC_SAFE_RETAIN(m_pName);
        addChild(m_pName);
    }
    m_pName->getAnimation()->playByIndex(0,-1,-1,true);
}


float UnitView::getActionDuration(const char *name)
{
    MovementData *pMovementData = m_ani->getAnimation()->getAnimationData()->getMovement(name);
    if(pMovementData) return pMovementData->getDuration() * m_ani->getAnimation()->getAnimationSpeed();
    CCLOG("找不到动作%s",name);
    return 0.0f;
}

CCPoint UnitView::positionToPoint(int pos,int count)
{
    //修正索引
    pos--;
    count--;
    return ccpAdd(D_FORMATION_POS[pos], D_FORMATION_OFFSET_POS[count]);
}

void UnitView::playNum(Result *pResult, float delay)
{
    if(isDead()) return;
    CCSequence *pNumDelay = CCSequence::create(CCDelayTime::create(delay),
                                               CCCallFuncO::create(this, callfuncO_selector(UnitView::playNum),pResult),
                                               NULL);
    runAction(pNumDelay);
    
}

void UnitView::playNum(CCObject *obj)
{
	Result *pResult = (Result *)obj;
    
    Armature *armature = NULL;
    CCNode *_ttf = NULL;
    switch (pResult->type) {
        case kResultHeal:
        {
            ResultHeal* pResultHeal = dynamic_cast<ResultHeal*>(pResult);
            if(pResultHeal->is_critical)//是否暴击
            {
                //暴击特效字
                _ttf =CCNode::create();
                
                CCSprite* pBao = CCSprite::create("battle/ui/font_bao.png");
                //暴击特效字
                CCLabelBMFont * pLabel = CCLabelBMFont::create(CCString::createWithFormat("%d",pResultHeal->heal)->getCString(), D_FONT_BATTLE_HEAL);
                pLabel->setAnchorPoint(ccp(0,0.5f));
                pLabel->setPosition(ccp(10, 0));
                _ttf->setContentSize(CCSizeMake(pLabel->getPositionX()+pLabel->getContentSize().width, pLabel->getContentSize().height));
                _ttf->addChild(pBao);
                _ttf->addChild(pLabel);
                armature = Armature::create(NUM_SK,NUM_CRIT_ARMATURE,sp::ZORDER);
                
            }else
            {
                _ttf = CCLabelBMFont::create(CCString::createWithFormat("%d",pResultHeal->heal)->getCString(), D_FONT_BATTLE_HEAL);
                armature = Armature::create(NUM_SK,NUM_ORDINARY_ARMATURE,sp::ZORDER);
            }
            break;
        }
        case kResultDamage:
        {
            ResultDamage* pResultDamage = dynamic_cast<ResultDamage*>(pResult);
            if (pResultDamage->is_block)//是否格挡
            {
                //伤害数值+ 格挡
                _ttf =CCNode::create();
                CCSprite* pBlock = CCSprite::create("battle/ui/font_zhaojia.png");
                CCLabelBMFont * pLabel = CCLabelBMFont::create(CCString::createWithFormat("%d",pResultDamage->damange)->getCString(), D_FONT_BATTLE_DMG);
                pLabel->setPosition(ccp(30, 0));
                pLabel->setAnchorPoint(ccp(0,0.5));
                _ttf->addChild(pBlock);
                _ttf->addChild(pLabel);
                _ttf->setContentSize(CCSizeMake(pLabel->getPositionX()+pLabel->getContentSize().width, pLabel->getContentSize().height));
                armature = Armature::create(NUM_SK,NUM_CRIT_ARMATURE,sp::ZORDER);
            }
            else if(pResultDamage->is_critical)//是否暴击
            {
                _ttf =CCNode::create();
                
                CCSprite* pBao = CCSprite::create("battle/ui/font_bao.png");
                //暴击特效字
                CCLabelBMFont * pLabel = CCLabelBMFont::create(CCString::createWithFormat("%d",pResultDamage->damange)->getCString(), D_FONT_BATTLE_DMG);
                pLabel->setAnchorPoint(ccp(0,0.5f));
                pLabel->setPosition(ccp(10, 0));
                _ttf->setContentSize(CCSizeMake(pLabel->getPositionX()+pLabel->getContentSize().width, pLabel->getContentSize().height));
                _ttf->addChild(pBao);
                _ttf->addChild(pLabel);
                armature = Armature::create(NUM_SK,NUM_CRIT_ARMATURE,sp::ZORDER);
            }else if(pResultDamage->is_dodge)//是否躲闪
            {
                //显示躲闪
                _ttf = CCSprite::create("battle/ui/font_shanbi.png");
                armature = Armature::create(NUM_SK,NUM_CRIT_ARMATURE,sp::ZORDER);
            }else
            {
                _ttf = CCLabelBMFont::create(CCString::createWithFormat("%d",pResultDamage->damange)->getCString(), D_FONT_BATTLE_DMG);
                armature = Armature::create(NUM_SK,NUM_ORDINARY_ARMATURE,sp::ZORDER);
            }
            break;
        }
        default:
            _ttf = CCLabelBMFont::create("", D_FONT_BATTLE_DMG);
            armature = Armature::create(NUM_SK,NUM_ORDINARY_ARMATURE,sp::ZORDER);
            break;
    }
    
    
    _ttf->setAnchorPoint(ccp(0.5,0.5));
    
    SPContainer *pSPContainer =  SPContainer::create(armature->getRenderType());
    pSPContainer->addChild(_ttf);
    armature ->getBone("num")->replaceDisplayAt(pSPContainer, 0);
    
    //armature->getAnimation()->MovementEventSignal.connect(this, &UnitView::onNumEvent);
    
    armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onNumEvent), COMPLETE);
    armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onNumEvent), LOOP_COMPLETE);
    
    armature->getAnimation()->playByIndex(0);
    
    _ttf->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?1.0f:-1.0f);
    armature->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?1.0f:-1.0f);
    addChild(armature);
}

float UnitView::playAddBuff(uint32_t id,float delay)
{
    if(isDead()) return delay;
    BuffEffect* pBuffEffect = BuffEffect::create(id,BUFF_ADDBUFF_ARMATURE,delay);
    //pBuffEffect->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?1.0f:-1.0f);
    pBuffEffect->setZOrder(2);
    addChild(pBuffEffect);
    
    //buff持续时间改为配置 不再读取动画时间（有些动画跳字时间太久）
    //return pBuffEffect->getDuration();
	m_pBuffArray->addObject(pBuffEffect);
	pBuffEffect->retain();
    return delay+DM_GET_BUFF_MODEL->getBuff(id)->getDuration();
    
}

float UnitView::playFateAddBuff(uint32_t id,float delay)
{
	if(isDead()) return delay;
	Armature* pArmature = Armature::create(CCString::createWithFormat("%s_%d",FATE_BUFF_SK, id)->getCString(),SKILL_UNIT_EFFECT_ARMATURE,sp::ZORDER);
	addChild(pArmature, 2);

	runAction(CCSequence::create(CCDelayTime::create(delay), CCCallFuncO::create(this, callfuncO_selector(UnitView::playArmature), pArmature), NULL));
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onComplete), COMPLETE);
	return delay;

}

void UnitView::onComplete(sp::Armature *_armature, const char *_eventType, const char *_movementID)
{
	_armature->removeFromParent();
}


void UnitView::playArmature(CCObject* obj)
{
	Armature* pArmature = (Armature*)obj;
	pArmature->getAnimation()->playByIndex(0, -1, -1, true);
}

float UnitView::playBuffAction(uint32_t id,float delay)
{
    if(isDead()) return delay;
    BuffEffect* pBuffEffect = BuffEffect::create(id,BUFF_BUFF_ARMATURE,delay);
    //pBuffEffect->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?1.0f:-1.0f);
    pBuffEffect->setZOrder(2);
    addChild(pBuffEffect);
	showName();
	m_pBuffArray->addObject(pBuffEffect);
	pBuffEffect->retain();
    return pBuffEffect->getDuration();
}

float UnitView::playUnitEffect(battle::Skill *pSkill,float delay)
{
    if(isDead()) return delay;
    SkillUnitEffect* pSkillUnitEffect = SkillUnitEffect::create(pSkill,delay);
	float scalex = m_pUnit->m_pForce->id==(int)kForceLeft && pSkill->getSkillInfo()->getHitFlip() > 0 ? -1.f : 1.f;
    pSkillUnitEffect->setScaleX(scalex);
    if(pSkill->unit_effect_layer==kSkillLayerTypeUp)
    {
        pSkillUnitEffect->setZOrder(getZOrder()+1);
    }else
    {
        pSkillUnitEffect->setZOrder(getZOrder()-1);
    }
    pSkillUnitEffect->setPosition(getPosition());
    CCNode* pContainer = getParent();
    if(pContainer) pContainer->addChild(pSkillUnitEffect);
    return pSkillUnitEffect->getDuration();
}

float UnitView::playUnitEffect(battle::ResultDamage* pResultDamage,float delay)
{
    if(isDead()) return delay;
    UnitEffect* pUnitEffect = UnitEffect::create(pResultDamage,delay);
    pUnitEffect->setScaleX(m_pUnit->m_pForce->id==(int)kForceLeft?1.0f:-1.0f);
    pUnitEffect->setZOrder(1);
    addChild(pUnitEffect);
    return pUnitEffect->getDuration();
}


#define UNIT_SHOW_HIDE_DUR 0.6f

float UnitView::playHide(float delay)
{
    this->runAction(CCSequence::create(CCDelayTime::create(delay),CCCallFunc::create(this, callfunc_selector(UnitView::playHide)),NULL));
    return delay+UNIT_SHOW_HIDE_DUR;
}


float UnitView::playShow(float delay)
{
    this->runAction(CCSequence::create(CCDelayTime::create(delay),CCCallFunc::create(this, callfunc_selector(UnitView::playShow)),NULL));
    return delay+UNIT_SHOW_HIDE_DUR;
}

void UnitView::playHide()
{
    Armature *armature;
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("battle/skill/common/unit_hide.png",UNIT_HIDE_SK);
    armature = Armature::create(UNIT_HIDE_SK,UNIT_SHOW_HIDE);
    armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onHideEvent), COMPLETE);
    armature->getAnimation()->playByIndex(0);
    armature->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?-1.0f:1.0f);
    this->runAction(CCSequence::create(CCDelayTime::create(UNIT_SHOW_HIDE_DUR),CCCallFunc::create(this,callfunc_selector(UnitView::hideUnit)),NULL));
    addChild(armature,2);
}


void UnitView::onHideEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID)
{
    _armature->removeFromParent();
}


void UnitView::playShow()
{
    Armature *armature;
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("battle/skill/common/unit_show.png",UNIT_SHOW_SK);
    armature = Armature::create(UNIT_SHOW_SK,UNIT_SHOW_HIDE);
    armature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(UnitView::onShowEvent), COMPLETE);
    armature->getAnimation()->playByIndex(0);
    armature->setScaleX(m_pUnit->m_pForce->id==(int)kForceRight?-1.0f:1.0f);
    this->runAction(CCSequence::create(CCDelayTime::create(UNIT_SHOW_HIDE_DUR),CCCallFunc::create(this,callfunc_selector(UnitView::showUnit)),NULL));
    addChild(armature,2);
}

void UnitView::onShowEvent(sp::Armature *_armature, const char *_eventType, const char *_movementID)
{
    _armature->removeFromParent();
}



//float UnitView::doPlayEffect(uint32_t skill_id,float delay)
//{
//    const char* name = DEATH;
//    Armature *armature;
//    armature = Armature::create("01","unit");
//    armature->getAnimation()->MovementEventSignal.connect(this, &UnitView::onNumEvent);
//    armature->getAnimation()->play(name);
//    armature->setZOrder(-1);
//
//
//    if(m_pUnit->m_pForce->id==(int)kForceLeft)
//    {
//        armature->setScaleX(-1);
//    }
//
//    armature->setPosition(position);
//
//    addChild(armature);
//
//
//    MovementData *pMovementData = armature->getAnimation()->getAnimationData()->getMovement(name);
//    return delay + pMovementData->getDuration() * armature->getAnimation()->getAnimationSpeed();
//}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void UnitView::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
    CCLOG("Unit%d Type:%s , id:%s",m_pUnit->sid, _eventType,_movementID);
	if(strcmp(_eventType, COMPLETE)==0)
	{
        if(strcmp(_movementID, DEATH)!=0)
        {
            if(strcmp(_movementID, CONJURE_BEGIN)==0)
            {
                playAction(CONJURE);
            }
            else if(isVertigo)
            {
                playAction(VERTIGO);
            }
            else
            {
                playAction(STAND);
            }
        }
	}
	else if (strcmp(_eventType, LOOP_COMPLETE) == 0)
	{
		if(strcmp(_movementID, DEATH)!=0)
		{
			if(strcmp(_movementID, CONJURE)==0)
			{
				playAction(CONJURE_END);
			}
		}
	}
}



void UnitView::onNumEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
	//if (strcmp(_eventType, COMPLETE) == 0 || strcmp(_eventType, LOOP_COMPLETE) == 0 )
	//{
    _armature->removeFromParent();
	//}
}

void UnitView::removeBuffEffect()
{
	BuffEffect* pBuffEff = NULL;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(m_pBuffArray, pObj)
	{
		pBuffEff = (BuffEffect*)pObj;
		pBuffEff->stopAnimation();
	}
}
