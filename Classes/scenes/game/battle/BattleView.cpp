//
//  BattleView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-16.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "BattleView.h"
#include "DragonBone.h"
#include "UnitView.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "SkillView.h"
#include "BattleDefine.h"

USING_NS_CC;
using namespace sp;
using namespace battle;

/*
enum
{
    k1x =1,
    k2x =2,
};
*/

enum KLayerZOrder {
    kBgZOrder,
    kSkillBottomZOrder,
    KUnitUIZOder,
    KUnitZOder,
    kSceneEffectZOrder,
    kSkillUpZOrder,
    kInfoZOder,
    //  kSkillCoverZOrder,
    kSkipButtonZOder,
    kSkillCoverZOrder,
    kEffectLayerZOrder,
    kDialogZOrder,
	kFateZOrder
};


BattleView::BattleView()
:m_pModel(NULL)
,m_pPveModel(NULL)
,m_pWarriorModel(NULL)
,m_pDelegate(NULL)
,units(NULL)
,m_pUnitLayer(NULL)
,m_pUnitUILayer(NULL)
,m_PowerLeft(NULL)
,m_PowerRight(NULL)
,m_dPowerLeftTotal(0)
,m_dPowerRightTotal(0)
,m_pEffectLayer(NULL)
,m_pNameLeft(NULL)
,m_pNameRight(NULL)
,m_pSceneName(NULL)
,m_pRichSceneName(NULL)
,m_pSceneNode(NULL)
,m_pDialog(NULL)
,m_p1x(NULL)
,m_p2x(NULL)
,m_EffectNodeLeft(NULL)
,m_EffectNodeRight(NULL)
,m_pPowerAniLeft(NULL)
,m_pPowerAniRight(NULL)
,m_pSpeedUp(NULL)
,m_pSkipMenu(NULL)
,m_pBattleGridCenter(CCPointZero)
,m_pGridCenter(NULL)
,m_dLastPercentLeft(0.f)
,m_dLastPercentRight(0.f)
,m_pFateNode(NULL)
,m_bUnitInited(false)
,m_pArmatureArr1(NULL)
,m_pArmatureArr2(NULL)
,m_pArmatureArr3(NULL)
,m_pLeftUnits(NULL)
,m_pRightUnits(NULL)
{
    D_RESET_C_ARRAY(m_pBloodStar, BATTLE_BLOOD_STAR);
	D_RESET_C_ARRAY(m_pRightBloodStar, BATTLE_BLOOD_STAR);
}

BattleView::~BattleView()
{
    removeAllChildren();
    CCDictElement *_object = NULL;
	CCDICT_FOREACH(units, _object)
	{
		UnitView *pUnitView = (UnitView*)_object->getObject();
		pUnitView->stopAllActions();
	}
    CC_SAFE_RELEASE(units);
    CC_SAFE_RELEASE(m_PowerLeft);
    CC_SAFE_RELEASE(m_PowerRight);
    CC_SAFE_RELEASE(m_pNameLeft);
    CC_SAFE_RELEASE(m_pNameRight);
    CC_SAFE_RELEASE(m_pSceneNode);
    CC_SAFE_RELEASE(m_pSceneName);
	CC_SAFE_RELEASE(m_pRichSceneName);
    CC_SAFE_RELEASE(m_pDialog);
    CC_SAFE_RELEASE(m_p1x);
    CC_SAFE_RELEASE(m_p2x);
    CC_SAFE_RELEASE(m_EffectNodeLeft);
    CC_SAFE_RELEASE(m_EffectNodeRight);
	CC_SAFE_RELEASE(m_pSpeedUp);
	CC_SAFE_RELEASE(m_pSkipMenu);
	CC_SAFE_RELEASE(m_pGridCenter);
	D_SAFE_RELEASE_C_ARRAY(m_pBloodStar, BATTLE_BLOOD_STAR);
	D_SAFE_RELEASE_C_ARRAY(m_pRightBloodStar, BATTLE_BLOOD_STAR);
	CC_SAFE_RELEASE(m_pFateNode);
	CC_SAFE_RELEASE(m_pArmatureArr1);
	CC_SAFE_RELEASE(m_pArmatureArr2);
	CC_SAFE_RELEASE(m_pArmatureArr3);
	CC_SAFE_RELEASE(m_pRightUnits);
	CC_SAFE_RELEASE(m_pLeftUnits);
}

bool BattleView::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    
    m_pModel = DM_GET_BATTLE_MODEL;
    m_pPveModel = DM_GET_PVE_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
	CC_SAFE_RELEASE(m_pLeftUnits);
	m_pLeftUnits = CCArray::create();
	CC_SAFE_RETAIN(m_pLeftUnits);

	CC_SAFE_RELEASE(m_pRightUnits);
	m_pRightUnits = CCArray::create();
	CC_SAFE_RETAIN(m_pRightUnits);

    initBattleInfo();
    initUnits();
    //initSkipButton();
    
    m_pEffectLayer = DEffectLayer::create();
    //m_pEffectLayer->setPosition(D_DESIGN_POINT_CENTER);
    addChild(m_pEffectLayer,kEffectLayerZOrder);

	/*for(int i=0; i<BATTLE_BLOOD_STAR; i++)
	{
		m_pBloodStar[i]->playWithName("ani1");
		m_pRightBloodStar[i]->playWithName("ani1");
	}*/
	m_pFateNode = CCNode::create();
	CC_SAFE_RETAIN(m_pFateNode);
	//m_pFateNode->setPosition(D_DESIGN_POINT_CENTER);
	m_pFateNode->setAnchorPoint(ccp(0.5,0.5));
	addChild(m_pFateNode, kFateZOrder);

	CC_SAFE_RELEASE(m_pArmatureArr1);
	m_pArmatureArr1=CCArray::createWithCapacity(7);
	CC_SAFE_RETAIN(m_pArmatureArr1);

	CC_SAFE_RELEASE(m_pArmatureArr2);
	m_pArmatureArr2=CCArray::createWithCapacity(7);
	CC_SAFE_RETAIN(m_pArmatureArr2);

	CC_SAFE_RELEASE(m_pArmatureArr3);
	m_pArmatureArr3=CCArray::createWithCapacity(7);
	CC_SAFE_RETAIN(m_pArmatureArr3);
    return true;
}


