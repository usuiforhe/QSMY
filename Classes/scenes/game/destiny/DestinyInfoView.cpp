//
//  DestinyInfoView.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyInfoView.h"
#include "managers/DataManager.h"
#include "managers/ResourceManager.h"
#include "DestinyWarriorListCell.h"
#include "network/ErrorCode.h"

USING_NS_CC;
USING_NS_CC_EXT;

DestinyInfoView::DestinyInfoView()
:m_pDestinyInfo(NULL)
,m_DestinyDelegate(NULL)

,m_pProperty(NULL)
,m_pLevel(NULL)
,m_pName(NULL)
,m_pHighLight(NULL)
,m_pFullEffect(NULL)
,m_pGrade(NULL)
,m_pContainer(NULL)
,m_pProgressBar(NULL)

,m_pWarriorModel(NULL)
,m_pDestinyModel(NULL)
{
    
}

DestinyInfoView::~DestinyInfoView()
{
    CC_SAFE_RELEASE(m_pProperty);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pName);
    CC_SAFE_RELEASE(m_pHighLight);
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pProgressBar);
    CC_SAFE_RELEASE(m_pDestinyInfo);
    CC_SAFE_RELEASE(m_pFullEffect);
}

void DestinyInfoView::setDestinyInfo(destiny::DestinyInfo* pDestinyInfo)
{
    CC_SAFE_RETAIN(pDestinyInfo);
    SAFE_RELEASE_UNBINDING_ALL(m_pDestinyInfo, this);
    m_pDestinyInfo = pDestinyInfo;
}

bool DestinyInfoView::init()
{
    if(!DLayer::init()) return false;
    m_pDestinyModel = DM_GET_DESTINY_MODEL;
    m_pWarriorModel = DM_GET_WARRIOR_MODEL;
    m_pFormationModel = DM_GET_FORMATION_MODEL;
    return initView();
}

bool DestinyInfoView::initView()
{
    return true;
}

bool DestinyInfoView::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pName", CCLabelTTF *, this->m_pName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCNode *, this->m_pHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFullEffect", CCNode *, this->m_pFullEffect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProgressBar", DProgressBar *, this->m_pProgressBar);
    return false;
}

cocos2d::SEL_MenuHandler DestinyInfoView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", DestinyInfoView::closeCallBack);
    return NULL;
}

void DestinyInfoView::closeCallBack(cocos2d::CCObject* sender)
{
    removeFromParent();
}

void DestinyInfoView::onEnter()
{
    DLayer::onEnter();
    CCAssert(m_pDestinyInfo, "");
    BINDING_EXEC(m_pDestinyInfo, this, DestinyInfoView::_updateDestinyInfo, D_EVENT_DESTINY_UPDATED);
    BINDING_EXEC(m_pWarriorModel, this, DestinyInfoView::_updateWarrior, D_EVENT_WARRIORLIST_UPDATE);
}

void DestinyInfoView::onExit()
{
    DLayer::onExit();
    UNBINDING_ALL(m_pDestinyInfo, this);
    UNBINDING_ALL(m_pWarriorModel, this);
}
void DestinyInfoView::_updateWarrior()
{
    updateWarrior();
}

void DestinyInfoView::updateWarrior()
{
    
}


void DestinyInfoView::_updateDestinyInfo()
{
    updateDestinyInfo();
}

void DestinyInfoView::updateDestinyInfo()
{
    m_pProgressBar->setPercent(getDestinyInfo()->getPercent());
    
    m_pHighLight->setVisible(getDestinyInfo()->isCanReward());
    m_pHighLight->removeAllChildren();
    if(m_pHighLight)
    {
        m_pHighLight->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("destinycomplete"));
    }
    
    m_pProperty->setVisible(!getDestinyInfo()->isNeedReAward());//属性加成
    if(m_pProperty->isVisible()) m_pProperty->setString(CCString::createWithFormat("+%d",m_pDestinyInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
    
    m_pGrade->setColor(m_pDestinyModel->getDestinyColorByGrade(m_pDestinyInfo->getDestinyGrade()));         //品质
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getDestinyIcon(m_pDestinyInfo->getID()));
    
    //更新等级
    m_pLevel->setString(CCString::createWithFormat("%d",m_pDestinyInfo->getLvDisplay())->getCString());//级别
    
    m_pName->setString(m_pDestinyInfo->getName().c_str());
}

