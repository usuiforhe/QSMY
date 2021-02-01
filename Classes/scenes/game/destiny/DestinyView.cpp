//
//  DestinyView.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-18.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DestinyView.h"
#include "managers/ResourceManager.h"
#include "managers/GameManager.h"
#include "utils/TimerUtil.h"
#include "utils/TimeUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PAGE_SIZE_WIDTH   D_DESIGN_SIZE_WIDTH
#define PAGE_SIZE_HEIGHT  D_DESIGN_SIZE_HEIGHT

#define CountEveryPage 4
static const cocos2d::CCPoint D_DESTINY_POS[CountEveryPage] =
{
    ccp(75,320),//
    ccp(384,320),//
    ccp(75,40),//
    ccp(384,40)//
};

#pragma mark - -------------------------------------------------
#pragma mark - -------------------DestinyView-------------------
#pragma mark - -------------------------------------------------

#pragma mark - - init etc.
DestinyView::DestinyView()
:m_pDelegate(NULL)
,m_pPageView(NULL)
,m_pPageDot(NULL)
,m_pPages(NULL)
,m_pDestinyModel(NULL)
{
    
}

DestinyView::~DestinyView()
{
    CC_SAFE_RELEASE(m_pPages);
    CC_SAFE_RELEASE(m_pCCData);
}

DestinyView* DestinyView::create(destiny::DestinyRequireType type)
{
    DestinyView* pRet = new DestinyView();
    if(pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool DestinyView::init(destiny::DestinyRequireType type)
{
    if(!DLayer::init()) return false;
    
    m_kRequireType = type;
    m_pPages = CCArray::create();
    CC_SAFE_RETAIN(m_pPages);
    m_pDestinyModel = DM_GET_DESTINY_MODEL;
    
    m_pCCData = readCCData("ui/ccb/destiny/destinyIcon");
    CC_SAFE_RETAIN(m_pCCData);
    
    /**
     *   初始化天命展示界面
     */
    m_pPageView = DPageView::create(this , CCSizeMake(PAGE_SIZE_WIDTH, PAGE_SIZE_HEIGHT));
    m_pPageView->setFade(false);
    m_pPageView->setTouchEnabled(false);
    m_pPageView->setDelegate(this);
    addChild(m_pPageView);
    
    /**
     *   初始化Dot点
     */
    m_pPageDot = DPageDot::create();
    m_pPageDot->setPosition(ccp(D_DESIGN_SIZE_WIDTH_HALF,0));
    addChild(m_pPageDot);
    
    
    return true;
}



float DestinyView::cellSizeForTable(DPageView *view)
{
    return PAGE_SIZE_WIDTH;
}

cocos2d::CCNode* DestinyView::cellAtIndex(DPageView *view, unsigned int idx)
{
    return (cocos2d::CCNode* )m_pPages->objectAtIndex(idx);
}

uint32_t DestinyView::numberOfCellsInTableView(DPageView *view)
{
    return m_pPages->count();
}

void DestinyView::onEnter()
{
    BINDING_EXEC(m_pDestinyModel, this, DestinyView::showDestiny, D_EVENT_DESTINY_UPDATED);
    
    DLayer::onEnter();
}

void DestinyView::onExit()
{
    UNBINDING_ALL(m_pDestinyModel, this);
    
    DLayer::onExit();
}

#pragma mark - - CCScrollViewDelegate
void DestinyView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    m_pPageDot->setCurrentPage(m_pPageView->getCurrentPage());
}



void DestinyView::showItemPage(const uint32_t did)
{
    uint32_t index = m_pDestinyModel->getDestinyIndexByDid(did);
    uint32_t page = floorf((float)index/CountEveryPage);
    setCurrentPage(page);
}

void DestinyView::setCurrentPage(const uint32_t page)
{
    m_pPageDot->setCurrentPage(page);
    m_pPageView->setCurrentPage(page);
}

#pragma mark - - methods private
void DestinyView::showDestiny()
{   
    int nDestinyCount = m_pDestinyModel->getDestinyCount(m_kRequireType);
    int nPages = ceilf((float)nDestinyCount/CountEveryPage);
    
    m_pPages->removeAllObjects();
    m_pPageDot->setPages(nPages);
    for(int i = 0 ; i < nPages ; i++)
    {
        CCNode *pPageNode = CCNode::create();
        m_pPages->addObject(pPageNode);
        
        for(int j = 0 ; j < CountEveryPage ; j++)
        {
            int nCurrentDestinyIndex = i*CountEveryPage + j;
            if(nCurrentDestinyIndex >= nDestinyCount)
                break;
            
            destiny::DestinyInfo *pDestinyInfo = m_pDestinyModel->getDestinyInfoByIndex(m_kRequireType,nCurrentDestinyIndex);
            DestinyViewItem *pItem = DestinyViewItem::create(m_pCCData);
            pItem->setDestinyInfo(pDestinyInfo);
            pItem->setPosition(D_DESTINY_POS[j]);
            pItem->setDelegate(m_pDelegate);
            pPageNode->addChild(pItem);
        }
    }
    m_pPageView->reloadData();
    
}



#pragma mark - -------------------------------------------------
#pragma mark - -----------------DestinyViewItem-----------------
#pragma mark - -------------------------------------------------
#pragma mark - - init etc.
DestinyViewItem::DestinyViewItem()
:m_pDestinyInfo(NULL)
,m_pDelegate(NULL)

,m_pHighLight(NULL)
,m_pCanBeCollected(NULL)
,m_pRewarded(NULL)
,m_pGrade(NULL)
,m_pRewardMenuItem(NULL)
,m_pRewardEffect(NULL)
,m_pProperty(NULL)
,m_pLevel(NULL)
,m_pInterval(NULL)
,m_pContainer(NULL)
,m_pModel(NULL)
{
    
}
DestinyViewItem::~DestinyViewItem()
{
    removeAllChildren();
    CC_SAFE_RELEASE(m_pHighLight);
    CC_SAFE_RELEASE(m_pCanBeCollected);
    CC_SAFE_RELEASE(m_pRewarded);
    CC_SAFE_RELEASE(m_pGrade);
    CC_SAFE_RELEASE(m_pRewardMenuItem);
    CC_SAFE_RELEASE(m_pProperty);
    CC_SAFE_RELEASE(m_pLevel);
    CC_SAFE_RELEASE(m_pInterval);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pRewardEffect);
    SAFE_RELEASE_UNBINDING_ALL(m_pDestinyInfo, this);
}

