//
//  Mall.cpp
//  市集商城
//
//  Created by cll on 13-6-8.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "Mall.h"
#include "../user/UserInfo.h"
#include "../../../managers/GameManager.h"
#include "MallView.h"
#include "utils/TimerUtil.h"

USING_NS_CC;


Mall::Mall()
:m_pDTabView(NULL)
,m_pContainer(NULL)
,m_pRecharge(NULL)
,m_pPopupLayer(NULL)
,m_pSoulExchangeLayer(NULL)
,m_pItemInfo(NULL)
,m_pSellButton(NULL)
,m_pSellButtonFirst(NULL)
,m_pBottom(NULL)
,m_pTableNode(NULL)
,m_isLottery(false)
,m_pFreeCardBg(NULL)
,m_pFreeCardLabel(NULL)
{
	
}

Mall::~Mall()
{
	CC_SAFE_RELEASE(m_pDTabView);
    CC_SAFE_RELEASE(m_pRecharge);
    CC_SAFE_RELEASE(m_pContainer);
    CC_SAFE_RELEASE(m_pPopupLayer);
	CC_SAFE_RELEASE(m_pSoulExchangeLayer);
    CC_SAFE_RELEASE(m_pItemInfo);
	CC_SAFE_RELEASE(m_pSellButton);
	CC_SAFE_RELEASE(m_pSellButtonFirst);
	CC_SAFE_RELEASE(m_pBottom);
	CC_SAFE_RELEASE(m_pTableNode);
	CC_SAFE_RELEASE(m_pFreeCardBg);
	CC_SAFE_RELEASE(m_pFreeCardLabel);
}

Mall* Mall::create(bool isLottery)
{
	Mall *pRet = new Mall();
	if (pRet && pRet->init(isLottery))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		CC_SAFE_DELETE(pRet); 
		return NULL; 
	} 
}

bool Mall::init(bool isLottery)
{
	if(!DLayer::init())
	{
		return false;
	}
    
	m_isLottery = isLottery;
	//CCB_READER_AND_ADDCHILD("ui/ccb/mall/mall");
    CCB_READER_AND_ADDCHILD("PL_ui/ccb/pl_shop/PL_shop");

	//m_pRecharge->getButton()->setTarget(this, menu_selector(Mall::rechargeCallBack));

	float posY = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getPosition().y, PL_MAX_HEIGHT_DIFF / 2);
	float height = PL_CAL_POSY_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize().height+4, -PL_MAX_HEIGHT_DIFF);
	m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, posY, m_pContainer->getContentSize().width, height),25);
	m_pDTabView->setPadding(15.0);
	/*m_pDTabView = DTabView::create(CCRectMake(m_pContainer->getPosition().x, m_pContainer->getPosition().y, m_pContainer->getContentSize().width, m_pContainer->getContentSize().height+24)
		,ccWHITE
		,D_FONT_DEFAULT
		,D_FONT_SIZE_14
		,15
		,50);*/
	
	//m_pDTabView->setTopMenuisLeftAlignment(false);
	m_pDTabView->setDelegate(this);
	m_pDTabView->retain();
    
    
	//std::string tabName[] = {"font_sheet_item","font_sheet_gift"/*,"font_sheet_dress"*/};
	std::string tabName[] = {"pl_bb_wenzi_daoju_x","pl_bb_wenzi_libao","pl_bb_wenzi_shizhuang"};
	
    mall::ProductType tabType[] = {mall::kProductTypeItem,mall::kProductTypeVipPk,mall::kProductTypeAvatar};
    
	uint8_t lotteryTabIndex = 0; 

    for(uint8_t i(0);i<3;++i)
    {
        MallView * pItemList = MallView::create(tabType[i],PL_CAL_SIZE_WITH_FLEXIBLE_HEIGHT(m_pContainer->getContentSize(), -PL_MAX_HEIGHT_DIFF));
        if(pItemList)
        {
			lotteryTabIndex++;
            pItemList->setDelegate(this);
            //m_pDTabView->addTabWithFontImg(pItemList,CCString::createWithFormat("ui/common/%s.png",tabName[i].c_str())->getCString());
            m_pDTabView->addTab(pItemList,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,CCString::createWithFormat("PL_ui/beibao/%s.png",tabName[i].c_str())->getCString());
        }
	}

	m_pSoulExchangeLayer= CCNode::create();
	addChild(m_pSoulExchangeLayer , 1);
	CC_SAFE_RETAIN(m_pSoulExchangeLayer);

	Lottery* pLotteryPage = Lottery::create(this);
	m_pDTabView->addTab(pLotteryPage,"","PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL,"PL_ui/beibao/pl_bb_wenzi_chouka.png");
	
	//如果是从招募界面进入的 切换界面
	if(m_isLottery) m_pDTabView->selectedTab(lotteryTabIndex);

	//m_pDTabView->addTabWithImage(Lottery::create(),"PL_ui/beibao/pl_bb_btn_xiaofenye.png","PL_ui/beibao/pl_bb_btn_xiaofenye_up.png",NULL);
    m_pTableNode->addChild(m_pDTabView);
    
    
	UserInfoView::sharedUserInfoView()->removeFromParentAndCleanup(false);
	addChild(UserInfoView::sharedUserInfoView());
    
    m_pPopupLayer= CCNode::create();
    addChild(m_pPopupLayer);
    CC_SAFE_RETAIN(m_pPopupLayer);
    
    if(!DM_GET_MALL_MODEL->isSynced) HTTP_CLIENT->getProducts();
	return true;
}