void BattleView::reset()
{
    //TODO: 清除正在播放的技能？
}

void BattleView::toggleSpeed(cocos2d::CCObject* sender)
{
	/*
    CCMenuItem* pItem = static_cast<CCMenuItem*>(sender);
    bool speedUp(false);
    switch (pItem->getTag()) {
        case k1x:
            speedUp = false;
            break;
        case k2x:
            speedUp = true;
            break;
        default:
            break;
    }*/
	bool speedUp = !m_pModel->getSpeedUp();
    m_pModel->setSpeedUp(speedUp);
    updateSpeedUp();
}

void BattleView::updateSpeedUp()
{
    m_p2x->setVisible(!m_pModel->getSpeedUp());
    m_p1x->setVisible(!m_p2x->isVisible());
    m_pScheduler->setTimeScale(m_pModel->getBattleSpeed());
}

void BattleView::initBattleInfo()
{
    CCSprite* pBg = ResourceManager::sharedResourceManager()->getSceneIcon(m_pModel->getBattleInfo()->scene);
    if(pBg){
		pBg->setPosition(D_DESIGN_POINT_CENTER);
		this->addChild(pBg,kBgZOrder);
	}
    
    
    CCNode* pGrid = NULL;
    CCB_READER("PL_ui/ccb/pl_battle/PL_battleGrid", pGrid);
    //CCSprite* pGrid = CCSprite::create("battle/ui/grid.png");
    //pGrid->setPosition(D_DESIGN_POINT_CENTER);
    //CCNode* pGridSprite = pGrid->getChildByTag(1);
    if(pGrid) this->addChild(pGrid,kBgZOrder);

    battle::SceneInfo* pSceneInfo = m_pModel->getSceneInfo(m_pModel->getBattleInfo()->scene);
    Armature* pArmature = ResourceManager::sharedResourceManager()->getEffect(pSceneInfo->effect);
    pArmature->setPosition(D_DESIGN_POINT_CENTER);
    
    addChild(pArmature,kSceneEffectZOrder);
    
    //    CCParticleSystemQuad *m_emitter = CCParticleSystemQuad::create(CCString::createWithFormat("effect/%d.plist",m_pPveModel->getChapterInfo(m_pModel->getBattleInfo()->scene)->effect)->getCString());
    //    m_emitter->retain();
    //    CCParticleBatchNode *batch = CCParticleBatchNode::createWithTexture(m_emitter->getTexture());
    //
    //    batch->addChild(m_emitter);
    //    //batch->setAnchorPoint(D_ANCHOR_POINT_TOP);
    //    batch->setPosition(D_DESIGN_POINT_CENTER);
    
    //addChild(batch, kBgZOrder);
    
    
    //////////////////////////////////////////////////////////////////
    
    CCNode* pNode = NULL;
	//ui/ccb/battle/battle
    CCB_READER("PL_ui/ccb/pl_battle/PL_battle", pNode);
    if(pNode) this->addChild(pNode,kInfoZOder);
    
    /*
    m_p1x->setTarget(this, menu_selector(BattleView::toggleSpeed));
    m_p2x->setTarget(this, menu_selector(BattleView::toggleSpeed));

	m_p1x->setTag(k1x);
	m_p2x->setTag(k2x);
    */
	m_pSpeedUp->setTarget(this, menu_selector(BattleView::toggleSpeed));
    updateSpeedUp();
    
    
    pve::MapInfo* pMap = NULL;
    
    m_pSceneNode ->setVisible(false);
    if(m_pModel->getBattleInfo()->map!=NO_PVE_MAP)
    {
        pMap = m_pPveModel->getMapInfo(m_pModel->getBattleInfo()->map);
        if(pMap)
        {
			m_pSceneNode ->setVisible(true);
			m_pSceneName->setString(pMap->map_name.c_str());
			/*
			if (NULL == m_pRichSceneName)
			{
				m_pRichSceneName = CCRichLabelTTF::create();
				CC_SAFE_RETAIN(m_pRichSceneName);
				replaceLabel(m_pSceneName,m_pRichSceneName);
				m_pRichSceneName->setFontFillColor(m_pSceneName->getColor());
				m_pRichSceneName->enableStroke(m_pSceneName->getColor(), 1.0);
			}
            m_pRichSceneName->setString(pMap->map_name.c_str());
			*/
        }
    }
	else{
		//处理杯赛和竞技场的 名字
		if(DM_GET_BATTLE_MODEL->getBattleType() == battle::kBattleTypeMatch){
			//杯赛
			m_pSceneName->setString(D_LOCAL_STRING("beisaizhandoumap").c_str());
		}
		else if(DM_GET_BATTLE_MODEL->getBattleType() == battle::kBattleTypePvp){
			//论剑
			m_pSceneName->setString(D_LOCAL_STRING("lunjianzhandoumap").c_str());
		}
		else
		{
			m_pSceneName->setString("");
		}
	}
    
    const BattleForce *pForce;
    
    pForce = m_pModel->getBattleInfo()->getForce(kForceLeft);
    
    if(pForce)
    {
        m_pNameLeft->setString(pForce->name.c_str());
    }else
    {
        m_pNameLeft->setString("");
    }
    
    pForce = m_pModel->getBattleInfo()->getForce(kForceRight);
    
    if(pForce)
    {
        
        std::string name = pForce->name;
        if(name.compare("")==0)
        {
            if(pMap)
            {
                warrior::WarriorBaseInfo *pWarrior = m_pWarriorModel->getWarriorBaseByWID(pMap->enemy_id);
                if(pWarrior) name= pWarrior->getWarriorName();
            }
			else
			{
				name = DM_GET_FRIENDS_MODEL->getBattleName(); // Team被销毁时返回""
			}
        }
        m_pNameRight->setString(name.c_str());
    }else
	{
        m_pNameRight->setString("");
    }
    
    {
        CCLayerColor *stencil = CCLayerColor::create(ccc4(255,255,255,255),m_EffectNodeLeft->getContentSize().width,200);
        stencil->setPosition(ccp(-m_EffectNodeLeft->getContentSize().width,-100));
        CCClippingNode* pClippingNode = CCClippingNode::create(stencil);
        /*m_pPowerAniLeft = ResourceManager::sharedResourceManager()->getUIAnimate("bloodSmoke");
        pClippingNode->addChild(m_pPowerAniLeft);*/
        m_EffectNodeLeft->addChild(pClippingNode);
    }
    
    {
        CCLayerColor *stencil = CCLayerColor::create(ccc4(255,255,255,255),m_EffectNodeRight->getContentSize().width,200);
        stencil->setPosition(ccp(-m_EffectNodeRight->getContentSize().width,-100));
        CCClippingNode* pClippingNode = CCClippingNode::create(stencil);
       /* m_pPowerAniRight = ResourceManager::sharedResourceManager()->getUIAnimate("bloodSmoke");
        pClippingNode->addChild(m_pPowerAniRight);*/
        m_EffectNodeRight->addChild(pClippingNode);
    }
    m_EffectNodeLeft->ignoreAnchorPointForPosition(true);
    m_EffectNodeRight->ignoreAnchorPointForPosition(true);

	m_pBattleGridCenter = m_pGridCenter->getPosition()+ccp(m_pGridCenter->getContentSize().width / 2, m_pGridCenter->getContentSize().height / 2);
}


