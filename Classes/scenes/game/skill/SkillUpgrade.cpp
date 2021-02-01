//  SkillUpgrade.cpp
//  突破界面
//  Created by cll on 13-5-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#define SFX_SKILL_UPGRADE CCString::create("skill_lvup")

#include "SkillUpgrade.h"
#include "managers/GameManager.h"
#include "managers/ResourceManager.h"
#include "../formation/Formation.h"
#include "managers/TipsManager.h"
#include "../user/UserInfo.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

SkillUpgradeView::SkillUpgradeView()
:m_pDelegate(NULL)
,m_pSkillModel(NULL)
,m_pWarriorModel(NULL)
,m_pWarriorInfo(NULL)
,m_pCurUpLevel(NULL)
,m_pToUpLevel(NULL)
,m_pBigSkillLevel(NULL)
,m_pCurProps(NULL)
,m_pToProps(NULL)
,m_pSubmitButton(NULL)
,m_pSkillBox(NULL)
,m_pEffect(NULL)
,m_pEffectNode(NULL)
,m_pReturnButton(NULL)
,m_pCloseButton(NULL)
,m_pIconContainer(NULL)
,m_pItemName(NULL)
,m_pItemNeedCount(NULL)
,m_pItemCurrentCount(NULL)
{
    D_RESET_C_ARRAY(m_pSkillType,skill::kSkillTypes);
}

SkillUpgradeView::~SkillUpgradeView()
{
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
    
    CC_SAFE_RELEASE(m_pCurUpLevel);
    CC_SAFE_RELEASE(m_pToUpLevel);
    CC_SAFE_RELEASE(m_pBigSkillLevel);
    CC_SAFE_RELEASE(m_pCurProps);       //当前属性
    CC_SAFE_RELEASE(m_pToProps);        //提升后属性
    
    CC_SAFE_RELEASE(m_pSubmitButton);
    CC_SAFE_RELEASE(m_pSkillBox);
    CC_SAFE_RELEASE(m_pEffect);
    CC_SAFE_RELEASE(m_pEffectNode);

	CC_SAFE_RELEASE(m_pReturnButton);
	CC_SAFE_RELEASE(m_pCloseButton);
	CC_SAFE_RELEASE(m_pIconContainer);
	CC_SAFE_RELEASE(m_pItemName);
	CC_SAFE_RELEASE(m_pItemNeedCount);
	CC_SAFE_RELEASE(m_pItemCurrentCount);
	D_SAFE_RELEASE_C_ARRAY(m_pSkillType,skill::kSkillTypes);
}




bool SkillUpgradeView::init()
{
    
	if(!DLayer::init())
	{
		return false;
	}
    
	m_pSkillModel = DM_GET_SKILL_MODEL;
	m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("SkillBox", SkillBoxLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_jineng/PL_jinengshengji", pNodeLoaderLibrary);

    m_pSubmitButton->getButton()->setTarget(this, menu_selector(SkillUpgradeView::menuCallBack));
    m_pReturnButton->getButton()->setTarget(this, menu_selector(SkillUpgradeView::closeCallBack));
	m_pCloseButton->getButton()->setTarget(this, menu_selector(SkillUpgradeView::closeCallBack));
    
	return true;
}


bool SkillUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurUpLevel", CCLabelBMFont *, this->m_pCurUpLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToUpLevel", CCLabelBMFont *, this->m_pToUpLevel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurProps", CCLabelBMFont *, this->m_pCurProps);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pToProps", CCLabelBMFont *, this->m_pToProps);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillBox", SkillBox *, this->m_pSkillBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton", DButton *, this->m_pSubmitButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pEffectNode", CCNode *, this->m_pEffectNode);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBigSkillLevel", CCLabelBMFont *, this->m_pBigSkillLevel);
     
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pReturnButton", DButton *, this->m_pReturnButton);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCloseButton", DButton *, this->m_pCloseButton);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pIconContainer", CCNode *, this->m_pIconContainer);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemName", CCLabelTTF *, this->m_pItemName);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pNeedCount", CCLabelBMFont *, this->m_pItemNeedCount);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCurrentCount", CCLabelBMFont *, this->m_pItemCurrentCount);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillType0" , CCNode*, m_pSkillType[0]);
	  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSkillType1" , CCNode*, m_pSkillType[1]);
    return false;
}

SEL_MenuHandler SkillUpgradeView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", SkillUpgradeView::closeCallBack);
    return NULL;
}

void SkillUpgradeView::setWarriorInfo(warrior::WarriorCurInfo * pWarrior)
{
	if(m_pWarriorInfo == pWarrior) return;
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	CC_SAFE_RETAIN(pWarrior);
    m_pWarriorInfo= pWarrior;
	binding();
}

void SkillUpgradeView::onEnter()
{
	binding();
	DLayer::onEnter();
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());	//添加用户基本信息
}


void SkillUpgradeView::onExit()
{
	UNBINDING_ALL(m_pWarriorInfo, this);
	DLayer::onExit();
}