#pragma mark DestinyInfoViewAny
DestinyInfoViewAny::DestinyInfoViewAny()
:m_pListContainer(NULL)
,m_pWarriorList(NULL)
,m_pGiveup(NULL)
,m_pSubmit(NULL)
,m_pDestinyWarriorDatas(NULL)
,m_pTempInfo(NULL)
,m_pCompleteEffect(NULL)
,m_nCurrentLv(0)
{
    
}

DestinyInfoViewAny::~DestinyInfoViewAny()
{
    CC_SAFE_RELEASE(m_pListContainer);
    CC_SAFE_RELEASE(m_pData);
    CC_SAFE_RELEASE(m_pGiveup);
    CC_SAFE_RELEASE(m_pSubmit);
    CC_SAFE_RELEASE(m_pDestinyWarriorDatas);
    CC_SAFE_RELEASE(m_pTempInfo);
    CC_SAFE_RELEASE(m_pCompleteEffect);
}


#define WARRIOR_LIST_SIZE CCSize(560,220)
#define WARRIOR_LIST_CELL_SIZE CCSizeMake(160, 220);

bool DestinyInfoViewAny::init()
{
    m_pDestinyWarriorDatas = CCArray::create();
    CC_SAFE_RETAIN(m_pDestinyWarriorDatas);
    
    m_pTempInfo = destiny::DestinyInfo::create();
    CC_SAFE_RETAIN(m_pTempInfo);
    
    return DestinyInfoView::init();
}

bool DestinyInfoViewAny::initView()
{
    CCB_READER_AND_ADDCHILD("ui/ccb/destiny/destinyInfoViewAny");
    m_pData = readCCData("ui/ccb/destiny/destinyWarriorCell");
    CC_SAFE_RETAIN(m_pData);
    
    m_pWarriorList = DTableView::create(this, WARRIOR_LIST_SIZE);
	m_pWarriorList->setDirection(kCCScrollViewDirectionHorizontal);
	m_pWarriorList->setDelegate(this);
	m_pListContainer->addChild(m_pWarriorList);
    
    
    m_pGiveup->setTarget(this, menu_selector(DestinyInfoViewAny::resetCallBack));
    m_pSubmit->setTarget(this, menu_selector(DestinyInfoViewAny::submitCallBack));
    m_pGiveup->setEnabled(false);
    m_pSubmit->setEnabled(false);
    
    m_pCompleteEffect = ResourceManager::sharedResourceManager()->getUIAnimate("destinyfull");
    CC_SAFE_RETAIN(m_pCompleteEffect);
    return true;
}

bool DestinyInfoViewAny::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pListContainer", CCNode *, this->m_pListContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGiveup", CCMenuItemImage *, this->m_pGiveup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSubmit", CCMenuItemImage *, this->m_pSubmit);
    return DestinyInfoView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

void DestinyInfoViewAny::resetCallBack(cocos2d::CCObject* sender)
{
    m_nCurrentLv = 0;
    updateDestinyInfo();
}

void DestinyInfoViewAny::submitCallBack(cocos2d::CCObject* sender)
{
    Json::Value warriors = Json::arrayValue;
    Json::Value souls = Json::arrayValue;
    
    
    DestinyWarriorData* pData = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pDestinyWarriorDatas,obj)
    {
        pData = (DestinyWarriorData*)obj;
        if(pData->getAdded()>0)
        {
            if(pData->getIsWarrior())
            {
                warriors.append(pData->getID());
            }else
            {
                Json::Value soul;
                soul["wid"] = pData->getID();
                soul["num"] = pData->getAdded();
                souls.append(soul);
            }
        }
    }
    
    m_nCurrentLv = 0;
    
    if(getDelegate()) getDelegate()->improveDestinys(m_pDestinyInfo->getID(), warriors, souls,this,callfuncJson_selector(DestinyInfoViewAny::improveCallBack));
    
}

void DestinyInfoViewAny::improveCallBack(const Json::Value &requestData, const Json::Value &responseData)
{
    if(responseData["code"].asString().compare(SUC_CODE)!=0) return;
    if(!isRunning()) return;
    
    closeCallBack(NULL);
    m_pWarriorList->setContentOffset(m_pWarriorList->maxContainerOffset());
    m_pWarriorList->updateContentOffset();
}

cocos2d::SEL_MenuHandler DestinyInfoViewAny::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "closeCallBack", DestinyInfoView::closeCallBack);
    return DestinyInfoView::onResolveCCBCCMenuItemSelector(pTarget, pSelectorName);
}