bool BattleView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_PowerLeft", DProgressBar *, this->m_PowerLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_PowerRight", DProgressBar *, this->m_PowerRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameLeft", CCLabelTTF *, this->m_pNameLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNameRight", CCLabelTTF *, this->m_pNameRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSceneName", CCLabelBMFont *, this->m_pSceneName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSceneNode", CCNode *, this->m_pSceneNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedUp", CCMenuItem *, this->m_pSpeedUp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkipMenu", CCMenuItem *, this->m_pSkipMenu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_p1x", CCNode *, this->m_p1x);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_p2x", CCNode *, this->m_p2x);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EffectNodeLeft", CCNode *, this->m_EffectNodeLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_EffectNodeRight", CCNode *, this->m_EffectNodeRight);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGridCenter", CCNode *, this->m_pGridCenter);
    D_CCB_ASSIGN_ARRAY("m_pBloodStar",UIAnimate*, m_pBloodStar, BATTLE_BLOOD_STAR);
	D_CCB_ASSIGN_ARRAY("m_pRightBloodStar", UIAnimate*, m_pRightBloodStar, BATTLE_BLOOD_STAR);
    return false;
}

void BattleView::playDialog(battle::BattleDialogInfo* pInfo)
{
    if(m_pDialog==NULL)
    {
        CCNode* pNode = NULL;
        CCB_READER("PL_ui/ccb/pl_common/storyTalk",pNode);
        m_pDialog = static_cast<StoryTalk*>(pNode);
        CC_SAFE_RETAIN(m_pDialog);
        m_pDialog->getNextDialogButton()->setTarget(this, menu_selector(BattleView::nextDialog));
        addChild(m_pDialog,kDialogZOrder);
    }
    m_pDialog->showInfo(pInfo);
}

void BattleView::hideDialog()
{
    if(m_pDialog)
    {
       m_pDialog->removeFromParent();
        CC_SAFE_RELEASE_NULL(m_pDialog);
    }
}


void BattleView::nextDialog(CCObject* obj)
{
    if(m_pDelegate) m_pDelegate->playNextDialog();
}