void SkillUpgradeView::binding()
{
    BINDING_EXEC(m_pWarriorInfo,this,SkillUpgradeView::updateSkillView,D_EVENT_WARRIORINFO_UPDATE);
}


void SkillUpgradeView::menuCallBack(CCObject * obj)
{
	skill::Skill * pSkillInfo = m_pSkillModel->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
	item::ItemInfo* pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ITEM_NEILIDAN_ID);
	uint32_t curNum = 0;
	if (pItemInfo)
	{
		curNum = pItemInfo->getNum();
	}
	
	uint32_t needNum = m_pSkillModel->getSkillUpExp(pSkillInfo->grade, m_pWarriorInfo->getSlv()) - m_pWarriorInfo->getSexp();
	if (curNum < needNum)
	{
		NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS, CCInteger::create(ITEM_NEILIDAN_ID));
		return;
	}
	
    UNBINDING_ALL(m_pWarriorInfo,this); //这里不用绑定刷新 防止数据更新顺序不对
	HTTP_CLIENT->upgradeSkill(m_pWarriorInfo->getWid(),this,callfuncJson_selector(SkillUpgradeView::upgradeCallBack));
}

void SkillUpgradeView::upgradeCallBack(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    binding(); //这里保证数据都已经更新完毕 重新绑定 防止数据变化
	if(m_pEffect==NULL)
    {
        m_pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("skillupgrade");
        m_pEffect->getAnimation()->addObserver(this, SPNotification_callfunc_selector(SkillUpgradeView::animateEnd), COMPLETE);
        CC_SAFE_RETAIN(m_pEffect);
    }
    if(m_pEffect)
    {
        m_pEffect->removeFromParent();
        m_pEffect->getAnimation()->stop();
        m_pEffect->getAnimation()->playByIndex(0,-1,-1,true);
        m_pEffectNode->addChild(m_pEffect);
    }
	CCNotificationCenter::sharedNotificationCenter()->postNotification(PLAY_SFX, SFX_SKILL_UPGRADE);
}

void SkillUpgradeView::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    if(target) target->removeFromParent();
}


void SkillUpgradeView::closeCallBack(cocos2d::CCObject* obj)
{
    if(m_pDelegate) m_pDelegate->closeInfoView(kSkillUpgradeView);
    else removeFromParent();
}


void SkillUpgradeView::updateSkillView()
{
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("SkillInfoError").c_str());
		return ;
	}
    
	skill::Skill * pSkillInfo = m_pSkillModel->getSkill(m_pWarriorInfo->getBaseInfo()->getSkillId());
	item::ItemInfo* pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(ITEM_NEILIDAN_ID);
	item::ItemBase* pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(ITEM_NEILIDAN_ID);
    
	for(uint8_t i =0;i<skill::kSkillTypes;i++)
	{
		m_pSkillType[i]->setVisible(pSkillInfo->type == i);
	}

    m_pSkillBox->updateView(pSkillInfo, m_pWarriorInfo->getSlv());
	m_pBigSkillLevel->setString(CCString::createWithFormat("%d", m_pWarriorInfo->getSlv())->getCString());
    
	float attack_ratio = pSkillInfo->getAttackRatio(m_pWarriorInfo->getSlv());
	float attack_ratio_to = pSkillInfo->getAttackRatio(m_pWarriorInfo->getSlv()+1);
    
	m_pIconContainer->removeAllChildren();
	m_pIconContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(ITEM_NEILIDAN_ID));
    
    uint32_t needNum = m_pSkillModel->getSkillUpExp(pSkillInfo->grade, m_pWarriorInfo->getSlv()) - m_pWarriorInfo->getSexp();
    if(m_pWarriorInfo->checkSkillLimit()) needNum =0;
    
    
    m_pItemNeedCount->setString(CCString::createWithFormat("X%d",needNum)->getCString());
	m_pItemName->setString(pItemBase->getItemName().c_str());
    
    uint32_t curNum = pItemInfo?pItemInfo->getNum():0;
	m_pItemCurrentCount->setString(CCString::createWithFormat("%d",curNum)->getCString());
    
    m_pCurUpLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getSlv())->getCString());
    m_pCurProps->setString(CCString::createWithFormat("x%3.0f%%", attack_ratio*100.0f)->getCString());
    if(!m_pWarriorInfo->checkSkillLimit())
    {
        m_pToUpLevel->setString(CCString::createWithFormat("%d",m_pWarriorInfo->getSlv()+1)->getCString());
        m_pToProps->setString(CCString::createWithFormat("x%3.0f%%",attack_ratio_to*100.0f)->getCString());
        
    }else
    {
        m_pToProps->setString("");
    }

    m_pSubmitButton->getButton()->setEnabled(!m_pWarriorInfo->checkSkillLimit()/* && curNum >= needNum*/);
    //if(m_pWarriorInfo->checkSkillLimit()) closeCallBack(NULL);
}


/////////////////////////////////////////////////////