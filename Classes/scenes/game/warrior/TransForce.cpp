//  TransForce.cpp
//  传功界面
//  Created by cll on 13-3-29
//  Copyright (c) 2013, thedream. All rights reserved.
//

#include "TransForce.h"
#include "Warrior.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "../Game.h"
#include "managers/TipsManager.h"

USING_NS_CC;
using namespace warrior;

#define ITEM_NEED 1




bool TransForceWarriorFilter::operator()(warrior::WarriorCurInfo *pWarrior)
{
    return pWarrior->getWarriorLv() > 1 && pWarrior->getWid() != m_ignoreWarriorId;
};

TransForceView::TransForceView()
:m_pDelegate(NULL)
,m_pWarriorInfo(NULL)
,m_pWarriorInfoFrom(NULL)
,m_pWarriorModel(NULL)
,m_pSelectFrom(NULL)
,m_pFrom(NULL)
,m_pTo(NULL)
,m_pSelectFromEffect(NULL)

,m_pLabelExpNormal(NULL)
,m_pLabelLvNormal(NULL)
,m_pLabelSkillNormal(NULL)
,m_pLabelNumNormal(NULL)
,m_pLabelExpGreat(NULL)
,m_pLabelLvGreat(NULL)
,m_pLabelSkillGreat(NULL)
,m_pLabelNumGreat(NULL)
{
	items[0] = CHUANGONG_DAN_ID1;
	items[1] = CHUANGONG_DAN_ID2;
	types[0] = WARRIOR_TRANSFORCE_NORMAL;
	types[1] = WARRIOR_TRANSFORCE_ADV;

    for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
    {
        m_pSubmitButton[i] = NULL;
        m_pItemCount[i] = NULL;
        m_pItemBox[i] = NULL;
    }
}

TransForceView::~TransForceView()
{
	removeAllChildren();
	SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo,this);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfoFrom,this);
    
    CC_SAFE_RELEASE(m_pSelectFrom);
    CC_SAFE_RELEASE(m_pFrom);
    CC_SAFE_RELEASE(m_pTo);
    CC_SAFE_RELEASE(m_pSelectFromEffect);
    
    for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
    {
        CC_SAFE_RELEASE(m_pSubmitButton[i]);
        CC_SAFE_RELEASE(m_pItemCount[i]);
        CC_SAFE_RELEASE(m_pItemBox[i]);
    }
    
	CC_SAFE_RELEASE(m_pLabelExpNormal);
	CC_SAFE_RELEASE(m_pLabelLvNormal);
	CC_SAFE_RELEASE(m_pLabelSkillNormal);
	CC_SAFE_RELEASE(m_pLabelNumNormal);
	CC_SAFE_RELEASE(m_pLabelExpGreat);
	CC_SAFE_RELEASE(m_pLabelLvGreat);
	CC_SAFE_RELEASE(m_pLabelSkillGreat);
	CC_SAFE_RELEASE(m_pLabelNumGreat);
}

bool TransForceView::init()//uint32_t wid)
{
    
	if(!DLayer::init())
	{
		return false;
	}
    
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
	pNodeLoaderLibrary->registerCCNodeLoader("TransForceHead", TransForceHeadLoader::loader());

    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/warrior/transForce", pNodeLoaderLibrary);
    
    
    for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
    {
        m_pSubmitButton[i]->getButton()->setTarget(this, menu_selector(TransForceView::submitCallBack));
        m_pSubmitButton[i]->getButton()->setTag(i);
    }
    
    m_pFrom->reset();
    m_pTo->reset();
    m_pSelectFromEffect = ResourceManager::sharedResourceManager()->getUIAnimate("trans_add_button");
    CC_SAFE_RETAIN(m_pSelectFromEffect);
    updateTransForceFrom();
	return true;
}


bool TransForceView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSelectFrom", CCNode *, this->m_pSelectFrom);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton0", DButton *, this->m_pSubmitButton[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmitButton1", DButton *, this->m_pSubmitButton[1]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCount0", CCLabelBMFont *, this->m_pItemCount[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemCount1", CCLabelBMFont *, this->m_pItemCount[1]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox0", ItemBox *, this->m_pItemBox[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pItemBox1", ItemBox *, this->m_pItemBox[1]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFrom", TransForceHead *, this->m_pFrom);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTo", TransForceHead *, this->m_pTo);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelExpNormal", CCLabelBMFont *, this->m_pLabelExpNormal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelLvNormal", CCLabelBMFont *, this->m_pLabelLvNormal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelSkillNormal", CCLabelBMFont *, this->m_pLabelSkillNormal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelNumNormal", CCLabelBMFont *, this->m_pLabelNumNormal);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelExpGreat", CCLabelBMFont *, this->m_pLabelExpGreat);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelLvGreat", CCLabelBMFont *, this->m_pLabelLvGreat);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelSkillGreat", CCLabelBMFont *, this->m_pLabelSkillGreat);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLabelNumGreat", CCLabelBMFont *, this->m_pLabelNumGreat);
    
    return false;
}