void BattleView::initUnits()
{
    m_pUnitLayer = CCLayer::create();
    addChild(m_pUnitLayer,KUnitZOder);
    m_pUnitUILayer = CCLayer::create();
    addChild(m_pUnitUILayer,KUnitUIZOder);
    
    units = new CCDictionary();
    UnitView *pUnitView;
    BattleUnit *pUnit;
    
    m_dPowerLeftTotal = 0;
    m_dPowerRightTotal = 0;
    
    for (uint32_t i=0; i<m_pModel->getBattleInfo()->m_pUnits->count(); ++i)
    {
        pUnit = (BattleUnit*)m_pModel->getBattleInfo()->m_pUnits->objectAtIndex(i);
		if(!pUnit->position && !pUnit->is_show)
			continue;
        switch (pUnit->m_pForce->id) {
            case kForceLeft:
                m_dPowerLeftTotal+=pUnit->max_hp*pUnit->fight_factor;
				m_pLeftUnits->addObject(pUnit);
                break;
            case kForceRight:
                m_dPowerRightTotal+=pUnit->max_hp*pUnit->fight_factor;
				m_pRightUnits->addObject(pUnit);
                break;
            default:
                break;
        }
        pUnitView = UnitView::create(pUnit);
        pUnitView->addObserver(this, notification_callfunc_selector(BattleView::updatePower),NOTIFY_HP_CHANGED);
        units->setObject(pUnitView,pUnit->id);
        m_pUnitUILayer->addChild(pUnitView,-pUnitView->getPosition().y);
    }
    updatePower();
	m_bUnitInited = true;
}


 
//void BattleView::initSkipButton()
void BattleView::createSkipButton(bool canSkip)
{
	/*
    CCMenuItem * item = NULL;
    if(canSkip)
    {
        item = CCMenuItemImage::create("battle/ui/btn_skip_n.png","battle/ui/btn_skip_h.png",this,menu_selector(BattleView::skipBattle));
    }else
    {
        CCSprite* n = CCSprite::create("battle/ui/btn_skip_n.png");
        CCSprite* h = CCSprite::create("battle/ui/btn_skip_h.png");
        n->setShaderProgram(D_SHADER_GARY);
        h->setShaderProgram(D_SHADER_GARY);
        item = CCMenuItemSprite::create(n,h,this,menu_selector(BattleView::skipBattle));
    }
    
    item->setPosition(ccpAdd(D_DESIGN_POINT_BOTTOM,ccp(0,60)));
    
    CCMenu * menu = CCMenu::create(item,NULL);
    
#ifdef QSMY_DEBUG
    CCMenuItemImage * item2 = CCMenuItemImage::create("battle/ui/btn_skip_n.png","battle/ui/btn_skip_h.png",this,menu_selector(BattleView::testPause));
    item2->setAnchorPoint(D_ANCHOR_POINT_TOP);
    item2->setPosition(D_DESIGN_POINT_TOP);
    menu->addChild(item2);
#endif
    menu->setTouchEnabled(false);
    menu->setPosition(CCPointZero);
    this->addChild(menu,kSkipButtonZOder);
	*/
	/*if(canSkip)
	{
		m_pSkipMenu->setTarget(this, menu_selector(BattleView::skipBattle));
	} else {
		m_pSkipMenu->setShaderProgram(D_SHADER_GARY);
		m_pSkipMenu->setEnabled(false);
	}*/
	/*
#ifdef QSMY_DEBUG
	m_pSkipMenu->setTarget(this, menu_selector(BattleView::testPause));
#endif
	*/
	//this->addChild(m_pSkipMenu, kSkipButtonZOder);
	m_pSkipMenu->setTarget(this, menu_selector(BattleView::skipBattle));
}

void BattleView::playEffect(DEffectNode* pEffect)
{
    m_pEffectLayer->addNode(pEffect);
}

UnitView* BattleView::getUnit(uint8_t id)
{
    return (UnitView*)units->objectForKey(id);
}

void BattleView::testPause(CCObject *sender)
{
    m_pScheduler->setTimeScale(m_pScheduler->getTimeScale()!=1.0f?1.0f:0.f);
}

void BattleView::skipBattle(CCObject *sender)
{
    if(m_pDelegate!=NULL)
    {
        m_pDelegate->skipBattle();
    }
}


float BattleView::playSkillCover(ActionSkill *pAction,float delay)
{
    SkillCover* pSkillCover = SkillCover::create(pAction,delay);
    addChild(pSkillCover,kSkillCoverZOrder);
    return pSkillCover->getDuration();
}


float BattleView::playSkill(ActionSkill *pAction,float delay)
{
    battle::Skill *pSkill = m_pModel->getSkill(pAction->skill_id);
    SkillView* pSkillView = SkillView::create(pAction->skill_id,delay);
    const BattleUnit *pUnit = m_pModel->getBattleInfo()->getUnit(pAction->actor);
	skill::Skill *pSkillInfo = pSkill->getSkillInfo();
	float scalex = pUnit->m_pForce->id==(int)kForceRight && pSkillInfo->getSkillFlip() > 0 ? -1.f : 1.f;
    pSkillView->setScaleX(scalex);
    pSkillView->setPosition(m_pBattleGridCenter);
	PL_MOVE_WITH_FLEXIBLE_HEIGHT(pSkillView, 88.0f);
    
    if(pSkill->skill_layer == kSkillLayerTypeUp)
    {
        addChild(pSkillView,kSkillUpZOrder);
    }else
    {
        addChild(pSkillView,kSkillBottomZOrder);
    }
    
    return pSkillView->getDuration();
}

float BattleView::playSkillEffect(ActionSkill *pAction,float delay)
{
    battle::Skill *pSkill = m_pModel->getSkill(pAction->skill_id);
    const BattleUnit *pUnit = m_pModel->getBattleInfo()->getUnit(pAction->actor);
    
    SkillEffect* pSkillEffect = SkillEffect::create(pAction->skill_id,delay);
    
    pSkillEffect->setScaleX(pUnit->m_pForce->id==(int)kForceLeft?1.0f:-1.0f);
    
    CCPoint pos = UnitView::positionToPoint(pAction->position, 1);
    
    if(pAction->target==battle::kSkillTargetTypeSelf && pUnit->m_pForce->id==(int)kForceLeft)
    {
        pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(pos.x,pos.y));
    }else
    {
        pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(-pos.x,pos.y));
    }
    
    switch (pSkill->effect_type)
    {
        case battle::kEffectTypeH:
            pSkillEffect->setPosition(ccp(pos.x,0));
            break;
        case battle::kEffectTypeV:
            pSkillEffect->setPosition(ccp(0,pos.y));
        case battle::kEffectTypeNone:
            pSkillEffect->setPosition(D_DESIGN_POINT_CENTER);
        default:
            break;
    }
    
    if(pSkill->effect_layer == kSkillLayerTypeUp)
    {
        addChild(pSkillEffect,kSkillUpZOrder);
    }else
    {
        addChild(pSkillEffect,kSkillBottomZOrder);
    }
    return pSkillEffect->getDuration();
}


