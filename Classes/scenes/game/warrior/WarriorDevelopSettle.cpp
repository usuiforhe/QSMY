//  WarriorDevelopSettle.cpp
//  培养结算界面
//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "WarriorDevelopSettle.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "Warrior.h"
#include "managers/TipsManager.h"

USING_NS_CC;


WarriorDevelopSettleView::WarriorDevelopSettleView()
:m_pWarriorInfo(NULL)
,m_pWarriorModel(NULL)
,m_pContainer(NULL)
,m_pAttack(NULL)
,m_pDefence(NULL)
//,m_pSpeed(NULL)
,m_pHp(NULL)
,m_pGrade(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pGiveup(NULL)
,m_pSubmit(NULL)
,m_pCurPot(NULL)
,m_pPotCost(NULL)
,m_pItemNum(NULL)
,m_pItemCost(NULL)
,m_pEffectNode(NULL)
,m_pAttackAdd(NULL)
,m_pDefenceAdd(NULL)
,m_pHpAdd(NULL)
//,m_pSpeedAdd(NULL)
,m_pAttackAni(NULL)
,m_pDefenceAni(NULL)
,m_pHpAni(NULL)
//,m_pSpeedAni(NULL)
,m_pWarriorTypeMark(NULL)
,m_pDelegate(NULL)
{
  
}

WarriorDevelopSettleView::~WarriorDevelopSettleView()
{
    removeAllChildren();
    
    CC_SAFE_RELEASE(m_pContainer);      //弟子头像
	CC_SAFE_RELEASE(m_pAttack);
	CC_SAFE_RELEASE(m_pDefence);
	//CC_SAFE_RELEASE(m_pSpeed);
	CC_SAFE_RELEASE(m_pHp);
	CC_SAFE_RELEASE(m_pGrade);			//品阶
	CC_SAFE_RELEASE(m_pLevel);
	CC_SAFE_RELEASE(m_pName);
    
    CC_SAFE_RELEASE(m_pGiveup);
    CC_SAFE_RELEASE(m_pSubmit);
    
    CC_SAFE_RELEASE(m_pCurPot);
    CC_SAFE_RELEASE(m_pPotCost);
    CC_SAFE_RELEASE(m_pItemNum);
    CC_SAFE_RELEASE(m_pItemCost);
    
    CC_SAFE_RELEASE(m_pEffectNode);
    
    CC_SAFE_RELEASE(m_pAttackAdd);
	CC_SAFE_RELEASE(m_pDefenceAdd);
	//CC_SAFE_RELEASE(m_pSpeedAdd);
	CC_SAFE_RELEASE(m_pHpAdd);
    
    
    CC_SAFE_RELEASE(m_pAttackAni);
    CC_SAFE_RELEASE(m_pDefenceAni);
    //CC_SAFE_RELEASE(m_pSpeedAni);
    CC_SAFE_RELEASE(m_pHpAni);
    CC_SAFE_RELEASE(m_pWarriorTypeMark);
}

bool WarriorDevelopSettleView::init()
{
    
	if(!DPopup::init())
	{
		return false;
	}
    
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    CCB_READER_AND_ADDCHILD("ui/ccb/warrior/warriorDevelopSettle");
    
    m_pSubmit->getButton()->setTarget(this, menu_selector(WarriorDevelopSettleView::submitCallBack));
    m_pGiveup->getButton()->setTarget(this, menu_selector(WarriorDevelopSettleView::closeCallBack));
	
	return true;
}

bool WarriorDevelopSettleView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttack", CCLabelBMFont *, this->m_pAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefence", CCLabelBMFont *, this->m_pDefence);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeed", CCLabelBMFont *, this->m_pSpeed);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHp", CCLabelBMFont *, this->m_pHp);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackAdd", CCLabelBMFont *, this->m_pAttackAdd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceAdd", CCLabelBMFont *, this->m_pDefenceAdd);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedAdd", CCLabelBMFont *, this->m_pSpeedAdd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpAdd", CCLabelBMFont *, this->m_pHpAdd);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAttackAni", CCNode *, this->m_pAttackAni);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pDefenceAni", CCNode *, this->m_pDefenceAni);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSpeedAni", CCNode *, this->m_pSpeedAni);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHpAni", CCNode *, this->m_pHpAni);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGiveup", DButton *, this->m_pGiveup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmit", DButton *, this->m_pSubmit);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurPot", CCLabelBMFont *, this->m_pCurPot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pPotCost", CCLabelBMFont *, this->m_pPotCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemNum", CCLabelBMFont *, this->m_pItemNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCost", CCLabelBMFont *, this->m_pItemCost);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorTypeMark", WarriorTypeMark *, this->m_pWarriorTypeMark);
    
    
    return false;
}