SEL_MenuHandler TransForceView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", TransForceView::closeCallBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "selectCallBack", TransForceView::selectCallBack);
    
    return NULL;
}

void TransForceView::setWarrior(warrior::WarriorCurInfo *pWarrior)
{
	if (pWarrior == m_pWarriorInfo) return;
    CC_SAFE_RETAIN(pWarrior);
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfo, this);
	m_pWarriorInfo = pWarrior;
	binding();
}

void TransForceView::setFromWarriorID(const uint32_t &wid)
{
    warrior::WarriorCurInfo *pWarrior = DM_GET_WARRIOR_MODEL->getWarriorCurInfoByWID(wid);
    if (pWarrior == m_pWarriorInfoFrom) return;
    SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfoFrom, this);
    CC_SAFE_RETAIN(pWarrior);
    m_pWarriorInfoFrom = pWarrior;
	BINDING_EXEC(m_pWarriorInfoFrom,this,TransForceView::updateTransForceFrom,D_EVENT_WARRIORINFO_UPDATE);
}


void TransForceView::binding()
{
	BINDING_EXEC(m_pWarriorInfo,this,TransForceView::updateTransForce,D_EVENT_WARRIORINFO_UPDATE);
}


void TransForceView::onEnter()
{
	binding();
	DLayer::onEnter();
    
}

void TransForceView::onExit()
{
	//SAFE_RELEASE_UNBINDING_ALL(m_pWarriorInfoFrom, this);
    //m_pWarriorInfoFrom = NULL;
    setFromWarriorID(0); //reset
	DLayer::onExit();
}


//void TransForceView::updateButtonStatus()
//{
//
//    if(!m_pWarriorInfoFrom || m_pWarriorInfoFrom->warriorLv<=1 || m_pWarriorInfo->checkLevelLimit()) //等级满了 或者没有选合适的弟子
//    {
//        for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
//        {
//            m_pSubmitButton[i]->getButton()->setEnabled(false);
//        }
//        return;
//    }
//
//    uint32_t items[USE_ITEM_TYPES] = {CHUANGONG_DAN_ID1,CHUANGONG_DAN_ID2};
//    uint32_t itemCurrent = 0;
//
//    for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
//    {
//        item::ItemInfo * pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(items[i]);
//        itemCurrent = pItemInfo?pItemInfo->num:0;
//        m_pSubmitButton[i]->getButton()->setEnabled(itemCurrent>=ITEM_NEED);
//    }
//}

void TransForceView::updateTransForceFrom()
{
    
    m_pSelectFrom->setVisible(m_pWarriorInfoFrom==NULL);
    m_pSelectFromEffect->removeFromParent();
    if(m_pSelectFrom->isVisible())
    {
        m_pSelectFrom->addChild(m_pSelectFromEffect);
    }
    m_pFrom->setWarriorInfo(m_pWarriorInfoFrom);
    
	// updateButtonStatus();

	updateTransForceExp();
}

void TransForceView::updateTransForce()
{
    
	if (!m_pWarriorInfo || !m_pWarriorInfo->getBaseInfo())
	{
		Tips_Error(D_LOCAL_STRING("WarriorInfoError").c_str());
		return ;
	}
    
	m_pTo->setWarriorInfo(m_pWarriorInfo);
    
    for (uint8_t i=0; i<USE_ITEM_TYPES; ++i)
    {
        item::ItemInfo * pItemInfo = DM_GET_ITEM_MODEL->getItemInfoByID(items[i]);
        item::ItemBase * pItemBase = DM_GET_ITEM_MODEL->getItemBaseByID(items[i]);
        m_pItemBox[i]->getContainer()->removeAllChildren();
        m_pItemBox[i]->getContainer()->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(items[i]));
        m_pItemBox[i]->getName()->setString(pItemBase->getItemName().c_str());
        m_pItemBox[i]->getCount()->setString(CCString::createWithFormat("X%d",ITEM_NEED)->getCString());
        m_pItemCount[i]->setString(CCString::createWithFormat("%d",pItemInfo?pItemInfo->getNum():0)->getCString());
    }
    
	//updateButtonStatus();

	updateTransForceExp();
}