void BattleView::updatePower()
{
    UnitView *pUnitView = NULL;
    CCDictElement* pElement = NULL;
    
    uint64_t m_dPowerLeft = 0;
    uint64_t  m_dPowerRight = 0;
    
    CCDICT_FOREACH(units, pElement)
    {
        pUnitView = (UnitView*)pElement->getObject();
        switch (pUnitView->getData()->m_pForce->id)
        {
            case kForceLeft:
                m_dPowerLeft+=pUnitView->getHp()*pUnitView->getData()->fight_factor;
                break;
            case kForceRight:
                m_dPowerRight+=pUnitView->getHp()*pUnitView->getData()->fight_factor;
                break;
            default:
                break;
        }
    }
    
    float percentLeft = (float)m_dPowerLeft/m_dPowerLeftTotal;
    float percentRight = (float)m_dPowerRight/m_dPowerRightTotal;
	
	uint8_t pGrade = DM_GET_BATTLE_MODEL->getBattleInfo()->grade;
	if(m_dLastPercentLeft > percentLeft)
	{
		if(percentLeft <= 2/3.f && m_dLastPercentLeft > 2/3.f)
		{
			m_pBloodStar[BATTLE_BLOOD_STAR - 1]->playWithName("ani2");
		} 
		if(percentLeft <= 1/3.f && m_dLastPercentLeft > 1/3.f)
		{
			m_pBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani2");
		}
		if(D_FLOAT_EQUALS(0.f, percentLeft) && pGrade < 1)
		{
			m_pBloodStar[0]->playWithName("ani2");
		}
	} else 
	{
		if(percentLeft > 2/3.f && m_dLastPercentLeft <= 2/3.f)
		{
			m_pBloodStar[BATTLE_BLOOD_STAR - 1]->playWithName("ani1");
			m_pBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani1");
			m_pBloodStar[0]->playWithName("ani1");
		} else if(percentLeft > 1/3.f && m_dLastPercentLeft <= 1/3.f)
		{
			m_pBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani1");
			m_pBloodStar[0]->playWithName("ani1");
		} 
	}
	
	if(m_dLastPercentRight > percentRight)
	{
		if(percentRight <= 2/3.f && m_dLastPercentRight > 2/3.f)
		{
			m_pRightBloodStar[BATTLE_BLOOD_STAR - 1]->playWithName("ani2");
		}
		if(percentRight <= 1/3.f && m_dLastPercentRight > 1/3.f)
		{
			m_pRightBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani2");
		}
		if(D_FLOAT_EQUALS(0.f, percentRight) && pGrade > 0)
		{
			m_pRightBloodStar[0]->playWithName("ani2");
		}
	} else 
	{
		if(percentRight > 2/3.f && m_dLastPercentRight <= 2/3.f)
		{
			m_pRightBloodStar[BATTLE_BLOOD_STAR - 1]->playWithName("ani1");
			m_pRightBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani1");
			m_pRightBloodStar[0]->playWithName("ani1");
		} else if(percentRight > 1/3.f && m_dLastPercentRight <= 1/3.f)
		{
			m_pRightBloodStar[BATTLE_BLOOD_STAR - 2]->playWithName("ani1");
			m_pRightBloodStar[0]->playWithName("ani1");
		}
	}
	

	m_dLastPercentLeft = percentLeft;
	m_dLastPercentRight = percentRight;
    m_PowerLeft->setPercent(percentLeft,0.3f);
    m_PowerRight->setPercent(percentRight,0.3f);
    
    
//     cocos2d::CCMoveTo *pMoveLeft = cocos2d::CCMoveTo::create(0.3, ccp(-(1-percentLeft)*m_EffectNodeLeft->getContentSize().width,0));
//     cocos2d::CCMoveTo *pMoveRight = cocos2d::CCMoveTo::create(0.3, ccp(-(1-percentRight)*m_EffectNodeRight->getContentSize().width,0));
//     m_pPowerAniLeft->stopAllActions();
//     m_pPowerAniRight->stopAllActions();
//     m_pPowerAniLeft->runAction(pMoveLeft);
//     m_pPowerAniRight->runAction(pMoveRight);
}

void BattleView::aniEnd(sp::Armature * target, const char* event,const char* data){
	target->getParent()->removeFromParent();
}

void BattleView::removeStar(uint8_t grade){
	for(int i=0; i<BATTLE_BLOOD_STAR; i++){
		if(i < grade) continue;
		if(m_pBloodStar[i])
		{
			m_pBloodStar[i]->playWithName("ani2");
			m_pBloodStar[i]->getArmature()->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::aniEnd), COMPLETE);
		}
	}
	//如果胜利，右边的星星全部清除
	if(grade > 0){
		for(int i=0; i<BATTLE_BLOOD_STAR; i++){
			if(m_pRightBloodStar[i])
			{
				m_pRightBloodStar[i]->playWithName("ani2");
				m_pRightBloodStar[i]->getArmature()->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::aniEnd), COMPLETE);
			}
		}
	}
}