cocos2d::CCSize DestinyInfoViewAny::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    return WARRIOR_LIST_CELL_SIZE;
}

void DestinyInfoViewAny::tableCellTouched(cocos2d::extension::CCTableView * table,cocos2d::extension::CCTableViewCell * cell)
{
    
}

cocos2d::extension::CCTableViewCell* DestinyInfoViewAny::tableCellAtIndex(cocos2d::extension::CCTableView *table, uint32_t idx)
{
    DestinyWarriorListCell *cell = (DestinyWarriorListCell*)table->dequeueCell();
	if (!cell)
	{
		cell = DestinyWarriorListCell::create(m_pData);
	}
    cell->getDestinyWarriorHead()->setData((DestinyWarriorData*)m_pDestinyWarriorDatas->objectAtIndex(idx));
	return cell;
}

uint32_t DestinyInfoViewAny::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return m_pDestinyWarriorDatas->count();
}


void DestinyInfoViewAny::updateView(cocos2d::CCObject* sender)
{
    DestinyWarriorData* pTarget = (DestinyWarriorData*)sender;
    
    destiny::DestinySlotAny* pTempSlot = (destiny::DestinySlotAny*)m_pTempInfo->getDestinySlotByIndex(0);
    destiny::DestinySlotAny* pSlot = (destiny::DestinySlotAny*)getDestinyInfo()->getDestinySlotByIndex(0);
    
    if(m_pTempInfo->isEqUserLevel() || m_pTempInfo->isFullLevel())
    {
        if(pTarget) pTarget->fixAdd(pTarget->getAdded()-1); //如果已经满了 直接修正回去
        return;
    }
    
    DestinyWarriorData* pData = NULL;
    CCObject* obj = NULL;
    warrior::WarriorGrowInfo* pInfo = NULL;
    uint32_t n = 0;
    CCARRAY_FOREACH(m_pDestinyWarriorDatas,obj)
    {
        pData = (DestinyWarriorData*)obj;
        pInfo = m_pWarriorModel->getWarriorGrowInfoByGrade(pData->getID());
        if(pData->getIsWarrior())
        {
            n+= pData->getAdded()*pInfo->getRctSoul()*pInfo->getSoulValue();
        }else
        {
            
            n+= pData->getAdded()*pInfo->getSoulValue();
        }
        
    }
    
    pTempSlot->setCurrentNum(pSlot->getCurrentNum()+n);
    while (pTempSlot->getLvCurrentNum()>=pTempSlot->getLvUpgradeNum())
    {
        if(m_pTempInfo->isEqUserLevel() || m_pTempInfo->isFullLevel())
        {
            break;
        }
        m_pTempInfo->setLevel(m_pTempInfo->getLevel()+1);
    }
    float percent = (float)(pTempSlot->getLvCurrentNum())/pTempSlot->getLvUpgradeNum();
    
    m_pSubmit->setEnabled(n>0);
    m_pGiveup->setEnabled(n>0);
    percent = MIN(percent, 1.f);
    m_pProgressBar->setPercent(percent);
    
    
    if(m_nCurrentLv!=0 && m_nCurrentLv!=m_pTempInfo->getLvDisplay())
    {
        if(m_pCompleteEffect->getParent()==NULL) m_pFullEffect->addChild(m_pCompleteEffect);
        m_pCompleteEffect->getAnimation()->playByIndex(0,-1,-1,true);
    }
    /*else
    {
        m_pFullEffect->setVisible(false);
    }*/
    
    m_nCurrentLv = m_pTempInfo->getLvDisplay();
    
    if(m_pProperty->isVisible()) m_pProperty->setString(CCString::createWithFormat("+%d",m_pTempInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
    
    //更新等级
    m_pLevel->setString(CCString::createWithFormat("%d",m_pTempInfo->getLvDisplay())->getCString());//级别
}

void DestinyInfoViewAny::clearDestinyWarriorDatas()
{
    DestinyWarriorData* pData = NULL;
    CCObject* obj = NULL;
    CCARRAY_FOREACH(m_pDestinyWarriorDatas,obj)
    {
        pData = (DestinyWarriorData*)obj;
        UNBINDING_ALL(pData, this);
    }
    m_pDestinyWarriorDatas->removeAllObjects();
}

void DestinyInfoViewAny::onExit()
{
    m_nCurrentLv = 0;
    m_pCompleteEffect->removeFromParent();
    DestinyInfoView::onExit();
    clearDestinyWarriorDatas();
}

//void DestinyInfoViewAny::updateWarrior() //监听天命信息变化刷新弟子列表 避免重复刷新 后台返回数据先更新弟子 后更新天命
void DestinyInfoViewAny::updateDestinyInfo()
{
    m_pTempInfo->copyFrom(getDestinyInfo());
    
    DestinyInfoView::updateDestinyInfo();
    clearDestinyWarriorDatas();
    
    CCObject* obj = NULL;
    warrior::WarriorCurInfo* pW = NULL;
    warrior::SoulInfo *pS = NULL;
    DestinyWarriorData* pData = NULL;
    
    CCArray* pArray = m_pWarriorModel->getWarriorVec();
    CCARRAY_FOREACH(pArray,obj)
    {
        pW = (warrior::WarriorCurInfo*)obj;
        if(pW && !m_pFormationModel->isWarriorBattle(pW->getWid()))
        {
            pData = DestinyWarriorData::create(pW);
            BINDING_ONLY_O(pData, this, DestinyInfoViewAny::updateView, D_EVENT_DESTINYWARRIOR_ADD);
            m_pDestinyWarriorDatas->addObject(pData);
        }
    }
    
    pArray = m_pWarriorModel->getSoulVec();
    CCARRAY_FOREACH(pArray,obj)
    {
        pS = (warrior::SoulInfo*)obj;
        if(pS)
        {
            pData = DestinyWarriorData::create(pS);
            BINDING_ONLY_O(pData, this, DestinyInfoViewAny::updateView, D_EVENT_DESTINYWARRIOR_ADD);
            m_pDestinyWarriorDatas->addObject(pData);
        }
    }
    
    
    //sort
    int i,j,length = m_pDestinyWarriorDatas->data->num;
    DestinyWarriorData ** x = (DestinyWarriorData**)m_pDestinyWarriorDatas->data->arr;
    DestinyWarriorData *tempItem;
    
    for(i=1; i<length; i++)
    {
        tempItem = x[i];
        j = i-1;
        
        while(j>=0 && (tempItem->getGrade()<x[j]->getGrade() ||(tempItem->getGrade()==x[j]->getGrade() && tempItem->getID() < x[j]->getID())) )
        {
            x[j+1] = x[j];
            j = j-1;
        }
        x[j+1] = tempItem;
    }
    //sort
    
    m_pWarriorList->reloadData();
    updateView(NULL);
}

#pragma mark DestinyInfoViewNoAward
DestinyInfoViewNoAward::DestinyInfoViewNoAward()
{
    for (uint8_t i=0; i<MAX_SLOTS; ++i)
    {
        m_pSlots[i] = NULL;
    }
}

DestinyInfoViewNoAward::~DestinyInfoViewNoAward()
{
    for (uint8_t i=0; i<MAX_SLOTS; ++i)
    {
        CC_SAFE_RELEASE(m_pSlots[i]);
    }
}

bool DestinyInfoViewNoAward::initView()
{
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorIcon", DestinyWarriorIconLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorIconItem", DestinyWarriorIconItemLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/destiny/destinyInfoViewNoAward", pNodeLoaderLibrary);
    initSlots();
    return true;
}

void DestinyInfoViewNoAward::initSlots()
{
    
    for (uint8_t i =0; i<MAX_SLOTS; ++i)
    {
        m_pSlots[i]->getIcon()->getMenuItem()->setTag(i);
        m_pSlots[i]->getIcon()->getMenuItem()->setTarget(this, menu_selector(DestinyInfoViewNoAward::touchSlot));
    }
}
bool DestinyInfoViewNoAward::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot0", DestinyWarriorIconItem *, this->m_pSlots[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot1", DestinyWarriorIconItem *, this->m_pSlots[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot2", DestinyWarriorIconItem *, this->m_pSlots[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot3", DestinyWarriorIconItem *, this->m_pSlots[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot4", DestinyWarriorIconItem *, this->m_pSlots[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSlot5", DestinyWarriorIconItem *, this->m_pSlots[5]);
    return DestinyInfoView::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}

void DestinyInfoViewNoAward::touchSlot(cocos2d::CCObject* sender)
{
    CCMenuItem* pItem = (CCMenuItem*)sender;
    if(pItem && getDelegate()) getDelegate()->showDestinyWarriorView(getDestinyInfo(), pItem->getTag());
}

void DestinyInfoViewNoAward::updateDestinyInfo()
{
    DestinyInfoView::updateDestinyInfo();
    for (uint32_t i = 0 ; i<getDestinyInfo()->getDestinySlotCount(); ++i)
    {
        m_pSlots[i]->setVisible(true);
    }
    
    for (uint32_t i = getDestinyInfo()->getDestinySlotCount();i<MAX_SLOTS;++i)
    {
        m_pSlots[i]->setVisible(false);
    }
    updateSlots();
}

void DestinyInfoViewNoAward::updateWarrior()
{
    DestinyInfoView::updateWarrior();
    updateSlots();
}

void DestinyInfoViewNoAward::updateSlots()
{
    destiny::DestinySlot *pSlotInfo = NULL;
    for (uint32_t i = 0 ; i<getDestinyInfo()->getDestinySlotCount(); ++i)
    {
        pSlotInfo =getDestinyInfo()->getDestinySlotByIndex(i);
        
        m_pSlots[i]->setCanBeCollected(pSlotInfo->isCanCollect());
        
        m_pSlots[i]->getCount()->setString(CCString::createWithFormat("%d/%d",pSlotInfo->getCurrentNum(),pSlotInfo->getUpgradeNum())->getCString());
        
        
        m_pSlots[i]->getIcon()->getGrade()->setColor(m_pWarriorModel->getWarriorColorByWid(pSlotInfo->getWarriorID()));
        
        m_pSlots[i]->getIcon()->getMenuItem()->setEnabled(!pSlotInfo->isSlotUnlock());
        m_pSlots[i]->getIcon()->getHeadContainer()->removeAllChildren();
        m_pSlots[i]->getIcon()->getHeadContainer()->addChild(ResourceManager::sharedResourceManager()->getWarriorIcon40PForDestiny(pSlotInfo->getWarriorID()));
        m_pSlots[i]->getIcon()->getHighLight()->setVisible(pSlotInfo->isSlotUnlock());
        m_pSlots[i]->getIcon()->getHighLight()->removeAllChildren();
        if(m_pSlots[i]->getIcon()->getHighLight()->isVisible())
        {
            CCNode* pEffect = ResourceManager::sharedResourceManager()->getUIAnimate("destinycomplete");
            pEffect->setScale(0.6f);
            m_pSlots[i]->getIcon()->getHighLight()->addChild(pEffect);
        }
        m_pSlots[i]->getIcon()->getName()->setString(m_pWarriorModel->getWarriorBaseByWID(pSlotInfo->getWarriorID())->getWarriorName().c_str());
    }
}


#pragma mark DestinyInfoViewAward
DestinyInfoViewAward::DestinyInfoViewAward()
:m_pRewardMenuItem(NULL)
,m_pRewardEffect(NULL)
,m_pAwardName(NULL)
,m_pAwardContainer(NULL)
,m_pAwardGrade(NULL)
,m_pAwardCount(NULL)
,m_pItemModel(NULL)
,m_pEquipModel(NULL)
,m_pRewardDaily(NULL)
,m_pViewInfoMenuItem(NULL)
{
    
}

DestinyInfoViewAward::~DestinyInfoViewAward()
{
    CC_SAFE_RELEASE(m_pViewInfoMenuItem);
    CC_SAFE_RELEASE(m_pRewardMenuItem);
    CC_SAFE_RELEASE(m_pAwardName);
    CC_SAFE_RELEASE(m_pAwardContainer);
    CC_SAFE_RELEASE(m_pAwardGrade);
    CC_SAFE_RELEASE(m_pAwardCount);
    CC_SAFE_RELEASE(m_pRewardEffect);
    CC_SAFE_RELEASE(m_pRewardDaily);
}

bool DestinyInfoViewAward::init()
{
    m_pItemModel = DM_GET_ITEM_MODEL;
    m_pEquipModel = DM_GET_EQUIP_MODEL;
    if(!DestinyInfoViewNoAward::init()) return false;
    return true;
}
bool DestinyInfoViewAward::initView()
{
    
    cocos2d::extension::CCNodeLoaderLibrary * pNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibrary);
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorIcon", DestinyWarriorIconLoader::loader());
    pNodeLoaderLibrary->registerCCNodeLoader("DestinyWarriorIconItem", DestinyWarriorIconItemLoader::loader());
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/destiny/destinyInfoViewAward", pNodeLoaderLibrary);
    
    m_pRewardMenuItem->setTarget(this, menu_selector(DestinyInfoViewAward::reward));
    m_pViewInfoMenuItem->setTarget(this, menu_selector(DestinyInfoViewAward::viewInfo));
    
    m_pRewardEffect->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("destinyreward"));
    
    initSlots();
    
    return true;
}