void Mall::addHead(DLayer* pChild)
{
	m_pSoulExchangeLayer->addChild(pChild);
}

void Mall::SelectTabAtIndex(DTabView *tabView, uint32_t idx)
{
	bool bOnLottery = false;
	if(dynamic_cast<Lottery*>(tabView->getSelectedTab()))
	{
		bOnLottery = true;
		//DM_GET_ACTIVITY_MODEL->syncFromServer();
		//HTTP_CLIENT->activityGet();
	}
	m_pSoulExchangeLayer->setVisible(bOnLottery);
}

void Mall::closeInfoView(InventoryType type)
{
    if(m_pItemInfo != NULL) m_pItemInfo->removeFromParent();
}

void Mall::showAvatar(mall::ProductInfo  *pInfo)
{
	if(!pInfo) return;

	unsigned int dressid = pInfo->getBaseInfo()->getItemId();
	avatar::AvatarBaseInfo* pAvatarInfo = DM_GET_AVATAR_MODEL->getAvatarBaseInfoById(dressid);
	if(pAvatarInfo)
	{
		m_pContainer->removeAllChildren();
		if(m_pItemInfo == NULL)
		{
			m_pItemInfo = ItemInfoView::create();
			m_pItemInfo->setDelegate(this);
			m_pItemInfo->retain();
		}
		m_pItemInfo->setAvatarInfo(pAvatarInfo);
		m_pPopupLayer->addChild(m_pItemInfo);
	}
}

void Mall::showInfoView(InventoryType type,item::ItemInfo* pItemInfo,cocos2d::CCObject* param)
{
    m_pContainer->removeAllChildren();
    
	if(m_pItemInfo == NULL)
	{
		m_pItemInfo = ItemInfoView::create();
        m_pItemInfo->setDelegate(this);
		m_pItemInfo->retain();
	}
    
	m_pItemInfo->setItemInfo(pItemInfo);
	m_pPopupLayer->addChild(m_pItemInfo);
}

bool Mall::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pRecharge", DButton *, this->m_pRecharge);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pContainer", CCNode *, this->m_pContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellButton", CCMenuItem *, this->m_pSellButton);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pSellButtonFirst", CCMenuItem *, this->m_pSellButtonFirst);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pBottom", CCNode *, this->m_pBottom);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pTableNode", CCNode *, this->m_pTableNode);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFreeCardLabel", CCLabelBMFont *, this->m_pFreeCardLabel);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_pFreeCardBg", CCNode *, this->m_pFreeCardBg);
    return false;
}

SEL_MenuHandler Mall::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "sellCallBack", Mall::rechargeCallBack);
	return NULL;
}


void Mall::rechargeCallBack(cocos2d::CCObject*)
{
    NotificationCenter::sharedNotificationCenter()->postNotification(D_EVENT_SHOW_RECHARGE);
}

void Mall::onBackKey()
{
	if (m_pPopupLayer && m_pPopupLayer->getChildrenCount())
	{
		m_pPopupLayer->removeAllChildren();
	}
	else
	{
		BackKeyInterface::onBackKey();
	}
}

void Mall::onEnter()
{
	DLayer::onEnter();
	//新增免费抽卡角标 五秒刷新一次
	showLotteryDing(0);
	TimerUtil::sharedTimerUtil()->schedule(schedule_selector(Mall::showLotteryDing), this, 5); 
	//是否是首充
	m_pSellButton->setVisible(DM_GET_USER_MODEL->getUserInfo()->isPayed());
	m_pSellButtonFirst->setVisible(!m_pSellButton->isVisible());
}

void Mall::onExit()
{
    TimerUtil::sharedTimerUtil()->unscheduleAllForTarget(this);
    DLayer::onExit();
}

void Mall::showLotteryDing(float dt)
{
	unsigned int times = DM_GET_LOTTERY_MODEL->getFreeCount();
	if(times==0)
	{
		m_pFreeCardBg->setVisible(false);
	}
	else{
		unsigned int viplevel = DM_GET_USER_MODEL->getUserInfo()->getVipLevel();
		//bool bVipMax = (viplevel == DM_GET_USER_MODEL->getMaxVipLevel());

		MallModel::ProductInfoVec products;
		DM_GET_MALL_MODEL->getProductsInMallByType(mall::kProductTypeVipPk,products);
		bool bShowVipPackage = (products.count() > 0);
		m_pFreeCardBg->setPositionX(bShowVipPackage?460:345);

		m_pFreeCardBg->setVisible(true);
		m_pFreeCardLabel->setString(D_CSTRING_FROM_UINT(times));
	}
}