bool DestinyViewItem::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pHighLight", CCNode*, this->m_pHighLight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pCanBeCollected", CCNode *, this->m_pCanBeCollected);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewarded", CCSprite *, this->m_pRewarded);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pGrade", CCSprite *, this->m_pGrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardMenuItem", CCMenuItem *, this->m_pRewardMenuItem);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pProperty", CCLabelBMFont *, this->m_pProperty);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pLevel", CCLabelBMFont *, this->m_pLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pInterval", CCLabelBMFont *, this->m_pInterval);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRewardEffect", CCNode *, this->m_pRewardEffect);
    
    return false;
}

cocos2d::SEL_MenuHandler DestinyViewItem::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "touchItem", DestinyViewItem::touchItem);
    return NULL;
}

DestinyViewItem* DestinyViewItem::create(cocos2d::extension::CCData * pData)
{
    DestinyViewItem* pRet = new DestinyViewItem();
    if(pRet && pRet->init(pData))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}


bool DestinyViewItem::init(cocos2d::extension::CCData* pData)
{
    m_pModel = DM_GET_DESTINY_MODEL;
    CCB_READER_DATA_AND_ADDCHILD(pData);
    m_pRewardMenuItem->setTarget(this, menu_selector(DestinyViewItem::reward));
    m_pRewardEffect->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("destinyreward"));
    return true;
}
    
#pragma mark - - methods private
void DestinyViewItem::updateView()
{
    if(!m_pDestinyInfo)
    {
        CCLOGERROR("错误的天命信息");
        setVisible(false);
        return;
    }
    setVisible(true);
    
    m_pCanBeCollected->setVisible(getDestinyInfo()->isCanCollect()); //可收集
    m_pCanBeCollected->removeAllChildren();
    if(m_pCanBeCollected->isVisible())
    {
        m_pCanBeCollected->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("collect"));
    }
    m_pInterval->setVisible(getDestinyInfo()->isNeedShowInterval());//领奖计时
    startInterval();
    m_pHighLight->setVisible(getDestinyInfo()->isCanReward());//高亮
    m_pHighLight->removeAllChildren();
    if(m_pHighLight)
    {
        m_pHighLight->addChild(ResourceManager::sharedResourceManager()->getUIAnimate("destinycomplete"));
    }
    m_pRewardMenuItem ->setVisible(getDestinyInfo()->isCanReward()); //点击领取
    m_pRewardEffect->setVisible(m_pRewardMenuItem->isVisible());
    m_pRewarded ->setVisible(getDestinyInfo()->isReWard()); //已领奖
    
    m_pProperty->setVisible(!getDestinyInfo()->isNeedReAward());//属性加成
    if(m_pProperty->isVisible()) m_pProperty->setString(CCString::createWithFormat("+%d",m_pDestinyInfo->getCurrentDestinyUpgradeInfo()->getRewardItemNum())->getCString());
    
    
    m_pGrade->setColor(m_pModel->getDestinyColorByGrade(m_pDestinyInfo->getDestinyGrade()));         //品质
    
    m_pContainer->removeAllChildren();
    m_pContainer->addChild(ResourceManager::sharedResourceManager()->getDestinyIcon(m_pDestinyInfo->getID()));
    
    //更新等级
    m_pLevel->setString(CCString::createWithFormat("%d",m_pDestinyInfo->getLvDisplay())->getCString());//级别
    
}

void DestinyViewItem::onEnter()
{
    CCNode::onEnter();
    startInterval();
}

void DestinyViewItem::onExit()
{
    CCNode::onExit();
    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
}

void DestinyViewItem::startInterval()
{
    TimerUtil::sharedTimerUtil()->unscheduleUpdateForTarget(this);
    if(getDestinyInfo()->isNeedShowInterval())
    {
        TimerUtil::sharedTimerUtil()->scheduleUpdateForTarget(this, 0, false);
        update(0.f);
    }
}

void DestinyViewItem::update(float dt)
{
    uint32_t time= MAX(0,TimeUtil::getTimeDiffToTomorrow(GameManager::sharedGameManager()->getServerTime()));
    m_pInterval->setString(TimeUtil::getSeconds2String(time));
}

void DestinyViewItem::touchItem(CCObject *obj)
{
    if(m_pDelegate)
    {
        m_pDelegate->showDestinyInfoView(m_pDestinyInfo);
    }
}

void DestinyViewItem::reward(CCObject*obj)
{
    if(m_pDelegate) m_pDelegate->reward(m_pDestinyInfo->getID());
}

void DestinyViewItem::setDestinyInfo(destiny::DestinyInfo* pDestinyInfo)
{
    if(pDestinyInfo==m_pDestinyInfo) return;
    SAFE_RELEASE_UNBINDING_ALL(m_pDestinyInfo, this);
    CC_SAFE_RETAIN(pDestinyInfo);
    m_pDestinyInfo = pDestinyInfo;
    
    BINDING_EXEC(m_pDestinyInfo, this, DestinyViewItem::updateView, D_EVENT_DESTINY_UPDATED);
    
}