bool DestinyInfoViewAward::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pViewInfoMenuItem", CCMenuItem*, this->m_pViewInfoMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardMenuItem", CCMenuItem*, this->m_pRewardMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardName", CCLabelTTF *, this->m_pAwardName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardContainer", CCNode *, this->m_pAwardContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardEffect", CCNode *, this->m_pRewardEffect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardGrade", CCScale9Sprite *, this->m_pAwardGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAwardCount", CCLabelBMFont *, this->m_pAwardCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardDaily", CCSprite *, this->m_pRewardDaily);
    
    return DestinyInfoViewNoAward::onAssignCCBMemberVariable(pTarget,pMemberVariableName,pNode);
}


void DestinyInfoViewAward::onEnter()
{
    DestinyInfoViewNoAward::onEnter();
    updateAward();
}

void DestinyInfoViewAward::updateDestinyInfo()
{
    DestinyInfoViewNoAward::updateDestinyInfo();
    m_pRewardMenuItem ->setVisible(getDestinyInfo()->isCanReward());
    m_pRewardEffect->setVisible(m_pRewardMenuItem->isVisible());
}

void DestinyInfoViewAward::updateAward()
{
    m_pAwardContainer->removeAllChildren();
    
    m_pRewardDaily->setVisible(getDestinyInfo()->getRewardType()== destiny::kDestinyRewardDaily);
    m_pAwardGrade->setVisible(true);
    switch (getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemType())
    {
        case destiny::kDestinyRewardItemEquip:
        {
            equip::EquipBaseInfo* pEquip = m_pEquipModel->getEquipBaseInfoByID(getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemID());
            m_pAwardContainer->addChild(ResourceManager::sharedResourceManager()->getEquipIcon40P(pEquip->getEquipId()));
            m_pAwardName->setString(pEquip->getEquipName().c_str());
            m_pAwardGrade->setColor(m_pEquipModel->getEquipColorByGrade(pEquip->getEquipGrade()));
            m_pAwardCount->setVisible(false);
            break;
        }
        case destiny::kDestinyRewardItemItem:
        {
            item::ItemBase* pItem = m_pItemModel->getItemBaseByID(getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemID());
            m_pAwardContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(pItem->getItemId()));
            m_pAwardName->setString(pItem->getItemName().c_str());
            m_pAwardGrade->setVisible(false);
            m_pAwardCount->setVisible(true);
            m_pAwardCount->setString(CCString::createWithFormat("x%d",getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
            break;
        }
        case destiny::kDestinyRewardItemCoin:
            m_pAwardCount->setVisible(true);
            m_pAwardGrade->setVisible(false);
            m_pAwardName->setString(D_LOCAL_STRING("Coin").c_str());
            m_pAwardCount->setString(CCString::createWithFormat("x%d",getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
            m_pAwardContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(COIN_ID));
            break;
        case destiny::kDestinyRewardItemPower:
            m_pAwardCount->setVisible(true);
            m_pAwardGrade->setVisible(false);
            m_pAwardName->setString(D_LOCAL_STRING("Power").c_str());
            m_pAwardCount->setString(CCString::createWithFormat("x%d",getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
            m_pAwardContainer->addChild(ResourceManager::sharedResourceManager()->getItemIcon40P(POWER_ID));
            break;
        default:
            
            break;
    }
}

void DestinyInfoViewAward::viewInfo(CCObject*obj)
{
    if(m_DestinyDelegate==NULL) return;
    switch (getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemType())
    {
        case destiny::kDestinyRewardItemEquip:
        {
            m_DestinyDelegate->showInfoView(kEquipInfo, equip::EquipCurInfo::create(getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemID()));
            break;
        }
        case destiny::kDestinyRewardItemItem:
        {
            m_DestinyDelegate->showInfoView(kItemInfo, item::ItemInfo::create(getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemID(),getDestinyInfo()->getCurrentDestinyUpgradeInfo()->getRewardItemNum()));
            break;
        }
        case destiny::kDestinyRewardItemCoin:
        case destiny::kDestinyRewardItemPower:
        default:
            
            break;
    }
}

void DestinyInfoViewAward::reward(CCObject*obj)
{
    if(m_DestinyDelegate) m_DestinyDelegate->reward(getDestinyInfo()->getID());
}