void WarriorDevelopSettleView::onEnter()
{
	updateWarriorSettle();
	DPopup::onEnter();
}


void WarriorDevelopSettleView::onExit()
{
	
	DPopup::onExit();
}


void WarriorDevelopSettleView::updateWarriorSettle()
{
    
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
    m_pEffectNode->removeAllChildren();
    m_pEffectNode->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("deyelop"));
    
    m_pContainer->removeAllChildren();
	m_pContainer->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon50PForDetail(m_pWarriorInfo->getAvatarId()));
	
	m_pGrade->setColor(m_pWarriorModel->getWarriorColorByGrade(m_pWarriorInfo->getBaseInfo()->getRealGrade()));
	m_pLevel->setString(CCString::createWithFormat("%i",m_pWarriorInfo->getWarriorLv())->getCString());
	m_pName->setString(m_pWarriorInfo->getBaseInfo()->getWarriorName().c_str());
	m_pAttack->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeAttak))->getCString());
	m_pDefence->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeDefence))->getCString());
	m_pHp->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeHp))->getCString());
	//m_pSpeed->setString(CCString::createWithFormat("%d",m_pWarriorInfo->computeWarriorFinalProps(warrior::kPropTypeSpeed))->getCString());
	
    
    m_pWarriorTypeMark->setWarriorInfo(m_pWarriorInfo->getBaseInfo());
    
    item::ItemInfo * itemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(PEIYANG_DAN_ID);
    
    m_pCurPot->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getPotential())->getCString());
    
    
	warrior::PotBuff * pPotBuff = m_pWarriorModel->getWarriorPotBuff(m_pWarriorInfo);
        
	//uint32_t costPot = pPotBuff->hp+pPotBuff->att+pPotBuff->def;
    m_pPotCost->setString(CCString::createWithFormat("- %d",pPotBuff->cost)->getCString());
    
    uint32_t num = pPotBuff->type>warrior::PotBuff::kTypeSpecial?K_WARRIOR_DEV_ONCE_MAX:1;
    
    m_pItemCost->setString(CCString::createWithFormat("- %d",num)->getCString());
    m_pItemNum->setString(CCString::createWithFormat("%d",itemInfo?itemInfo->getNum()+num:num)->getCString());

    
    showAtt(m_pAttackAni,m_pAttackAdd,pPotBuff->att);
    showAtt(m_pDefenceAni,m_pDefenceAdd,pPotBuff->def);
    //showAtt(NULL,m_pSpeedAdd,0);
    showAtt(m_pHpAni,m_pHpAdd,pPotBuff->hp);
    
}

void WarriorDevelopSettleView::showAtt(cocos2d::CCNode* pAniNode, cocos2d::CCLabelBMFont* pLabel,int val)
{
    if(pAniNode)
    {
        pAniNode -> removeAllChildren();
        CCNodeRGBA* pw = ResourceManager::sharedResourceManager()->getUIAnimate("aw");
        if(val!=0)
        {
            pAniNode->addChild(pw);
            pw->setColor(ccc3(255, 255, 255));
        }
        if(val<0)
        {
            pw->setColor(ccc3(80, 100, 145));
            pw->setScaleY(-1);
        }
    }
    
    std::string sign = val>=0?"+":"";
    pLabel->setString(CCString::createWithFormat("%s %d",sign.c_str(),val)->getCString());
}


void WarriorDevelopSettleView::closeCallBack(cocos2d::CCObject * obj)
{
    HTTP_CLIENT->confDevelopWarrior(m_pWarriorInfo->getWid(),WARRIOR_DEV_CANCEL,this,callfuncJson_selector(WarriorDevelopSettleView::close));
	//closePopup();
}

void WarriorDevelopSettleView::submitCallBack(CCObject * obj)
{
	HTTP_CLIENT->confDevelopWarrior(m_pWarriorInfo->getWid(),WARRIOR_DEV_CONFIRM,this,callfuncJson_selector(WarriorDevelopSettleView::close));
	//closePopup();
}

void WarriorDevelopSettleView::close(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	if(m_pDelegate) m_pDelegate->showInfoView(kDevelopView, m_pWarriorInfo);
	closePopup();
}

void WarriorDevelopSettleView::onBackKey()
{

}
