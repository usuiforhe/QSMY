//
//  Wish.cpp
//  QSMY
//
//  Created by wyj on 13-8-21.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Wish.h"
#include "managers/GameManager.h"
#include "WishCell.h"
#include "../warrior/WarriorListView.h"
#include "../Game.h"
#include "../common/AwardListView.h"
#include "managers/ResourceManager.h"
#include <algorithm>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace sp;

#define MIN_ROUND_TIMES 20
#define MIN_ANIMATE_DELAY 0.05f
#define ANIMATE_ACCELORATE 0.03f
#define MAX_ANIMATE_DELAY 0.15f
#define STOP_DELAY 0.4f

bool WishWarriorFilter::operator()(warrior::WarriorCurInfo *pWarrior)
{
    return !DM_GET_WISH_MODEL->isWarriorWished(pWarrior->getWid());
}

Wish::~Wish()
{
    for(uint8_t i(0); i < TOTAL_WISH_NUM; ++i)
    {
        CC_SAFE_RELEASE(m_pWishCells[i]);
    }
    
    CC_SAFE_RELEASE(m_pLaternNum);
    CC_SAFE_RELEASE(m_pLightCost);
    CC_SAFE_RELEASE(m_pWishButton);
    CC_SAFE_RELEASE(m_pWishTenButton);
    CC_SAFE_RELEASE(m_pAnimation);
    CC_SAFE_RELEASE(m_pItem);
    CC_SAFE_RELEASE(m_pWishItemCellMap);
    CC_SAFE_RELEASE(m_pDrops);
	CC_SAFE_RELEASE(m_pTargetSoul);
	//CC_SAFE_RELEASE(m_pPopup);
	CC_SAFE_RELEASE(m_pWarriorGradeBg);
	CC_SAFE_RELEASE(m_pCenterNode);
	CC_SAFE_RELEASE(m_pAvatorContainer);
}

bool Wish::init()
{
    if(!DLayer::init())
    {
        return false;
    }
    m_pActionDelegate = CCSprite::create();
    addChild(m_pActionDelegate);
    
    m_pWishItemCellMap = CCDictionary::create();
    m_pWishItemCellMap->retain();
    
    m_pDrops = CCArray::create();
    m_pDrops->retain();
    
    WishModel * pWishModel = DM_GET_WISH_MODEL;
    
    //解锁顺序 m_pCell7 >> m_pCell5 >> m_pCell9 >> m_pCell3 >> m_pCell1
    m_pWishItemCellMap->setObject(pWishModel->getWishItem(0), 6);
    m_pWishItemCellMap->setObject(pWishModel->getWishItem(1), 4);
    m_pWishItemCellMap->setObject(pWishModel->getWishItem(2), 8);
    m_pWishItemCellMap->setObject(pWishModel->getWishItem(3), 2);
    m_pWishItemCellMap->setObject(pWishModel->getWishItem(4), 0);
            
    CCNodeLoaderLibrary * pNodeLoaderLibarary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    CUSTOM_CCB_LOADER(pNodeLoaderLibarary);
    pNodeLoaderLibarary->registerCCNodeLoader("WishCell", WishCellLoader::loader());
    pNodeLoaderLibarary->registerCCNodeLoader("WishCellVip", WishCellVipLoader::loader());
    //CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("ui/ccb/wish/wish", pNodeLoaderLibarary);
    CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER("PL_ui/ccb/pl_zhuanhun/pl_zhuanhun", pNodeLoaderLibarary);

    return true;
}

void Wish::onEnter()
{
    DLayer::onEnter();
    //m_pLightCost->setString(CCString::createWithFormat("%d", K_WISH_SKY_LIGHT_NUM_PER_TIME)->getCString());
    m_pWishButton->getButton()->setTarget(this, menu_selector(Wish::onWishClick));
    m_pWishTenButton->getButton()->setTarget(this, menu_selector(Wish::onWishTenClick));
    
    BINDING_ONLY(DM_GET_WISH_MODEL, this, Wish::showWishRes, D_EVENT_WISH_RES_GET);
    NotificationCenter::sharedNotificationCenter()->addObserver(this, notification_callfunc_selector(Wish::updateView), D_EVENT_USER_VIP_LEVELUP);
    BINDING_EXEC(DM_GET_WISH_MODEL, this, Wish::updateView, D_EVENT_WISHES_GET);
    DM_GET_WISH_MODEL->syncFromServer();
}

void Wish::onExit()
{
    UNBINDING_ALL(DM_GET_WISH_MODEL, this);
    UNBINDING_ALL(m_pItem, this);
	DM_GET_WISH_MODEL->saveWishWarriors();
    NotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    DLayer::onExit();
}

void Wish::showWarriorSelectList(WarriorSelectDelegate * pDelegate)
{
	m_pPopup = WarriorSelectPopup::create(NULL);
	m_pPopup->setDelegate(pDelegate);
	m_pPopup->setFilter(WishWarriorFilter::create());
	//CC_SAFE_RETAIN(m_pPopup);
    addChild(m_pPopup);
}