void BattleView::showUnits(bool bShow)
{
	if(!m_bUnitInited && bShow) initUnits();
	if(!units) return;
	CCDictElement *_object = NULL;
	CCDICT_FOREACH(units, _object)
	{
		UnitView *pUnitView = (UnitView*)_object->getObject();
		if(bShow)
		{
			//initUnits();
			//addChild(m_pUnitUILayer,KUnitUIZOder);
			pUnitView->showUnit();
		} else
		{
			pUnitView->hideUnit();
			//m_pUnitUILayer->removeFromParentAndCleanup(false);
		}
	}
	
}
void BattleView::playFateEffect()
{	
	playSideFateEffect(CCInteger::create(kForceLeft));
	SkipBattleType battleType = m_pDelegate->getPrevSkipType();

	if(battleType == kSkipBattleTypePVP || battleType == kSkipBattleTypeTeam)
	{
		runAction(CCSequence::create(CCDelayTime::create(7.6f), CCCallFuncO::create(this, callfuncO_selector(BattleView::playSideFateEffect), CCInteger::create(kForceRight)),
			CCDelayTime::create(7.6f), CCCallFunc::create(this, callfunc_selector(BattleView::startBattle)), NULL));
	} else 
	{
		runAction(CCSequence::create(CCDelayTime::create(7.6f), CCCallFunc::create(this, callfunc_selector(BattleView::startBattle)), NULL));
	}
	
}


//攻防血动画1-4.1秒 缘分加成字3.6秒-6.1秒 爆炸5.8-6.8 技能动画6.1-7.8
void BattleView::playSideFateEffect(CCObject* pObj)
{
	/*bool bPlay = true;
	int force = ((CCInteger*)pObj)->getValue();

	if(force == kForceRight)
	{
		CCObject* obj=NULL;
		BattleUnit* pUnit=NULL;
		CCArray* pFateArray = NULL;
		uint32_t fateActiveNum = 0;
		warrior::WarriorBaseInfo* warriorInfo=NULL;
		CCARRAY_FOREACH(m_pRightUnits, obj)
		{
			pUnit = (BattleUnit*)obj;
			warriorInfo = m_pWarriorModel->getWarriorBaseByWID(pUnit->getAvatarId());
			pFateArray = warriorInfo->getFateArray();
			if(pFateArray)
			{
				for (uint32_t i = 0; i < pFateArray->count(); i++)
				{
					warrior::FateInfo *pFateInfo = (warrior::FateInfo *)pFateArray->objectAtIndex(i);
					if (pFateInfo == NULL && pFateInfo->type != warrior::kFateTypeWarrior) continue;
				
					bool isActivated = false;
					if(force == kForceRight){
						isActivated = DM_GET_OTHERFORMATION_MODEL->checkFateActivated(pFateInfo);
					} else {
						isActivated = m_pWarriorModel->checkFateActivated(pFateInfo);
					}
					if(!isActivated) continue;
					fateActiveNum++;
				}
			}
		}
		if(fateActiveNum < 1)
		{
			bPlay = false;
			startBattle();
		}
	}
	if(bPlay)
	{
		runAction(CCSequence::create(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(BattleView::playTitle)), CCDelayTime::create(2.2f), CCCallFuncO::create(this, callfuncO_selector(BattleView::playFateNum), pObj), NULL));
	}*/
	runAction(CCSequence::create(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(BattleView::playTitle)), 
		CCDelayTime::create(2.2f), CCCallFuncO::create(this, callfuncO_selector(BattleView::playFateNum), pObj), NULL));
}