void TransForceView::updateTransForceExp()
{
	uint32_t expNormal = 0;
	uint32_t expGreat = 0;
	uint32_t skillNormal = 0;
	uint32_t skillGreat = 0;
	uint32_t numNormal = 0;
	uint32_t numGreat = 0;
	if (m_pWarriorInfoFrom)
	{	
		expNormal = warrior::TransferExpNormal(m_pWarriorInfoFrom->getWarriorExp());
		expGreat = warrior::TransferExpGreat(m_pWarriorInfoFrom->getWarriorExp());
		skillNormal = warrior::TransferExpNormal(m_pWarriorInfoFrom->getSexp());
		skillGreat = warrior::TransferExpGreat(m_pWarriorInfoFrom->getSexp());
		numNormal = warrior::ReturnItemNormal(m_pWarriorInfoFrom->getDevelopItemNum());
		numGreat = warrior::ReturnItemGreat(m_pWarriorInfoFrom->getDevelopItemNum());
	}
	if (m_pWarriorInfo)
	{
		m_pLabelExpNormal->setString(CCString::createWithFormat("%u", expNormal)->getCString());
		m_pLabelExpGreat->setString(CCString::createWithFormat("%u", expGreat)->getCString());
		warrior::WarriorCurInfo *m_pWarriorNormal = warrior::WarriorCurInfo::create(m_pWarriorInfo->getWid());
		m_pWarriorNormal->addExp(m_pWarriorInfo->getWarriorExp() + expNormal);
		warrior::WarriorCurInfo *m_pWarriorGreat = warrior::WarriorCurInfo::create(m_pWarriorInfo->getWid());
		m_pWarriorGreat->addExp(m_pWarriorInfo->getWarriorExp() + expGreat);
		skillNormal += m_pWarriorInfo->getSexp();
		skillGreat += m_pWarriorInfo->getSexp();
		uint32_t skillID = m_pWarriorInfo->getBaseInfo()->getSkillId();
		m_pLabelSkillNormal->setString(CCString::createWithFormat("%u", DM_GET_SKILL_MODEL->getSkillLevelByExp(skillID, skillNormal))->getCString());
		m_pLabelSkillGreat->setString(CCString::createWithFormat("%u", DM_GET_SKILL_MODEL->getSkillLevelByExp(skillID, skillGreat))->getCString());
		m_pLabelLvNormal->setString(CCString::createWithFormat("%u", m_pWarriorNormal->getWarriorLv())->getCString());
		m_pLabelLvGreat->setString(CCString::createWithFormat("%u", m_pWarriorGreat->getWarriorLv())->getCString());
		m_pLabelNumNormal->setString(CCString::createWithFormat("%u", numNormal)->getCString());
		m_pLabelNumGreat->setString(CCString::createWithFormat("%u", numGreat)->getCString());
	}

}

void TransForceView::closeCallBack(cocos2d::CCObject * obj)
{
    if(m_pDelegate)	m_pDelegate->closeInfoView(kTransForceView);
    else removeFromParent();
}

void TransForceView::selectCallBack(cocos2d::CCObject * obj)
{
    TransForceWarriorFilter * pFilter = new TransForceWarriorFilter(m_pWarriorInfo->getWid());
    WarriorSelectPopup * pPopup = WarriorSelectPopup::create(pFilter);
    pFilter->release();
    pPopup->setDelegate(this);
    pPopup->setSelectedWarrior(m_pWarriorInfoFrom);
    pPopup->setOrder(false);
    
    addChild(pPopup);
}

void TransForceView::submitCallBack(cocos2d::CCObject * obj)
{
    CCMenuItem * pMenuItem = (CCMenuItemImage*) obj;
    int index = pMenuItem->getTag();
    
    if(m_pWarriorInfo->checkLevelLimit())
    {
        Tips_Alert(D_LOCAL_STRING("WarriorIsMaxLevel").c_str());
        return;
    }
    
    if(DM_GET_ITEM_MODEL->getItemNum(items[index])<=0)
    {
        NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_NOT_ENOUGH_ITEMS, CCInteger::create(items[index]));
        return;
    }
    
    if(!m_pWarriorInfoFrom || m_pWarriorInfoFrom->getWarriorLv()<=1) //没有选合适的弟子
    {
        Tips_Alert(D_LOCAL_STRING("selectWarrior").c_str());
        return;
    }
    
    
    UNBINDING(m_pWarriorInfo, this, D_EVENT_WARRIORINFO_UPDATE);
    HTTP_CLIENT->inheritWarrior(m_pWarriorInfoFrom->getWid(),m_pWarriorInfo->getWid(),types[index],this,callfuncJson_selector(TransForceView::submitCallBack));
}

void TransForceView::submitCallBack(const Json::Value &requestData,const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
	m_pDelegate->showInfoView(kTransSettleView, m_pWarriorInfo);
    BINDING_EXEC(m_pWarriorInfo,this,TransForceView::updateTransForce,D_EVENT_WARRIORINFO_UPDATE);
}



/////////////////////////////////////////////////////