bool Wish::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLaternNum", cocos2d::CCLabelBMFont *, this->m_pLaternNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLightCost", cocos2d::CCLabelBMFont *, this->m_pLightCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWishButton", DButton *, this->m_pWishButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWishTenButton", DButton *, this->m_pWishTenButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAnimation", CCNode *, this->m_pAnimation);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pWarriorGradeBg", WarriorGradeBg *, this->m_pWarriorGradeBg);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCenterNode", CCNode *, this->m_pCenterNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pAvatorContainer", CCNode *, this->m_pAvatorContainer);
    
    for(uint8_t i(1); i <= TOTAL_WISH_NUM; ++i)
    {
        const char* cellName = CCString::createWithFormat("m_pCell%d", i)->getCString();
        WishCell * pCell(NULL);
        if (pTarget == this && 0 == strcmp(pMemberVariableName, cellName)) {
            pCell = dynamic_cast<WishCell*>(pNode);
            CC_ASSERT(pCell);
            pCell->retain();
            m_pWishCells[i - 1] = pCell;

			if(i == 4 || i == 10){
				pCell->m_pNodeBlue->setVisible(true);
			}
			if(i == 8){
				pCell->m_pNodeOrange->setVisible(true);
			}
			if(i == 6){
				pCell->m_pNodePurple->setVisible(true);
			}

            
            return true;
        }
    }
    
    return false;
}

void Wish::updateView()
{
    if(!DM_GET_WISH_MODEL->isSynced)
    {
        return;
    }
    
	UNBINDING_ALL(m_pItem, this);
    CC_SAFE_RELEASE_NULL(m_pItem);
    m_pItem = DM_GET_ITEM_MODEL->getItemInfoByID(SKY_LIGHT_ID);
    CC_SAFE_RETAIN(m_pItem);
    BINDING_ONLY(m_pItem, this, Wish::updateView, D_EVENT_ITEMINFO_UPDATE);
    uint32_t laternNum = 0;
    if(m_pItem != NULL)
    {
        laternNum = m_pItem->getNum();
    }
    
    m_pLaternNum->setString(CCString::createWithFormat("%d", laternNum)->getCString());
    
    for(uint8_t i(0); i < TOTAL_WISH_NUM; ++i)
    {
        wish::WishItem * pWishItem = dynamic_cast<wish::WishItem *>(m_pWishItemCellMap->objectForKey(i));
        if(pWishItem != NULL)
        {
            WishCellVip * pWishCellVip = dynamic_cast<WishCellVip *>(m_pWishCells[i]);
            pWishCellVip->setDelegate(this);
            pWishCellVip->setWishItem(pWishItem);
            pWishCellVip->updateView();
        }
        else
        {
            warrior::WarriorGrade grade = getGrade(i);
            WishCell * pWishCell = dynamic_cast<WishCell *>(m_pWishCells[i]);
            pWishCell->setColorGrade(grade);
        }
    }
}

void Wish::showWishRes()
{
    m_pWishButton->getButton()->setEnabled(false);
    m_pWishTenButton->getButton()->setEnabled(false);
    CCArray * pWishRes = DM_GET_WISH_MODEL->getWishRes();
    CCDictionary * pSouls = CCDictionary::create();
    CCObject * pObj(NULL);
    CCARRAY_FOREACH(pWishRes, pObj)
    {
        CCInteger * pInt = (CCInteger *) pObj;
        warrior::SoulInfo * pSoul = (warrior::SoulInfo *)pSouls->objectForKey(pInt->getValue());
        if(pSoul == NULL)
        {
            pSoul = warrior::SoulInfo::create(pInt->getValue(),1);
        }
        else
        {
            pSoul = warrior::SoulInfo::create(pInt->getValue(),pSoul->getNum()+1);
        }        
        pSouls->setObject(pSoul, pSoul->getWid());
    }

	

    m_pDrops->removeAllObjects();
    WarriorModel* pWarriorModel = DM_GET_WARRIOR_MODEL;
    CCDictElement * pElement(NULL);
    CCDICT_FOREACH(pSouls, pElement)
    {
        warrior::SoulInfo * pSoul = (warrior::SoulInfo *)pElement->getObject();
        drop::Drop * pDrop = drop::Drop::create(pSoul);
        m_pDrops->addObject(pDrop);
        pWarriorModel->addSoul(pSoul);
    }

	//對掉落結果排序
	std::sort(m_pDrops->data->arr , m_pDrops->data->arr+m_pDrops->data->num , DropModel::dropSort);
    
    if(pWishRes->count() == 1)
    {
        drop::Drop * pDrop = (drop::Drop *) m_pDrops->objectAtIndex(0);
        
        CC_SAFE_RELEASE_NULL(m_pTargetSoul);
        m_pTargetSoul = pDrop->getSoul();
        m_pTargetSoul->retain();
        
        m_remainActionNum = MIN_ROUND_TIMES + (uint32_t)(10 * CCRANDOM_0_1());
        showSingleRes();
    }
    else
    {
        showMultiRes();
    }
}