void BattleView::playTitle()
{
	//缘分加成字3.6秒-6.1秒
	Armature* pArmature = Armature::create(FATE_TITLE_SK,"ani",sp::ZORDER);
	pArmature->setAnchorPoint(ccp(0.5, 0.5));
	pArmature->setPosition(D_DESIGN_POINT_CENTER);
	m_pFateNode->addChild(pArmature, 0);
	//runAction(CCSequence::create(CCDelayTime::create(delay), CCCallFuncO::create(this, callfuncO_selector(BattleView::playArmature), pArmature), NULL));
	pArmature->getAnimation()->playByIndex(0, -1, -1, true);
	pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::onComplete), COMPLETE);
}
void BattleView::playFateNum(cocos2d::CCObject* pObj)
{
	int force = ((CCInteger*)pObj)->getValue();
	CCArray* pUnits = force == kForceRight ? m_pRightUnits : m_pLeftUnits;
	showUnits(false);
	BattleUnit* pUnit=NULL;
	warrior::WarriorBaseInfo* warriorInfo=NULL;
	CCArray* pFateArray = NULL;
	CCSprite* pSpriteStencil = NULL;
	CCClippingNode* pCNode = NULL;
	CCClippingNode* pCNode1 = NULL;
	uint32_t warriorId = 0;
	uint32_t dressId = 0;
	Armature* pArmature0 = NULL;
	Armature* pArmature1 = NULL;
	Armature* pArmature2 = NULL;
	Armature* pArmature = NULL;
	std::string warriorName;
	//m_pFateNode->setPositionY(-50);
	uint32_t fateAtt = 0;
	uint32_t fateDef = 0;
	uint32_t fateHp = 0;
	uint32_t fateActiveNum = 0;
	uint8_t fateBuffNum = 0;
	m_pArmatureArr1->removeAllObjects();
	m_pArmatureArr2->removeAllObjects();
	m_pArmatureArr3->removeAllObjects();
	for(int i=1; i<8; i++)
	{
		pUnit = NULL;
		fateAtt = 0;
		fateDef = 0;
		fateHp = 0;
		fateActiveNum = 0;
		fateBuffNum = 0;
		warriorId = 0;
		if(pUnits->count() > i-1) pUnit = (BattleUnit*)pUnits->objectAtIndex(i-1);
		if(pUnit){
			warriorId = pUnit->sid;
			dressId = pUnit->getAvatarId();
			warriorInfo = m_pWarriorModel->getWarriorBaseByWID(warriorId);
			pFateArray = warriorInfo->getFateArray();
			warriorName = warriorInfo->getWarriorName();
			if(pFateArray)
			{
				for (uint32_t i = 0; i < pFateArray->count(); i++)
				{
					warrior::FateInfo *pFateInfo = (warrior::FateInfo *)pFateArray->objectAtIndex(i);
					if (pFateInfo == NULL) continue;

					bool isActivated = false;
					if(force == kForceRight){
						isActivated = DM_GET_OTHERFORMATION_MODEL->getBattleOtherForamtionInfo()->checkFateActivated(pFateInfo);
					} else {
						isActivated = m_pWarriorModel->checkFateActivated(pFateInfo);
					}
					if(!isActivated) continue;
					fateActiveNum++;
					fateAtt += pFateInfo->add_attack;		
					fateDef += pFateInfo->add_defence;
					fateHp += pFateInfo->add_hp;
				}
			}

		} else {
			warriorId = 0;
		}
		//底
		pArmature0 = Armature::create(CCString::createWithFormat("%s_%d_0",FATE_SK, i)->getCString(),"ani",sp::ZORDER);
		pArmature0->setAnchorPoint(ccp(0.5, 0.5));
		pArmature0->setPosition(D_DESIGN_POINT_CENTER);
		m_pFateNode->addChild(pArmature0, 0);

		//人物层
		pArmature1 = Armature::create(CCString::createWithFormat("%s_%d_1",FATE_SK, i)->getCString(),"ani",sp::ZORDER);
		pArmature1->setAnchorPoint(ccp(0.5, 0.5));
		pArmature1->setPosition(D_DESIGN_POINT_CENTER);
		if(warriorId)
		{
			CCSprite* pIcon = ResourceManager::sharedResourceManager()->getUnitImage(dressId);
			CCSprite* pIcon1 = ResourceManager::sharedResourceManager()->getUnitImage(dressId);
			//pIcon->setPositionY(pIcon->getPositionY()-30);
			pIcon->setAnchorPoint(ccp(0.5, 1));
			pIcon1->setAnchorPoint(ccp(0.5, 1));
			//CCSprite* pIcon1 = ResourceManager::sharedResourceManager()->getUnitImage(warriorId);
			pArmature1->getBone("hero1")->replaceDisplayAt(pIcon, 0);
			pArmature1->getBone("hero2")->replaceDisplayAt(/*DSprite::createWithTexture(pIcon->getTexture(), pIcon->getTextureRect())*/pIcon1, 0);

			CCNode *stencil = CCNode::create();
			pSpriteStencil = CCSprite::create(CCString::createWithFormat("battle/fate/fate_%d_mask.png",i)->getCString());
			stencil->addChild(pSpriteStencil);
			pCNode = CCClippingNode::create();
			pCNode->addChild(pArmature1);

			pCNode->setStencil(stencil);
			pCNode->setAlphaThreshold(0.5);
			pCNode->setPosition(pArmature1->getPosition());
			pArmature1->setPosition(CCPointZero);
			//pCNode->setPositionY(pCNode->getPositionY()+80);
			m_pFateNode->addChild(pCNode, 1);

			//攻防血名字层
			pArmature2 = Armature::create(CCString::createWithFormat("%s_%d_2",FATE_SK, i)->getCString(),"ani",sp::ZORDER);
			pArmature2->setAnchorPoint(ccp(0.5, 0.5));
			pArmature2->setPosition(D_DESIGN_POINT_CENTER);

			m_pFateNode->addChild(pArmature2, 2);

			CCNode *attTTf = CCLabelBMFont::create(CCString::createWithFormat("%d",fateAtt)->getCString(), D_FONT_BATTLE_DMG);
			attTTf->setScale(1.2f);
			SPContainer *attSPContainer =  SPContainer::create(pArmature2->getRenderType());
			attSPContainer->setPositionX(attSPContainer->getPositionX()+10);
			attSPContainer->addChild(attTTf);
			pArmature2->getBone("gong1")->replaceDisplayAt(attSPContainer, 0);
			pArmature2->getBone("gong2")->replaceDisplayAt(DSprite::createWithTexture(attSPContainer->getTexture(), attSPContainer->getTextureRect()), 0);
			pArmature2->getBone("gong3")->replaceDisplayAt(DSprite::createWithTexture(attSPContainer->getTexture(), attSPContainer->getTextureRect()), 0);

			CCNode *defTTf = CCLabelBMFont::create(CCString::createWithFormat("%d",fateDef)->getCString(), D_FONT_BATTLE_DMG);
			defTTf->setScale(1.2f);
			SPContainer *defSPContainer =  SPContainer::create(pArmature2->getRenderType());
			defSPContainer->setPositionX(defSPContainer->getPositionX()+10);
			defSPContainer->addChild(defTTf);

			pArmature2->getBone("fang1")->replaceDisplayAt(defSPContainer, 0);
			pArmature2->getBone("fang2")->replaceDisplayAt(DSprite::createWithTexture(defSPContainer->getTexture(), defSPContainer->getTextureRect()), 0);
			pArmature2->getBone("fang3")->replaceDisplayAt(DSprite::createWithTexture(defSPContainer->getTexture(), defSPContainer->getTextureRect()), 0);

			CCNode *hpTTf = CCLabelBMFont::create(CCString::createWithFormat("%d",fateHp)->getCString(), D_FONT_BATTLE_DMG);
			hpTTf->setScale(1.2f);
			SPContainer *hpSPContainer =  SPContainer::create(pArmature2->getRenderType());
			hpSPContainer->setPositionX(hpSPContainer->getPositionX()+10);
			hpSPContainer->addChild(hpTTf);


			pArmature2->getBone("xue1")->replaceDisplayAt(hpSPContainer, 0);
			pArmature2->getBone("xue2")->replaceDisplayAt(DSprite::createWithTexture(hpSPContainer->getTexture(), hpSPContainer->getTextureRect()), 0);
			pArmature2->getBone("xue3")->replaceDisplayAt(DSprite::createWithTexture(hpSPContainer->getTexture(), hpSPContainer->getTextureRect()), 0);


			SPContainer *pNameSPContainer =  SPContainer::create(pArmature2->getRenderType());
			CCLabelTTF * pLabel = CCLabelTTF::create(warriorName.c_str(), D_FONT_DEFAULT, D_FONT_SIZE_12);
			pNameSPContainer->addChild(pLabel);
			pArmature2->getBone("name1")->replaceDisplayAt(pNameSPContainer, 0);
			pArmature2->getBone("name2")->replaceDisplayAt(DSprite::createWithTexture(pNameSPContainer->getTexture(), pNameSPContainer->getTextureRect()), 0);
		} else 
		{
			m_pFateNode->addChild(pArmature1, 1);
		}

		if(warriorId && fateActiveNum)
		{
			//技能动画6.1-7.8
			fateBuffNum = fateActiveNum > 2 ? 3 : fateActiveNum;
			//pArmature = Armature::create(CCString::createWithFormat("%s_%d",FATE_BUFF_SK, fateBuffNum)->getCString(),SKILL_UNIT_EFFECT_ARMATURE,sp::ZORDER);
			//pArmature->setAnchorPoint(ccp(0.5, 0.5));

			//CCPoint pos = UnitView::positionToPoint(pUnit->position, 1);

			//if(pUnit->m_pForce->id==(int)kForceLeft)
			//{
			//	pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(pos.x,pos.y));
			//}else
			//{
			//	pos = ccpAdd(D_DESIGN_POINT_CENTER, ccp(-pos.x,pos.y));
			//}
			//pArmature->setPosition(pos);
			//PL_MOVE_WITH_FLEXIBLE_HEIGHT(pArmature, 88.f); 
			//m_pFateNode->addChild(pArmature, 3);
			////m_pUnitUILayer->addChild(pArmature, -3000);
			//runAction(CCSequence::create(CCDelayTime::create(4.1f), CCCallFuncO::create(this, callfuncO_selector(BattleView::playArmature), pArmature), NULL));
			//pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::onComplete), COMPLETE);
			UnitView* unitView = getUnit(pUnit->id);
			if(unitView){
				unitView->playFateAddBuff(fateBuffNum, 4.1f);
			}
		}

		m_pArmatureArr1->addObject(pArmature0);
		m_pArmatureArr2->addObject(pArmature1);
		m_pArmatureArr3->addObject(pArmature2);
	}
	CCObject* obj=NULL;
	CCARRAY_FOREACH(m_pArmatureArr1, obj)
	{
		pArmature = (Armature*)obj;
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::onMovementEvent), COMPLETE);
		pArmature->getAnimation()->playByIndex(0, -1, -1, true);
	}
	CCARRAY_FOREACH(m_pArmatureArr2, obj)
	{
		pArmature = (Armature*)obj;
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::onMovementEvent), COMPLETE);
		pArmature->getAnimation()->playByIndex(0, -1, -1, true);
	}
	CCARRAY_FOREACH(m_pArmatureArr3, obj)
	{
		pArmature = (Armature*)obj;
		pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(BattleView::onMovementEvent), COMPLETE);
		pArmature->getAnimation()->playByIndex(0, -1, -1, true);
	}

}

void BattleView::onMovementEvent(Armature *_armature, const char *_eventType, const char *_movementID)
{
	if(strcmp(_eventType, COMPLETE)==0)
	{
		_armature->removeFromParent();
		/*Armature* pArmature = NULL;
		CCObject* obj=NULL;
		CCARRAY_FOREACH(m_pArmatureArr1, obj)
		{
		pArmature = (Armature*)obj;
		pArmature->removeFromParentAndCleanup(true);
		}

		CCARRAY_FOREACH(m_pArmatureArr2, obj)
		{
		pArmature = (Armature*)obj;
		pArmature->removeFromParentAndCleanup(true);
		}

		CCARRAY_FOREACH(m_pArmatureArr3, obj)
		{
		pArmature = (Armature*)obj;
		pArmature->removeFromParentAndCleanup(true);
		}*/

		showUnits(true);
	}
}

void BattleView::onComplete(Armature *_armature, const char *_eventType, const char *_movementID)
{
	_armature->removeFromParent();
}

void BattleView::playArmature(CCObject* obj)
{
	Armature* pArmature = (Armature*)obj;
	pArmature->getAnimation()->playByIndex(0, -1, -1, true);
}

void BattleView::startBattle()
{
	m_pDelegate->beforeStartBattle();
}