void Wish::showMultiRes()
{
    showDrop();    
}

void Wish::showSingleRes()
{
    WishCell * pCell = m_pWishCells[m_index % TOTAL_WISH_NUM];
    pCell->hideLight();
    
    pCell = m_pWishCells[++m_index % TOTAL_WISH_NUM];    
    pCell->showLight();
    
    bool stop(false);
    if(m_remainActionNum > 0)
    {
        --m_remainActionNum;
    }
    else
    {
        WishCellVip * pCellVip = dynamic_cast<WishCellVip * >(pCell);
        if(DM_GET_WISH_MODEL->isWarriorWished(m_pTargetSoul->getWid()))
        {
            if(pCellVip != NULL && pCellVip->getWishItem()->getWarriorId() == m_pTargetSoul->getWid())
            {
                stop = true;
            }
        }
        else if((pCellVip == NULL || !pCellVip->isUnlocked()) && m_pTargetSoul->getBaseInfo()->getColorGrade() == pCell->getColorGrade())
        {
            stop = true;
        }
    }
    
    if(stop)
    {
        CCSequence * pSequence =
        CCSequence::create(
                           CCDelayTime::create(STOP_DELAY),
                           CCCallFuncO::create(this, callfuncO_selector(Wish::showAnimation), pCell)
						   ,NULL
                           );
        m_pActionDelegate->runAction(pSequence);
        return;
    }
    else
    {
        float delay = MAX_ANIMATE_DELAY - m_remainActionNum * ANIMATE_ACCELORATE;
        if(delay < MIN_ANIMATE_DELAY)
        {
            delay = MIN_ANIMATE_DELAY;
        }
        CCSequence * pSequence =
        CCSequence::create(
                           CCDelayTime::create(delay),
                           CCCallFunc::create(this, callfunc_selector(Wish::showSingleRes))
						   ,NULL
                           );
        m_pActionDelegate->runAction(pSequence);
    }
}

void Wish::showAnimation(CCObject * pObj)
{
    WishCell * pCell = (WishCell*) pObj;
    sp::Armature * pArmature = ResourceManager::sharedResourceManager()->getUIAnimate("wish");
    pArmature->setPosition(pCell->getPosition());
//    pArmature->setAnchorPoint(ccp(0.5*pCell->getPositionX(), 0.5*pCell->getPositionY()));
    pArmature->getAnimation()->addObserver(this, SPNotification_callfunc_selector(Wish::animateEnd), COMPLETE);
    m_pAnimation->addChild(pArmature);    
}

void Wish::animateEnd(sp::Armature * target, const char* event,const char* data)
{
    m_pAnimation->removeAllChildren();
    //showDrop();
	showOneDrop();
}

void Wish::showOneDrop()
{
	m_pWishButton->getButton()->setEnabled(true);
	m_pWishTenButton->getButton()->setEnabled(true);

	drop::Drop *oneDrop = dynamic_cast<drop::Drop*>(m_pDrops->objectAtIndex(0));
	warrior::SoulInfo *soulInfo = oneDrop->getSoul();
	m_pCenterNode->setVisible(true);
	m_pAvatorContainer->removeAllChildren();
	m_pAvatorContainer->addChild(ResourceManager::sharedResourceManager()->getUnitAnimate2XAsync(soulInfo->getWid()));
	m_pWarriorGradeBg->setWarriorGrade(soulInfo->getBaseInfo()->getColorGrade());
	
}

void Wish::showDrop()
{
    m_pWishButton->getButton()->setEnabled(true);
    m_pWishTenButton->getButton()->setEnabled(true);
    Game::sharedGame()->openPopup(AwardViewFactory::create(m_pDrops));
}

void Wish::onWishClick(cocos2d::CCObject *sender)
{
    DM_GET_WISH_MODEL->doWish(false);
}

void Wish::onWishTenClick(cocos2d::CCObject *sender)
{
    DM_GET_WISH_MODEL->doWish(true);
}

warrior::WarriorGrade Wish::getGrade(uint32_t index)
{
    if(index == 3 || index == 9)
    {
        return warrior::kWarGradeBlue;
    }
	else if(index == 7){
		return warrior::kWarGradeRed;
	}
	else if(index == 5){
		return warrior::kWarGradePurple;
	}
    else
    {
        return warrior::kWarGradeGreen;
    }
}

void Wish::onBackKey()
{
	if (m_pPopup && m_pPopup->getParent())
	{
		m_pPopup->removeFromParent();
		return;
	}
	
	if (m_pActionDelegate && m_pActionDelegate->numberOfRunningActions())
	{
		return;
	}
	BackKeyInterface::onBackKey();
}